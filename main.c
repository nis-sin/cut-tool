#include <stdio.h>
#include <wctype.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>
#include <math.h>

struct node {
    unsigned int field;
    struct node* next;
};

int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "");

    char* fileName = argv[argc - 1];
    struct stat64 buf;

    // Check if file exists
    if (stat64(fileName, &buf) == -1){
        printf("File does not exist\n");
        return 1;
    }

    FILE* fp = fopen(fileName, "r");

    wint_t c;
    char* fieldString;
    int arg;
    int foption = 0;
    unsigned int tabCount = 0;
    char t;

    // Parse command line arguments
    while ((arg = getopt(argc, argv, "-f")) != -1){
        switch (arg){
            case 'f':
                foption = 1;
                fieldString = argv[optind];
                break;
            case '?':
                printf("error\n");
                break;
        }
    }

    // Parse fields string to obtain the fields number

    unsigned int strptr = 0;
    unsigned int maxLength = strlen(fieldString);
    unsigned int field; 
    unsigned int length; 
    struct node* fieldsHead = NULL;
    struct node* fieldsTail = NULL;
    struct node* currentNode = NULL;

    while (strptr < maxLength){
        field = strtol(fieldString, NULL, 10);

        if (field == 0){
            printf("Invalid field\n");
            return 1;
        }

        if (field < 0){
            printf("Fields are numbered from 1\n");
            return 1;
        }

        length = floor(log10(field)) + 1;

        // linked list to store fields
        if (fieldsHead == NULL){
            fieldsHead = (struct node*) malloc(sizeof(struct node));
            if (fieldsHead == NULL){
                printf("Memory allocation failed\n");
                return 1;
            }
            fieldsHead->field = field;
            fieldsHead->next = NULL;
            fieldsTail = fieldsHead;
        }
        else {
            currentNode = (struct node*) malloc(sizeof(struct node));
            if (currentNode == NULL){
                printf("Memory allocation failed\n");
                return 1;
            }
            currentNode->field = field;
            currentNode->next = NULL;
            fieldsTail->next = currentNode;
            fieldsTail = currentNode;
        }

        fieldString += length+1;
        strptr += length+1;
    }

    // Read file and print characters

    while ((c = fgetwc(fp)) != WEOF) {

        if (foption == 1){
            currentNode = fieldsHead;
            while (currentNode != NULL){
                if ((currentNode->field-1) == tabCount){
                    printf("%lc", c);
                    break;
                }
                currentNode = currentNode->next;
            }
        }
        else {
            printf("%lc", c);
        }

        if (iswspace(c)) {
            tabCount++;
            if (c == L'\n'){
                tabCount = 0;
                printf("\n");
            }
        }
    }

    struct node* temp;
    currentNode = fieldsHead;
    while (currentNode != NULL){
        temp = currentNode->next;
        free(currentNode);
        currentNode = temp;
    }

    fclose(fp);
    return 0;
}
