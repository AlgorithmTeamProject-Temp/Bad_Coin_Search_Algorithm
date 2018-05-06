/*************************************************************************************************************************************

	1-1-1 Compare Algorithm (Non-Pivot)

	99���� ������ ���� 1-1-1 �񱳸� ���� �з��� ��, unsure_coin�� ��� ���Ұ� ������ ������ 1-1-1 �񱳸� ���� �з��Ѵ�.
	���ؾ��� ���� ������ 1 - 3 - 9 - 27, �� 3�� �ŵ������� ���� �����Ѵ�.

	** �񱳼��� **
	1. unsure_coin�� 99�� ����[0 - 98]�� ����ִ´�.
	2. compareUnsure() �Լ��� ���� unsure_coin �迭 �� ���ҳ��� compareOne()�� �����Ѵ�.
	3. depth ������ �̿��Ͽ� compareUnsure() �Լ��� ����� ������ ���ϴ� ������ ������ 3�� �ŵ�����, �� pw(3, depth)��ŭ ���� �� �ֵ��� �Ѵ�.
	   (compareOne()�� count �ƱԸ�Ʈ, compareUnsure()�� depth �ƱԸ�Ʈ ����)
	4. �̿� ������, 100��° ����[99]�� ���ϱ� ����, compareUnsure() �Լ� ���ο� coin[99]�� ���� �� �ִ� ������ �����Ͽ���,
	   �񱳰� �Ϸ�Ǿ����� üũ�ϱ� ���� flag_99 ����,
	   compareUnsure()�� �Ϸ�� ������ coin[99]�� �񱳵��� �ʾ��� ��� ���� ���� �� �ֵ��� compare99() �Լ��� �����Ͽ���.
	5. �񱳰� �Ϸ�Ǹ�, compareResult() �Լ��� ���� ���������� �ҷ� ������ �Ǻ��Ѵ�.
	
	** ��Ÿ **
	compare_count, _balance(), printCoin() �� ������� �� �̿��� �� �ִ�.

 *************************************************************************************************************************************/


#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "balance.h"


 // Declare Variables

int normal_coin[101];	// ���� ��� �迭
int bad_coin[101];
int unsure_coin[101];

int normal_count, bad_count, unsure_count;	// �迭 ī��Ʈ
int compare_count;	// ��Ƚ�� ī��Ʈ (����׿�)

bool flag_99 = false;	// 99��° ���� �񱳸� ���� ����


// Declare Functions

int  compare(int a, int b, int c);	// balance�� ���� ����
int  compareOne(int arr[], int a, int b, int c, int count);	// 1-1-1 ��
void compareUnsure(int arr[], int depth);	// unsure_coin ��
void compare99();	// 99��° ���� ��
void compareResult();	// ��� Ȯ��

int _balance(int a[], int b[], int c[]);	// compare ��Ƚ�� �ľǿ� _balance �Լ� (����׿�)
int  pw(int base, int index);	// pow�� ���� ����
void printCoin();	// �迭 ���� ��� (����׿�)


void main()
{
	int i;	// ���� ����
	int depth = 0;	// unsure_coin�� ����� ������ ���

	int p = 1;		// �ʱ�ȭ Ȯ��
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
	int d1[2] = { a, -1 };	// ���� �迭
	int d2[2] = { b, -1 };
	int d3[2] = { c, -1 };

	return _balance(d1, d2, d3);
}

