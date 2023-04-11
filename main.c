#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "log.txt"
#define MAXLENGTHFILE 5000

char fileStr[MAXLENGTHFILE];

int fileToStr(char* str, const char* filename){
    int status;
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("File does not exist\n");
        return -1;
    }
    status = fread(str, MAXLENGTHFILE, 1, fp);
    fclose(fp);
    fp = NULL;
    return status;
}

int printCmdSet(char* str){
    int count = 0;
    char* token;
    char* cmdSet = "\"cmd\":\"set\"";
    char* ptr = strstr(str, cmdSet);
    token = strtok(str, "\n");
    while (token != NULL){
        if (strstr(token, cmdSet) != NULL){
            printf("%s\n", token);
            count++;
        }
        token = strtok(NULL, "\n");
    }
    return count;
}

void cau1(){
    int count;
    fileToStr(fileStr, FILENAME);
    count = printCmdSet(fileStr);
    printf("So ban tin gui di: %d\n", count);
}

int countCmdSetByNetwork(char* str, const char* network){
    int count = 0;
    char* token;
    char* cmdSet = "\"cmd\":\"set\"";
    token = strtok(str, "\n");
    while (token != NULL){
        if (strstr(token, cmdSet) != NULL && strstr(token, network) != NULL){
            printf("%s\n", token);
            count++;
        }
        token = strtok(NULL, "\n");
    }
    return count;
}

void cau2(){
    char network[5];
    int count;
    printf("Nhap ma network cua thiet bi: ");
    scanf("%s", network);
    fileToStr(fileStr, FILENAME);
    count = countCmdSetByNetwork(fileStr, network);
    printf("So ban tin gui di: %d\n", count);
    return 0;
}

void cau3(){
    fileToStr(fileStr, FILENAME);
    char* msg = strtok(fileStr, "\n"); // tách từng bản tin log
    int index = 0;
    int ind_ep = 0;
    char networkAdd[1000][5]; // danh sách địa chỉ network của công tắc
    char endpoint[1000][2]; // danh sách endpoint của công tắc
    while (msg != NULL) {
        if (strstr(msg, "\"type\":\"switch\"") != NULL && strstr(msg, "\"cmd\":\"set\"")) { // kiểm tra xem bản tin có phải là công tắc hay không
            char *post = strstr(msg, "zwave-");
            char tmp[5];
            tmp[4] = '\0';
            strncpy(tmp, post+6, 4);
            int found = 0;
            for(int i=0;i<=index; i++){
                if(strcmp(tmp, networkAdd[i]) == 0){
                    found = 1;
                    break;
                }
            }
            if(found == 0){
                strcpy(networkAdd[index], tmp);
                index++;
            }
            //------------------------------------------
            char tmpep[2];
            tmpep[1] = '\0';
            strncpy(tmpep, post+13, 1);
            int foundep = 0;
            for(int i=0;i<=ind_ep; i++){
                if(strcmp(tmpep, endpoint[i]) == 0){
                    foundep = 1;
                    break;
                }
            }
            if(foundep == 0){
                strcpy(endpoint[ind_ep], tmpep);
                ind_ep++;
            }

        }
        msg = strtok(NULL, "\n"); // tách bản tin tiếp theo
    }

    printf("Danh sach dia chi cac cong tac nay: \n");

    for(int i=0;i<index;i++){
        printf("thiet bi %d co dia chi la: NWK - %s ENDPOINT - %s\n",i+1,networkAdd[i],endpoint[0]);
    }
}
void cau4(){
    fileToStr(fileStr, FILENAME);
    char* msg = strtok(fileStr, "\n"); // tách từng bản tin log
    int index = 0;
    int ind_ep = 0;
    int count = 0;
    while (msg != NULL) {
        char tmp1[5];
        char tmp2[5];
        char *post = strstr(msg, "reqid");
        tmp1[4] = '\0';
        strncpy(tmp1, post+9, 4);

        msg = strtok(NULL, "\n"); // tách bản tin tiếp theo
        char *post2 = strstr(msg, "reqid");
        tmp2[4] = '\0';
        strncpy(tmp2, post2+9, 4);
        if(strcmp(tmp1, tmp2) != 0){
            count++;
        }
        msg = strtok(NULL, "\n"); // tách bản tin tiếp theo
    }

    printf("So ban tin loi: %d", count);
}

