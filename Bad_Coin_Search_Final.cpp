#include "stdio.h"
#include "balance.h"
#include "stdlib.h"

#define TRUE	1
#define FALSE	0

void compare1Init();	// ������ �� ���� ��
void compare3Init();	// ���� �� ���� �� ��Ʈ�� �迭 �� �� ��

void oneHeavyCase(int set1[], int set2[], int set3[]);	// �� ���� ���� ������� �� ���� ���ſ� ���
void twoHeavyCase(int set1[], int set2[], int set3[]);	// �� ���� ���� ������� �� ���� ���ſ� ���
void equalAllCase(int set1[], int set2[], int set3[]);	// �� ���� ���� ������� ��� ���԰� ���� ���

void confirmCompare1();				// unsure_1compare�� �Ѿ�� Ȯ�ε��� ���� ������ �Ǻ�
void confirmCompare3andLastIndex();	// unsure_3compare�� �Ѿ�� Ȯ�ε��� ���� ������ �Ǻ��� 99��° �ε��� ���� Ȯ��

int dummySet(int set[]);			// �� ��Ʈ�� ���� �� ���� ������ ���� �� ���� ������ balance ����

void allNormalCoin(int set[]);		// ���� �� ���� ������ �迭�� ������ ��� ����
void allBadCoin(int set[]);			// ���� �� ���� ������ �迭�� ������ ��� �ҷ�

void allCase(int result, int check);// �� ���� ������ ������ �� ���� �� �ִ� ����� ó��

void after3NormalCoin(int index);	// ���ӵ� �� ���� ������ ��� ����
void after3BadCoin(int index);		// ���ӵ� �� ���� ������ ��� �ҷ�
void after9NormalCoin(int index);	// ���ӵ� ��ȩ ���� ������ ��� ����
void after9BadCoin(int index);		// ���ӵ� ��ȩ ���� ������ ��� �ҷ�

int compare_sort(const void* a, const void* b);	// qsort ���̺귯���� ����ϱ� ���� compare

int normal_coin[100];	// Ȯ���� ���� ���� �迭
int bad_coin[100];		// Ȯ���� �ҷ� ���� �迭

int normal_count = 0;	// ���� ���� ����
int bad_count = 0;		// �ҷ� ���� ����

int too_fast_convert_3compare = FALSE; // 9���� ������ Ȯ���ϰ� 333 compare�� �Ѿ� �� ��� �ŷڵ��� ����

int unsure_1compare[33];		// �� ���� �� ������ EQUAL_ALL �� ���� ��� ù��° �ε����� ����
int double_unsure_1compare[11]; // unsure_1compare�� ������ �� ���� �� ������ EQUAL_ALL �� ���� ��� �ش� �ε��� ��� ����

int unsure_3compare[10];		// �� ���� �� ������ EQUAL_ALL �� ���� ��� ù��° �ε����� ����

int unsure_count_1compare = 0;	// unsure_1compare �迭 ���� ���� ����
int double_unsure_count = 0;	// double_unsure_1compare �迭 ���� ���� ����

int unsure_count_3compare = 0;	//// unsure_3compare �迭 ���� ���� ����

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

int main(){
	//�ҷ������� �ʱ�ȭ
	initialize(80);

	// 111 compare�� ����
	compare1Init();

	// 98�� ° �ε������� ���� �� ���¸� �𸣴� �������� ���¸� Ȯ����
	confirmCompare1();
	confirmCompare3andLastIndex();

	// �ҷ� ������ ���� ���� �ε����� ����
	qsort(bad_coin, bad_count, sizeof(int), compare_sort);
	qsort(normal_coin, normal_count, sizeof(int), compare_sort);

	bad_coin[bad_count] = -1;
	b[0] = -1;
	c[0] = -1;

	balance(bad_coin, b, c);

	return 0;
}

