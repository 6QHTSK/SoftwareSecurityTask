// Target5.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<Windows.h>
#include<time.h>

#include <iostream>
#include <cstdio>
#include <ctime>
#include <cmath>

template<class intType>
inline intType HexRead() {
    int c;
    intType result = 0;
    while ((c = getchar()) < 0x30);
    do {
        if (c >= 0x61) {
            result = result * 16 + c - 0x61 + 0x0a;
        }
        else {
            result = result * 16 + c - 0x30;
        }
    } while ((c = getchar()) >= 0x30);
    return result;
}

inline unsigned long DecRead() {
    int c;
    unsigned long result = 0;
    while ((c = getchar()) < 0x30);
    do {
        result = result * 10 + c - 0x30;
    } while ((c = getchar()) >= 0x30);
    return result;
}

const unsigned char pi_s[0xFFu] = { 0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7 };
const unsigned char pi_de_s[0xFFu] = { 0xE, 0x3, 0x4, 0x8, 0x1, 0xC, 0xA, 0xF, 0x7, 0xD, 0x9, 0x6, 0xB, 0x2, 0x0, 0x5 };
unsigned short pi_s_total[0xFFFFu];
unsigned short pi_de_s_total[0xFFFFu];
unsigned short pi_p_total[0xFFFFu];
const char hexChar[] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
typedef union {
    struct {
        unsigned char bit16 : 1;
        unsigned char bit12 : 1;
        unsigned char bit8 : 1;
        unsigned char bit4 : 1;
        unsigned char bit15 : 1;
        unsigned char bit11 : 1;
        unsigned char bit7 : 1;
        unsigned char bit3 : 1;
        unsigned char bit14 : 1;
        unsigned char bit10 : 1;
        unsigned char bit6 : 1;
        unsigned char bit2 : 1;
        unsigned char bit13 : 1;
        unsigned char bit9 : 1;
        unsigned char bit5 : 1;
        unsigned char bit1 : 1;
    } pi_p;
    struct {
        unsigned char bit16 : 1;
        unsigned char bit15 : 1;
        unsigned char bit14 : 1;
        unsigned char bit13 : 1;
        unsigned char bit12 : 1;
        unsigned char bit11 : 1;
        unsigned char bit10 : 1;
        unsigned char bit9 : 1;
        unsigned char bit8 : 1;
        unsigned char bit7 : 1;
        unsigned char bit6 : 1;
        unsigned char bit5 : 1;
        unsigned char bit4 : 1;
        unsigned char bit3 : 1;
        unsigned char bit2 : 1;
        unsigned char bit1 : 1;
    } bitmap;
    struct {
        unsigned char p4 : 4;
        unsigned char p3 : 4;
        unsigned char p2 : 4;
        unsigned char p1 : 4;
    } partMap;
    unsigned short z;
} bitStruct;
typedef union {
    struct {
        unsigned short : 16;
        unsigned short key : 16;
    } current;
    unsigned long long key;
} keyStruct;

inline void HexPrint(bitStruct* num, char append) {
    putchar(hexChar[num->partMap.p1]);
    putchar(hexChar[num->partMap.p2]);
    putchar(hexChar[num->partMap.p3]);
    putchar(hexChar[num->partMap.p4]);
    putchar(append);
}

#define PI_S_PART(number) pi_s[number]
#define PI_S_DECRY_PART(number) pi_de_s[number]

inline void XOR(bitStruct* numberBitMap, keyStruct* key) {
    numberBitMap->z ^= key->current.key;
    key->key <<= 4u;
}

inline void PI_S(bitStruct* numberBitMap) {
    numberBitMap->partMap = {
            PI_S_PART(numberBitMap->partMap.p4),
            PI_S_PART(numberBitMap->partMap.p3),
            PI_S_PART(numberBitMap->partMap.p2),
            PI_S_PART(numberBitMap->partMap.p1),
    };
}

inline void PI_P(bitStruct* numberBitMap) {
    numberBitMap->bitmap = {
            numberBitMap->pi_p.bit16,
            numberBitMap->pi_p.bit15,
            numberBitMap->pi_p.bit14,
            numberBitMap->pi_p.bit13,
            numberBitMap->pi_p.bit12,
            numberBitMap->pi_p.bit11,
            numberBitMap->pi_p.bit10,
            numberBitMap->pi_p.bit9,
            numberBitMap->pi_p.bit8,
            numberBitMap->pi_p.bit7,
            numberBitMap->pi_p.bit6,
            numberBitMap->pi_p.bit5,
            numberBitMap->pi_p.bit4,
            numberBitMap->pi_p.bit3,
            numberBitMap->pi_p.bit2,
            numberBitMap->pi_p.bit1,
    };
}

inline void XOR_DECRY(bitStruct* numberBitMap, keyStruct* key) {
    key->key >>= 4u;
    numberBitMap->z ^= key->current.key;
}

inline void PI_S_DECRY(bitStruct* numberBitMap) {
    numberBitMap->partMap = {
            PI_S_DECRY_PART(numberBitMap->partMap.p4),
            PI_S_DECRY_PART(numberBitMap->partMap.p3),
            PI_S_DECRY_PART(numberBitMap->partMap.p2),
            PI_S_DECRY_PART(numberBitMap->partMap.p1),
    };
}

