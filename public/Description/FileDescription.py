from Description.File.CreateFileA import description as createFile
from Description.File.OpenFile import description as openFile
from Description.File.ReadFile import description as readFile
from Description.File.WriteFile import description as writeFile
from Description.File._lclose import description as _lclose
from Description.File.CloseHandle import description as closeHandle


def FileDescription(event):
    info = event["event"]
    operation = info["Operation"]
    print(operation)
    if operation == "CreateFileA":
        createFile(event)
    elif operation == "OpenFile":
        openFile(event)
    elif operation == "ReadFile":
        readFile(event)
    elif operation == "WriteFile":
        writeFile(event)
    elif operation == "_lclose":
        _lclose(event)
    elif operation == "CloseHandle":
        closeHandle(event)
