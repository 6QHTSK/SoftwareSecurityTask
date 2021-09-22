from utils import WarningEmit
from Description.MessageBoxDescription import description


class MessageBoxHandler:
    def __init__(self):
        self.name = "MessageBox"

    def event_handler(self, event):
        if event["eventID"] != self.name:
            WarningEmit(event, "Danger", 1, "错误的Handler")
            return
        description(event)

    def fin_event_handler(self, event):
        pass
