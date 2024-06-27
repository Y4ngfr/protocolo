#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <libAudioRecorder/libAudioRecorder.h>
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

void processRequestMessage(const char* requestMessage, RequestData* requestData)
{
    char instructionRequested[17];
    size_t typeOffset;

    bzero(&requestData->params, sizeof(RecordingParams));
    requestData->closeConnection = 0;
    requestData->pullAudio = 0;
    requestData->status = 0;
    requestData->stopMonitoring = 0;

    typeOffset = strcspn(&(requestMessage[36]), "\n");
    strncpy(instructionRequested, &(requestMessage[36]), typeOffset);
    instructionRequested[typeOffset] = '\0';

    if(!strcmp("START_MONITORING", instructionRequested))
    {
        char auxBuffer[10];
        size_t paramsOffset;

        paramsOffset = strcspn(&(requestMessage[37 + typeOffset]), " ");
        strncpy(auxBuffer, &(requestMessage[37 + typeOffset]), paramsOffset);
        auxBuffer[paramsOffset] = '\0';

        requestData->params.deviceIndex = atoi(auxBuffer);

        paramsOffset = strcspn(&requestMessage[38 + typeOffset + paramsOffset], " ");
        strncpy(auxBuffer, &(requestMessage[38 + typeOffset + strlen(auxBuffer)]), paramsOffset);
        auxBuffer[paramsOffset] = '\0';

        requestData->params.repetitions = atoi(auxBuffer);

        paramsOffset = strcspn(&requestMessage[39 + typeOffset + paramsOffset], "\n");
        strncpy(auxBuffer, &(requestMessage[40 + typeOffset + strlen(auxBuffer)]), paramsOffset);
        auxBuffer[paramsOffset] = '\0';

        requestData->params.recordingTime = atof(auxBuffer);

        requestData->params.helpArgument = 0;
        requestData->params.deviceArgument = 0;

        return;
    }

    if(!strcmp("STOP_MONITORING", instructionRequested))
    {
        requestData->stopMonitoring = 1;

        return;
        // interrompe qualquer gravação ou então apenas não faz nada
    }

    if(!strcmp("PULL_AUDIO", instructionRequested))
    {
        requestData->pullAudio = 1;

        return;
        // envia os dados disponíveis
    }

    if(!strcmp("STATUS", instructionRequested))
    {
        requestData->status = 1;
        requestData->params.deviceArgument = 1;
        requestData->params.helpArgument = 0;

        return;
        // envia informações importantes
        // --help, --devices, quantidade de áudios gravados,
        // último áudio gravado, tempo dos áudios, etc
    }

    if(!strcmp("CLOSE_CONNECTION", instructionRequested))
    {
        requestData->closeConnection = 1;

        return;
        // fecha a conexão
    }

    printf("Erro ao processar instrução: mensagem mal formatada\n");
}

u_int32_t perform(RequestData* requestData, void* buff)
{
    if(requestData->stopMonitoring){
        return stopMonitoring(buff);
    }
    else if(requestData->closeConnection){
        return closeConnection(requestData, buff);
    }
    else if(requestData->pullAudio){
        return pullAudio(buff);
    }
    else if(requestData->status){
        return status(&(requestData->params), buff);
    }
    else{
        return startMonitoring(&(requestData->params), buff);
    }

    return 0;
}

u_int32_t stopMonitoring(void *buff)
{
    strcpy((char*)buff, "STOP_MONITORING executed successfully");

    return strlen((char*)buff);
}

u_int32_t closeConnection(RequestData* requestData, void *buff)
{
    *(requestData->running) = 0;

    strcpy((char*)buff, "CLOSE_CONNECTION executed successfully");

    return strlen((char*)buff);
}

u_int32_t pullAudio(void *buff)
{
    FILE* fp;
    u_int32_t fileSize;

    fp = fopen("recordings/audio 25-06-2024 às 19:33:41:81.wav", "r");

    if(fp == NULL){
        printf("Não foi possível abrir o arquivo\n");
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    
    buff = (u_int8_t*)malloc(fileSize * sizeof(u_int8_t));

    if(buff == NULL){
        printf("Erro ao alocar buffer de dados para o arquivo\n");
        return 0;
    }

    fseek(fp, 0, SEEK_SET);

    fread(buff, sizeof(u_int8_t), fileSize, fp);

    fclose(fp);

    return fileSize;
}

u_int32_t status(RecordingParams *params, void *buff)
{
    strcpy((char*)buff, "START_MONITORING executed successfully");

    return strlen((char*)buff);
}

u_int32_t startMonitoring(RecordingParams *params, void* buff)
{
    pid_t PID;

    params->deviceArgument = 0;
    params->helpArgument = 0;
    params->directory = "recordings";

    PID = fork();

    if(PID == 0)
    { // Processo filho
        if(runRecorder(params) < 0){
            exit(-1);
        }

        exit(0);
    }
    else{
        int sig;

        wait(&sig);

        if(sig < 0){
            strcpy((char*)buff, "START_MONITORING failed");
        }

        strcpy((char*)buff, "START_MONITORING executed successfully");
    }

    return strlen((char*)buff);
}

// u_int8_t* generateReplyMessage(const char* serverMessage, uint8_t* messageBuffer)
// {
//     uint8_t *buffer;
//     long fileSize;
//     FILE *fp;

//     fp = fopen("recordings/audio 25-06-2024 às 19:32:26:45.wav", "r");

//     if(fp == NULL){
//         printf("arquivo não encontrado\n");
//         return NULL;
//     }

//     fseek(fp, 0, SEEK_END);
//     fileSize = ftell(fp);

//     printf("%ld bytes\n", fileSize);

//     buffer = (u_int8_t*)malloc(fileSize*sizeof(u_int8_t));

//     if(buffer == NULL){
//         printf("Erro ao alocar buffer\n");
//         return NULL;
//     }

//     fseek(fp, 0, SEEK_SET);

//     fread(buffer, sizeof(u_int8_t), fileSize, fp);

//     fclose(fp);

//     return buffer;
// }