void cau5(){
    int res = 0;
    fileToStr(fileStr, FILENAME);
    char* msg = strtok(fileStr, "\n"); // tách từng bản tin log
    int index = 0;
    int ind_ep = 0;
    int count = 0;
    while (msg != NULL) {
        char tmp1[5];
        char tmp2[5];
        char gio1[3];
        char phut1[3];
        char giay1[7];
        gio1[2] = '\0';
        phut1[2] = '\0';
        giay1[6] = '\0';
        tmp1[4] = '\0';
        char *post = strstr(msg, "reqid");

        strncpy(tmp1, post+9, 4);
        strncpy(gio1, msg+17, 2);

        strncpy(phut1, msg+20, 2);

        strncpy(giay1, msg+23, 6);

        msg = strtok(NULL, "\n"); // tách bản tin tiếp theo
        char *post2 = strstr(msg, "reqid");
        char gio2[3];
        char phut2[3];
        char giay2[7];
        gio2[2] = '\0';
        phut2[2] = '\0';
        giay2[6] = '\0';
        tmp2[4] = '\0';
        strncpy(tmp2, post2+9, 4);
        strncpy(gio2, msg+17, 2);
        strncpy(phut2, msg+20, 2);
        strncpy(giay2, msg+23, 6);

        //------------------------------------------------------


        if(strcmp(tmp1, tmp2) == 0){
            int tmp = (atoi(gio2) - atoi(gio1))* 3600000 + (atoi(phut2)- atoi(phut1))*60000
            + (atof(giay2)- atof(giay1)) * 1000;
            if(tmp > res){
                res = tmp;
            }
        }
        msg = strtok(NULL, "\n"); // tách bản tin tiếp theo
    }
    printf("Do tre lon nhat la : %d Millisecond", res);
}

void cau6(){
    int res = 0;
    int count =0;
    fileToStr(fileStr, FILENAME);
    char* msg = strtok(fileStr, "\n"); // tách từng bản tin log
    int index = 0;
    int ind_ep = 0;
    while (msg != NULL) {
        char tmp1[5];
        char tmp2[5];
        char gio1[3];
        char phut1[3];
        char giay1[7];
        gio1[2] = '\0';
        phut1[2] = '\0';
        giay1[6] = '\0';
        tmp1[4] = '\0';
        char *post = strstr(msg, "reqid");

        strncpy(tmp1, post+9, 4);
        strncpy(gio1, msg+17, 2);

        strncpy(phut1, msg+20, 2);

        strncpy(giay1, msg+23, 6);

        msg = strtok(NULL, "\n"); // tách bản tin tiếp theo
        char *post2 = strstr(msg, "reqid");
        char gio2[3];
        char phut2[3];
        char giay2[7];
        gio2[2] = '\0';
        phut2[2] = '\0';
        giay2[6] = '\0';
        tmp2[4] = '\0';
        strncpy(tmp2, post2+9, 4);
        strncpy(gio2, msg+17, 2);
        strncpy(phut2, msg+20, 2);
        strncpy(giay2, msg+23, 6);

        //------------------------------------------------------


        if(strcmp(tmp1, tmp2) == 0){
            int tmp = (atoi(gio2) - atoi(gio1))* 3600000 + (atoi(phut2)- atoi(phut1))*60000
            + (atof(giay2)- atof(giay1)) * 1000;
            count++;
            res = res + tmp;
        }
        msg = strtok(NULL, "\n"); // tách bản tin tiếp theo
    }
    printf("Do tre trung binh la : %d Millisecond", res/count);
}

int main() {

    cau3();

    cau4();

    cau5();
    cau6();
    return 0;
}
