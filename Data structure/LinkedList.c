#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma once

typedef struct ListNode {
    char data[5];    
    struct ListNode* link;
} listNode;

//head 노드 구조체로 정의
typedef struct {
    listNode* head;
} linkedList_h;

linkedList_h* createLinkedList_h(void);
void freeLinkedList_h(linkedList_h* L);
void printList(linkedList_h * L);
void insertFirstNode(linkedList_h * L, char*x);
void insertMiddleNode(linkedList_h * L, listNode * pre , char* x);
void insertLastNode(linkedList_h * L, char* x);
void deleteNode(linkedList_h* L, listNode* p);
listNode* searchNode(linkedList_h* L, char* x);

linkedList_h* createLinkedList_h(void){
    linkedList_h* L;
    L = (linkedList_h*)malloc(sizeof(linkedList_h));
    L -> head = NULL;
    return L;
}
//연결 리스트의 전체 메모리를 해제
void freeLinkedList_h(linkedList_h* L){
    listNode* p;
    while(L -> head != NULL){
        p = L -> head;
        L -> head = L -> head -> link;
        free(p);
        p = NULL;
    }
}

//연결 리스트를 순서대로 출력
void printList(linkedList_h* L){
    listNode* p;
    printf("L = (");
    p = L -> head;
    while(p!=NULL){
        printf("%s", p ->data);
        p = p -> link;
        if(p != NULL) printf(" , ");
    }
    printf(") \n");
}

void insertFirstNode(linkedList_h* L, char* x){
    listNode* newNode;
    newNode = (listNode*)malloc(sizeof(listNode));
    strcpy(newNode -> data, x);
    newNode -> link = L -> head;
    L -> head = newNode;
}

void insertMiddleNode(linkedList_h* L, listNode *pre, char*x){
    listNode* newNode;
    newNode = (listNode*)malloc(sizeof(listNode));
    strcpy(newNode -> data, x);
    if(L -> head == NULL){
        newNode -> link = NULL;
        L -> head = newNode;
    }
    else if(pre== NULL){
        newNode -> link = L -> head;
        L -> head = newNode;
    }
    else{
        newNode -> link = pre -> link;
        pre -> link = newNode;
    }
}
void insertLastNode(linkedList_h* L, char* x){
    listNode* newNode;
    listNode* temp;
    newNode =(listNode*)malloc(sizeof(listNode));
    strcpy(newNode -> data,x);
    newNode -> link = NULL;
    if(L -> head == NULL){
        L -> head = newNode;
        return;
    }
    temp = L -> head;
    while(temp -> link != NULL) {
        temp = temp -> link;
    }
    temp -> link = newNode;
}
void deleteNode(linkedList_h* L, listNode* p){
    listNode* pre;
    if(L -> head == NULL)
        return;
    if (L -> head -> link == NULL){
        free(L -> head);
        L -> head = NULL;
        return;
    }
    else if (p ==NULL)
        return;
    else{
        pre= L -> head;
        while(pre -> link != p){
            pre = pre -> link;
        }
        pre -> link = p -> link;
        free(p);
    }
}

listNode* searchNode(linkedList_h* L, char* x){
    listNode *temp;
    temp = L -> head;
    while(temp != NULL) {
        if(strcmp(temp -> data, x) == 0)
            return temp;
        else
            temp = temp -> link;
    }
    return temp;
}

int main(void){
    linkedList_h* L;
    listNode *p;
    L = createLinkedList_h();

    printf("(1) 나의 성을 제외하고, 우리나라의 5가지 성을 입력한다. \n");
    insertLastNode(L,"강");
    insertLastNode(L,"박");
    insertLastNode(L,"최");
    insertLastNode(L,"신");
    insertLastNode(L,"백");
    printList(L);
    
    printf("(2) 세 번째 입력한 성 [최]를 탐색한다. \n");
    p = searchNode(L, "최");
    if(p==NULL)
        printf("찾는 데이터가 없습니다.\n");
    else
        printf("[%s]를 찾았습니다. \n", p -> data);

    printf("(3) 나의 성을 (2)번에서 탐색한 성 [최] 뒤에 삽입한다. \n");
    insertMiddleNode(L, p, "고");
    printList(L);

    printf("(4) 두번쨰 성 [박]을 삭제한다. \n");
    p = searchNode(L, "박");
    deleteNode(L, p);
    printList(L);
}