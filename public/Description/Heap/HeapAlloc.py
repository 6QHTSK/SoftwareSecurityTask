from utils import convertOr, toMemorySize, to32Hex, convertDict


def description(event):
  info = event["event"]
  event["eventDescription"] = {
        "hHeap": {
            "description": "操作堆的句柄",
            "value": to32Hex(info["hHeap"], "NULL")
        },
        "dwFlags": {
            "description": "申请选项",
            "value": convertOr(info["dwFlags"], convertDict["HeapOptions"])
        },
        "dwBytes": {
            "description": "要分配的字节数",
            "value": toMemorySize(info["dwBytes"])
        },
        "return": {
            "description": "分配内存块的指针",
            "value": to32Hex(info["address"], "分配失败")
        }
  }
  event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/heapapi/nf-heapapi-heapalloc"
  event["description"] = "在堆[{}]上申请了{}大小的内存区域".format(
      event["eventDescription"]["hHeap"]["value"],
      event["eventDescription"]["dwBytes"]["value"]
  )
