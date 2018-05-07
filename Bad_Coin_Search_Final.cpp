#include "stdio.h"
#include "balance.h"
#include "stdlib.h"

#define TRUE	1
#define FALSE	0

void compare1Init();	// 동전을 한 개씩 비교
void compare3Init();	// 동전 세 개가 한 세트인 배열 세 개 비교

void oneHeavyCase(int set1[], int set2[], int set3[]);	// 세 개씩 비교한 결과에서 한 쪽이 무거운 경우
void twoHeavyCase(int set1[], int set2[], int set3[]);	// 세 개씩 비교한 결과에서 두 쪽이 무거운 경우
void equalAllCase(int set1[], int set2[], int set3[]);	// 세 개씩 비교한 결과에서 모두 무게가 같은 경우

void confirmCompare1();				// unsure_1compare로 넘어온 확인되지 않은 동전을 판별
void confirmCompare3andLastIndex();	// unsure_3compare로 넘어온 확인되지 않은 동전을 판별후 99번째 인덱스 상태 확인

int dummySet(int set[]);			// 한 세트가 동전 세 개로 구성된 것을 한 개씩 나눠서 balance 수행

void allNormalCoin(int set[]);		// 동전 세 개로 구성된 배열의 내용이 모두 정상
void allBadCoin(int set[]);			// 동전 세 개로 구성된 배열의 내용이 모두 불량

void allCase(int result, int check);// 한 개씩 동전을 비교했을 때 나올 수 있는 결과를 처리

void after3NormalCoin(int index);	// 연속된 세 개의 동전이 모두 정상
void after3BadCoin(int index);		// 연속된 세 개의 동전이 모두 불량
void after9NormalCoin(int index);	// 연속된 아홉 개의 동전이 모두 정상
void after9BadCoin(int index);		// 연속된 아홉 개의 동전이 모두 불량

int compare_sort(const void* a, const void* b);	// qsort 라이브러리를 사용하기 위한 compare

int normal_coin[100];	// 확정된 정상 동전 배열
int bad_coin[100];		// 확정된 불량 동전 배열

int normal_count = 0;	// 정상 동전 개수
int bad_count = 0;		// 불량 동전 개수

int too_fast_convert_3compare = FALSE; // 9개의 동전만 확인하고 333 compare로 넘어 간 경우 신뢰도가 낮음

int unsure_1compare[33];		// 한 개씩 비교 했을때 EQUAL_ALL 이 나온 경우 첫번째 인덱스를 저장
int double_unsure_1compare[11]; // unsure_1compare의 내용을 한 개씩 비교 했을때 EQUAL_ALL 이 나온 경우 해당 인덱스 모두 저장

int unsure_3compare[10];		// 한 개씩 비교 했을때 EQUAL_ALL 이 나온 경우 첫번째 인덱스를 저장

int unsure_count_1compare = 0;	// unsure_1compare 배열 내의 원소 개수
int double_unsure_count = 0;	// double_unsure_1compare 배열 내의 원소 개수

int unsure_count_3compare = 0;	//// unsure_3compare 배열 내의 원소 개수

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
	//불량동전을 초기화
	initialize(80);

	// 111 compare로 시작
	compare1Init();

	// 98번 째 인덱스까지 수행 후 상태를 모르는 동전들의 상태를 확정함
	confirmCompare1();
	confirmCompare3andLastIndex();

	// 불량 동전과 정상 동전 인덱스를 정렬
	qsort(bad_coin, bad_count, sizeof(int), compare_sort);
	qsort(normal_coin, normal_count, sizeof(int), compare_sort);

	bad_coin[bad_count] = -1;
	b[0] = -1;
	c[0] = -1;

	balance(bad_coin, b, c);

	return 0;
}