// ������ �� ���� ��
void compare1Init() {
	int balance_result;	// balance�� ����� ������
	int i = 0, j = 0, k = 0;
	int index_end = FALSE;	// 98��° �ε������� Ž���ߴ��� Ȯ����

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
		
		// 9��° �ε������� unsure compare�� ������ 2�� �̻��̸� 333 compare�� ��ȯ
		if (i == 6) {
			if (unsure_count_1compare >= 2) {
				// 9���� ����, �ҷ� ������ ���� ���·� ���� �������� ������ �����ϴ� ���� �ŷڵ��� ����
				too_fast_convert_3compare = TRUE;	
				for (i = 9; i < 99; i = i + 9) {
					for (j = 0; j < 3; j++) {
						a3[j] = i + j;
						b3[j] = i + 3 + j;
						c3[j] = i + 6 + j;
					}
					compare3Init();

					// 18��° �ε������� Ȯ���� ������ ������ ��ü�� 23% �̻����� Ȯ��
					if (i == 18 && (((float)(bad_count + normal_count) / (float)(i + 9)) * 100) >= 23) {
						// �ҷ� 1�� ���� 8���� ���� Ư�� ���̽��� �����ϱ� ���� ������ ���°� �������� ġ���� ������ Ȯ��
						// ġ���� ���� ������ 111 compare�� ��ȯ
						if ((bad_count / (bad_count + normal_count) * 100) <= 66 && (bad_count / (bad_count + normal_count) * 100) >= 34) {
							index_end = FALSE;
							break;
						}
					}
					// ������ ���°� �������� ġ���� ������ Ȯ��, ġ���� ���� ������ 111 compare�� ��ȯ
					if (i == 18 && ((float)bad_count / (float)(bad_count + normal_count) * 100) <= 66 && ((float)bad_count / (float)(bad_count + normal_count) * 100) >= 34) {
						index_end = FALSE;
						break;
					}
				}
				if (index_end == TRUE) {
					break;
				}
				else {	// index_end = FALSE
					i=i+6;
					continue;
				}
			}
		}
		else if (i == 15) {
			if (unsure_count_1compare >= 3) {
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

// ���� �� ���� �� ��Ʈ�� �迭 �� �� ��
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

// �� ���� ���� ������� �� ���� ���ſ� ���
void oneHeavyCase(int set1[], int set2[], int set3[]) {
	// ���ſ� ���� �� ���� ��
	int result = dummySet(set1);

	// �� ���� ���� ����� �� ���� ���ſ� ��� (ex. 7 5 5) �̹Ƿ� �� ���� ������ ���´� ��� �ҷ� ������ ��
	if (result == ONE || result == TWO || result == THREE) {
		allCase(result, 0);
		allBadCoin(set2);
		allBadCoin(set3);
	}
	// �� ���� ���� ����� �� ���� ���ſ� ��� ������ ������ ���´� �� �� �����Ƿ� �������� �ٽ� balance�� Ȯ��
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

// �� ���� ���� ������� �� ���� ���ſ� ���
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

// �� ���� ���� ������� ��� ���԰� ���� ���
void equalAllCase(int set1[], int set2[], int set3[]) {
	int result = dummySet(set1);

	if (result != EQUAL_ALL) {
		allCase(result, 0);
		allCase(dummySet(set2), 0);
		allCase(dummySet(set3), 0);
	}
	else {	// EQAUL_ALL �̸� ��� 5���� 7���� �� �� ����
		unsure_3compare[unsure_count_3compare] = set1[0];
		unsure_count_3compare = unsure_count_3compare + 1;
	}
}

// unsure_1compare�� �Ѿ�� Ȯ�ε��� ���� ������ �Ǻ�
void confirmCompare1() {
	int i, j;
	float percentage;	// ���°� Ȯ�ε� ���� �߿��� �ҷ� ������ ����
	
	percentage = ((float)bad_count / (float)(bad_count + normal_count)) * 100;

	// ����� ǥ���� �ְ�, �ҷ� ���� ������ 25% ������ ��� �ڿ� unsure_1compare�� �ش�Ǵ� ������ ��� ���� �������� �ٲ�
	if (percentage <= 25 && too_fast_convert_3compare == FALSE) {
		for (i = 0; i < unsure_count_1compare; i++) {
			after3NormalCoin(unsure_1compare[i]);
		}
	}
	// ����� ǥ���� �ְ�, �ҷ� ���� ������ 75% �̻��� ��� �ڿ� unsure_1compare�� �ش�Ǵ� ������ ��� �ҷ� �������� �ٲ�
	else if (percentage >= 75 && too_fast_convert_3compare == FALSE) {
		for (i = 0; i < unsure_count_1compare; i++) {
			after3BadCoin(unsure_1compare[i]);
		}
	}
	else {	// ����� ǥ���� ���ų�, �߰� Ȯ���� ���
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
			case EQUAL_ALL:
				double_unsure_1compare[double_unsure_count] = a[0];
				double_unsure_1compare[double_unsure_count + 1] = b[0];
				double_unsure_1compare[double_unsure_count + 2] = c[0];
				double_unsure_count=double_unsure_count+3;
				break;
			default:
				printf("���ǵ��� ���� ���");
				break;
			}
		}

		// 3���� ���� �������� ���� ��
		if ((unsure_count_1compare % 3) == 1) {	// �������� 1�϶�
			// ���� ���� �� ���� Ȯ�ε��� ���� ���� �� �� ��
			// �߰� Ȯ���� ����̹Ƿ� �Ǻ��� ���� ������ �ּ� 2�� �̻��̶�� ����
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
				printf("���ǵ��� ���� ���");
				break;
			}
		}
		else if ((unsure_count_1compare % 3) == 2) {	// �������� 2�϶�
			// ���� ���� �� ���� Ȯ�ε��� ���� ���� �� �� ��
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
				printf("���ǵ��� ���� ���");
				break;
			}
		}

		// equal_all�� �Ǻ����� �������� ������ �� �ٽ� equal_all�� ���Դ� ���
		if (double_unsure_count != 0) {
			// �ҷ� ���� ������ 60% �̻��̶�� �ǴܵǸ� Ȯ���� �ҷ� ���� �� ���� Ȯ������ ���� ���� �� �� ��
			if (percentage >= 60) {
					b[0] = bad_coin[0];
					c[0] = bad_coin[1];

				for (j = 0; j < double_unsure_count; j = j + 3) {
					a[0] = double_unsure_1compare[j];
					
					// ����� ���� ���ӵ� �� ���� �ε��� 3������ ���� ���·� ����
					switch (balance(b, c, a)) {
					case THREE:
						after3NormalCoin(double_unsure_1compare[j]);
						after3NormalCoin(double_unsure_1compare[j+1]);
						after3NormalCoin(double_unsure_1compare[j+2]);
						break;
					case EQUAL_ALL:
						after3BadCoin(double_unsure_1compare[j]);
						after3BadCoin(double_unsure_1compare[j + 1]);
						after3BadCoin(double_unsure_1compare[j + 2]);
						break;
					default:
						printf("���ǵ��� ���� ���");
						break;
					}
				}
			}
			else {	// �ҷ� ���� ������ 60% ������ ��� ���� ���� �� ���� �������� ��
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
						printf("���ǵ��� ���� ���");
						break;
					}
				}
			}
		}
	}
}

