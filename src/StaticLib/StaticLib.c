#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../include/lib_func.h"

// mem_size の容量でキュー用のメモリを確保する
void initialize(QUEUE* q, size_t mem_size)
{
    if (q == NULL) return;

    q->memory_begin = (int*)malloc(mem_size);
    if (q->memory_begin == NULL) return; // メモリ確保失敗

    q->memory_end = q->memory_begin + mem_size / sizeof(int);
    q->head = q->memory_begin;
    q->tail = q->memory_begin;
}

// 確保したメモリを解放する
void finalize(QUEUE* q)
{
    if (q == NULL || q->memory_begin == NULL) return;

    free(q->memory_begin);

    q->memory_begin = NULL;
    q->memory_end = NULL;
    q->head = NULL;
    q->tail = NULL;
}

// valの値をキューに入れる。実行の成否を返す
bool enqueue(QUEUE* q, int val)
{
    if (q == NULL || q->memory_begin == NULL) return false;

    if ((q->tail + 1 == q->head) ||
        (q->tail + 1 == q->memory_end && q->head == q->memory_begin))
        return false; // キューが満杯の場合

    *(q->tail) = val;
    q->tail++;
    if (q->tail == q->memory_end) q->tail = q->memory_begin; // 循環処理

    return true;
}

// addrから始まるnum個の整数をキューに入れる。実行の成否を返す
bool enqueue_array(QUEUE* q, int* addr, int num)
{
    if (q == NULL || q->memory_begin == NULL || addr == NULL || num <= 0) return false;

    int max_count = (int)(q->memory_end - q->memory_begin);
    int current_elements = countQueuedElements(q);
    int free_space = max_count - current_elements - 1;

    if (free_space < num) return false; // 十分な空きスペースがない場合

    for (int i = 0; i < num; i++)
    {
        if (!enqueue(q, addr[i])) return false;
    }
    return true;
}

// キューから一つの要素を取り出す(不具合時は0を返す)
int dequeue(QUEUE* q)
{
    if (q == NULL || isEmpty(q)) return 0;

    int val = *(q->head);
    q->head++;
    if (q->head == q->memory_end) q->head = q->memory_begin; // 循環処理

    return val;
}

// addrにキューからnumの要素を取り出す。取り出せた個数を返す
int dequeue_array(QUEUE* q, int* addr, int num)
{
    if (q == NULL || addr == NULL || num <= 0) return 0;

    int count = 0;
    while (count < num && !isEmpty(q))
    {
        addr[count] = dequeue(q);
        count++;
    }

    return count;
}

// キューが空かどうかを調べる
bool isEmpty(const QUEUE* q)
{
    if (q == NULL || q->memory_begin == NULL) return true;

    return q->head == q->tail;
}

// キューの最大格納数を計算する
static int getMaxCount(const QUEUE* q)
{
    if (q == NULL || q->memory_begin == NULL) return 0;

    return (int)(q->memory_end - q->memory_begin);
}

// 挿入されたデータ数を得る
int countQueuedElements(const QUEUE* q)
{
    if (q == NULL || q->memory_begin == NULL) return 0;

    int max_count = getMaxCount(q);
    return (q->tail + max_count - q->head) % max_count;
}

// 挿入可能なデータ数を得る
int countQueueableElements(const QUEUE* q)
{
    return getMaxCount(q) - countQueuedElements(q) - 1;
}
