/*************************************************************************************************************************************

	1-1-1 Compare Algorithm (Non-Pivot)

	99개의 코인을 먼저 1-1-1 비교를 통해 분류한 후, unsure_coin에 담긴 원소가 없어질 때까지 1-1-1 비교를 통해 분류한다.
	비교해야할 원소 갯수는 1 - 3 - 9 - 27, 즉 3의 거듭제곱을 따라 증가한다.

	** 비교순서 **
	1. unsure_coin에 99개 원소[0 - 98]를 집어넣는다.
	2. compareUnsure() 함수를 통해 unsure_coin 배열 내 원소끼리 compareOne()을 진행한다.
	3. depth 변수를 이용하여 compareUnsure() 함수가 실행될 때마다 비교하는 원소의 갯수를 3의 거듭제곱, 즉 pw(3, depth)만큼 비교할 수 있도록 한다.
	   (compareOne()의 count 아규먼트, compareUnsure()의 depth 아규먼트 참조)
	4. 이와 별개로, 100번째 원소[99]를 비교하기 위해, compareUnsure() 함수 내부에 coin[99]를 비교할 수 있는 구문을 삽입하였고,
	   비교가 완료되었는지 체크하기 위한 flag_99 변수,
	   compareUnsure()가 완료될 때까지 coin[99]가 비교되지 않았을 경우 따로 비교할 수 있도록 compare99() 함수를 구현하였다.
	5. 비교가 완료되면, compareResult() 함수를 통해 최종적으로 불량 동전을 판별한다.
	
	** 기타 **
	compare_count, _balance(), printCoin() 를 디버그할 때 이용할 수 있다.

 *************************************************************************************************************************************/


#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "balance.h"


 // Declare Variables

int normal_coin[101];	// 코인 담는 배열
int bad_coin[101];
int unsure_coin[101];

int normal_count, bad_count, unsure_count;	// 배열 카운트
int compare_count;	// 비교횟수 카운트 (디버그용)

bool flag_99 = false;	// 99번째 코인 비교를 위한 변수


// Declare Functions

int  compare(int a, int b, int c);	// balance를 쓰기 쉽게
int  compareOne(int arr[], int a, int b, int c, int count);	// 1-1-1 비교
void compareUnsure(int arr[], int depth);	// unsure_coin 비교
void compare99();	// 99번째 코인 비교
void compareResult();	// 결과 확인

int _balance(int a[], int b[], int c[]);	// compare 비교횟수 파악용 _balance 함수 (디버그용)
int  pw(int base, int index);	// pow를 쓰기 쉽게
void printCoin();	// 배열 상태 출력 (디버그용)


void main()
{
	int i;	// 루프 변수
	int depth = 0;	// unsure_coin이 비워질 때까지 사용

	int p = 1;		// 초기화 확률
	initialize(p);

	for (i = 0; i < 100; i++)
		unsure_coin[i] = i;
	unsure_count = 99;

	while (unsure_count != 0)
	{
		compareUnsure(unsure_coin, depth);
		depth++;
	}

	if (flag_99 == false) compare99();
	compareResult();

	//printf("\nSum : %d\nCompare %d times\n", normal_count + bad_count + unsure_count, compare_count);
	system("pause");
	return;
}

int compare(int a, int b, int c)
{
	int d1[2] = { a, -1 };	// 더미 배열
	int d2[2] = { b, -1 };
	int d3[2] = { c, -1 };

	return _balance(d1, d2, d3);
}

