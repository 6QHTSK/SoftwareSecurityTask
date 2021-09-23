from utils import convertDict, convertEqual, formatErrorCode


def description(event, socketList: dict):
    info = event["event"]
    event["eventDescription"] = {
        "af": {
            "description": "地址族规范",
            "value": convertEqual(info["af"], convertDict["SocketAf"])
        },
        "type": {
            "description": "套接字的类型规范",
            "value": convertEqual(info["type"], convertDict["SocketType"])
        },
        "protocol": {
            "description": "要使用的协议",
            "value": convertEqual(info["protocol"], convertDict["SocketProtocol"])
        },
        "return": {
            "description": "socket描述符",
            "value": convertEqual(info["socket"], convertDict["SocketInfo"])
        }
    }
    if "errorCode" in info:
        event["eventDescription"].update({
            "errorCode": {
                "description": "错误代码",
                "value": formatErrorCode(info["errorCode"])
            }
        })
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket"
    event["description"] = "新建了一个套接字"
