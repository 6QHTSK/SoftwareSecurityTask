from utils import WarningEmit
from Description.RegeditDescription import regEditDescription


class RegHandler:
    def __init__(self):
        self.name = "Regedit"

    def event_handler(self, event):
        if event["eventID"] != self.name:
            WarningEmit(event, "Danger", 1, "错误的Handler")
            return

        regEditDescription(event)
        info = event["event"]

        if info["status"] != 0:
            WarningEmit(event, "Warning", 311, "未成功修改注册表项")

        if info.get("lpSubKey", "").lower() == "SOFTWARE\Microsoft\Windows\CurrentVersion\Run".lower():
            WarningEmit(event, "Danger", 301, "试图修改启动项")

    def fin_event_handler(self, event):
        pass
