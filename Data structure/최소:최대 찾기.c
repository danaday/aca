#include <stdio.h>
#define SIZE 10

int main(void)
{
	int b[SIZE] = { 12, 3, 9, 6, 18, 8, 12, 4, 1, 19 };
	int max, min;
	int i;

	printf("["); //넣어둔 내용 출력
	for (i = 0; i < SIZE; i++)
	{
		printf("%d ", b[i]);

	}
	printf("]\n");

	min = b[0];
	max = b[0];
	
	for (i = 1; i < SIZE; i++)
	{
		if (b[i] < min)	//최솟값인지 비교
		{
			min = b[i];
		}
		if (b[i] > max)	//최댓값인지 비교
		{
			max = b[i];
		}

	}
    printf("최대값은 %d이고 최소값은 %d입니다.\n", max, min);
    return 0;
}