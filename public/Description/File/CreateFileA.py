from utils import convertEqual, convertOr,convertDict, to32Hex
from os.path import basename

def description(event):
    info = event["event"]
    event["eventDescription"] = {
        "lpFileName":{
            "description": "创建或打开的文件或设备的名称",
            "value": info["lpFileName"]
        },
        "dwDesiredAccess":{
            "description": "申请访问权限",
            "value": convertOr(info["dwDesiredAccess"], convertDict["FileAccessMask"])
        },
        "dwShareMode":{
            "description": "共享模式",
            "value": convertOr(info["dwShareMode"], convertDict["FileShareMode"])
        },
        "lpSecurityAttributes":{
            "description": "安全参数结构指针",
            "value": to32Hex(info["lpSecurityAttributes"])
        },
        "dwCreationDisposition":{
            "description": "文件打开方式",
            "value": convertEqual(info["dwCreationDisposition"], convertDict["FileCreationDisposition"])
        },
        "dwFlagsAndAttributes":{
            "description": "设置文件参数",
            "value": convertOr(info["dwFlagsAndAttributes"], convertDict["FileFlagsAndAttributes"])
        },
        "return":{
            "description": "文件句柄",
            "value": to32Hex(info["handle"],"NULL")
        }
    }
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea"
    event["description"] = "打开了文件{}".format(
        basename(event["eventDescription"]["lpFileName"]["value"]))
