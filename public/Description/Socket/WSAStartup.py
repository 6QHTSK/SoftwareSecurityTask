from utils import to32Hex, convertResult


def join(primary, secondary):
    return "{}.{}".format(primary, secondary)


def description(event, socketList: dict):
    info = event["event"]
    event["eventDescription"] = {
        "wVersionRequired": {
            "description": "请求的套接字版本号",
            "value": join(info["wVersionRequiredPrimary"], info["wVersionRequiredSecondary"])
        },
        "wVersion": {
            "description": "返回的套接字版本号",
            "value": join(info["wVersionPrimary"], info["wVersionSecondary"])
        },
        "lpWSAData": {
            "description": "套接字信息结构地址",
            "value": to32Hex(info["lpWSAData"])
        },
        "return": {
            "description": "执行结果",
            "value": convertResult(info["status"], reverse=True)
        }
    }
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup"
    event["description"] = "初始化套接字"
