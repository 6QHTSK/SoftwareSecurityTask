import pipeThread
from concurrent.futures import ThreadPoolExecutor
from flask import Flask, request,render_template
from flask_socketio import SocketIO, emit
import subprocess
import ctypes
import sys
import os
from flask_cors import CORS, cross_origin

app = Flask(__name__, static_url_path='/', static_folder='..\\src\\web\\dist\\spa',template_folder='..\\src\\web\\dist\\spa')
CORS(app)
socketio = SocketIO(app, cors_allowed_origins='*',async_mode="threading")
executor = ThreadPoolExecutor(2)
targetProcess = None
targetThread = None


def stopTarget():
    global targetProcess
    if targetProcess is not None:
        if pipeThread.getPid() != 0:
            os.popen('taskkill.exe /pid:' + str(pipeThread.getPid()))
        targetProcess.terminate()


@app.route('/')
def index():
    return render_template("index.html")


@app.route('/trace/<command>')
def Trace(command):
    global targetProcess, targetThread, socketio
    path = request.args.get(
        'path', '')
    if command == "start":
        stopTarget()
        if os.path.exists(path):
            targetProcess = subprocess.Popen(["H:\\softwareSecurityTask\\src\\Injector\\Debug\\Injector.exe",
                                              path], shell=True)
            executor.submit(pipeThread.pipeThread, socketio)
            return {"result": True}
        else:
            return {"result": False}
    elif command == "stop":
        stopTarget()
        return {"result": True}


@socketio.on('connect')
def test_message(message):
    emit('ConnectInfo', {'data': "Hello SocketIO"})


# python 3 Only!
def is_admin():
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except:
        return False


WebStart = False

if __name__ == '__main__':
    if is_admin():
        socketio.run(app, debug=True, host='0.0.0.0', port=9013)
    else:
        ctypes.windll.shell32.ShellExecuteW(
            None, "runas", sys.executable, __file__, None, 1)
