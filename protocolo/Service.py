from twisted.internet import reactor, protocol
from twisted.internet.protocol import Protocol, connectionDone
from twisted.internet.protocol import ServerFactory
from twisted.internet.endpoints import TCP4ServerEndpoint
from twisted.python.failure import Failure
from CommandInterface import CommandInterface

# Baseado no tutorial https://www.youtube.com/watch?v=_XGlpzjbbvU&list=PLP6YTIItbLXPxv85Xx8sh8xVk8zMjjcCh
    
class ServiceControllerProtocol(Protocol):
    def __init__(self):
        super().__init__()
        self.header = ""        # 
        self.responseType = ""  # 
        self.request = ""       # Requisição a ser mandada para o agente

    def parseResponse(self, response:bytes):
        self.header = response.decode("utf-8")
        self.responseType = (self.header.split('\n')[1]).split()[1]

    def actOnResponse(self):
        if self.responseType == "START_MONITORING":
            print(self.header.split('\n')[2])

        if self.responseType == "PULL_AUDIO":
            state = self.header.split('\n')[2]
            recordingError = self.header.split('\n')[3]
            if state == "State: Ok":
                print("ok")
                # Chama protocolo de transferencia de arquivo
            elif state == "State: Error":
                print(f"Não foi possível gravar/obter audio: {recordingError}")

    def sendMessage(self):
        interface = CommandInterface()
        interface.cmdloop()
        self.request = interface.msg
        self.transport.write(bytes(self.request, 'utf-8'))
    
    def connectionMade(self):
        print("Bem-vindo! Digite 'help' ou '?' para listar os comandos.\n")

    def dataReceived(self, response):
        self.parseResponse(response)
        self.actOnResponse()
        self.sendMessage()

    def connectionLost(self, reason): 
        print("Conexão encerrada")
        reactor.stop()

# class ServiceDataTransporter(Protocol):
#     def connectionMade(self):
#         return

#     def dataReceived(self, response):
#         return

class ServiceControllerFactory(ServerFactory):
    def buildProtocol(self, addr):
        return ServiceControllerProtocol()
    
# class ServiceTransporterFactory(ServerFactory):
#     def buildProtocol(self, addr):
#         return ServiceDataTransporter()

if __name__ == '__main__':
    controllerEndpoint = TCP4ServerEndpoint(reactor, 2000)
    # transferEndpoint = TCP4ServerEndpoint(reactor, 2001)
    controllerEndpoint.listen(ServiceControllerFactory())
    # transferEndpoint.listen(ServiceTransporterFactory())
    reactor.run()