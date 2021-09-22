from utils import convertResult, to32Hex
from os.path import basename
def description(event):
  info = event["event"]
  event["eventDescription"] = {
    "hObject":{
      "description":"要关闭的文件句柄",
      "value":[to32Hex(info["hFile"]),info["path"]]
    },
    "return":{
      "description":"执行结果",
      "value":convertResult(info["return"])
    }
  }
  event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle"
  event["description"] = "关闭文件{}".format(basename(info["path"]))
