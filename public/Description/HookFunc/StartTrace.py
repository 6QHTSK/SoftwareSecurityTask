from utils import toMemorySize
import time


def convertTime(timeStamp):
    timeArray = time.localtime(timeStamp)
    return time.strftime("%Y/%m/%d %H:%M:%S", timeArray)


def description(event):
    info = event["event"]
    event["eventDescription"] = {
        "cwd": {
            "description": "工作目录",
            "value": event["cwd"]
        },
        "file": {
            "description": "当前文件绝对路径",
            "value": event["file"]
        },

    }
    if "st_size" in info:
        event["eventDescription"].update({
            "st_size": {
                "description": "当前文件大小",
                "value": toMemorySize(info["st_size"])
            },
            "st_atime": {
                "description": "上次访问时间",
                "value": convertTime(info["st_atime"])
            },
            "st_mtime": {
                "description": "上次修改时间",
                "value": convertTime(info["st_mtime"])
            },
            "st_ctime": {
                "description": "创建时间",
                "value": convertTime(info["st_ctime"])
            }
        })
    event['description'] = "开始追踪目标程序行为"
