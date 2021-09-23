from utils import to32Hex, formatErrorCode


def description(event):
    info = event["event"]
    event["eventDescription"] = {
        "lpFileName": {
            "description": "文件名",
            "value": info["lpFileName"]
        },
        "lpReOpenBuff": {
            "description": "参数结构体指针",
            "value": to32Hex(info["lpReOpenBuff"])
        },
        "uStyle": {
            "description": "操作代码",
            "value": to32Hex(info["uStyle"])
        },
        "return": {
            "description": "操作句柄",
            "value": to32Hex(info["hFile"])
        }
    }
    if "errorCode" in info:
        event["eventDescription"].update({
            "errorCode": {
                "description": "错误代码",
                "value": formatErrorCode(info["errorCode"])
            }
        })
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-openfile"
    event["description"] = "[过时的函数] 打开了{}文件".format(
        event["eventDescription"]["lpFileName"]["value"]
    )
