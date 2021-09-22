from utils import convertOr, toMemorySize, to32Hex, convertDict


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
      "dwMaxiumSize": {
          "description": "堆的最大大小",
          "value": toMemorySize(info["dwMaxiumSize"], "无限制")
      },
      "return": {
          "description": "创建堆的句柄",
          "value": to32Hex(info["hHeap"], '创建堆失败')
      }
      # TODO: 返回失败原因
  }
  event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/heapapi/nf-heapapi-heapcreate"
  event["description"] = "创建了一个初始大小为{}，最大大小为{}的堆[{}]。".format(
      event["eventDescription"]["dwInitialSize"]["value"],
      event["eventDescription"]["dwMaxiumSize"]["value"],
      event["eventDescription"]["return"]["value"])
