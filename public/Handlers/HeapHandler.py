from Description.HeapDescription import heapDescription
from utils import WarningEmit, to32Hex


class HeapHandler:
    def __init__(self):
        self.name = "Heap"
        self.heap_map = {}
        self.heap_alloc_map = {}

    def event_handler(self, event):
        # 不处理不属于Heap Event的操作
        if event["eventID"] != self.name:
            WarningEmit(event, "Danger", 1, "错误的Handler")
            return

        info = event["event"]
        hHeap = info["hHeap"]
        heapDescription(event)

        # heapCreate
        if info["Operation"] == "HeapCreate":

            if hHeap != 0:
                self.heap_map[hHeap] = True  # 对Create的Heap开始追踪
                self.heap_alloc_map[hHeap] = {}
            else:
                # hHeap为0(NULL)，创建Heap失败
                WarningEmit(event, "Warning", 111, "未成功创建堆")

        elif info["Operation"] == "HeapAlloc":

            if self.heap_map.get(hHeap, False):
                if info["address"] != 0:
                    self.heap_alloc_map[hHeap][info["address"]] = True  # 对Alloc的内存开始追踪
                else:
                    # address为0(NULL)，申请内存失败
                    WarningEmit(event, "Warning", 112, "未成功申请到内存")
            else:
                # 对Destroy的hHeap操作
                WarningEmit(event, "Danger", 101, "在不存在的堆上申请内存")

        elif info["Operation"] == "HeapDestroy":

            if self.heap_map.get(hHeap, False):
                self.heap_map.pop(hHeap)  # 取消对该堆的追踪
                if len(self.heap_alloc_map[hHeap]) != 0:  # 检查该堆是否有未释放内存
                    unfree_address = []
                    for k in self.heap_alloc_map[hHeap]:
                        unfree_address.append(to32Hex(k))
                    self.heap_alloc_map[hHeap] = {}  # 手动清空
                    WarningEmit(event, "Warning", 113,
                                "摧毁堆时有内存未释放[无害]", unfree_address)
            else:
                WarningEmit(event, "Danger", 102,
                            "试图摧毁不存在的堆")  # 发现调用不存在Heap

        elif info["Operation"] == "HeapFree":
            if self.heap_map.get(hHeap, False):
                if self.heap_alloc_map[hHeap].get(info["lpMem"], False):
                    self.heap_alloc_map[hHeap].pop(info["lpMem"])  # 取消对该地址的追踪
                else:
                    WarningEmit(event, "Danger", 103,
                                "试图释放未分配的地址")  # 该地址已释放再次释放
            else:
                WarningEmit(event, "Danger", 104, "在不存在得堆上释放地址")  # 发现调用不存在Heap

        else:
            WarningEmit(event, "Danger", 2, "无可用Operation")  # 不太可能到这里

        return {}

    def fin_event_handler(self, event):
        if len(self.heap_map) != 0:
            unfree_heap = []
            for k in self.heap_map:
                unfree_heap.append(to32Hex(k))
            WarningEmit(event, "Warning", 114, "应用结束时有堆未被摧毁", unfree_heap)
        return {}
