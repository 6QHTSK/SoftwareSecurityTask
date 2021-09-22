from Handlers.MessageBoxHandler import MessageBoxHandler
from Handlers.RegHandler import RegHandler
from Handlers.FileHandler import FileHandler
from Handlers.HeapHandler import HeapHandler
from Handlers.SocketHandler import SocketHandler
from Handlers.HookFuncHandler import HookFuncHandler


class Handler:
    def __init__(self):
        self.handler_name = "undefined"
        self.messageBox_handler = MessageBoxHandler()
        self.heap_handler = HeapHandler()
        self.file_handler = FileHandler()
        self.reg_handler = RegHandler()
        self.socket_handler = SocketHandler()
        self.hook_func_handler = HookFuncHandler()

    def event_handle(self, event):
        if event["eventID"] == "MessageBox":
            self.messageBox_handler.event_handler(event)
        if event["eventID"] == "Heap":
            self.heap_handler.event_handler(event)
        elif event["eventID"] == "File":
            self.file_handler.event_handler(event)
        elif event["eventID"] == "Regedit":
            self.reg_handler.event_handler(event)
        elif event["eventID"] == "HookFunc":
            self.hook_func_handler.event_handler(event)
            if event["event"]["Operation"] == "StopTrace":
                self.fin_event_handle(event)
        elif event["eventID"] == "Socket":
            self.socket_handler.event_handler(event)

    def fin_event_handle(self, event):
        self.messageBox_handler.fin_event_handler(event)
        self.heap_handler.fin_event_handler(event)
        self.file_handler.fin_event_handler(event)
        self.reg_handler.fin_event_handler(event)