int compareOne(int arr[], int a, int b, int c, int count)
{
	int d1[2] = { arr[a], -1 };	// ���� �迭
	int d2[2] = { arr[b], -1 };
	int d3[2] = { arr[c], -1 };

	int i;	// ���� ����

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

	int i;	// ���� ����
	for (i = 0; i <= unsureCount - pw(3, depth + 1); i += pw(3, depth + 1))
	{
		compareOne(arr, i, i + pw(3, depth), i + 2 * pw(3, depth), pw(3, depth));
		printCoin();
	}

	if (normal_count != 0 && bad_count != 0)
	{
		switch ((unsureCount % pw(3, depth + 1)) / pw(3, depth))	// unsure_coin ���� ����
		{
		case 0:
			break;
		case 1:
			if (flag_99 == false)	// 99��° ������ ���� ��
			{
				switch (compare(unsure_coin[unsureCount - pw(3, depth)], 99, bad_coin[0]))
				{
				case ONE:	// unsure_coin�� ���� ����
					for (i = 0; i < pw(3, depth); i++)
						normal_coin[normal_count + i] = unsure_coin[unsureCount - pw(3, depth) + i];
					bad_coin[bad_count] = 99;

					normal_count += pw(3, depth);
					bad_count += 1;
					break;
				case TWO:	// 99�� ������ ���� ����
					for (i = 0; i < pw(3, depth); i++)
						bad_coin[bad_count + i] = unsure_coin[unsureCount - pw(3, depth) + i];
					normal_coin[normal_count] = 99;

					normal_count += 1;
					bad_count += pw(3, depth);
					break;
				case EQUAL_ONETWO:	// �� �� ���� ����
					for (i = 0; i < pw(3, depth); i++)
						normal_coin[normal_count + i] = unsure_coin[unsureCount - pw(3, depth) + i];
					normal_coin[normal_count + pw(3, depth)] = 99;

					normal_count += pw(3, depth) + 1;
					break;
				case EQUAL_ALL:		// �� �� �ҷ� ����
					for (i = 0; i < pw(3, depth); i++)
						bad_coin[bad_count + i] = unsure_coin[unsureCount - pw(3, depth) + i];
					bad_coin[bad_count + pw(3, depth)] = 99;

					bad_count += pw(3, depth) + 1;
					break;
				default:	// ����
					printf("Error : compareUnsure(%d) Compare Error / unsure_count = %d", depth, unsure_count);
					break;
				}

				flag_99 = true;
			}
			else
			{
				switch (compare(unsure_coin[unsureCount - pw(3, depth)], normal_coin[0], bad_coin[0]))
				{
				case EQUAL_ONETWO:	// ���� ����
					for (i = 0; i < pw(3, depth); i++)
						normal_coin[normal_count + i] = unsure_coin[unsureCount - pw(3, depth) + i];
					normal_count += pw(3, depth);
					break;
				case TWO:	// �ҷ� ����
					for (i = 0; i < pw(3, depth); i++)
						bad_coin[bad_count + i] = unsure_coin[unsureCount - pw(3, depth) + i];
					bad_count += pw(3, depth);
					break;
				default:	// ����
					printf("Error : compareUnsure(%d) Compare Error / unsure_count = %d", depth, unsure_count);
					break;
				}
			}
			break;
		case 2:
			switch (compare(unsure_coin[unsureCount - 2 * pw(3, depth)], unsure_coin[unsureCount - pw(3, depth)], bad_coin[0]))
			{
			case ONE:	// 1���� ���� ����
				for (i = 0; i < pw(3, depth); i++)
				{
					normal_coin[normal_count + i] = unsure_coin[unsureCount - 2 * pw(3, depth) + i];
					bad_coin[bad_count + i] = unsure_coin[unsureCount - pw(3, depth) + i];
				}
				normal_count += pw(3, depth);
				bad_count += pw(3, depth);
				break;
			case TWO:	// 2���� ���� ����
				for (i = 0; i < pw(3, depth); i++)
				{
					normal_coin[normal_count + i] = unsure_coin[unsureCount - pw(3, depth) + i];
					bad_coin[bad_count + i] = unsure_coin[unsureCount - 2 * pw(3, depth) + i];
				}
				normal_count += pw(3, depth);
				bad_count += pw(3, depth);
				break;
			case EQUAL_ONETWO:	// �� �� ���� ����
				for (i = 0; i < pw(3, depth); i++)
				{
					normal_coin[normal_count + i] = unsure_coin[unsureCount - 2 * pw(3, depth) + i];
					normal_coin[normal_count + pw(3, depth) + i] = unsure_coin[unsureCount - pw(3, depth) + i];
				}
				normal_count += 2 * pw(3, depth);
				break;
			case EQUAL_ALL:		// �� �� �ҷ� ����
				for (i = 0; i < pw(3, depth); i++)
				{
					bad_coin[bad_count + i] = unsure_coin[unsureCount - 2 * pw(3, depth) + i];
					bad_coin[bad_count + pw(3, depth) + i] = unsure_coin[unsureCount - pw(3, depth) + i];
				}
				bad_count += 2 * pw(3, depth);
				break;
			default:	// ����
				printf("Error : compareUnsure(%d) Compare Error / unsure_count = %d", depth, unsure_count);
				break;
			}
			break;
		default:	// ����
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
	case EQUAL_ONETWO:	// ���� ����
		normal_coin[normal_count] = 99;
		normal_count++;
		break;
	case TWO:	// �ҷ� ����
		bad_coin[bad_count] = 99;
		bad_count++;
		break;
	default:	// ����
		printf("Error : compare99()");
		break;
	}

	flag_99 = true;
	return;
}

void compareResult()
{
	int dummy[] = { -1 };	// ��� Ȯ�ο� ���� �迭

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