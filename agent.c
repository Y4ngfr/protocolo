#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Interfaces/ClientInterface.h"
#include "Protocol/Protocol.h"

int main()
{
    Client* client;
    char replyToServer[100];
    char serverMessage[100];
    int running = 1;

    client = clientCreate();

    if(client == NULL){
        exit(1);
    }

    if(clientConnectToServer(client, "127.0.0.1", 3003) < 0){
        exit(1);
    }

    while(running)
    {
        RequestData requestData;
        u_int32_t size;

        printf("Aguardando instruções...\n");

        if(clientReciveServerMessage(client, serverMessage) < 0){
            exit(1);
        }

        printf("Instrução recebida!\n");
        printf("%s\n", serverMessage);

        requestData.running = &running;

        processRequestMessage(serverMessage, &requestData);
        size = perform(&requestData, replyToServer);

        if(clientSendMessageToServer(client, replyToServer, size) < 0){
            exit(1);
        }
    }

    clientCleanup(client);

    return 0;
}