int compareOne(int arr[], int a, int b, int c, int count)
{
	int d1[2] = { arr[a], -1 };	// 더미 배열
	int d2[2] = { arr[b], -1 };
	int d3[2] = { arr[c], -1 };

	int i;	// 루프 변수

	int result = _balance(d1, d2, d3);
	switch (result)
	{
	case ONE:
		for (i = 0; i < count; i++)
		{
			normal_coin[normal_count + i] = arr[a + i];
			bad_coin[bad_count + i] = arr[b + i];
			bad_coin[bad_count + count + i] = arr[c + i];
		}
		normal_count += count;
		bad_count += 2 * count;
		break;
	case TWO:
		for (i = 0; i < count; i++)
		{
			normal_coin[normal_count + i] = arr[b + i];
			bad_coin[bad_count + i] = arr[a + i];
			bad_coin[bad_count + count + i] = arr[c + i];
		}
		normal_count += count;
		bad_count += 2 * count;
		break;
	case THREE:
		for (i = 0; i < count; i++)
		{
			normal_coin[normal_count + i] = arr[c + i];
			bad_coin[bad_count + i] = arr[a + i];
			bad_coin[bad_count + count + i] = arr[b + i];
		}
		normal_count += count;
		bad_count += 2 * count;
		break;
	case EQUAL_ONETWO:
		for (i = 0; i < count; i++)
		{
			normal_coin[normal_count + i] = arr[a + i];
			normal_coin[normal_count + count + i] = arr[b + i];
			bad_coin[bad_count + i] = arr[c + i];
		}
		normal_count += 2 * count;
		bad_count += count;
		break;
	case EQUAL_ONETHREE:
		for (i = 0; i < count; i++)
		{
			normal_coin[normal_count + i] = arr[a + i];
			normal_coin[normal_count + count + i] = arr[c + i];
			bad_coin[bad_count + i] = arr[b + i];
		}
		normal_count += 2 * count;
		bad_count += count;
		break;
	case EQUAL_TWOTHREE:
		for (i = 0; i < count; i++)
		{
			normal_coin[normal_count + i] = arr[b + i];
			normal_coin[normal_count + count + i] = arr[c + i];
			bad_coin[bad_count + i] = arr[a + i];
		}
		normal_count += 2 * count;
		bad_count += count;
		break;
	case EQUAL_ALL:
		for (i = 0; i < count; i++)
		{
			unsure_coin[unsure_count + i] = arr[a + i];
			unsure_coin[unsure_count + count + i] = arr[b + i];
			unsure_coin[unsure_count + count * 2 + i] = arr[c + i];
		}
		unsure_count += 3 * count;
		break;
	default:
		printf("Error : compareOne(%d, %d, %d, %d)", a, b, c, count);
		break;
	}

	return result;
}

