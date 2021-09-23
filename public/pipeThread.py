from handleMain import Handler
from Handlers.RegHandler import RegHandler
from Handlers.HeapHandler import HeapHandler
from Handlers.FileHandler import FileHandler
import win32file
import win32pipe
import json

pid = 0
eventUID = 1


def getPid():
    global pid
    return pid


def pipeThread(sio):
    global pid, eventUID
    pid = 0
    PIPE_NAME = r'\\.\pipe\mypipe'
    PIPE_BUFFER_SIZE = 65535
    named_pipe = win32pipe.CreateNamedPipe(PIPE_NAME,
                                           win32pipe.PIPE_ACCESS_DUPLEX,
                                           win32pipe.PIPE_TYPE_MESSAGE | win32pipe.PIPE_WAIT | win32pipe.PIPE_READMODE_MESSAGE,
                                           win32pipe.PIPE_UNLIMITED_INSTANCES,
                                           PIPE_BUFFER_SIZE,
                                           PIPE_BUFFER_SIZE, 500, None)
    event_handler = Handler()
    eventUID = 1
    try:
        while True:
            rawjson = dict()
            try:
                win32pipe.ConnectNamedPipe(named_pipe, None)
                data = win32file.ReadFile(
                    named_pipe, PIPE_BUFFER_SIZE, None)

                if data is None or len(data) < 2:
                    continue

                # print('receive msg:', data)
                # print(str(data[1]));

                rawstr = data[1].decode()
                rawstr = rawstr.replace("\x00", "")
                # rawstr = rawstr.replace("\\\\","\\")
                rawjson = json.loads(rawstr)
                if pid == 0:
                    pid = rawjson["pid"]
                # print(rawjson)
                rawjson['warnings'] = []
                rawjson['warningLevel'] = 0
                rawjson['description'] = ""
                rawjson['eventDescription'] = {}
                rawjson['id'] = eventUID
                eventUID = eventUID + 1
                event_handler.event_handle(rawjson)
                sio.emit('DetourEvent', {'data': rawjson})

            except BaseException as e:
                print("exception:", e)
                event_handler.fin_event_handle(rawjson)
                break
    finally:
        try:
            print("断开了管道连接")
            win32pipe.DisconnectNamedPipe(named_pipe)
        except:
            pass
