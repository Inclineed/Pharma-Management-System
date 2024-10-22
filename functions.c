#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Medicine{
    char name[50];
    char batchnumber[20];
    int quantity;
    float price;
    char expriyDate[11];
};

int med_write(struct Medicine *med,FILE* fptr){
    if (fptr == NULL) {
        return -1;
    }
    int result=fprintf(fptr,"%s,%s,%d,%.2f,%s\n",med->name,med->batchnumber,med->quantity,med->price,med->expriyDate);
    if (result < 0) {
        return -1; 
    }
    return 0;
}

int main(){
    FILE* fptr;
    fptr=fopen("../inventory.csv","w+");

    if(fptr==NULL){
        printf("Error opening file\n");
        return 1;
    }

    struct Medicine *med = (struct Medicine *)malloc(sizeof(struct Medicine));
    strcpy(med->name,"Paracetemol");
    strcpy(med->batchnumber,"qwerty12345");
    med->quantity=50;
    med->price=10;
    strcpy(med->expriyDate,"20/10/2025");
    fprintf(fptr,"%s,%s,%s,%s,%s\n","Name","BatchNumber","Quantity","Price","Expiry Date");
    int w_msg=med_write(med,fptr);
    printf("%s\n%s\n%d\n%f\n%s",med->name,med->batchnumber,med->quantity,med->price,med->expriyDate);
}