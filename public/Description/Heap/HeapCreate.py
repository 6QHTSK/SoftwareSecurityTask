from utils import convertOr, toMemorySize, to32Hex, convertDict, formatErrorCode


def description(event):
    info = event["event"]
    event["eventDescription"] = {
        "fIOptions": {
            "descrioption": "创建选项",
            "value": convertOr(info["fIOptions"], convertDict["HeapOptions"]),
        },
        "dwInitialSize": {
            "description": "堆的初始大小",
            "value": toMemorySize(info["dwInitialSize"], "宿主机内存一页分页"),
        },
        "dwMaximumSize": {
            "description": "堆的最大大小",
            "value": toMemorySize(info["dwMaximumSize"], "无限制")
        },
        "return": {
            "description": "创建堆的句柄",
            "value": to32Hex(info["hHeap"], '创建堆失败')
        }
    }
    if "errorCode" in info:
        event["eventDescription"].update({
            "errorCode": {
                "description": "错误代码",
                "value": formatErrorCode(info["errorCode"])
            }
        })
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/heapapi/nf-heapapi-heapcreate"
    event["description"] = "创建了一个初始大小为{}，最大大小为{}的堆[{}]。".format(
        event["eventDescription"]["dwInitialSize"]["value"],
        event["eventDescription"]["dwMaximumSize"]["value"],
        event["eventDescription"]["return"]["value"])
