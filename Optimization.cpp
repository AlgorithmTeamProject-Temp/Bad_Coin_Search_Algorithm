#include "stdio.h"
#include "balance.h"
#include "stdlib.h"

#define TRUE	1
#define FALSE	0

void compare1Init();
void confirmCompare1();
void confirmCompare3andLastIndex();

int dummySet(int set[]);
void allNormalCoin(int set[]);
void allBadCoin(int set[]);
void allCase(int result, int check);
void oneHeavyCase(int set1[], int set2[], int set3[]);
void twoHeavyCase(int set1[], int set2[], int set3[]);
void equalAllCase(int set1[], int set2[], int set3[]);
void compare3Init();
void after3BadCoin(int index);
void after3NormalCoin(int index);
void after9BadCoin(int index);
void after9NormalCoin(int index);
int compare_sort(const void* a, const void* b);

int normal_coin[100];
int bad_coin[100];

int normal_count = 0;
int bad_count = 0;

int too_fast_convert_3compare = FALSE;

int unsure_1compare[33];
int double_unsure_1compare[11];
int unsure_3compare[10];

int unsure_count_1compare = 0;
int double_unsure_count = 0;
int unsure_count_3compare = 0;

int a[2] = { 0, -1 };
int b[2] = { 0, -1 };
int c[2] = { 0, -1 };

int a3[4] = { 0, 0, 0, -1 };
int b3[4] = { 0, 0, 0, -1 };
int c3[4] = { 0, 0, 0, -1 };

int set1[4] = { 0, 0, 0, -1 };
int set2[4] = { 0, 0, 0, -1 };
int set3[4] = { 0, 0, 0, -1 };

extern int coin[];

int main() {
	//불량동전을 초기화한다. 이때 불량동전의 발생 확률 = 10
	initialize(40);

	compare1Init();
	confirmCompare1();
	confirmCompare3andLastIndex();

	qsort(bad_coin, bad_count, sizeof(int), compare_sort);
	qsort(normal_coin, normal_count, sizeof(int), compare_sort);

	bad_coin[bad_count] = -1;
	b[0] = -1;
	c[0] = -1;

	balance(bad_coin, b, c);

	return 0;
}

void compare1Init() {
	int balance_result;
	int i = 0, j = 0, k = 0;
	int bool_end = FALSE;

	for (i; i < 99; i = i + 3) {
		a[0] = i;
		b[0] = i + 1;
		c[0] = i + 2;

		balance_result = balance(a, b, c);

		if (balance_result != EQUAL_ALL)
			allCase(balance_result, 0);
		else {	// EQUALL_ALL
			unsure_1compare[unsure_count_1compare] = a[0];
			unsure_count_1compare++;
		}

		if (i == 6) {
			if (unsure_count_1compare >= 2) {
				too_fast_convert_3compare = TRUE;
				for (i = 9; i < 99; i = i + 9) {
					for (j = 0; j < 3; j++) {
						a3[j] = i + j;
						b3[j] = i + 3 + j;
						c3[j] = i + 6 + j;
					}
					compare3Init();

					if (i == 27 && (((float)(bad_count + normal_count) / (float)(i + 9)) * 100) >= 23) {
						if ((bad_count / (bad_count + normal_count) * 100) <= 77 && (bad_count / (bad_count + normal_count) * 100) >= 23) {
							bool_end = TRUE;
							break;
						}
					}
					if (i == 27 && ((float)bad_count / (float)(bad_count + normal_count) * 100) <= 77 && ((float)bad_count / (float)(bad_count + normal_count) * 100) >= 23) {
						bool_end = TRUE;
						break;
					}
				}
				if (bool_end == FALSE) {
					break;
				}
				else {
					i = i + 6;
					continue;
				}
			}
		}
		else if (i == 15) {
			if (unsure_count_1compare >= 3) {
				printf("인덱스 18에서 넘어갑니다.");
				for (i = 18; i < 99; i = i + 9) {
					for (j = 0; j < 3; j++) {
						a3[j] = i + j;
						b3[j] = i + 3 + j;
						c3[j] = i + 6 + j;
					}
					compare3Init();
				}
				break;
			}
		}
	}
}

