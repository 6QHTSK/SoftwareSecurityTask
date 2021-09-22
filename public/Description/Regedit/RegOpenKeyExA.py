from utils import convertEqual,convertResult


from utils import convertEqual,convertDict,convertOr,to32Hex
def description(event):
    info = event["event"]
    event["eventDescription"] = {
        "hKey": {
            "description": "主键名称",
            "value": convertEqual(info["hKey"], convertDict["RegeditHkey"])
        },
        "ulOptions":{
            "description": "创建参数",
            "value": convertOr(info["ulOptions"], convertDict["RegeditOptions"])
        },
        "samDesired": {
            "description": "申请访问权限",
            "value": convertOr(info["samDesired"], convertDict["RegeditKeyAccess"])
        },
        "phkResult": {
            "description": "返回操作句柄",
            "value": to32Hex(info["phkResult"])
        },
        "return": {
            "description": "执行结果 [系统错误代码]",
            "value": convertResult(info["status"],True)
        }
    }
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regopenkeyexa"
    event["description"] = "{}地打开了[{}]注册表键值".format(
        event["eventDescription"]["return"]["value"],
        event["eventDescription"]["hKey"]["value"][-1]
    )
