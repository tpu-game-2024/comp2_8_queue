#pragma once // インクルードガード

#include <stdbool.h>

// C++ でCのライブラリを使うときのおまじない
#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
	int* head;
	int* tail;

	int* memory_begin;
	int* memory_end;
}QUEUE;

//               head           tail
//      ------------------------------------ 
//      |    |    |****|****|****|oooo|    |
//      ------------------------------------  
// memory_begin                          memory_end
//
// 次にデータを追加するときはooooの位置に追加する


void initialize(QUEUE* q, size_t mem_size);       // mem_size の容量でキュー用のメモリを確保する
void finalize(QUEUE* q);                          // 確保したメモリを解放する

bool enqueue(QUEUE* q, int val);                  // valの値をキューに入れる。実行の成否を返す
bool enqueue_array(QUEUE* q, int* addr, int num); // addrから始まるnum個の整数をキューに入れる。実行の成否を返す

int dequeue(QUEUE* q);                            // キューから一つの要素を取り出す(不具合時は0を返す)
int dequeue_array(QUEUE* q, int* addr, int num);  // addrにキューからnumの要素を取り出す。取り出せた個数を返す

bool isEmpty(const QUEUE* s);                     // キューが空かどうかを調べる
int countQueuedElements(const QUEUE* q);          // 挿入されたデータ数を得る
int countQueueableElements(const QUEUE* q);       // 挿入可能なデータ数を得る

	// C++ でCのライブラリを使うときのおまじない
#ifdef __cplusplus
} // extern "C"
#endif
