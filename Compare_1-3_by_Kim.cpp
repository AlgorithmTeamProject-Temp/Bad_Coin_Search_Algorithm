#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "balance.h"

typedef enum { one, two, all } COMPARE;
typedef enum { normal, bad, unsure } COIN;


// Declare Variables

int init_coin[101];		// �ʱ� ���� ��� �迭
int normal_coin[101];	// �Ϲ� ���� ��� �迭
int bad_coin[101];		// �ҷ� ���� ��� �迭
int unsure_coin_1[101];	// 1-1-1 �񱳿����� ��Ȯ�� ����
int unsure_coin_3[101];	// 3-3-3 �񱳿����� ��Ȯ�� ����

int normal_count, bad_count, unsure_count_1, unsure_count_3;	// �迭 ī��Ʈ
int compare_count;	// ��Ƚ�� ī��Ʈ (����׿�)

bool flag_99 = false;	// 99��° ���� �񱳸� ���� ����


// Declare Functions

void initCoin();	// ���� �迭 �ʱ�ȭ
int  compare(int a, int b, int c);	// balance�� ���� ����
int  compareOne(int *arr, int a, int b, int c, int count = 1, COIN type = unsure);		// 1-1-1 ��
int  compareThree(int *arr, int *a, int *b, int *c);	// 3-3-3 ��
void compareThree_2nd(int *a, int *b, int *c, COMPARE mode);	// 3-3-3 �� �ι�°
void compareUnsure(int *arr, int depth);	// unsure_coin ��
void compare99();	// 99��° ���� ��
void compareResult();	// ��� Ȯ��

int _balance(int *a, int *b, int *c);	// compare ��Ƚ�� �ľǿ� _balance �Լ� (����׿�)
int  pw(int base, int index);	// pow�� ���� ����
void printCoin();	// �迭 ���� ��� (����׿�)


void main()
{
	int i, j;	// ���� ����
	int depth = 2;	// unsure_coin_1�� ����� ������ ���
	int d1[4] = { 0, 0, 0, -1 }, d2[4] = { 0, 0, 0, -1 }, d3[4] = { 0, 0, 0, -1 };	// 3-3-3 �񱳿� ���� ���� �迭
	initCoin();

	int p = 1;		// �ʱ�ȭ Ȯ��
	initialize(p);

	bool probability = true;	// Ȯ�� ����
	for (i = 0; i < 99; i += 9)	// 99�� ���� ���� ��
	{
		if (!probability)
		{
			printf("\n1-1-1 Compare\n");
			for (j = 0; j < 3; j++)
			{
				compareOne(init_coin, j * 3 + i, j * 3 + i + 1, j * 3 + i + 2);
			}
		}
		else
		{
			printf("\n3-3-3 Compare\n");
			for (j = 0; j < 3; j++)
			{
				d1[j] = i + j;
				d2[j] = i + j + 3;
				d3[j] = i + j + 6;
			}
			compareThree(init_coin, d1, d2, d3);
		}

		// ���ǽ�
		probability = (normal_count + bad_count < unsure_count_1 + unsure_count_3) || ((normal_count <  bad_count ? bad_count : normal_count) / (normal_count <  bad_count ? normal_count : bad_count) > 3);
	}

	printCoin();

	if (unsure_count_1 != 0)	// unsure_coin_1�� 3���� ���� ��
		compareUnsure(unsure_coin_1, 1);

	if (unsure_count_3 != 0)	// unsure_coin_3�� unsure_coin_1�� ����
	{
		for (i = 0; i < unsure_count_3; i++)
			unsure_coin_1[unsure_count_1 + i] = unsure_coin_3[i];
		unsure_count_1 += unsure_count_3;
	}

	while (unsure_count_1 != 0)		// unsure_coin�� ����������� ��
	{
		compareUnsure(unsure_coin_1, depth);
		depth++;
	}

	if (flag_99 == false)	// 99�� ���� �з��� �ȵ����� ��, compareUnsure() �ȿ��� ���ǿ� ���� �̹� �񱳵Ǵ� ��찡 �� ����
		compare99();

	compareResult();
	system("pause");
	return;
}

void initCoin()
{
	for (int i = 0; i < 100; i++)
	{
		init_coin[i] = i;
		normal_coin[i] = 0;
		bad_coin[i] = 0;
		unsure_coin_1[i] = 0;
		unsure_coin_3[i] = 0;
	}
	return;
}

int compare(int a, int b, int c)
{
	int d1[2] = { a, -1 };	// ���� �迭
	int d2[2] = { b, -1 };
	int d3[2] = { c, -1 };

	return _balance(d1, d2, d3);
}

