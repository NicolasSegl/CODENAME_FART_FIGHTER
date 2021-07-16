#include "Server.h"

#include <iostream>
#include <thread>
#include <mutex>

struct Peer
{
	ENetPeer* peer;
	int id;
};

Server::Server()
    : NetworkBase()
{
    isHosting = false;

    // setup the level (TEST ONLY FOR NOW)
    m_level->loadLevel("test");
}

void Server::shutdown()
{
    std::cout << "Server shutting down\n";
    if (isHosting)
    {
        isHosting = false;
        m_hostThread.join();
    }

    Packet packet;
    packet.packetRequest = PacketRequest::ServerShutdown;
    packet.sendToAllPeers(m_serverHost, UDP::RELIABLE);

    // this actually sends the packet, as we will not be serving the host again
    enet_host_flush(m_serverHost);
}

Server::~Server()
{
    if (isHosting)
        shutdown();
    enet_host_destroy(m_serverHost);
}

void Server::sendData()
{
    Sleep(1);
    static EntityUpdatePacket packet;

    for (auto& client : m_clients)
    {
        if (!client->connected)
            continue;

        packet.clientID = client->id;
        packet.position = client->character->getPos();
        packet.sendToAllPeers(m_serverHost, UDP::UNRELIABLE);
    }
}

void Server::sendIDToClient(ENetPeer* peer)
{
    static Packet packet;
    packet.clientID = m_latestID;
    packet.packetRequest = PacketRequest::SendID;
    packet.sendToPeer(peer, UDP::RELIABLE);
}

bool Server::clientInit()
{
    if (m_latestID >= PLAYER_CAP)
        return false;
    /*for (int client = 0; client < PLAYER_CAP; client++)
        if (!m_clients[m_latestID])
            for (int i = 0; i < PLAYER_CAP; i++)
                if (m_clients[i]->clientIP == receivedPacket.senderIP)
                    return;
    */

    // create a new client and initialize it with default values
    m_clients.push_back(new Client);
    m_clients[m_clients.size() - 1]->id = m_latestID;
    m_clients[m_clients.size() - 1]->serverCreationInit();
    m_clients[m_clients.size() - 1]->connected = true;
    m_latestID++;

    return true;
}

void Server::updateClientCharacterList()
{
    // we need to send this when a client connects.
    for (int id = 0; id < m_clients.size(); id++)
    {
        static NewEntityPacket packet;
        packet.clientID = id;

        if (!m_clients[id]->connected)
            packet.connected = false;
        else
        {
            packet.connected = true;
            packet.newCharacter = *(m_clients[id]->character);
            packet.newCharacter.id = m_clients[id]->character->id;
        }

        packet.sendToAllPeers(m_serverHost, UDP::RELIABLE);
    }
}

void Server::updateEntity(Packet* packet)
{
    EntityUpdatePacket* packetReceived = (EntityUpdatePacket*)(packet);

    // update the position of the sprite to be drawn
    m_clients[packetReceived->clientID]->character->setPos(packetReceived->position);
    m_clients[packetReceived->clientID]->character->sprite.setPosition(sf::Vector2f
    (
        packetReceived->position.x,
        packetReceived->position.y
    ));
}

void Server::clientDisconnect(int id)
{
    m_clients[id]->connected = false;
    std::cout << "Client " << id << " has disconnected\n";

    // send the client disconnect packet to other clients as well, so they can handle the disconnection of a client
    Packet* newPacket = new Packet;
    newPacket->clientID = id;
    newPacket->packetRequest = PacketRequest::ClientDisconnect;
    newPacket->sendToAllPeers(m_serverHost, UDP::RELIABLE);
}

void Server::receiveAcknowledgement(Packet* packet, ENetPeer* peer)
{
    if (clientInit())
    {
        std::cout << "Client " << packet->clientID << " successfully initialized\n";
        updateClientCharacterList();

        LevelPacket levelPacket;

        //this is scuffed lmao
        for (int character = 0; character < m_level->levelName.size(); character++)
            levelPacket.levelName[character] = m_level->levelName[character];

        // make the final character a terminating 0
        levelPacket.levelName[m_level->levelName.size()] = '\0';
        levelPacket.sendToPeer(peer, UDP::RELIABLE);
    }
    else
        std::cout << "Client failed to initialize\n";
}

void Server::serviceHost()
{
    // iterate through all the packets received by the server
    ENetEvent event;
    static std::vector<Peer> peers;

    while (enet_host_service(m_serverHost, &event, 0) > 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
            {
                std::cout << "A client has connected from: " << &event.peer->address.host << std::endl;
                // sends to the same peer that connected...
                sendIDToClient(event.peer);
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE:
            {
                Packet* packetReceived = (Packet*)(event.packet->data);

                if (packetReceived->packetRequest == PacketRequest::AcknowledgeID)
                {
                    receiveAcknowledgement(packetReceived, event.peer);
                    peers.push_back({ event.peer, packetReceived->clientID });
                    break;
                }
                if (m_clients[packetReceived->clientID] >= 0)
                {
                    switch (packetReceived->packetRequest)
                    {
                    case PacketRequest::EntityUpdate:
                    {
                        updateEntity(packetReceived);
                        break;
                    }
                    case PacketRequest::ClientDisconnect:
                    {
                        clientDisconnect(packetReceived->clientID);
                        break;
                    }
                    }
                }

                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT:
            {
                // fill a vector with all of the enet peers currently connected to the server host
                // this is necessary to properly disconnect the client, as it allows us to nullify the client
                std::vector<ENetPeer*> enetPeers;
                for (int peer = 0; peer < peers.size(); peer++)
                    enetPeers.push_back(peers[peer].peer);

                int index = std::distance(enetPeers.begin(), std::find(enetPeers.begin(), enetPeers.end(), event.peer));

                if (index >= 0 && index < m_clients.size())
                {
                    peers.erase(peers.begin() + index);

                    // if the client has already disconnected, no need to disconnect it again
                    if (m_clients[index]->connected)
                    {
                        clientDisconnect(index);
                        std::cout << "Client " << index << " has timed out\n";
                    }
                }
            
                break;
            }
        }
    }
}

void Server::start(std::string ip)
{
    enet_address_set_host(&m_serverIPAddress, ip.c_str());

    m_serverIPAddress.host = ENET_HOST_ANY;
    m_serverHost = enet_host_create(&m_serverIPAddress, PLAYER_CAP, 2, 0, 0);
    if (m_serverHost == nullptr)
    {
        std::cout << "Error creating ENet server host\n";
        return;
    }
    isHosting = true;
    // create a thread that will send/receive data for the server.
    m_hostThread = std::thread(&Server::handleClients, this);
}

void Server::handleClients()
{
    while (isHosting)
        update();
}

std::mutex mutex;
void Server::update()
{
    mutex.lock();
    serviceHost();
    // comment sendData out if there are problems XDDDDD
    sendData();
    mutex.unlock();
}
