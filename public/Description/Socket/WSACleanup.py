from utils import to32Hex, convertResult


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
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsacleanup"
    event["description"] = "清理套接字"