int compareOne(int *arr, int a, int b, int c, int count, COIN type)
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
		switch (type)
		{
		case normal:
			for (i = 0; i < count; i++)
			{
				normal_coin[normal_count + i] = arr[a + i];
				normal_coin[normal_count + count + i] = arr[b + i];
				normal_coin[normal_count + count * 2 + i] = arr[c + i];
			}
			normal_count += 3 * count;
			break;
		case bad:
			for (i = 0; i < count; i++)
			{
				bad_coin[bad_count + i] = arr[a + i];
				bad_coin[bad_count + count + i] = arr[b + i];
				bad_coin[bad_count + count * 2 + i] = arr[c + i];
			}
			bad_count += 3 * count;
			break;
		case unsure:
			for (i = 0; i < count; i++)
			{
				unsure_coin_1[unsure_count_1 + i] = arr[a + i];
				unsure_coin_1[unsure_count_1 + count + i] = arr[b + i];
				unsure_coin_1[unsure_count_1 + count * 2 + i] = arr[c + i];
			}
			unsure_count_1 += 3 * count;
			break;
		}
		break;
	default:
		printf("Error : compareOne(%d, %d, %d, %d)", a, b, c, count);
		break;
	}
	return result;
}

int compareThree(int *arr, int *a, int *b, int *c)
{
	int result = _balance(a, b, c);
	switch (result)
	{
	case ONE:
		compareThree_2nd(a, b, c, one);
		break;
	case TWO:
		compareThree_2nd(b, a, c, one);
		break;
	case THREE:
		compareThree_2nd(c, a, b, one);
		break;
	case EQUAL_ONETWO:
		compareThree_2nd(a, b, c, two);
		break;
	case EQUAL_ONETHREE:
		compareThree_2nd(a, c, b, two);
		break;
	case EQUAL_TWOTHREE:
		compareThree_2nd(b, c, a, two);
		break;
	case EQUAL_ALL:
		compareThree_2nd(a, b, c, all);
		break;
	default:
		printf("Error : compareThree() = %d", result);
		break;
	}
	return result;
}

void compareThree_2nd(int *a, int *b, int *c, COMPARE mode)
{
	int i;	// ���� ����
	switch (mode)
	{
	case one:
		switch (compareOne(a, 0, 1, 2, 1, normal))
		{
		case ONE: case TWO: case THREE:
			for (i = 0; i < 3; i++)
			{
				bad_coin[bad_count + i] = b[i];
				bad_coin[bad_count + i + 3] = c[i];
			}
			bad_count += 6;
			break;
		case EQUAL_ONETWO: case EQUAL_ONETHREE: case EQUAL_TWOTHREE:
			compareOne(b, 0, 1, 2, 1, bad);
			compareOne(c, 0, 1, 2, 1, bad);
			break;
		case EQUAL_ALL:
			compareOne(b, 0, 1, 2, 1, bad);
			compareOne(c, 0, 1, 2, 1, bad);
			break;
		default:
			printf("Error : compareThree_2nd(ONE)\n");
			break;
		}
		break;
	case two:
		switch (compareOne(a, 0, 1, 2, 1, normal))
		{
		case ONE: case TWO: case THREE:
			compareOne(b, 0, 1, 2, 1, normal);
			for (i = 0; i < 3; i++)
				bad_coin[bad_count + i] = c[i];
			bad_count += 3;
			break;
		case EQUAL_ONETWO: case EQUAL_ONETHREE: case EQUAL_TWOTHREE:
			compareOne(b, 0, 1, 2, 1, normal);
			compareOne(c, 0, 1, 2, 1, bad);
			break;
		case EQUAL_ALL:
			for (i = 0; i < 3; i++)
				normal_coin[normal_count + i] = b[i];
			normal_count += 3;
			compareOne(c, 0, 1, 2, 1, bad);
			break;
		default:
			printf("Error : compareThree_2nd(TWO)\n");
			break;
		}
		break;
	case all:
		if (compareOne(a, 0, 1, 2) != EQUAL_ALL)
		{
			compareOne(b, 0, 1, 2);
			compareOne(c, 0, 1, 2);
		}
		else
		{
			unsure_count_1 -= 3;	// unsure_coin_1�� ��� ���� ����
			for (i = 0; i < 3; i++)
			{
				unsure_coin_3[unsure_count_3 + i] = a[i];
				unsure_coin_3[unsure_count_3 + i + 3] = b[i];
				unsure_coin_3[unsure_count_3 + i + 6] = c[i];
			}
			unsure_count_3 += 9;
		}
		break;
	}
	return;
}

