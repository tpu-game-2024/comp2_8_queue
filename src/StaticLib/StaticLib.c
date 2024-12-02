#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "../include/lib_func.h"

// キューの初期化
void initialize(QUEUE* q, size_t mem_size) {
    if (q == NULL || mem_size < sizeof(int)) {
        fprintf(stderr, "Invalid memory size for queue initialization.\n");
        return;
    }
    q->memory_begin = (int*)malloc(mem_size);
    if (!q->memory_begin) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }
    q->memory_end = q->memory_begin + mem_size / sizeof(int);
    q->head = q->memory_begin;
    q->tail = q->memory_begin;
}

// キューの解放
void finalize(QUEUE* q) {
    if (q == NULL) return;

    free(q->memory_begin);
    q->memory_begin = NULL;
    q->memory_end = NULL;
    q->head = NULL;
    q->tail = NULL;
}

// キューに値を追加
bool enqueue(QUEUE* q, int val) {
    if (q == NULL || q->memory_begin == NULL) return false;

    // 次に挿入する位置を計算
    int* next = q->head + 1;
    if (next == q->memory_end) next = q->memory_begin;

    // キューが満杯の場合は失敗
    if (next == q->tail) {
        fprintf(stderr, "Queue overflow: cannot enqueue %d.\n", val);
        return false;
    }

    // 値を格納し、headを更新
    *(q->head) = val;
    q->head = next;
    return true;
}

// 配列をキューに追加
bool enqueue_array(QUEUE* q, int* addr, int num) {
    if (q == NULL || q->memory_begin == NULL || addr == NULL || num <= 0) return false;

    for (int i = 0; i < num; i++) {
        if (!enqueue(q, addr[i])) {
            fprintf(stderr, "Failed to enqueue array at index %d.\n", i);
            return false;
        }
    }
    return true;
}

// キューから値を取得
int dequeue(QUEUE* q) {
    if (q == NULL || q->memory_begin == NULL || isEmpty(q)) return 0;

    int val = *(q->tail);
    q->tail++;
    if (q->tail == q->memory_end) q->tail = q->memory_begin;

    return val;
}

// 配列にキューから値を取得
int dequeue_array(QUEUE* q, int* addr, int num) {
    if (q == NULL || q->memory_begin == NULL || addr == NULL || num <= 0) return 0;

    int dequeued_count = 0;
    for (int i = 0; i < num; i++) {
        if (isEmpty(q)) break;

        addr[i] = dequeue(q);
        dequeued_count++;
    }
    return dequeued_count;
}

// キューが空かどうかを確認
bool isEmpty(const QUEUE* q) {
    if (q == NULL) return false;

    return q->head == q->tail;
}

// キューの最大容量を取得
static int getMaxCount(const QUEUE* q) {
    if (q == NULL || q->memory_begin == NULL) return 0;

    return (int)(q->memory_end - q->memory_begin);
}

// キュー内のデータ数を取得
int countQueuedElements(const QUEUE* q) {
    if (q == NULL || q->memory_begin == NULL) return 0;

    int max_counts = getMaxCount(q);
    return (q->head + max_counts - q->tail) % max_counts;
}

// 挿入可能なデータ数を取得
int countQueueableElements(const QUEUE* q) {
    return getMaxCount(q) - countQueuedElements(q) - 1;
}
