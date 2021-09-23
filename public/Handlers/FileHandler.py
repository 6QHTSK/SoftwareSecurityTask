import os

from Description.FileDescription import FileDescription
from utils import WarningEmit, to32Hex

Sensitive_suffix = (".exe", ".sys", ".com", ".bat", ".dll", ".vbs", ".ocx")


def to_abs_path(cwd, path):
    if os.path.isabs(path):
        return path
    else:
        return os.path.join(cwd, path)


class FileHandler:
    def __init__(self):
        self.name = "File"
        self.fileHandleMap = {}

    def event_handler(self, event):

        # 不处理不属于Heap Event的操作
        if event["eventID"] != self.name:
            WarningEmit("Danger", 1, "错误的Handler")
            return

        FileDescription(event)
        info = event["event"]
        operationPath = ""

        if info["Operation"] == "CreateFileA":
            if info["handle"] == -1:
                WarningEmit(event, "Warning", 211, "未成功创建文件")
            operationPath = info["path"]
            self.fileHandleMap[info["handle"]] = operationPath

        elif info["Operation"] == "OpenFile":
            if info["hFile"] == -1:
                WarningEmit(event, "Warning", 211, "未成功创建文件")
            WarningEmit(event, "Warning", 212, "使用过时的Win32API")
            operationPath = to_abs_path(
                event["cwd"], info["lpFileName"])
            self.fileHandleMap[info["hFile"]] = operationPath

        elif info["Operation"] == "_lclose":
            if not info["return"]:
                WarningEmit(event, "Warning", 213, "未成功关闭文件")
            WarningEmit(event, "Warning", 212, "使用过时的Win32API")
            operationPath = self.fileHandleMap.pop(info["hFile"], "")

        elif info["Operation"] == "CloseHandle":
            if info["return"] == 0:
                WarningEmit(event, "Warning", 213, "未成功关闭文件")
            self.fileHandleMap.pop(info["hFile"], "")
            operationPath = info["path"]

        else:
            if info["return"] == 0:
                WarningEmit(event, "Warning", 214, "未成功执行指令")
            operationPath = info["path"]

        if operationPath != "":
            if os.path.samefile(operationPath, event["file"]):
                WarningEmit(event, "Danger", 201, "使用自身作为目标文件")

            dirname = os.path.dirname(operationPath)
            for file in os.listdir(dirname):
                if os.path.isdir(os.path.join(dirname, file)):
                    WarningEmit(event, "Warning", 215, "目标文件所在目录存在目录")
                    break

            if operationPath.endswith(Sensitive_suffix):
                WarningEmit(event, "Danger", 202, "目标文件是敏感后缀文件")
        else:
            WarningEmit(event, "Warning", 216, "未追踪的文件句柄")

            return {}

    def fin_event_handler(self, event):
        if len(self.fileHandleMap) > 0:
            unfree_handler = []
            for k in self.fileHandleMap:
                unfree_handler.append(to32Hex(k))
            WarningEmit(event, "Warning", 217, "未释放所有的文件句柄", unfree_handler)
        return {}
