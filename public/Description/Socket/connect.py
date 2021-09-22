from utils import convertDict, convertResult, convertEqual, to32Hex, joinSocketAddr


def description(event, socketList):
    info = event["event"]
    event["eventDescription"] = {
        "s": {
            "description": "连接的套接字",
            "value": to32Hex(info["s"])
        },
        "sockaddr": {
            "description": "套接字连接信息",
            "value": to32Hex(info["sockaddr"])
        },
        "sa_family": {
            "description": "连接地址族规范",
            "value": convertEqual(info["sa_family"], convertDict["SocketAf"])
        }
    }
    if "sin_addr" in info:
        event["eventDescription"]["sin_addr"] = {
            "description": "连接目标",
            "value": joinSocketAddr(info["sin_addr"], info["sin_port"])
        }
        event["description"] = "套接字连接 {}".format(event["eventDescription"]["sin_addr"]["value"])
        socketList[info["s"]] = event["eventDescription"]["sin_addr"]["value"]
    else:
        event["description"] = "套接字连接 [无法解析地址]"
    event["eventDescription"]["return"] = {
        "description": "执行结果",
        "value": convertResult(info["status"], reverse=True)
    }
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-connect"
