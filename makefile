all:
	@ gcc service.c Interfaces/ServerInterface.c Interfaces/ClientInterface.c Protocol/Protocol.c -g -o service -lAudioRecorder
	@ gcc agent.c Interfaces/ClientInterface.c Protocol/Protocol.c -g -o agent