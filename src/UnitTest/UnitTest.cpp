#include <tchar.h>
#include "CppUnitTest.h"

#include "../include/lib_func.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(基本チェック)
	{
	public:

		TEST_METHOD(一要素)
		{
			QUEUE q;
			initialize(&q, sizeof(int) * 2);

			Assert::IsTrue(isEmpty(&q), _T("入れていなければ空"));
			Assert::AreEqual(0, countQueuedElements(&q), _T("入れていなければ空"));
			Assert::AreEqual(1, countQueueableElements(&q), _T("1個格納可能"));

			Assert::IsTrue(enqueue(&q, 2), _T("2を送るのに成功する"));

			Assert::AreEqual(2, dequeue(&q), _T("2を入れて2を取り出す"));

			Assert::IsTrue(isEmpty(&q), _T("全て取り出したら空"));

			finalize(&q);
		}

		TEST_METHOD(一つでも何回も使える)
		{
			QUEUE q;
			initialize(&q, sizeof(int) * 2);

			Assert::IsTrue(isEmpty(&q), _T("入れていなければ空"));
			Assert::AreEqual(0, countQueuedElements(&q), _T("入れていなければ空"));
			Assert::AreEqual(1, countQueueableElements(&q), _T("1個格納可能"));

			Assert::IsTrue(enqueue(&q, 3), _T("3を送るのに成功する"));
			Assert::AreEqual(3, dequeue(&q), _T("3を入れて3を取り出す"));

			Assert::IsTrue(isEmpty(&q), _T("全て取り出したら空"));

			Assert::IsTrue(enqueue(&q, 4), _T("4を送るのに成功する"));
			Assert::AreEqual(4, dequeue(&q), _T("4を入れて4を取り出す"));

			finalize(&q);
		}

		TEST_METHOD(大量に（１０００万）入れる)
		{
			QUEUE q;
			initialize(&q, sizeof(int) * 10000001);

			Assert::AreEqual(10000000, countQueueableElements(&q), _T("10000000個格納可能"));


			for (int i = 0; i < 10000000; i++) {
				Assert::IsTrue(enqueue(&q, i), _T("入れるのに成功する"));
			}
			Assert::IsFalse(isEmpty(&q), _T("全て入れ切った"));

			for (int i = 0; i < 10000000; i++) {
				Assert::AreEqual(i, dequeue(&q), _T("前から取り出す"));
			}

			finalize(&q);
		}

		TEST_METHOD(複数要素)
		{
			QUEUE q;
			initialize(&q, sizeof(int) * 3);

			int a[2] = { 5, 7 };
			Assert::IsTrue(enqueue_array(&q, a, 2), _T("2要素を入れるのに成功する"));

			int b[2];
			Assert::AreEqual(2, dequeue_array(&q, b, 2), _T("2要素取り出す"));
			Assert::AreEqual(b[0], a[0], _T("入れた値の確認"));
			Assert::AreEqual(b[1], a[1], _T("入れた値の確認"));

			finalize(&q);
		}

		TEST_METHOD(二個入れる場合に１つ入れて取り出した後に二個入れられる)
		{
			QUEUE q;
			initialize(&q, sizeof(int) * 3);

			Assert::IsTrue(isEmpty(&q), _T("入れていなければ空"));
			Assert::AreEqual(0, countQueuedElements(&q), _T("入れていなければ空"));
			Assert::AreEqual(2, countQueueableElements(&q), _T("2個格納可能"));

			Assert::IsTrue(enqueue(&q, 6), _T("6を送るのに成功する"));
			Assert::AreEqual(6, dequeue(&q), _T("6を入れて6を取り出す"));

			Assert::IsTrue(isEmpty(&q), _T("全て取り出したら空"));
			Assert::AreEqual(0, countQueuedElements(&q), _T("入れていなければ空"));
			Assert::AreEqual(2, countQueueableElements(&q), _T("2個格納可能"));

			int a[2] = { 7, 9 };
			Assert::IsTrue(enqueue_array(&q, a, 2), _T("2要素を入れるのに成功する"));
			int b[2];
			Assert::AreEqual(2, dequeue_array(&q, b, 2), _T("2要素取り出す"));
			Assert::AreEqual(7, b[0], _T("前の要素を先頭に入れる"));
			Assert::AreEqual(9, b[1], _T("前から２番目の要素が2番目に入る"));


			finalize(&q);
		}


		TEST_METHOD(まとめて入れてバラバラに取り出す)
		{
			QUEUE q;
			initialize(&q, sizeof(int) * 3);

			int a[2] = { 9, 10 };
			Assert::IsTrue(enqueue_array(&q, a, 2), _T("2要素を入れるのに成功する"));

			Assert::AreEqual(9, dequeue(&q), _T("先頭から格納されることを想定"));
			Assert::AreEqual(10, dequeue(&q), _T("２番目の要素を取り出す"));

			finalize(&q);
		}

		TEST_METHOD(バラバラに入れてまとめて取り出す)
		{
			QUEUE q;
			initialize(&q, sizeof(int) * 3);

			Assert::IsTrue(enqueue(&q, 14), _T("14を入れるのに成功する"));
			Assert::IsTrue(enqueue(&q, 20), _T("20を入れるのに成功する"));
			int b[2];
			Assert::AreEqual(2, dequeue_array(&q, b, 2), _T("2要素取り出す"));
			Assert::AreEqual(14, b[0], _T("前の要素を先頭に入れる"));
			Assert::AreEqual(20, b[1], _T("前から２番目の要素が2番目に入る"));

			finalize(&q);
		}

		TEST_METHOD(要素を超えては詰めない)
		{
			QUEUE q;
			initialize(&q, sizeof(int) * 3);

			Assert::IsTrue(enqueue(&q, 31), _T("14を入れるのに成功する"));
			int a[2] = { -1, -2 };
			Assert::IsFalse(enqueue_array(&q, a, 2), _T("2要素を入れるのに失敗する"));
			Assert::IsTrue(enqueue(&q, 37), _T("37を入れるのに成功する"));
			int b[2];
			Assert::AreEqual(dequeue(&q), 31, _T("先頭から格納されることを想定"));
			Assert::AreEqual(dequeue_array(&q, b, 2), 1, _T("1要素しか残っていない"));
			Assert::AreEqual(b[0], 37, _T("1要素は正しく取り出せる"));

			finalize(&q);
		}
	};

	TEST_CLASS(品質チェック)
	{
	public:

		TEST_METHOD(NULLチェック)
		{
			initialize(NULL, 0);
			Assert::AreEqual(1, 1, _T("NULLで初期化しても破綻しない"));

			int i = 0;
			Assert::IsFalse(enqueue(NULL, 1), _T("NULLポインタでのenqueueはできない"));
			Assert::IsFalse(enqueue_array(NULL, &i, 1), _T("NULLポインタでのenqueue_arrayはできない"));
			Assert::AreEqual(dequeue(NULL), 0, _T("NULLポインタでのdequeueはできない"));
			Assert::IsFalse(dequeue_array(NULL, &i, 1), _T("NULLポインタでのdequeue_arrayはできない"));

			QUEUE q;
			initialize(&q, sizeof(int) * 1);
			Assert::IsFalse(enqueue_array(&q, NULL, 2), _T("NULLポインタでのenqueueはできない"));
			Assert::AreEqual(0, dequeue_array(&q, NULL, 2), _T("NULLポインタでのdequeueはできない"));
			Assert::IsFalse(enqueue_array(&q, &i, 0), _T("０個のenqueueはできない"));
			Assert::IsFalse(enqueue_array(&q, &i, -1), _T("負の数のenqueueはできない"));
			Assert::AreEqual(0, dequeue_array(&q, &i, 0), _T("０個のdequeueはできない"));
			Assert::AreEqual(0, dequeue_array(&q, &i, -1), _T("負の数のdequeueはできない"));

			finalize(NULL);
			Assert::AreEqual(1, 1, _T("NULLで解放しても破綻しない"));

			finalize(&q);
		}

		TEST_METHOD(ゼロサイズ確保)
		{
			QUEUE q;
			initialize(&q, 0);

			finalize(&q);
			Assert::AreEqual(1, 1, _T("サイズを0で指定しても問題は起きない"));
		}

		TEST_METHOD(確保した数を超えて積めない)
		{
			QUEUE q;
			initialize(&q, sizeof(int)*2);

			Assert::IsTrue(enqueue(&q, 2), _T("2を入れるのに成功する"));
			Assert::IsFalse(enqueue(&q, 3), _T("1要素分しか確保していなので積めない"));

			finalize(&q);
		}

		TEST_METHOD(取り出す要素がない時は0)
		{
			QUEUE q;
			initialize(&q, sizeof(int)*2);

			Assert::IsTrue(enqueue(&q, 2), _T("2を入れるのに成功する"));

			Assert::AreEqual(2, dequeue(&q), _T("2を入れて2を取り出す"));
			Assert::AreEqual(0, dequeue(&q), _T("残っていないければ0を返す"));
			Assert::AreEqual(0, dequeue(&q), _T("残っていないければ0を返す"));

			finalize(&q);
		}

		TEST_METHOD(取り出す要素がない時は0（複数版）)
		{
			QUEUE q;
			initialize(&q, sizeof(int) * 4);

			Assert::IsTrue(enqueue(&q, 2), _T("2を入れるのに成功する"));
			Assert::IsTrue(enqueue(&q, 3), _T("3を入れるのに成功する"));
			Assert::AreEqual(2, dequeue(&q), _T("2が一番前のはず"));
			Assert::IsTrue(enqueue(&q, 4), _T("4を入れるのに成功する"));
			Assert::AreEqual(3, dequeue(&q), _T("3が一番前のはず"));
			Assert::AreEqual(4, dequeue(&q), _T("4が一番前のはず"));
			Assert::AreEqual(0, dequeue(&q), _T("残っていないければ0を返す"));

			finalize(&q);
		}

		TEST_METHOD(複数要素で限界を超えて入れない・出せない)
		{
			QUEUE q;
			initialize(&q, sizeof(int) * 3);

			int a[2] = { 5, 7 };
			Assert::IsTrue(enqueue_array(&q, a, 2), _T("2要素を入れるのに成功する"));
			Assert::IsFalse(enqueue_array(&q, a, 2), _T("限界を超えて詰めない"));
			Assert::IsFalse(enqueue(&q, 6), _T("限界を超えて詰めない"));

			int b[2];
			Assert::AreEqual(dequeue_array(&q, b, 2), 2, _T("2要素取り出す"));
			Assert::AreEqual(a[0], b[0], _T("入れた値の確認"));
			Assert::AreEqual(a[1], b[1], _T("入れた値の確認"));
			Assert::AreEqual(dequeue_array(&q, b, 2), 0, _T("要素がなければ取り出せない"));
			Assert::AreEqual(dequeue(&q), 0, _T("要素がなければ取り出せない"));

			finalize(&q);
		}

		TEST_METHOD(バラバラに入れてバラバラに取り出す)
		{
			QUEUE q;
			initialize(&q, sizeof(int) * 4);

			int a[2] = { 13, 17 };
			Assert::IsTrue(enqueue_array(&q, a, 2), _T("2要素を入れるのに成功する"));
			Assert::IsTrue(enqueue(&q, 19), _T("19を入れるのに成功する"));
			Assert::IsFalse(enqueue(&q, 23), _T("23を入れるのに失敗する"));

			int b[2];
			Assert::AreEqual(2, dequeue_array(&q, b, 2), _T("2要素取り出す"));
			Assert::AreEqual(13, b[0], _T("前の要素を先頭に入れる"));
			Assert::AreEqual(17, b[1], _T("前から２番目の要素が2番目に入る"));
			Assert::AreEqual(19, dequeue(&q), _T("19が一番前のはず"));
			Assert::AreEqual(dequeue_array(&q, b, 1), 0, _T("要素がなければ取り出せない"));
			Assert::AreEqual(13, b[0], _T("要素は変更されていないはず"));
			Assert::AreEqual(dequeue(&q), 0, _T("要素がなければ取り出せない"));

			finalize(&q);
		}

		TEST_METHOD(多重解放)
		{
			QUEUE q;
			initialize(&q, 1);

			finalize(&q);
			finalize(&q);
			Assert::AreEqual(1, 1, _T("finalizeを2度呼んでも破綻しない"));
		}
	};
}
