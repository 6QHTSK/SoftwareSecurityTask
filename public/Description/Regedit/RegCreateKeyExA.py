from utils import convertEqual, convertOr, convertDict, to32Hex, formatErrorCode


def description(event):
    info = event["event"]
    event["eventDescription"] = {
        "hKey": {
            "description": "主键名称",
            "value": convertEqual(info["hKey"], convertDict["RegeditHkey"])
        },
        "lpSubKey": {
            "description": "子键名称",
            "value": info["lpSubKey"],
        },
        "Reserved": {
            "description": "备用（为0）",
            "value": to32Hex(info["Reserved"], "NULL")
        },
        "lpClass": {
            "description": "用户定义类类型",
            "value": info["lpClass"]
        },
        "dwOptions": {
            "description": "创建参数",
            "value": convertOr(info["dwOptions"], convertDict["RegeditOptions"])
        },
        "samDesired": {
            "description": "申请访问权限",
            "value": convertOr(info["samDesired"], convertDict["RegeditKeyAccess"])
        },
        "lpSecurityAttributes": {
            "description": "安全参数结构指针",
            "value": to32Hex(info["lpSecurityAttributes"], "NULL")
        },
        "phkResult": {
            "description": "返回操作句柄",
            "value": to32Hex(info["phkResult"])
        },
        "lpdwDisposition": {
            "description": "处置信息",
            "value": convertEqual(info["lpdwDisposition"], convertDict["RegeditDisposition"])
        },
        "return": {
            "description": "执行结果 [系统错误代码]",
            "value": formatErrorCode(info["status"])
        }
    }
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regcreatekeyexa"
    event["description"] = "创建/打开了注册表键值"
