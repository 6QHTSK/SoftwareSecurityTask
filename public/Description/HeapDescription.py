from Description.Heap.HeapAlloc import description as heapAlloc
from Description.Heap.HeapCreate import description as heapCreate
from Description.Heap.HeapDestroy import description as heapDestroy
from Description.Heap.HeapFree import description as heapFree


def heapDescription(event):
    info = event["event"]
    operation = info["Operation"]
    if operation == "HeapAlloc":
        heapAlloc(event)
    elif operation == "HeapCreate":
        heapCreate(event)
    elif operation == "HeapDestroy":
        heapDestroy(event)
    elif operation == "HeapFree":
        heapFree(event)
