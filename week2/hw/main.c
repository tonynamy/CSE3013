#include "header.h"

int main() {
	
	// 변수 정의
	int t, n;

	// 테스트 케이스 개수 입력
	scanf("%d", &t);

	// 각 테스트 케이스에 대하여
	while(t--) {

		// n 입력
		scanf("%d", &n);

		// 개수 계산을 위한 배열 생성
		int* a = (int*) malloc(sizeof(int)*10);
		
		// 배열 초기화
		for(int i=0;i<10;i++)
			a[i] = 0;

		// 1~n 까지의 숫자를 계산하여 배열에 저장
		calc(a, n);

		// 배열을 형식에 맞게 출력
		print(a);
		
		// 변수 할당 해제
		free(a);

	}

	return 0;

}
