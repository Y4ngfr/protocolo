from twisted.internet import reactor
from twisted.internet.protocol import Protocol
from twisted.internet.protocol import ClientFactory
from twisted.internet.endpoints import TCP4ClientEndpoint
import sys

class AgentDataTransporter(Protocol):
    def __init__(self):
        super().__init__()
        self.audioBuffer = b''
        try:
            self.audioFile = sys.argv[1]
        except Exception as error:
            print("Faltando argumento")
            self.audioFile = "ERROR"

    def connectionMade(self):
        if self.audioFile != "ERROR":
            try:
                with open(self.audioFile, "rb") as file:
                    self.audioBuffer = file.read()
                    self.transport.write(self.audioBuffer)
                    print("ok")
            except Exception as error:
                print(error)

        self.transport.loseConnection()

    def connectionLost(self, reason):
        reactor.stop()

class AgentTransporterFactory(ClientFactory):
    def buildProtocol(self, addr):
        return AgentDataTransporter()
    
if __name__ == '__main__':
    try:
        endpoint = TCP4ClientEndpoint(reactor, '127.0.0.1', 2002)
        endpoint.connect(AgentTransporterFactory())
        reactor.run()
    except Exception as error:
        print(error)