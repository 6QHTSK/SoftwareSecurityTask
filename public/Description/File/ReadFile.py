from utils import convertResult, to32Hex, toMemorySize, formatErrorCode
from os.path import basename


def description(event):
    info = event["event"]
    event["eventDescription"] = {
        "hFile": {
            "description": "文件句柄",
            "value": [to32Hex(info["hFile"]), info["path"]]
        },
        "lpBuffer": {
            "description": "Buffer地址",
            "value": to32Hex(info["lpBuffer"]),
            "buffer": info.get("lpBufferValue", "")
        },
        "nNumberOfBytesToRead": {
            "description": "最大读取字节数",
            "value": toMemorySize(info["nNumberOfBytesToRead"])
        },
        "lpNumberOfBytesRead": {
            "description": "读取字节数的指针",
            "value": to32Hex(info["lpNumberOfBytesRead"], "NULL")
        },
        "lpOverlapped": {
            "description": "指向OVERLAPPED参数的指针",
            "value": to32Hex(info["lpOverlapped"], "NULL")
        },
        "return": {
            "description": "执行结果",
            "value": convertResult(info['return'])
        }
    }
    if "errorCode" in info:
        event["eventDescription"].update({
            "errorCode": {
                "description": "错误代码",
                "value": formatErrorCode(info["errorCode"])
            }
        })
    if info["lpNumberOfBytesRead"] != 0:
        event["eventDescription"]["lpNumberOfBytesRead"] = {
            "description": "读取字节数",
            "value": toMemorySize(info["lpNumberOfBytesReadValue"])
        }
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile"
    event["description"] = "读取文件{}内容".format(basename(info["path"]))
