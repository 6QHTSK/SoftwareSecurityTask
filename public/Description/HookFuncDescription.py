from Description.HookFunc.StartTrace import description as StartTrace
from Description.HookFunc.StopTrace import description as StopTrace


def HookFuncDescription(event):
    operation = event["event"]["Operation"]
    if operation == "StartTrace":
        StartTrace(event)
    elif operation == "StopTrace":
        StopTrace(event)