void confirmCompare1() {
	int i, j;
	float percentage;

	percentage = ((float)bad_count / (float)(bad_count + normal_count)) * 100;

	if (percentage <= 25 && too_fast_convert_3compare == FALSE) {
		for (i = 0; i < unsure_count_1compare; i++) {
			after3NormalCoin(unsure_1compare[i]);
		}
	}
	else if (percentage >= 75 && too_fast_convert_3compare == FALSE) {
		for (i = 0; i < unsure_count_1compare; i++) {
			after3BadCoin(unsure_1compare[i]);
		}
	}
	else {	// 중간 확률 경우
		for (i = 0; i < unsure_count_1compare; i = i + 3) {
			if ((i + 3) > unsure_count_1compare)
				break;

			a[0] = unsure_1compare[i];
			b[0] = unsure_1compare[i + 1];
			c[0] = unsure_1compare[i + 2];

			switch (balance(a, b, c)) {
			case ONE:
				after3NormalCoin(a[0]);
				after3BadCoin(b[0]);
				after3BadCoin(c[0]);
				break;
			case TWO:
				after3NormalCoin(b[0]);
				after3BadCoin(a[0]);
				after3BadCoin(c[0]);
				break;
			case THREE:
				after3NormalCoin(c[0]);
				after3BadCoin(a[0]);
				after3BadCoin(b[0]);
				break;
			case EQUAL_ONETWO:
				after3NormalCoin(a[0]);
				after3NormalCoin(b[0]);
				after3BadCoin(c[0]);
				break;
			case EQUAL_ONETHREE:
				after3NormalCoin(a[0]);
				after3NormalCoin(c[0]);
				after3BadCoin(b[0]);
				break;
			case EQUAL_TWOTHREE:
				after3NormalCoin(b[0]);
				after3NormalCoin(c[0]);
				after3BadCoin(a[0]);
				break;
			case EQUAL_ALL:	// 항상 3의 배수로 존재
				double_unsure_1compare[double_unsure_count] = a[0];
				double_unsure_1compare[double_unsure_count + 1] = b[0];
				double_unsure_1compare[double_unsure_count + 2] = c[0];
				double_unsure_count = double_unsure_count + 3;
				break;
			default:
				printf("정의되지 않은 결과");
				break;
			}
		}

		// 3으로 나눠 떨어지지 않을 때
		if ((unsure_count_1compare % 3) == 1) {	// 1개 남음
			a[0] = normal_coin[0];
			b[0] = normal_coin[1];
			c[0] = unsure_1compare[unsure_count_1compare - 1];

			switch (balance(a, b, c)) {
			case EQUAL_ONETWO:
				after3BadCoin(c[0]);
				break;
			case EQUAL_ALL:
				after3NormalCoin(c[0]);
				break;
			default:
				printf("정의되지 않은 결과");
				break;
			}
		}
		else if ((unsure_count_1compare % 3) == 2) {
			a[0] = normal_coin[0];
			b[0] = unsure_1compare[unsure_count_1compare - 2];
			c[0] = unsure_1compare[unsure_count_1compare - 1];

			switch (balance(a, b, c)) {
			case ONE:
				after3BadCoin(b[0]);
				after3BadCoin(c[0]);
				break;
			case EQUAL_ONETWO:
				after3NormalCoin(b[0]);
				after3BadCoin(c[0]);
				break;
			case EQUAL_ONETHREE:
				after3NormalCoin(c[0]);
				after3BadCoin(b[0]);
				break;
			case EQUAL_ALL:
				after3NormalCoin(b[0]);
				after3NormalCoin(c[0]);
				break;
			default:
				printf("정의되지 않은 결과");
				break;
			}
		}

		if (double_unsure_count != 0) {
			if (percentage >= 60) {
				b[0] = bad_coin[0];
				c[0] = bad_coin[1];

				for (j = 0; j < double_unsure_count; j = j + 3) {
					a[0] = double_unsure_1compare[j];

					switch (balance(b, c, a)) {
					case THREE:
						after3NormalCoin(double_unsure_1compare[j]);
						after3NormalCoin(double_unsure_1compare[j + 1]);
						after3NormalCoin(double_unsure_1compare[j + 2]);
						break;
					case EQUAL_ALL:
						after3BadCoin(double_unsure_1compare[j]);
						after3BadCoin(double_unsure_1compare[j + 1]);
						after3BadCoin(double_unsure_1compare[j + 2]);
						break;
					default:
						printf("정의되지 않은 결과");
						break;
					}
				}
			}
			else {
				b[0] = normal_coin[0];
				c[0] = normal_coin[1];

				for (j = 0; j < double_unsure_count; j = j + 3) {
					a[0] = double_unsure_1compare[j];

					switch (balance(b, c, a)) {
					case EQUAL_ONETWO:
						after3BadCoin(double_unsure_1compare[j]);
						after3BadCoin(double_unsure_1compare[j + 1]);
						after3BadCoin(double_unsure_1compare[j + 2]);
						break;
					case EQUAL_ALL:
						after3NormalCoin(double_unsure_1compare[j]);
						after3NormalCoin(double_unsure_1compare[j + 1]);
						after3NormalCoin(double_unsure_1compare[j + 2]);
						break;
					default:
						printf("정의되지 않은 결과");
						break;
					}
				}
			}
		}
	}
}

