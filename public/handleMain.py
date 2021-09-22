
from Handlers.MessageBoxHandler import MessageBoxHandler
from Handlers.RegHandler import RegHandler
from Handlers.FileHandler import FileHandler
from Handlers.HeapHandler import HeapHandler


class Handler:
    def __init__(self):
        self.hanler_name = "undefined"
        self.messageBox_handler = MessageBoxHandler()
        self.heap_handler = HeapHandler()
        self.file_handler = FileHandler()
        self.reg_handler = RegHandler()

    def event_handle(self,event):
        if event["eventID"] == "MessageBox":
            self.messageBox_handler.event_handler(event)
        if event["eventID"] == "Heap":
            self.heap_handler.event_handler(event)
        elif event["eventID"] == "File":
            self.file_handler.event_handler(event) 
        elif event["eventID"] == "Regedit":
            self.reg_handler.event_handler(event)
        elif event["eventID"] == "HookFunc":
            if event['event']['Operation'] == 'StartTrace':
                event['description'] = "开始追踪目标程序行为"
            elif event["event"]["Operation"] == "StopTrace":
                event['description'] = "结束追踪目标程序行为"
                self.fin_event_handle(event)
        
    def fin_event_handle(self,event):
        self.messageBox_handler.fin_event_handler(event)
        self.heap_handler.fin_event_handler(event)
        self.file_handler.fin_event_handler(event)
        self.reg_handler.fin_event_handler(event)
