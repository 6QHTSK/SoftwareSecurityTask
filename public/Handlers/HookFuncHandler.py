from utils import WarningEmit
from Description.HookFuncDescription import HookFuncDescription


class HookFuncHandler:
    def __init__(self):
        self.name = "HookFunc"

    def event_handler(self, event):
        if event["eventID"] != self.name:
            WarningEmit(event, "Danger", 1, "错误的Handler")
            return
        HookFuncDescription(event)

    def fin_event_handler(self, event):
        pass
