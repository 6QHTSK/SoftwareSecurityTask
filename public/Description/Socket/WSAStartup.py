from utils import to32Hex, convertResult, formatErrorCode


def join(primary, secondary):
    return "{}.{}".format(primary, secondary)


def description(event, socketList: dict):
    info = event["event"]
    event["eventDescription"] = {
        "wVersionRequired": {
            "description": "请求的套接字版本号",
            "value": [to32Hex(info["wVersionRequired"]), join(info["wVersionRequiredPrimary"],
                                                              info["wVersionRequiredSecondary"])]
        },
        "wVersion": {
            "description": "返回的套接字版本号",
            "value": "[未返回]"
        },
        "lpWSAData": {
            "description": "套接字信息结构地址",
            "value": to32Hex(info["lpWSAData"], "NULL")
        },
        "return": {
            "description": "执行结果",
            "value": formatErrorCode(info["status"])
        }
    }
    if info["lpWSAData"] != 0 and info["status"] != 10014:
        event["eventDescription"]["wVersion"]["value"] = [to32Hex(info["wVersion"]), join(info["wVersionPrimary"],
                                                                                          info["wVersionSecondary"])]
        event["eventDescription"]["lpWSAData"]["buffer"] = info["lpWSADataValue"]
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup"
    event["description"] = "初始化套接字"
