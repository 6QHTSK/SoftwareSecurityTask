from Description.Socket.WSACleanup import description as WSACleanup
from Description.Socket.WSAStartup import description as WSAStartup
from Description.Socket.closesocket import description as closesocket
from Description.Socket.connect import description as connect
from Description.Socket.recv import description as recv
from Description.Socket.send import description as send
from Description.Socket.socket import description as socket


def SocketDescription(event,socketDict):
    operation = event["event"]["Operation"]
    if operation == "socket":
        socket(event, socketDict)
    elif operation == "WSAStartup":
        WSAStartup(event, socketDict)
    elif operation == "connect":
        connect(event, socketDict)
    elif operation == "recv":
        recv(event, socketDict)
    elif operation == "send":
        send(event, socketDict)
    elif operation == "closesocket":
        closesocket(event, socketDict)
    elif operation == "WSACleanup":
        WSACleanup(event, socketDict)
