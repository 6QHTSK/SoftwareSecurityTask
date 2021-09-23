from utils import convertResult, toMemorySize, to32Hex, formatErrorCode


def checkReturn(status):
    if status >= 0:
        return toMemorySize(status)
    else:
        return "[SOCKET_ERROR]"


def description(event, socketList):
    info = event["event"]
    event["eventDescription"] = {
        "s": {
            "description": "连接的套接字",
            "value": [to32Hex(info["s"]), socketList.get(info["s"], "[UnKnown]")]
        },
        "buf": {
            "description": "缓冲区地址",
            "value": to32Hex(info["buf"]),
            "buffer": info.get('bufValue', "")
        },
        "len": {
            "description": "缓冲区长度",
            "value": toMemorySize(info["len"])
        },
        "flags": {
            "description": "函数执行参数标志",
            "value": to32Hex(info["flags"])
        },
        "return": {
            "description": "接受字节数",
            "value": checkReturn(info["status"])
        }
    }
    if "errorCode" in info:
        event["eventDescription"].update({
            "errorCode": {
                "description": "错误代码",
                "value": formatErrorCode(info["errorCode"])
            }
        })
    event["description"] = "套接字接受来自 {} 的信息".format(event["eventDescription"]["s"]["value"][-1])
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-recv"
