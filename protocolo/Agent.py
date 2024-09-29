from twisted.internet import reactor
from twisted.internet.protocol import Protocol
from twisted.internet.protocol import ClientFactory
from twisted.internet.endpoints import TCP4ClientEndpoint
import subprocess

class AgentControllerProtocol(Protocol):
    def __init__(self):
        super().__init__()
        self.header = ""        #
        self.requestType = ""   #
        self.response = ""      # Resposta a ser mandada para o serviço

    def parseRequest(self, request:bytes):
        self.header = request.decode("utf-8")
        self.requestType = (self.header.split('\n')[1]).split()[1]

    def actOnRequest(self):
        if self.requestType == "START_MONITORING":
            argumentsField = (self.header.split('\n')[1]).split()[2:]
            result = self.recordAudio(argumentsField)
            
            if result.returncode == 0:
                self.transport.write(b"REPLY - Monitoring Protocol\nType: START_MONITORING\nState: Ok")
            else:
                self.transport.write(b"REPLY - Monitoring Protocol\nType: START_MONITORING\nState: Error")
        
        if self.requestType == "PULL_AUDIO":
            return
        
        if self.requestType == "CLOSE_CONNECTION":
            print("Fechando Conexão...")
            self.transport.loseConnection()
        
    def recordAudio(self, headerArgumentField: list[str]):
        deviceIndex = "-i" + headerArgumentField[0]
        recordingTime = "-t" + headerArgumentField[1]
        recordingLoops = "-r" + headerArgumentField[2]
        output = "-o" + "audios"
        return subprocess.run(['audioRecorder', deviceIndex, recordingTime, recordingLoops, output])
    
    # LIST DELETE audios/audio 29-07-2024 às 09:40:38:02.wav

    def connectionMade(self):
        self.transport.write(b"REPLY - Monitoring Protocol\nType: READY_TO_START")
        
    def dataReceived(self, request):
        self.parseRequest(request)
        self.actOnRequest()

    def connectionLost(self, reason):
        reactor.stop()

class AgentControllerFactory(ClientFactory):
    def buildProtocol(self, addr):
        return AgentControllerProtocol()
    
# class AgentDataTransporter(Protocol):
#     def __init__(self):
#         super().__init__()
#         self.data = b''

#     def connectionMade(self):
#         with open("audios/audio 01-08-2024 às 10:23:56:51.wav", "rb") as file:
#             self.data = file.read()
        

#     def dataReceived(self, data):
#         return
    
# class AgentTransporterFactory(ClientFactory):
#     def buildProtocol(self, addr):
#         return AgentDataTransporter()
    
if __name__ == '__main__':
    controllerEndpoint = TCP4ClientEndpoint(reactor, '127.0.0.1', 2000)
    # transporterEndpoint = TCP4ClientEndpoint(reactor, '127.0.0.1', 2001)
    controllerEndpoint.connect(AgentControllerFactory())
    # transporterEndpoint.connect(AgentTransporterFactory())
    reactor.run()