void confirmCompare3andLastIndex() {
	int i;
	a3[0] = normal_coin[0];
	if (unsure_count_3compare != 0 && unsure_count_3compare % 2 == 0) {	// unsure_count_3compare가 0이 아닌 짝수
		for (i = 0; i < unsure_count_3compare; i = i + 2) {		// 경우의 수 : ONE, EQUAL_ONETWO, EQUAL_ONETHREE, EQUAL_ALL
			a3[1] = unsure_3compare[i];
			a3[2] = unsure_3compare[i + 1];

			switch (dummySet(a3)) {
			case ONE:
				after9BadCoin(unsure_3compare[i]);
				after9BadCoin(unsure_3compare[i + 1]);
				break;
			case EQUAL_ONETWO:
				after9NormalCoin(unsure_3compare[i]);
				after9BadCoin(unsure_3compare[i + 1]);
				break;
			case EQUAL_ONETHREE:
				after9BadCoin(unsure_3compare[i]);
				after9NormalCoin(unsure_3compare[i + 1]);
				break;
			case EQUAL_ALL:
				after9NormalCoin(unsure_3compare[i]);
				after9NormalCoin(unsure_3compare[i + 1]);
				break;
			default:
				printf("정의되지 않은 결과");
				break;
			}
		}
		a3[1] = normal_coin[1];
		a3[2] = 99;

		switch (dummySet(a3)) {	// 경우의 수 : EQUAL_ONETWO, EQUAL_ALL
		case EQUAL_ONETWO:
			bad_coin[bad_count] = 99;
			bad_count = bad_count + 1;
			break;
		case EQUAL_ALL:
			normal_coin[normal_count] = 99;
			normal_count = normal_count + 1;
			break;
		default:
			printf("정의되지 않은 결과\n");
			break;
		}
	}
	else if (unsure_count_3compare != 0 && unsure_count_3compare % 2 == 1) {	// unsure_count_3compare가 홀수
		unsure_3compare[unsure_count_3compare] = 99;
		unsure_count_3compare = unsure_count_3compare + 1;

		for (i = 0; i < unsure_count_3compare; i = i + 2) {
			a3[1] = unsure_3compare[i];
			a3[2] = unsure_3compare[i + 1];

			if ((i + 2) >= unsure_count_3compare) {		// 경우의 수 ONE, EQUAL_ONETWO, EQUAL_ONETHREE EQUALL_ALL
				a3[2] = 99;

				switch (dummySet(a3)) {
				case ONE:
					after9BadCoin(unsure_3compare[i]);
					bad_coin[bad_count] = 99;
					bad_count = bad_count + 1;
					break;
				case EQUAL_ONETWO:
					after9NormalCoin(unsure_3compare[i]);
					bad_coin[bad_count] = 99;
					bad_count = bad_count + 1;
					break;
				case EQUAL_ONETHREE:
					after9BadCoin(unsure_3compare[i]);
					normal_coin[normal_count] = 99;
					normal_count = normal_count + 1;
					break;
				case EQUAL_ALL:
					after9NormalCoin(unsure_3compare[i]);
					normal_coin[normal_count] = 99;
					normal_count = normal_count + 1;
					break;
				default:
					printf("정의되지 않은 결과");
					break;
				}
			}
			else {
				switch (dummySet(a3)) {
				case ONE:
					after9BadCoin(unsure_3compare[i]);
					after9BadCoin(unsure_3compare[i + 1]);
					break;
				case EQUAL_ONETWO:
					after9NormalCoin(unsure_3compare[i]);
					after9BadCoin(unsure_3compare[i + 1]);
					break;
				case EQUAL_ONETHREE:
					after9BadCoin(unsure_3compare[i]);
					after9NormalCoin(unsure_3compare[i + 1]);
					break;
				case EQUAL_ALL:
					after9NormalCoin(unsure_3compare[i]);
					after9NormalCoin(unsure_3compare[i + 1]);
					break;
				default:
					printf("정의되지 않은 결과");
					break;
				}
			}
		}
	}
	else {	// unsure_count_3compare가 0일 때 99번째 인덱스만 탐색
		a3[0] = normal_coin[0];
		a3[1] = normal_coin[1];
		a3[2] = 99;

		switch (dummySet(a3)) {	// 경우의 수 : EQUAL_ONETWO, EQUAL_ALL		
		case EQUAL_ONETWO:
			bad_coin[bad_count] = 99;
			bad_count = bad_count + 1;
			break;
		case EQUAL_ALL:
			normal_coin[normal_count] = 99;
			normal_count = normal_count + 1;
			break;
		default:
			printf("정의되지 않은 결과");
			break;
		}
	}
}

