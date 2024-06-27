#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libAudioRecorder/InputReader.h>
#include "Interfaces/ServerInterface.h"
#include "Protocol/Protocol.h"

int main()
{
    Server* server;
    RecordingParams params;
    char instruction[100];
    char requestMessage[100];
    char clientMessage[50];
    char clientIp[20];
    // u_int8_t *buffer;
    int running = 1;

    server = serverCreate(3003, "127.0.0.1");

    if(server == NULL) {
        exit(1);
    }

    serverWaitForConnections(server);

    serverGetClientIP(server->client, clientIp);
    printf("Ip cliente: %s\n", clientIp);

    params.deviceIndex = 0;
    params.repetitions = 3;
    params.recordingTime = 4.32;

    printf("Linha de comando Servidor:\n\n");

    while(running)
    {
        scanf("%s", instruction);

        if(!strcmp("START_MONITORING", instruction))
        {
            generateRequestMessage(START_MONITORING, &params, requestMessage);
        }
        if(!strcmp("STOP_MONITORING", instruction))
        {
            generateRequestMessage(STOP_MONITORING, &params, requestMessage);
        }
        if(!strcmp("STATUS", instruction))
        {
            generateRequestMessage(STATUS, &params, requestMessage);
        }
        if(!strcmp("PULL_AUDIO", instruction))
        {
            generateRequestMessage(PULL_AUDIO, &params, requestMessage);
        }
        if(!strcmp("CLOSE_CONNECTION", instruction))
        {
            generateRequestMessage(CLOSE_CONNECTION, &params, requestMessage);
            running = 0;
        }

        serverSendMessageToClient(server->client, requestMessage);

        serverReciveClientMessage(server->client, clientMessage);

        printf("%s\n", clientMessage);
    }

    serverCleanup(server);

    return 0;
}