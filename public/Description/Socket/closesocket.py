from utils import to32Hex, convertResult, formatErrorCode


def join(primary, secondary):
    return "{}.{}".format(primary, secondary)


def description(event, socketList: dict):
    info = event["event"]
    event["eventDescription"] = {
        "s": {
            "description": "连接的套接字",
            "value": [to32Hex(info["s"]), socketList.get(info["s"], "[UnKnown]")]
        },
        "return": {
            "description": "执行结果",
            "value": convertResult(info["status"], reverse=True)
        }
    }
    if "errorCode" in info:
        event["eventDescription"].update({
            "errorCode": {
                "description": "错误代码",
                "value": formatErrorCode(info["errorCode"])
            }
        })
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-closesocket"
    event["description"] = "关闭套接字 {}".format(event["eventDescription"]["s"]["value"][-1])
