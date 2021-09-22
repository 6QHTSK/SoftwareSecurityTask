from utils import convertEqual, convertDict, convertOr, convertResult, to32Hex, toMemorySize


def description(event):
    info = event["event"]
    event["eventDescription"] = {
        "hKey": {
            "description": "键值句柄",
            "value": convertEqual(info["hKey"], convertDict["RegeditHkey"])
        },
        "lpValueName": {
            "description": "键名",
            "value": info["lpValueName"]
        },
        "Reserved": {
            "description" : "备用（NULL）",
            "value": to32Hex(info["Reserved"],"NULL")
        },
        "dwType":{
            "description": "键值类型",
            "value": convertEqual(info["dwType"], convertDict["RegeditType"])
        },
        "lpData":{
            "description": "键值内容",
            "value":to32Hex(info["lpData"])
        },
        "cbData":{
            "description": "内容长度",
            "value": toMemorySize(info["cbData"])
        },
        "return":{
            "description": "执行结果",
            "value": convertResult(info["status"], True)
        }
    }
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regsetvalueexa"
    event["description"] = "{}地写注册表句柄{}下{}键值".format(
        event["eventDescription"]["return"]["value"],
        event["eventDescription"]["hKey"]["value"][-1],
        event["eventDescription"]["lpValueName"]["value"],
    )
