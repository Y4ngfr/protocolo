#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include "ClientInterface.h"

Client* clientCreate()
{
    Client* newClient;
    struct sockaddr_in adress;

    newClient = (Client*)malloc(sizeof(Client));

    if(newClient == NULL)
    {
        printf("Error: Unable to allocate memory to client");
        return NULL;
    }

    memset(&adress, '0', sizeof(struct sockaddr_in));

    newClient->clientAdress = adress;
    newClient->clientSocket = -1;

    return newClient;
}

int clientConnectToServer(Client* client, const char* serverIp, const int port)
{
    struct sockaddr_in serverAdress;

    if(inet_aton(serverIp, &(serverAdress.sin_addr)) == 0){
        printf("Error: invalid ip adress format\n");
        return -1;
    }

    client->clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(client->clientSocket < 0){
        perror("Unnable to open socket. Error");
        return -1;
    }

    serverAdress.sin_family = AF_INET;
    serverAdress.sin_port = htons(port);

    if(connect(client->clientSocket, (struct sockaddr*)&serverAdress, sizeof(serverAdress)) < 0)
    {
        perror("Unnable to connect. Error");
        return -1;
    }

    return 0;
}

void clientCleanup(Client* client)
{
    close(client->clientSocket);
    free(client);
}

ssize_t clientReciveServerMessage(Client* client, char* buffer)
{
    ssize_t numberOfBytesRead;

    numberOfBytesRead = recv(client->clientSocket, buffer, 100, 0);

    if(numberOfBytesRead < 0) perror("Unnable to recive message. Error");

    return numberOfBytesRead;
}

ssize_t clientSendMessageToServer(Client* client, const char* message)
{
    ssize_t numberOfBytesSent;

    numberOfBytesSent = send(client->clientSocket, message, strlen(message) + 1, 0);

    if(numberOfBytesSent < 0) perror("Unnable to send message. Error");

    return numberOfBytesSent;
}
