from utils import WarningEmit
from Description.SocketDescription import SocketDescription


class SocketHandler:
    def __init__(self):
        self.name = "Socket"
        self.socketDict = {}

    def event_handler(self, event):
        if event["eventID"] != self.name:
            WarningEmit(event, "Danger", 1, "错误的Handler")
            return
        SocketDescription(event,self.socketDict)

    def fin_event_handler(self, event):
        pass
