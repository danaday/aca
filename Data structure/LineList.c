#include <stdio.h>

int main(void){
    int list[5] = {1,2,4};
    int i = 0;
    int size = 3; //리스트에 있는 원소의 갯수
    int k = 0;
    int lastIn, listAdd, listDel = 0;
    

    printf("\n 현재 선형 리스트 : ");

    for ( i = 0 ; i <= size ; i++){

        if (list[i] == NULL)
            break;
        printf("%2d", list[i]);
    }
    printf("\n 원소의 갯수 :  %d \n", size);

    //원소 삽입
    for( i = 0 ; i < size ; i++){
        if (list[i] <= 3 && 3 <= list[i+1]){
            k = i+1;
            listAdd = i+1;
            lastIn = k;
            break;
        }
    }

    if (i == size){
        k = size;
        lastIn = k;
        }
    for (i = size ; i > k ; i = (i-1)){
        list[i] = list[i-1];
        
    }
    list[k] = 3;


    printf("\n 삽입 후 선형 리스트 : ");

    for ( i=0 ; i<=size ; i++){

        if (list[i] == NULL)
            break;
        printf("%2d", list[i]);
    }
    printf("\n 삽입 후 원소의 갯수 :  %d \n", ++size);
    printf(" 자리 이동 횟수 : %d\n",(lastIn - listAdd + 1));
    size +=1;

    //원소 삭제
    for( i = 0 ; i < size ; i++){
        if (list[i] == 3){
            k = i;
            listDel = i;
            lastIn = k+1;
            break;
        }
    }

    for (i = k ; i < (size-1) ; i++){
        list[i] = list[i+1];
        
    }
    size -= 1; 

    printf("\n 삭제 후 선형 리스트 : ");

    for ( i=0 ; i<=size ; i++){

        if (list[i] == NULL)
            break;
        printf("%2d", list[i]);
    }
    printf("\n 삭제 후 원소의 갯수 :  %d \n", --size);
    printf("\n 자리 이동 횟수 : %d",(lastIn - listDel));
}