int dummySet(int set[]) {
	a[0] = set[0];
	b[0] = set[1];
	c[0] = set[2];

	return balance(a, b, c);
}

void allNormalCoin(int set[]) {
	int i = 0;

	for (i = 0; i < 3; i++) {
		normal_coin[normal_count] = set[i];
		normal_count = normal_count + 1;
	}
}

void allBadCoin(int set[]) {
	int i = 0;

	for (i = 0; i < 3; i++) {
		bad_coin[bad_count] = set[i];
		bad_count = bad_count + 1;
	}
}

void allCase(int result, int check) {
	switch (result) {
	case ONE:
		normal_coin[normal_count] = a[0];
		bad_coin[bad_count] = b[0];
		bad_coin[bad_count + 1] = c[0];

		normal_count = normal_count + 1;
		bad_count = bad_count + 2;
		break;
	case TWO:
		normal_coin[normal_count] = b[0];
		bad_coin[bad_count] = a[0];
		bad_coin[bad_count + 1] = c[0];

		normal_count = normal_count + 1;
		bad_count = bad_count + 2;
		break;
	case THREE:
		normal_coin[normal_count] = c[0];
		bad_coin[bad_count] = a[0];
		bad_coin[bad_count + 1] = b[0];

		normal_count = normal_count + 1;
		bad_count = bad_count + 2;
		break;
	case EQUAL_ONETWO:
		normal_coin[normal_count] = a[0];
		normal_coin[normal_count + 1] = b[0];
		bad_coin[bad_count] = c[0];

		normal_count = normal_count + 2;
		bad_count = bad_count + 1;
		break;
	case EQUAL_ONETHREE:
		normal_coin[normal_count] = a[0];
		normal_coin[normal_count + 1] = c[0];
		bad_coin[bad_count] = b[0];

		normal_count = normal_count + 2;
		bad_count = bad_count + 1;
		break;
	case EQUAL_TWOTHREE:
		normal_coin[normal_count] = b[0];
		normal_coin[normal_count + 1] = c[0];
		bad_coin[bad_count] = a[0];

		normal_count = normal_count + 2;
		bad_count = bad_count + 1;
		break;
	case EQUAL_ALL:
		if (check == 0) {
			normal_coin[normal_count] = a[0];
			normal_coin[normal_count + 1] = b[0];
			normal_coin[normal_count + 2] = c[0];

			normal_count = normal_count + 3;
		}
		else if (check == 1) {
			bad_coin[bad_count] = a[0];
			bad_coin[bad_count + 1] = b[0];
			bad_coin[bad_count + 2] = c[0];

			bad_count = bad_count + 3;
		}
		else
			printf("정의되지 않은 결과");
		break;
	default:
		printf("정의되지 않은 결과");
		break;
	}
}

