#ifndef _SERVERINTERFACE_H
#define _SERVERINTERFACE_H

#include "ClientInterface.h"

typedef struct Server{

    int serverSocket;
    struct sockaddr_in serverAdress;
    Client* client;

} Server;

// Construtor
Server* serverCreate(int serverPort, const char* serverIp);
//Destrutor
void serverCleanup(Server* server);

int serverWaitForConnections(Server* server);
void serverGetClientIP(Client* client, char* buffer);
ssize_t serverReciveClientMessage(Client* client, char* buffer);
ssize_t serverSendMessageToClient(Client* client, const char* message);

#endif