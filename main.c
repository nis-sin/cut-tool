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

    // Obtain the option values
    int arg;
    int foption = 0;
    int doption = 0;
    char delimiter = '\t';
    int delimiterIndex = -1;
    char* fieldString = "";

    // Parse command line arguments
    while ((arg = getopt(argc, argv, "-fd")) != -1){
        switch (arg){
            case 'f':
                foption = 1;
                fieldString = argv[optind];
                break;
            case 'd':
                doption = 1;
                delimiterIndex = optind;
                break;
            case '?':
                printf("error\n");
                return 1;
                break;
        }
    }

    // Set file pointer
    char* fileName = argv[argc - 1];
    struct stat64 buf;
    FILE* fp;

    if (fileName[0] == '-' || strcmp(fileName,argv[0]) == 0){
        fp = stdin;
    }
    else {
        // Check if file exists
        if (stat64(fileName, &buf) == -1){
            printf("File does not exist\n");
            return 1;
        }
        
        fp = fopen(fileName, "r");
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

    wint_t c;
    char* strC;
    unsigned int tabCount = 0;

    if (doption == 1){
        delimiter = argv[delimiterIndex][0];
    }

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


        if (c == L'\n'){
            tabCount = 0;
            printf("\n");
        }

        if (delimiter == c) {      // change here to check for delimiter
            tabCount++;
        }
    }

    // free memory and close file
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
