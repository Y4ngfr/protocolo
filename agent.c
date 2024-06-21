#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Interfaces/ClientInterface.h"
#include "Protocol/Protocol.h"

int main()
{
    Client* client;
    char replyToServer[] = "dados, dados, dados, dados";
    char serverMessage[100];

    client = clientCreate();

    if(client == NULL){
        exit(1);
    }

    if(clientConnectToServer(client, "127.0.0.1", 3014) < 0){
        exit(1);
    }

    if(clientReciveServerMessage(client, serverMessage) < 0){
        exit(1);
    }

    printf("%s\n", serverMessage);

    if(clientSendMessageToServer(client, replyToServer) < 0){
        exit(1);
    }

    printf("\nMensagem Resposta:\n");
    generateReplyMessage(serverMessage, replyToServer);
    // printf("%s\n", replyToServer);

    clientCleanup(client);

    return 0;
}