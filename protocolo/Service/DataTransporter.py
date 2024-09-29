from twisted.internet import reactor, protocol
from twisted.internet.protocol import Protocol, connectionDone
from twisted.internet.protocol import ServerFactory
from twisted.internet.endpoints import TCP4ServerEndpoint
from twisted.python.failure import Failure
import sys
    
class ServiceDataTransporter(Protocol):
    def __init__(self):
        super().__init__()
        self.data = b''
        try:
            self.audioFile = sys.argv[1]
        except Exception as error:
            print("Faltando argumento nome do arquivo")
            self.audioFile = "ERROR"

    def dataReceived(self, data):
        self.data += data
    
    def connectionLost(self, reason):
        if self.audioFile != "ERROR" and self.data != b'':
            try:
                with open(self.audioFile, "wb") as file:
                    file.write(self.data)
            except Exception as error:
                print(error)

        reactor.stop()
    
class ServiceTransporterFactory(ServerFactory):
    def buildProtocol(self, addr):
        return ServiceDataTransporter()

if __name__ == '__main__':
    try:    
        transferEndpoint = TCP4ServerEndpoint(reactor, 2002)
        transferEndpoint.listen(ServiceTransporterFactory())
        reactor.run()
    except Exception as error:
        print(error)