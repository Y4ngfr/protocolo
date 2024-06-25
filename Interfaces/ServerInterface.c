#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include "ServerInterface.h"

Server* serverCreate(int serverPort, const char* serverIp)
{
    Server* newServer;
    struct in_addr networkServerIp;
    struct sockaddr* convertedServerAdress;
    
    if(inet_aton(serverIp, &networkServerIp) == 0){
        printf("Error: invalid ip adress format\n");
        return NULL;
    }

    newServer = (Server*)malloc(sizeof(Server));

    if(newServer == NULL){
        printf("Error: Unable to allocate memory to server\n");
        return NULL;
    }

    newServer->serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(newServer->serverSocket < 0){
        perror("Unnable to open socket. Error");                          
        //printf("Error: %s\n", strerror(errno)); // Mesma coisa que perror("Error")
        free(newServer);
        return NULL;
    }

    newServer->serverAdress.sin_family = AF_INET;
    newServer->serverAdress.sin_port = htons(serverPort);
    newServer->serverAdress.sin_addr = networkServerIp;

    convertedServerAdress = (struct sockaddr*)(&(newServer->serverAdress));

    if(bind(newServer->serverSocket, convertedServerAdress, sizeof(struct sockaddr_in)) < 0)
    {
        perror("Unnable to open port. Error");
        close(newServer->serverSocket);
        free(newServer);
        return NULL;
    }

    if(listen(newServer->serverSocket, 2) < 0){
        perror("Unnable to start listen. Error");
        close(newServer->serverSocket);
        free(newServer);
        return NULL;
    }

    newServer->client = NULL;

    return newServer;
}

void serverCleanup(Server* server)
{
    char buff[1];

    while(read(server->client->clientSocket, buff, 1) != 0); // Espera o cliente encerrar primeiro

    close(server->serverSocket);
    free(server);
}

int serverWaitForConnections(Server* server)
{
    struct sockaddr_in clientAdress;
    socklen_t clientAdressLenght;
    Client* newClient;
    int clientSocket;

    clientAdressLenght = sizeof(struct sockaddr_in);

    clientSocket = accept(server->serverSocket, (struct sockaddr*)&clientAdress, &clientAdressLenght);

    if(clientSocket == -1){
        printf("Erro: Socket do cliente inválido\n");
        return -1;
    }

    newClient = clientCreate();

    if(newClient == NULL){
        printf("Erro: Não foi possível criar novo cliente\n");
        return -1;
    }

    newClient->clientSocket = clientSocket;
    newClient->clientAdress = clientAdress;

    server->client = newClient;
    newClient = NULL;

    return 0;
}

void serverGetClientIP(Client* client, char* buffer)
{
    inet_ntop(AF_INET, &(client->clientAdress.sin_addr), buffer, INET_ADDRSTRLEN);
}

ssize_t serverReciveClientMessage(Client* client, char* buffer)
{
    ssize_t numberOfBytesRead;

    numberOfBytesRead = recv(client->clientSocket, buffer, 100, 0);

    if(numberOfBytesRead < 0) perror("Unnable to recive message. Error");

    return numberOfBytesRead;
}

ssize_t serverSendMessageToClient(Client* client, const char* message)
{
    ssize_t numberOfBytesSent;   

    send(client->clientSocket, message, strlen(message) + 1, 0); // + 1 para contar o \0

    if(numberOfBytesSent < 0) perror("Unnable to send message. Error");

    return numberOfBytesSent;
}