#define PI_P_DECRY(numberBitMap) PI_P(numberBitMap)

inline void generatePi() {
    for (unsigned short i = 0; ; i++) {
        bitStruct t1, t2, t3;
        t1.z = t2.z = t3.z = i;
        PI_S(&t1);
        PI_S_DECRY(&t2);
        PI_P(&t3);
        pi_s_total[i] = t1.z;
        pi_de_s_total[i] = t2.z;
        pi_p_total[i] = t3.z;
        if (i == 0xFFFFu) break;
    }
}

bitStruct plainText[8000], crypt[8000];

int main() {
    generatePi();
    unsigned n = DecRead();
    for (unsigned i = 0; i < n; i++) {
        for (int j = 0; j < 8000; j++) {
            plainText[j].z = HexRead<unsigned short>();
            crypt[j].z = HexRead<unsigned short>();
        }
        // decrypt K52,K54
        unsigned countK52K54[16][16] = { 0 };
        unsigned countK51K53[16][16] = { 0 };

        auto start = clock();
        for (int j = 0; j < 8000; j++) {
            bitStruct x = plainText[j], y = crypt[j], u4;
            unsigned char part = x.bitmap.bit5 ^ x.bitmap.bit7 ^ x.bitmap.bit8;
            for (unsigned char L1 = 0; L1 < 16; L1++) {
                for (unsigned char L2 = 0; L2 < 16; L2++) {
                    u4.partMap = {
                            PI_S_DECRY_PART((L2 ^ y.partMap.p4) & 0xf),0,
                            PI_S_DECRY_PART((L1 ^ y.partMap.p2) & 0xf),0
                    };
                    countK52K54[L1][L2] += (part ^ u4.bitmap.bit6 ^ u4.bitmap.bit8 ^ u4.bitmap.bit14 ^ u4.bitmap.bit16) & 0x1u;
                }
            }
        }

        auto max1 = 0;
        unsigned char K52, K54;
        for (unsigned char L1 = 0; L1 < 16; L1++) {
            for (unsigned char L2 = 0; L2 < 16; L2++) {
                int t1 = abs((int)countK52K54[L1][L2] - 4000);
                if (t1 > max1) {
                    max1 = t1;
                    K52 = L1;
                    K54 = L2;
                }
            }
        }
        for (int j = 0; j < 8000; j++) {
            bitStruct x = plainText[j], y = crypt[j], u4;
            unsigned char part = x.bitmap.bit5 ^ x.bitmap.bit6 ^ x.bitmap.bit7 ^ x.bitmap.bit8;
            for (unsigned char L1 = 0; L1 < 16; L1++) {
                for (unsigned char L2 = 0; L2 < 16; L2++) {
                    u4.partMap = {
                            0,
                            PI_S_DECRY_PART((L2 ^ y.partMap.p3) & 0xf),
                            PI_S_DECRY_PART((K52 ^ y.partMap.p2) & 0xf),
                            PI_S_DECRY_PART((L1 ^ y.partMap.p1) & 0xf)
                    };
                    countK51K53[L1][L2] += (part ^ u4.bitmap.bit2 ^ u4.bitmap.bit4 ^ u4.bitmap.bit6 ^ u4.bitmap.bit8 ^ u4.bitmap.bit10 ^ u4.bitmap.bit12) & 0x1u;
                }
            }
        }

        auto max2 = 0;
        unsigned char K51, K53;
        for (unsigned char L1 = 0; L1 < 16; L1++) {
            for (unsigned char L2 = 0; L2 < 16; L2++) {
                int t2 = abs((int)countK51K53[L1][L2] - 4000);
                if (t2 > max2) {
                    max2 = t2;
                    K51 = L1;
                    K53 = L2;
                }
            }
        }
        auto stop = clock();
        printf("%u ms\n", stop - start);
        printf("%x%x%x%x", K51, K52, K53, K54);
    }
    return 0;
}


//void main()
//{
	/*HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, 100, 100);
	HANDLE hHeap2 = HeapCreate(HEAP_NO_SERIALIZE, 100, 100);	// WARNING 114
	if (hHeap != NULL) {
		auto hptest = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 100);
		auto hptest2 = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 100);
		HeapFree(hHeap, 0, hptest);
		/*__try {
			HeapFree(hHeap, 0, hptest);	// ERROR 103
		}
		__except (1) {
			printf("重复释放内存");
		}*/
		//HeapDestroy(hHeap);	// WARNING 113
		//printf("Destroy 堆时不释放Alloc");
		/*try {
			hptest = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 100);	// ERROR 101
		}
		catch (std::exception& e1) {
			printf("Destroy Heap Alloc");
		}
		try {
			HeapFree(hHeap, 0, hptest);	// ERROR 104
		}catch (std::exception& e1) {
			printf("Destroy Heap Free");
		}*/
		//try {
		//	HeapDestroy(hHeap);	// ERROR 102
		//}
		//catch (std::exception& e1) {
		//	printf("Destroy Heap Destroy");
		//}
	//}*/
//}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
