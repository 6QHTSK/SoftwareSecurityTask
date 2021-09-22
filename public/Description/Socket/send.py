from utils import convertResult, toMemorySize, to32Hex


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
            "description": "发送字节数",
            "value": checkReturn(info["status"])
        }
    }
    event["description"] = "套接字发送信息到 {}".format(event["eventDescription"]["s"]["value"][-1])
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-send"