void oneHeavyCase(int set1[], int set2[], int set3[]) {
	int result = dummySet(set1);

	if (result == ONE || result == TWO || result == THREE) {
		allCase(result, 0);
		allBadCoin(set2);
		allBadCoin(set3);
	}
	else if (result == EQUAL_ONETWO || result == EQUAL_TWOTHREE || result == EQUAL_ONETHREE) {
		allCase(result, 0);
		allCase(dummySet(set2), 1);
		allCase(dummySet(set3), 1);
	}
	else {	// EQUAL_ALL
		allNormalCoin(set1);
		allCase(dummySet(set2), 1);
		allCase(dummySet(set3), 1);
	}
}

void twoHeavyCase(int set1[], int set2[], int set3[]) {
	int result = dummySet(set1);

	if (result == ONE || result == TWO || result == THREE) {
		allCase(result, 0);
		allCase(dummySet(set2), 0);
		allBadCoin(set3);
	}
	else if (result == EQUAL_ONETWO || result == EQUAL_TWOTHREE || result == EQUAL_ONETHREE) {
		allCase(result, 0);
		allCase(dummySet(set2), 0);
		allCase(dummySet(set3), 1);
	}
	else {	// EQUAL_ALL
		allNormalCoin(set1);
		allNormalCoin(set2);
		allCase(dummySet(set3), 1);
	}
}

void equalAllCase(int set1[], int set2[], int set3[]) {
	int result = dummySet(set1);

	if (result != EQUAL_ALL) {
		allCase(result, 0);
		allCase(dummySet(set2), 0);
		allCase(dummySet(set3), 0);
	}
	else {
		unsure_3compare[unsure_count_3compare] = set1[0];
		unsure_count_3compare = unsure_count_3compare + 1;
	}
}

void compare3Init() {
	int result = balance(a3, b3, c3);

	if (result == ONE)
		oneHeavyCase(a3, b3, c3);
	else if (result == TWO)
		oneHeavyCase(b3, a3, c3);
	else if (result == THREE)
		oneHeavyCase(c3, a3, b3);
	else if (result == EQUAL_ONETWO)
		twoHeavyCase(a3, b3, c3);
	else if (result == EQUAL_ONETHREE)
		twoHeavyCase(a3, c3, b3);
	else if (result == EQUAL_TWOTHREE)
		twoHeavyCase(b3, c3, a3);
	else
		equalAllCase(a3, b3, c3);
}

void after3BadCoin(int index) {
	int i;
	for (i = 0; i < 3; i++) {
		bad_coin[bad_count] = index + i;
		bad_count++;
	}
}

void after3NormalCoin(int index) {
	int i;
	for (i = 0; i < 3; i++) {
		normal_coin[normal_count] = index + i;
		normal_count++;
	}
}

void after9BadCoin(int index) {
	int i;
	for (i = 0; i < 9; i++) {
		bad_coin[bad_count] = index + i;
		bad_count++;
	}
}

void after9NormalCoin(int index) {
	int i;
	for (i = 0; i < 9; i++) {
		normal_coin[normal_count] = index + i;
		normal_count++;
	}
}

int compare_sort(const void* a, const void* b)
{
	if (*(int *)a > *(int *)b)
		return 1;
	else if (*(int *)a < *(int *)b)
		return -1;
	else
		return 0;
}