// 동전을 한 개씩 비교
void compare1Init() {
	int balance_result;	// balance의 결과를 저장함
	int i = 0, j = 0, k = 0;
	int index_end = FALSE;	// 98번째 인덱스까지 탐색했는지 확인함

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
		
		// 9번째 인덱스에서 unsure compare의 개수가 2개 이상이면 333 compare로 변환
		if (i == 6) {
			if (unsure_count_1compare >= 2) {
				// 9개의 정상, 불량 동전의 비율 상태로 이후 동전들의 비율을 결정하는 것은 신뢰도가 낮음
				too_fast_convert_3compare = TRUE;	
				for (i = 9; i < 99; i = i + 9) {
					for (j = 0; j < 3; j++) {
						a3[j] = i + j;
						b3[j] = i + 3 + j;
						c3[j] = i + 6 + j;
					}
					compare3Init();

					// 18번째 인덱스에서 확정된 동전의 비율이 전체의 23% 이상인지 확인
					if (i == 18 && (((float)(bad_count + normal_count) / (float)(i + 9)) * 100) >= 23) {
						// 불량 1개 정상 8개와 같은 특이 케이스를 제외하기 위해 동전의 상태가 한쪽으로 치우쳐 졌는지 확인
						// 치우쳐 지지 않으면 111 compare로 전환
						if ((bad_count / (bad_count + normal_count) * 100) <= 66 && (bad_count / (bad_count + normal_count) * 100) >= 34) {
							index_end = FALSE;
							break;
						}
					}
					// 동전의 상태가 한쪽으로 치우쳐 졌는지 확인, 치우쳐 지지 않으면 111 compare로 전환
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

// 동전 세 개가 한 세트인 배열 세 개 비교
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

// 세 개씩 비교한 결과에서 한 쪽이 무거운 경우
void oneHeavyCase(int set1[], int set2[], int set3[]) {
	// 무거운 쪽을 한 개씩 비교
	int result = dummySet(set1);

	// 한 개씩 비교한 결과가 한 쪽이 무거운 경우 (ex. 7 5 5) 이므로 이 보다 가벼운 상태는 모두 불량 동전일 때
	if (result == ONE || result == TWO || result == THREE) {
		allCase(result, 0);
		allBadCoin(set2);
		allBadCoin(set3);
	}
	// 한 개씩 비교한 결과가 두 쪽이 무거운 경우 나머지 동전의 상태는 알 수 없으므로 나머지는 다시 balance로 확인
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

// 세 개씩 비교한 결과에서 두 쪽이 무거운 경우
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

// 세 개씩 비교한 결과에서 모두 무게가 같은 경우
void equalAllCase(int set1[], int set2[], int set3[]) {
	int result = dummySet(set1);

	if (result != EQUAL_ALL) {
		allCase(result, 0);
		allCase(dummySet(set2), 0);
		allCase(dummySet(set3), 0);
	}
	else {	// EQAUL_ALL 이면 모두 5인지 7인지 알 수 없음
		unsure_3compare[unsure_count_3compare] = set1[0];
		unsure_count_3compare = unsure_count_3compare + 1;
	}
}

// unsure_1compare로 넘어온 확인되지 않은 동전을 판별
void confirmCompare1() {
	int i, j;
	float percentage;	// 상태가 확인된 동전 중에서 불량 동전의 비율
	
	percentage = ((float)bad_count / (float)(bad_count + normal_count)) * 100;

	// 충분한 표본이 있고, 불량 동전 비율이 25% 이하일 경우 뒤에 unsure_1compare에 해당되는 내용은 모두 정상 동전으로 바꿈
	if (percentage <= 25 && too_fast_convert_3compare == FALSE) {
		for (i = 0; i < unsure_count_1compare; i++) {
			after3NormalCoin(unsure_1compare[i]);
		}
	}
	// 충분한 표본이 있고, 불량 동전 비율이 75% 이상일 경우 뒤에 unsure_1compare에 해당되는 내용은 모두 불량 동전으로 바꿈
	else if (percentage >= 75 && too_fast_convert_3compare == FALSE) {
		for (i = 0; i < unsure_count_1compare; i++) {
			after3BadCoin(unsure_1compare[i]);
		}
	}
	else {	// 충분한 표본이 없거나, 중간 확률일 경우
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
				printf("정의되지 않은 결과");
				break;
			}
		}

		// 3으로 나눠 떨어지지 않을 때
		if ((unsure_count_1compare % 3) == 1) {	// 나머지가 1일때
			// 정상 동전 두 개와 확인되지 않은 동전 한 개 비교
			// 중간 확률일 경우이므로 판별된 정상 동전은 최소 2개 이상이라고 생각
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
		else if ((unsure_count_1compare % 3) == 2) {	// 나머지가 2일때
			// 정상 동전 한 개와 확인되지 않은 동전 두 개 비교
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

		// equal_all로 판별받은 동전끼리 비교했을 때 다시 equal_all이 나왔던 경우
		if (double_unsure_count != 0) {
			// 불량 동전 비율이 60% 이상이라고 판단되면 확정된 불량 동전 두 개와 확정되지 않은 동전 한 개 비교
			if (percentage >= 60) {
					b[0] = bad_coin[0];
					c[0] = bad_coin[1];

				for (j = 0; j < double_unsure_count; j = j + 3) {
					a[0] = double_unsure_1compare[j];
					
					// 결과에 따라 연속된 세 개의 인덱스 3묶음을 한쪽 상태로 보냄
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
						printf("정의되지 않은 결과");
						break;
					}
				}
			}
			else {	// 불량 동전 비율이 60% 이하일 경우 정상 동전 두 개를 기준으로 비교
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

// unsure_3compare로 넘어온 확인되지 않은 동전을 판별후 99번째 인덱스 상태 확인
void confirmCompare3andLastIndex() {
	int i;
	a3[0] = normal_coin[0];
	if (unsure_count_3compare != 0 && unsure_count_3compare % 2 == 0) {	// unsure_count_3compare가 0이 아닌 짝수
		// 정상 동전 한 개와 확인되지 않은 두 개의 동전 비교
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
				printf("정의되지 않은 결과");
				break;
			}
		}

		// 정상 동전 두 개와 마지막 인덱스 비교
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

			// unsure_count_3compare의 마지막 인덱스의 도착한 경우 뒤에 마지막 인덱스를 삽입하여비교
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
	else {	// unsure_count_3compare에 아무것도 없을 때 마지막 인덱스만 탐색
		// 정상 동전 두 개와 마지막 인덱스 비교
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
			printf("정의되지 않은 결과");
			break;
		}
	}
}

// 한 세트가 동전 세 개로 구성된 것을 한 개씩 나눠서 balance 수행
int dummySet(int set[]) {
	a[0] = set[0];
	b[0] = set[1];
	c[0] = set[2];

	return balance(a, b, c);
}

// 동전 세 개로 구성된 배열의 내용이 모두 정상
void allNormalCoin(int set[]) {
	int i = 0;

	for (i = 0; i < 3; i++) {
		normal_coin[normal_count] = set[i];
		normal_count = normal_count + 1;
	}
}

// 동전 세 개로 구성된 배열의 내용이 모두 불량
void allBadCoin(int set[]) {
	int i = 0;

	for (i = 0; i < 3; i++) {
		bad_coin[bad_count] = set[i];
		bad_count = bad_count + 1;
	}
}

// 한 개씩 동전을 비교했을 때 나올 수 있는 결과를 처리
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

// 연속된 세 개의 동전이 모두 정상
void after3NormalCoin(int index) {
	int i;
	for (i = 0; i < 3; i++) {
		normal_coin[normal_count] = index + i;
		normal_count++;
	}
}

// 연속된 세 개의 동전이 모두 불량
void after3BadCoin(int index) {
	int i;
	for (i = 0; i < 3; i++) {
		bad_coin[bad_count] = index + i;
		bad_count++;
	}
}

// 연속된 아홉 개의 동전이 모두 정상
void after9NormalCoin(int index) {
	int i;
	for (i = 0; i < 9; i++) {
		normal_coin[normal_count] = index + i;
		normal_count++;
	}
}

// 연속된 아홉 개의 동전이 모두 불량
void after9BadCoin(int index) {
	int i;
	for (i = 0; i < 9; i++) {
		bad_coin[bad_count] = index + i;
		bad_count++;
	}
}

// qsort 라이브러리를 사용하기 위한 compare
int compare_sort(const void* a, const void* b)
{
	if (*(int *)a > *(int *)b)
		return 1;
	else if (*(int *)a < *(int *)b)
		return -1;
	else
		return 0;
}