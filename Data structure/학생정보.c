#include <stdio.h>

int main() {
    int name, hobby = 0;
    int number = 0;
    char student[3][3][100];
    for (name = 0; name < 3; name++) {
        printf("\n학생 %d의 이름 : ", name + 1);
        scanf("%s", student[name][0]);
        printf("학생 %d의 학번 : ", name + 1);
        scanf("%s", student[name][1]);
        printf("학생 %d의 취미(공백없이) : ", name + 1);
        scanf("%s", student[name][2]);
    }

    for (name = 0; name < 3; name++) {
        printf("\n\n학생 %d ", name + 1);
        printf("\n\t이름 : %s", student[name][0]);
        printf("\n\t학번 : %s", student[name][1]);
        printf("\n\t취미 : %s", student[name][2]);
        }
}