// unsure_3compare�� �Ѿ�� Ȯ�ε��� ���� ������ �Ǻ��� 99��° �ε��� ���� Ȯ��
void confirmCompare3andLastIndex() {
	int i;
	a3[0] = normal_coin[0];
	if (unsure_count_3compare != 0 && unsure_count_3compare % 2 == 0) {	// unsure_count_3compare�� 0�� �ƴ� ¦��
		// ���� ���� �� ���� Ȯ�ε��� ���� �� ���� ���� ��
		for (i = 0; i < unsure_count_3compare; i = i + 2) {
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
				printf("���ǵ��� ���� ���");
				break;
			}
		}

		// ���� ���� �� ���� ������ �ε��� ��
		a3[1] = normal_coin[1];
		a3[2] = 99;

		switch (dummySet(a3)) {	
		case EQUAL_ONETWO:
			bad_coin[bad_count] = 99;
			bad_count = bad_count + 1;
			break;
		case EQUAL_ALL:
			normal_coin[normal_count] = 99;
			normal_count = normal_count + 1;
			break;
		default:
			printf("���ǵ��� ���� ���\n");
			break;
		}
	}
	else if (unsure_count_3compare != 0 && unsure_count_3compare % 2 == 1) {	// unsure_count_3compare�� Ȧ��
		unsure_3compare[unsure_count_3compare] = 99;
		unsure_count_3compare = unsure_count_3compare + 1;

		for (i = 0; i < unsure_count_3compare; i = i + 2) {
			a3[1] = unsure_3compare[i];
			a3[2] = unsure_3compare[i + 1];

			// unsure_count_3compare�� ������ �ε����� ������ ��� �ڿ� ������ �ε����� �����Ͽ���
			if ((i + 2) >= unsure_count_3compare) {
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
					printf("���ǵ��� ���� ���");
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
					printf("���ǵ��� ���� ���");
					break;
				}
			}
		}
	}
	else {	// unsure_count_3compare�� �ƹ��͵� ���� �� ������ �ε����� Ž��
		// ���� ���� �� ���� ������ �ε��� ��
		a3[0] = normal_coin[0];
		a3[1] = normal_coin[1];
		a3[2] = 99;

		switch (dummySet(a3)) {
		case EQUAL_ONETWO:
			bad_coin[bad_count] = 99;
			bad_count = bad_count + 1;
			break;
		case EQUAL_ALL:
			normal_coin[normal_count] = 99;
			normal_count = normal_count + 1;
			break;
		default:
			printf("���ǵ��� ���� ���");
			break;
		}
	}
}

