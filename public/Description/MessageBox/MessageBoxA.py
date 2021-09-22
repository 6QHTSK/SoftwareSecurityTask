from utils import convertEqual, to32Hex, convertDict, convertOr


def description(event):
    info = event["event"]
    event["eventDescription"] = {
        "hWnd": {
            "description": "父窗口句柄",
            "value": to32Hex(info['hWnd'], "无父窗口")
        },
        "lpText": {
            "description": "弹窗标题",
            "value": info['lpText']
        },
        "lpCaption": {
            "description": "弹窗内容",
            "value": info['lpCaption']
        },
        "uType": {
            "description": "弹窗样式选项（部分解析）",
            "value": convertOr(info['uType'], convertDict['MessageBoxType'])
        },
        "return": {
            "description": "按下的按钮",
            "value": convertEqual(info['return'], convertDict['MessageBoxReturn'])
        }
    }
    event["document"] = "https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messageboxa"
    event["description"] = "弹了一个窗"
