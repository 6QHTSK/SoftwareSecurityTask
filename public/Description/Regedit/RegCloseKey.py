from utils import convertEqual, convertDict, formatErrorCode


def description(event):
    info = event["event"]
    event["eventDescription"] = {
        "hKey": {
            "description": "操作键句柄",
            "value": convertEqual(info["hKey"], convertDict["RegeditHkey"])
        },
        "return": {
            "description": "执行结果 [系统错误代码]",
            "value": formatErrorCode(info["status"])
        }
    }
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regclosekey"
    event["description"] = "关闭了注册表句柄"
