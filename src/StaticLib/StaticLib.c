#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"


// mem_size の容量でキュー用のメモリを確保する
void initialize(QUEUE* q, size_t mem_size)
{
	if (q == NULL) return;

	q->memory_begin = (int*)malloc(mem_size);
	q->memory_end = q->memory_begin + mem_size / sizeof(int);
	q->head = q->memory_begin;
	q->tail = q->memory_begin;
}


// 確保したメモリを解放する
void finalize(QUEUE* q)
{
	if (q == NULL) return;

	free(q->memory_begin);

	q->memory_begin = NULL;
	q->memory_end = NULL;
	q->head = NULL;
	q->tail = NULL;
}


// valの値をキューに入れる。実行の成否を返す
bool enqueue(QUEUE* q, int val)
{
	// ToDo: valのデータをキューに追加します
	// 上手くいかない場合にはfalseを返します
	// メモリを使い切ったら先頭アドレスに戻って追加して下さい
	if (q == NULL) return false;

	if (countQueueableElements(q) == 0) {
		return false;
	}

	if (q->tail == q->memory_end) {
		if (q->memory_begin != q->head) {
			*q->memory_begin = val;
			q->tail = q->memory_begin + 1;
		}
		else
			return false;
	}
	else {
		*q->tail = val;
		q->tail++;
	}
	return true;
}


// addrから始まるnum個の整数をキューに入れる。実行の成否を返す
bool enqueue_array(QUEUE* q, int* addr, int num)
{
	// ToDo: addrからnum個のデータをキューに追加します
	// 上手くいかない場合にはfalseを返します
	// メモリを使い切ったら先頭アドレスに戻って追加して下さい
	if (q == NULL) return false;
	if (addr == NULL)return false;
	if (num <= 0) return false;

	if (num > countQueueableElements(q)) return false;

	for (int i = 0; i < num; i++) {
		if (!enqueue(q, addr[i]))
			return false;

	}
	return true;
}

// キューから一つの要素を取り出す(不具合時は0を返す)
int dequeue(QUEUE* q)
{
	// ToDo: 先頭のデータを返します
	if (q == NULL) return 0;

	if (countQueuedElements(q) == 0)
		return 0;

	int a = *q->head;
	q->head++;
	if (q->head == q->memory_end)
		q->head = q->memory_begin;

	return a;

}

// addrにキューからnumの要素を取り出す。取り出せた個数を返す
int dequeue_array(QUEUE* q, int* addr, int num)
{
	// ToDo: 先頭からnum個のデータをaddrに格納します
	if (q == NULL) return 0;
	if (num <= 0) return 0;


	int i;
	for (i = 0; i < num; i++) {
		if (countQueuedElements(q) == 0) {
			break;
		}
		addr[i] = dequeue(q);
	}

	return i;
}

// キューが空かどうかを調べる
bool isEmpty(const QUEUE* q)
{
	if (q == NULL) return false;

	return q->head == q->tail;
}

static int getMaxCount(const QUEUE* q)
{
	if (q == NULL || q->memory_begin == NULL) return 0;

	return (int)(q->memory_end - q->memory_begin);
}

// 挿入されたデータ数を得る
int countQueuedElements(const QUEUE* q)
{
	if (q == NULL || q->memory_begin == NULL) return 0;

	int max_counts = getMaxCount(q);
	return (max_counts - (q->head + max_counts - q->tail)) % max_counts;
}

// 挿入可能なデータ数を得る
int countQueueableElements(const QUEUE* q)
{
	return getMaxCount(q) - countQueuedElements(q) - 1;
}
