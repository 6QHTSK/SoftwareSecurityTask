from utils import convertResult, to32Hex


def description(event):
  info = event["event"]
  event["eventDescription"] = {
      "hHeap": {
          "description": "要摧毁的堆的句柄",
          "value": to32Hex(info["hHeap"], "NULL")
      },
      "return": {
          "description": "摧毁结果",
          "value": convertResult(info["return"])
      }
  }
  event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/heapapi/nf-heapapi-heapdestroy"
  event["description"] = "{}地摧毁了堆[{}]".format(
      event["eventDescription"]["return"]["value"],
      event["eventDescription"]["hHeap"]["value"]
  )
