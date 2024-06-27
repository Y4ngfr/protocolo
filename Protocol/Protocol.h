#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#define START_MONITORING 0
#define STOP_MONITORING 1
#define PULL_AUDIO 2
#define STATUS 3
#define CLOSE_CONNECTION 4

#include <libAudioRecorder/libAudioRecorder.h>

typedef struct RequestData{

    RecordingParams params;
    int stopMonitoring;
    int closeConnection;
    int pullAudio;
    int status;
    int *running;

} RequestData;

size_t generateRequestMessage(int type, RecordingParams *params, char* messageBuffer);
void processRequestMessage(const char* requestMessage, RequestData* requestData);
u_int32_t perform(RequestData* requestData, void* buff);

u_int32_t startMonitoring(RecordingParams* params, void* buff);
u_int32_t status(RecordingParams* params, void* buff);
u_int32_t stopMonitoring(void* buff);
u_int32_t closeConnection(RequestData* requestData, void* buff);
u_int32_t pullAudio(void* buff);

// u_int8_t* generateReplyMessage();

#endif