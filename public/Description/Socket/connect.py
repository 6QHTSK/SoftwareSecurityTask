from utils import convertDict, convertResult, convertEqual, to32Hex, joinSocketAddr, formatErrorCode,toMemorySize


def description(event, socketList):
    info = event["event"]
    event["eventDescription"] = {
        "s": {
            "description": "连接的套接字",
            "value": to32Hex(info["s"])
        },
        "name": {
            "description": "套接字连接信息指针",
            "value": to32Hex(info["sockaddr"])
        },
        "namelen": {
            "description": "套接字连接信息长度",
            "value": toMemorySize(info["namelen"])
        }
    }

    event["description"] = "套接字连接 [无法解析地址]"

    if "sa_family" in info:

        event["eventDescription"].update(
            {
                "sa_family": {
                    "description": "连接地址族规范",
                    "value": convertEqual(info["sa_family"], convertDict["SocketAf"])
                }
            }
        )
        event["eventDescription"]["name"]["buffer"] = info["nameValue"]

        if "sin_addr" in info:

            event["eventDescription"]["sin_addr"] = {
                "description": "连接目标",
                "value": joinSocketAddr(info["sin_addr"], info["sin_port"])
            }

            event["description"] = "套接字连接 {}".format(event["eventDescription"]["sin_addr"]["value"])
            socketList[info["s"]] = event["eventDescription"]["sin_addr"]["value"]

    event["eventDescription"].update(
        {
            "return": {
                "description": "执行结果",
                "value": convertResult(info["status"], reverse=True)
            }
        }
    )

    if "errorCode" in info:
        event["eventDescription"].update({
            "errorCode": {
                "description": "错误代码",
                "value": formatErrorCode(info["errorCode"])
            }
        })
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-connect"
