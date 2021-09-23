from utils import to32Hex, toMemorySize, convertResult, formatErrorCode
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
        "nNumberOfBytesToWrite": {
            "description": "最大写字节数",
            "value": toMemorySize(info["nNumberOfBytesToWrite"])
        },
        "lpNumberOfBytesWritten": {
            "description": "写字节数的指针",
            "value": to32Hex(info["lpNumberOfBytesWritten"], "NULL")
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
    if info["lpNumberOfBytesWritten"] != 0:
        event["eventDescription"]["lpNumberOfBytesWritten"] = {
            "description": "写字节数",
            "value": toMemorySize(info["lpNumberOfBytesWrittenValue"])
        }
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile"
    event["description"] = "写文件{}内容".format(basename(info["path"]))