void compareUnsure(int arr[], int depth)
{
	int unsureCount = unsure_count;
	unsure_count = 0;

	int i;	// 루프 변수
	for (i = 0; i <= unsureCount - pw(3, depth + 1); i += pw(3, depth + 1))
	{
		compareOne(arr, i, i + pw(3, depth), i + 2 * pw(3, depth), pw(3, depth));
		printCoin();
	}

	if (normal_count != 0 && bad_count != 0)
	{
		switch ((unsureCount % pw(3, depth + 1)) / pw(3, depth))	// unsure_coin 남은 갯수
		{
		case 0:
			break;
		case 1:
			if (flag_99 == false)	// 99번째 코인을 같이 비교
			{
				switch (compare(unsure_coin[unsureCount - pw(3, depth)], 99, bad_coin[0]))
				{
				case ONE:	// unsure_coin이 정상 코인
					for (i = 0; i < pw(3, depth); i++)
						normal_coin[normal_count + i] = unsure_coin[unsureCount - pw(3, depth) + i];
					bad_coin[bad_count] = 99;

					normal_count += pw(3, depth);
					bad_count += 1;
					break;
				case TWO:	// 99번 코인이 정상 코인
					for (i = 0; i < pw(3, depth); i++)
						bad_coin[bad_count + i] = unsure_coin[unsureCount - pw(3, depth) + i];
					normal_coin[normal_count] = 99;

					normal_count += 1;
					bad_count += pw(3, depth);
					break;
				case EQUAL_ONETWO:	// 둘 다 정상 코인
					for (i = 0; i < pw(3, depth); i++)
						normal_coin[normal_count + i] = unsure_coin[unsureCount - pw(3, depth) + i];
					normal_coin[normal_count + pw(3, depth)] = 99;

					normal_count += pw(3, depth) + 1;
					break;
				case EQUAL_ALL:		// 둘 다 불량 코인
					for (i = 0; i < pw(3, depth); i++)
						bad_coin[bad_count + i] = unsure_coin[unsureCount - pw(3, depth) + i];
					bad_coin[bad_count + pw(3, depth)] = 99;

					bad_count += pw(3, depth) + 1;
					break;
				default:	// 에러
					printf("Error : compareUnsure(%d) Compare Error / unsure_count = %d", depth, unsure_count);
					break;
				}

				flag_99 = true;
			}
			else
			{
				switch (compare(unsure_coin[unsureCount - pw(3, depth)], normal_coin[0], bad_coin[0]))
				{
				case EQUAL_ONETWO:	// 정상 코인
					for (i = 0; i < pw(3, depth); i++)
						normal_coin[normal_count + i] = unsure_coin[unsureCount - pw(3, depth) + i];
					normal_count += pw(3, depth);
					break;
				case TWO:	// 불량 코인
					for (i = 0; i < pw(3, depth); i++)
						bad_coin[bad_count + i] = unsure_coin[unsureCount - pw(3, depth) + i];
					bad_count += pw(3, depth);
					break;
				default:	// 에러
					printf("Error : compareUnsure(%d) Compare Error / unsure_count = %d", depth, unsure_count);
					break;
				}
			}
			break;
		case 2:
			switch (compare(unsure_coin[unsureCount - 2 * pw(3, depth)], unsure_coin[unsureCount - pw(3, depth)], bad_coin[0]))
			{
			case ONE:	// 1번이 정상 코인
				for (i = 0; i < pw(3, depth); i++)
				{
					normal_coin[normal_count + i] = unsure_coin[unsureCount - 2 * pw(3, depth) + i];
					bad_coin[bad_count + i] = unsure_coin[unsureCount - pw(3, depth) + i];
				}
				normal_count += pw(3, depth);
				bad_count += pw(3, depth);
				break;
			case TWO:	// 2번이 정상 코인
				for (i = 0; i < pw(3, depth); i++)
				{
					normal_coin[normal_count + i] = unsure_coin[unsureCount - pw(3, depth) + i];
					bad_coin[bad_count + i] = unsure_coin[unsureCount - 2 * pw(3, depth) + i];
				}
				normal_count += pw(3, depth);
				bad_count += pw(3, depth);
				break;
			case EQUAL_ONETWO:	// 둘 다 정상 코인
				for (i = 0; i < pw(3, depth); i++)
				{
					normal_coin[normal_count + i] = unsure_coin[unsureCount - 2 * pw(3, depth) + i];
					normal_coin[normal_count + pw(3, depth) + i] = unsure_coin[unsureCount - pw(3, depth) + i];
				}
				normal_count += 2 * pw(3, depth);
				break;
			case EQUAL_ALL:		// 둘 다 불량 코인
				for (i = 0; i < pw(3, depth); i++)
				{
					bad_coin[bad_count + i] = unsure_coin[unsureCount - 2 * pw(3, depth) + i];
					bad_coin[bad_count + pw(3, depth) + i] = unsure_coin[unsureCount - pw(3, depth) + i];
				}
				bad_count += 2 * pw(3, depth);
				break;
			default:	// 에러
				printf("Error : compareUnsure(%d) Compare Error / unsure_count = %d", depth, unsure_count);
				break;
			}
			break;
		default:	// 에러
			printf("Error : compareUnsure(%d) Count Error / unsure_count = %d", depth, unsure_count);
			break;
		}
	}
	return;
}

void compare99()
{
	switch (compare(99, normal_coin[0], bad_coin[0]))
	{
	case EQUAL_ONETWO:	// 정상 코인
		normal_coin[normal_count] = 99;
		normal_count++;
		break;
	case TWO:	// 불량 코인
		bad_coin[bad_count] = 99;
		bad_count++;
		break;
	default:	// 에러
		printf("Error : compare99()");
		break;
	}

	flag_99 = true;
	return;
}

void compareResult()
{
	int dummy[] = { -1 };	// 결과 확인용 더미 배열

	printCoin();
	bad_coin[bad_count] = -1;
	_balance(bad_coin, dummy, dummy);
	printf("\n");
	return;
}

int _balance(int a[], int b[], int c[])
{
	compare_count++;
	return balance(a, b, c);
}

int pw(int base, int index)
{
	return (int)pow((double)base, (double)index);
}

void printCoin()
{
	int i;
	printf("\n\nNormal Coin : %d\n", normal_count);
	for (i = 0; i < normal_count; i++)
		printf("%d ", normal_coin[i]);
	printf("\n\nBad Coin : %d\n", bad_count);
	for (i = 0; i < bad_count; i++)
		printf("%d ", bad_coin[i]);
	printf("\n\nUnsure Coin : %d\n", unsure_count);
	for (i = 0; i < unsure_count; i++)
		printf("%d ", unsure_coin[i]);
	printf("\n\n");
}