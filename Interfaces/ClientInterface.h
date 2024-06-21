#ifndef _CLIENTINTERFACE_H
#define _CLIENTINTERFACE_H

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct Client{

    int clientSocket;
    struct sockaddr_in clientAdress;

} Client;

// Construtor
Client* clientCreate();
// Destrutor
void clientCleanup(Client* client);

int clientConnectToServer(Client* client, const char* serverIp, const int port);
ssize_t clientReciveServerMessage(Client* client, char* buffer);
ssize_t clientSendMessageToServer(Client* client, const char* message);

#endif