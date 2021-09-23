// Target5.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<Windows.h>
#include<time.h>




void main()
{
	HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, 100, 100);
	HANDLE hHeap2 = HeapCreate(HEAP_NO_SERIALIZE, 100, 100);	// WARNING 114
	DWORD flags = 0;
	if (hHeap != NULL) {
		auto hptest = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 100);
		auto hptest2 = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 100);
		printf("%p\n", hptest);
		HeapFree(hHeap, 0, hptest);
		printf("%d\n", GetHandleInformation(hHeap, &flags));
		printf("%d\n", GetLastError());
		printf("%d\n", HeapValidate(hHeap, 0, NULL));
		HeapFree(hHeap, 0, hptest);	// ERROR 103
		HeapDestroy(hHeap);	// WARNING 113
		//printf("%d\n", HeapValidate(hHeap, 0, NULL));
		//auto hptest3 = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 100); // ERROR 101
		//HeapFree(hHeap, 0, hptest2);	// ERROR 104
		//HeapDestroy(hHeap);	// WARNING 102
	}
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
