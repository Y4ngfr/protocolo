#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Protocol.h"

size_t generateRequestMessage(int type, RecordingParams *params, char* messageBuffer)
{
    switch (type)
    {
        case START_MONITORING:
        {
            const char* startMonitoringTemplate;
            char* message;
            size_t messageLength;

            startMonitoringTemplate = "REQUEST - Monitoring Protocol\nType: START_MONITORING";
            messageLength = snprintf(NULL, 0, "%s\n%d %d %f\n", startMonitoringTemplate, params->deviceIndex, params->repetitions, params->recordingTime);

            if(messageBuffer == NULL){
                return messageLength;
            }

            message = (char*)malloc(messageLength*sizeof(char));

            if(message == NULL){
                printf("Error: Unnable to allocate memory for START_MONITORING message\n");
                return -1;
            }

            sprintf(message, "%s\n%d %d %f\n", startMonitoringTemplate, params->deviceIndex, params->repetitions, params->recordingTime);
            strcpy(messageBuffer, message);
            free(message);

            return messageLength;
        }

        case STOP_MONITORING:
        {
            const char* stopMonitoringTemplate;
            size_t messageLength;

            stopMonitoringTemplate = "REQUEST - Monitoring Protocol\nType: STOP_MONITORING\n";
            messageLength = strlen(stopMonitoringTemplate);

            if(messageBuffer == NULL){
                return messageLength;
            }

            strcpy(messageBuffer, stopMonitoringTemplate);

            return messageLength;
        }

        case PULL_AUDIO:
        {
            const char* pullAudioTemplate;
            size_t messageLength;

            pullAudioTemplate = "REQUEST - Monitoring Protocol\nType: PULL_AUDIO\n";
            messageLength = strlen(pullAudioTemplate);

            if(messageBuffer == NULL){
                return messageLength;
            }

            strcpy(messageBuffer, pullAudioTemplate);

            return messageLength;
        }

        case STATUS:
        {
            const char* statusTemplate;
            size_t messageLength;

            statusTemplate = "REQUEST - Monitoring Protocol\nType: STATUS\n";
            messageLength = strlen(statusTemplate);

            if(messageBuffer == NULL){
                return messageLength;
            }

            strcpy(messageBuffer, statusTemplate);

            return messageLength;
        }

        case CLOSE_CONNECTION:
        {
            const char* closeConnectionTemplate;
            size_t messageLength;

            closeConnectionTemplate = "REQUEST - Monitoring Protocol\nType: CLOSE_CONNECTION\n";
            messageLength = strlen(closeConnectionTemplate);

            if(messageBuffer == NULL){
                return messageLength;
            }

            strcpy(messageBuffer, closeConnectionTemplate);

            return messageLength;
        }

        default:
            printf("Error: unknow flag\n");
            return -1;
    }
}

void generateReplyMessage(const char* requestMessage, char* messageBuffer)
{
    char instructionRequested[17];
    size_t typeOffset;

    typeOffset = strcspn(&(requestMessage[36]), "\n");
    strncpy(instructionRequested, &(requestMessage[36]), typeOffset);
    instructionRequested[typeOffset] = '\0';

    if(!strcmp("START_MONITORING", instructionRequested))
    {
        int deviceIndex, recordingLoops;
        float recordingTime;
        char auxBuffer[10];
        size_t paramsOffset;

        paramsOffset = strcspn(&(requestMessage[37 + typeOffset]), " ");
        strncpy(auxBuffer, &(requestMessage[37 + typeOffset]), paramsOffset);
        auxBuffer[paramsOffset] = '\0';

        deviceIndex = atoi(auxBuffer);

        paramsOffset = strcspn(&requestMessage[38 + typeOffset + paramsOffset], " ");
        strncpy(auxBuffer, &(requestMessage[38 + typeOffset + strlen(auxBuffer)]), paramsOffset);
        auxBuffer[paramsOffset] = '\0';

        recordingLoops = atoi(auxBuffer);

        paramsOffset = strcspn(&requestMessage[39 + typeOffset + paramsOffset], "\n");
        strncpy(auxBuffer, &(requestMessage[40 + typeOffset + strlen(auxBuffer)]), paramsOffset);
        auxBuffer[paramsOffset] = '\0';

        recordingTime = atof(auxBuffer);

        printf("deviceIndex: %d, recordingLoops: %d, recordingTime: %f\n", deviceIndex, recordingLoops, recordingTime);

        // abre o gravador e insere os parâmetros
        // executa a gravação ...
        // envia os dados gravados
    }

    if(!strcmp("STOP_MONITORING", instructionRequested))
    {
        printf("teste2\n");
        // interrompe qualquer gravação ou então apenas não faz nada
    }

    if(!strcmp("PULL_AUDIO", instructionRequested))
    {
        printf("teste3\n");
        // envia os dados disponíveis
    }

    if(!strcmp("STATUS", instructionRequested))
    {
        printf("teste4\n");
        // envia informações importantes
        // --help, --devices, quantidade de áudios gravados,
        // último áudio gravado, tempo dos áudios, etc
    }

    if(!strcmp("CLOSE_CONNECTION", instructionRequested))
    {
        printf("teste5\n");
        // fecha a conexão
    }

    printf("Erro ao captar instrução\n");
}