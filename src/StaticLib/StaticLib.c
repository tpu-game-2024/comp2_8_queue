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
	if (q->tail + 1 == q->head)return false;
	if (q->tail == q->memory_end - 1 && q->head == q->memory_begin)return false;

	*q->tail = val;
	if (++q->tail == q->memory_end)q->tail = q->memory_begin;

	return true;
}


// addrから始まるnum個の整数をキューに入れる。実行の成否を返す
bool enqueue_array(QUEUE* q, int* addr, int num)
{
	// ToDo: addrからnum個のデータをキューに追加します
	// 上手くいかない場合にはfalseを返します
	// メモリを使い切ったら先頭アドレスに戻って追加して下さい

	if (q == NULL)return false;
	if (q->memory_end - q->memory_begin <= num)return false;
	if (num <= 0)return false;

	if (q->tail < q->head) {
		if (q->head <= q->tail + num)return false;

		if (q->memory_end <= q->tail + num)
		{
			int num1 = (int)(q->memory_end - q->tail);
			memcpy_s(q->tail, sizeof(int) * num1, addr, sizeof(int) * num1);
			num -= num1;
			memcpy_s(q->memory_begin, sizeof(int) * (q->head - q->memory_begin), addr + num1, sizeof(int) * num);
			q->tail = q->memory_begin + num;
		}
		else {
			memcpy_s(q->tail, sizeof(int) * (q->memory_end - q->tail), addr, sizeof(int) * num);
			q->tail += num;
		}
	}
	else {
		if (q->memory_end <= q->tail + num)
		{
			int num1 = (int)(q->memory_end - q->tail);

			if (q->head <= q->memory_begin + num - num1)return false;

			memcpy_s(q->tail, sizeof(int) * num1, addr, sizeof(int) * num1);
			num -= num1;
			memcpy_s(q->memory_begin, sizeof(int) * (q->head - q->memory_begin), addr + num1, sizeof(int) * num);
			q->tail = q->memory_begin + num;
		}
		else {
			memcpy_s(q->tail, sizeof(int) * (q->memory_end - q->tail), addr, sizeof(int) * num);
			q->tail += num;
 		}
	}

	return true;
}

// キューから一つの要素を取り出す(不具合時は0を返す)
int dequeue(QUEUE* q)
{
	// ToDo: 先頭のデータを返します
	if (q == NULL) return 0;
	if (q->tail == q->head)return 0;

	int ret = *q->head;
	if (++q->head == q->memory_end)q->head = q->memory_begin;

	return ret;
}

// addrにキューからnumの要素を取り出す。取り出せた個数を返す
int dequeue_array(QUEUE* q, int* addr, int num)
{
	// ToDo: 先頭からnum個のデータをaddrに格納します
	if (q == NULL || addr == NULL)return 0;
	if (num <= 0)return false;

	if (q->head <= q->tail) {
		int n = (int)(q->tail - q->head);
		if (num < n)n = num;
		memcpy_s(addr, sizeof(int) * num, q->head, sizeof(int) * n);
		q->head += n;
		return n;
	}
	else {
		int left = (int)(q->memory_end - q->head);
		if (left <= num) {
			memcpy_s(addr, sizeof(int) * num, q->head, sizeof(int) * left);
			num = (int)(q->tail - q->memory_begin);
			if (q->tail - q->memory_begin < num) {
				num = (int)(q->tail - q->memory_begin);
			}
			memcpy_s(addr + left, sizeof(int) * num, q->memory_begin, sizeof(int) * num);
			q->head = q->memory_begin + num;
			
			return left + num;
		}
		else {
			memcpy_s(addr, sizeof(int) * num, q->head, sizeof(int) * num);
			q->head += num;
		}
	}

	return 0;
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
	return (q->head + max_counts - q->tail) % max_counts;
}

// 挿入可能なデータ数を得る
int countQueueableElements(const QUEUE* q)
{
	return getMaxCount(q) - countQueuedElements(q) - 1;
}
