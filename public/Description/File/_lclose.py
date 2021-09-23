from utils import to32Hex


def description(event):
    info = event["event"]
    event["eventDescription"] = {
        "hFile": {
            "description": "操作句柄",
            "value": to32Hex(info["hHeap"], "NULL")
        },
        "return": {
            "description": "文件关闭句柄",
            "value": to32Hex(info["return"], "NULL")
        }
    }
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-_lclose"
    event["description"] = "[过时的函数] 关闭文件句柄"