void compareUnsure(int *arr, int depth)
{
	int unsureCount = unsure_count_1;
	unsure_count_1 = 0;

	int i;	// ���� ����
	for (i = 0; i <= unsureCount - pw(3, depth + 1); i += pw(3, depth + 1))
	{
		compareOne(arr, i, i + pw(3, depth), i + 2 * pw(3, depth), pw(3, depth));
	}

	if (normal_count != 0 && bad_count != 0)
	{
		switch ((unsureCount % pw(3, depth + 1)) / pw(3, depth))	// unsure_coin_1 ���� ����
		{
		case 0:
			break;
		case 1:
			if (flag_99 == false)	// 99��° ������ ���� ��
			{
				switch (compare(unsure_coin_1[unsureCount - pw(3, depth)], 99, bad_coin[0]))
				{
				case ONE:	// unsure_coin_1�� ���� ����
					for (i = 0; i < pw(3, depth); i++)
						normal_coin[normal_count + i] = unsure_coin_1[unsureCount - pw(3, depth) + i];
					bad_coin[bad_count] = 99;

					normal_count += pw(3, depth);
					bad_count += 1;
					break;
				case TWO:	// 99�� ������ ���� ����
					for (i = 0; i < pw(3, depth); i++)
						bad_coin[bad_count + i] = unsure_coin_1[unsureCount - pw(3, depth) + i];
					normal_coin[normal_count] = 99;

					normal_count += 1;
					bad_count += pw(3, depth);
					break;
				case EQUAL_ONETWO:	// �� �� ���� ����
					for (i = 0; i < pw(3, depth); i++)
						normal_coin[normal_count + i] = unsure_coin_1[unsureCount - pw(3, depth) + i];
					normal_coin[normal_count + pw(3, depth)] = 99;

					normal_count += pw(3, depth) + 1;
					break;
				case EQUAL_ALL:		// �� �� �ҷ� ����
					for (i = 0; i < pw(3, depth); i++)
						bad_coin[bad_count + i] = unsure_coin_1[unsureCount - pw(3, depth) + i];
					bad_coin[bad_count + pw(3, depth)] = 99;

					bad_count += pw(3, depth) + 1;
					break;
				default:	// ����
					printf("Error : compareUnsure(%d) Compare Error / unsure_count_1 = %d", depth, unsure_count_1);
					break;
				}
				flag_99 = true;
			}
			else
			{
				switch (compare(unsure_coin_1[unsureCount - pw(3, depth)], normal_coin[0], bad_coin[0]))
				{
				case EQUAL_ONETWO:	// ���� ����
					for (i = 0; i < pw(3, depth); i++)
						normal_coin[normal_count + i] = unsure_coin_1[unsureCount - pw(3, depth) + i];
					normal_count += pw(3, depth);
					break;
				case TWO:	// �ҷ� ����
					for (i = 0; i < pw(3, depth); i++)
						bad_coin[bad_count + i] = unsure_coin_1[unsureCount - pw(3, depth) + i];
					bad_count += pw(3, depth);
					break;
				default:	// ����
					printf("Error : compareUnsure(%d) Compare Error / unsure_count_1 = %d", depth, unsure_count_1);
					break;
				}
			}
			break;
		case 2:
			switch (compare(unsure_coin_1[unsureCount - 2 * pw(3, depth)], unsure_coin_1[unsureCount - pw(3, depth)], bad_coin[0]))
			{
			case ONE:	// 1���� ���� ����
				for (i = 0; i < pw(3, depth); i++)
				{
					normal_coin[normal_count + i] = unsure_coin_1[unsureCount - 2 * pw(3, depth) + i];
					bad_coin[bad_count + i] = unsure_coin_1[unsureCount - pw(3, depth) + i];
				}
				normal_count += pw(3, depth);
				bad_count += pw(3, depth);
				break;
			case TWO:	// 2���� ���� ����
				for (i = 0; i < pw(3, depth); i++)
				{
					normal_coin[normal_count + i] = unsure_coin_1[unsureCount - pw(3, depth) + i];
					bad_coin[bad_count + i] = unsure_coin_1[unsureCount - 2 * pw(3, depth) + i];
				}
				normal_count += pw(3, depth);
				bad_count += pw(3, depth);
				break;
			case EQUAL_ONETWO:	// �� �� ���� ����
				for (i = 0; i < pw(3, depth); i++)
				{
					normal_coin[normal_count + i] = unsure_coin_1[unsureCount - 2 * pw(3, depth) + i];
					normal_coin[normal_count + pw(3, depth) + i] = unsure_coin_1[unsureCount - pw(3, depth) + i];
				}
				normal_count += 2 * pw(3, depth);
				break;
			case EQUAL_ALL:		// �� �� �ҷ� ����
				for (i = 0; i < pw(3, depth); i++)
				{
					bad_coin[bad_count + i] = unsure_coin_1[unsureCount - 2 * pw(3, depth) + i];
					bad_coin[bad_count + pw(3, depth) + i] = unsure_coin_1[unsureCount - pw(3, depth) + i];
				}
				bad_count += 2 * pw(3, depth);
				break;
			default:	// ����
				printf("Error : compareUnsure(%d) Compare Error / unsure_count_1 = %d", depth, unsure_count_1);
				break;
			}
			break;
		default:	// ����
			printf("Error : compareUnsure(%d) Count Error / unsure_count_1 = %d", depth, unsure_count_1);
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
	if (_balance(bad_coin, dummy, dummy) == SUCCESS)
	{
		printf("\n");
		system("pause");
		exit(0);
	}
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
	printf("\n\nUnsure Coin 1 : %d\n", unsure_count_1);
	for (i = 0; i < unsure_count_1; i++)
		printf("%d ", unsure_coin_1[i]);
	printf("\n\nUnsure Coin 3 : %d\n", unsure_count_3);
	for (i = 0; i < unsure_count_3; i++)
		printf("%d ", unsure_coin_3[i]);
	printf("\n\n");
}