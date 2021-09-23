from utils import to32Hex, convertResult, formatErrorCode


def join(primary, secondary):
    return "{}.{}".format(primary, secondary)


def description(event, socketList: dict):
    info = event["event"]
    event["eventDescription"] = {
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
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsacleanup"
    event["description"] = "清理套接字"
