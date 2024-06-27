all:
	@ gcc service.c Interfaces/ServerInterface.c Interfaces/ClientInterface.c Protocol/Protocol.c -g -o service -lAudioRecorder -lSDL2 -lsndfile
	@ gcc agent.c Interfaces/ClientInterface.c Protocol/Protocol.c -g -o agent -lAudioRecorder -lSDL2 -lsndfile