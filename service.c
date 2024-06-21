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
    char requestMessage[100];
    char clientMessage[50];
    char clientIp[20];

    server = serverCreate(3014, "127.0.0.1");

    if(server == NULL) {
        exit(1);
    }

    serverWaitForConnections(server);

    params.deviceIndex = 0;
    params.repetitions = 3;
    params.recordingTime = 4.32;

    serverGetClientIP(server->client, clientIp);
    printf("Ip cliente: %s\n", clientIp);

    generateRequestMessage(START_MONITORING, &params, requestMessage);
    serverSendMessageToClient(server->client, requestMessage);

    serverReciveClientMessage(server->client, clientMessage);

    printf("%s\n", clientMessage);

    serverCleanup(server);

    return 0;
}