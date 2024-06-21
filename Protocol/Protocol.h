#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#define START_MONITORING 0
#define STOP_MONITORING 1
#define PULL_AUDIO 2
#define STATUS 3
#define CLOSE_CONNECTION 4

#include <libAudioRecorder/libAudioRecorder.h>

size_t generateRequestMessage(int type, RecordingParams *params, char* messageBuffer);
void generateReplyMessage(const char* serverMessage, char* messageBuffer);

#endif