// �� ��Ʈ�� ���� �� ���� ������ ���� �� ���� ������ balance ����
int dummySet(int set[]) {
	a[0] = set[0];
	b[0] = set[1];
	c[0] = set[2];

	return balance(a, b, c);
}

// ���� �� ���� ������ �迭�� ������ ��� ����
void allNormalCoin(int set[]) {
	int i = 0;

	for (i = 0; i < 3; i++) {
		normal_coin[normal_count] = set[i];
		normal_count = normal_count + 1;
	}
}

// ���� �� ���� ������ �迭�� ������ ��� �ҷ�
void allBadCoin(int set[]) {
	int i = 0;

	for (i = 0; i < 3; i++) {
		bad_coin[bad_count] = set[i];
		bad_count = bad_count + 1;
	}
}

// �� ���� ������ ������ �� ���� �� �ִ� ����� ó��
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
			printf("���ǵ��� ���� ���");
		break;
	default:
		printf("���ǵ��� ���� ���");
		break;
	}
}

// ���ӵ� �� ���� ������ ��� ����
void after3NormalCoin(int index) {
	int i;
	for (i = 0; i < 3; i++) {
		normal_coin[normal_count] = index + i;
		normal_count++;
	}
}

// ���ӵ� �� ���� ������ ��� �ҷ�
void after3BadCoin(int index) {
	int i;
	for (i = 0; i < 3; i++) {
		bad_coin[bad_count] = index + i;
		bad_count++;
	}
}

// ���ӵ� ��ȩ ���� ������ ��� ����
void after9NormalCoin(int index) {
	int i;
	for (i = 0; i < 9; i++) {
		normal_coin[normal_count] = index + i;
		normal_count++;
	}
}

// ���ӵ� ��ȩ ���� ������ ��� �ҷ�
void after9BadCoin(int index) {
	int i;
	for (i = 0; i < 9; i++) {
		bad_coin[bad_count] = index + i;
		bad_count++;
	}
}

// qsort ���̺귯���� ����ϱ� ���� compare
int compare_sort(const void* a, const void* b)
{
	if (*(int *)a > *(int *)b)
		return 1;
	else if (*(int *)a < *(int *)b)
		return -1;
	else
		return 0;
}