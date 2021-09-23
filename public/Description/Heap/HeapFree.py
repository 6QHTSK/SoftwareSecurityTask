from utils import convertResult, to32Hex, convertOr, convertDict, formatErrorCode


def description(event):
    info = event["event"]
    event["eventDescription"] = {
        "hHeap": {
            "description": "操作堆的句柄",
            "value": to32Hex(info["hHeap"], "NULL")
        },
        "dwFlags": {
            "description": "释放选项",
            "value": convertOr(info["dwFlags"], convertDict["HeapOptions"])
        },
        "lpMem": {
            "description": "释放地址",
            "value": to32Hex(info["lpMem"], "NULL")
        },
        "return": {
            "description": "释放结果",
            "value": convertResult(info["return"])
        }
    }
    if "errorCode" in info:
        event["eventDescription"].update({
            "errorCode": {
                "description": "错误代码",
                "value": formatErrorCode(info["errorCode"])
            }
        })
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/heapapi/nf-heapapi-heapfree"
    event["description"] = "{}地释放了{}处内存".format(
        event["eventDescription"]["return"]["value"],
        event["eventDescription"]["lpMem"]["value"]
    )
