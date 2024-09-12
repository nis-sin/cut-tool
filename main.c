#include <stdio.h>
#include <wctype.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>
#include <math.h>


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
    unsigned int fields[maxLength];
    unsigned int index = 0;

    memset(fields, 0, maxLength * sizeof(int));

    while (strptr < maxLength){
        field = strtol(fieldString, NULL, 10);

        if (field == 0){
            printf("Invalid field\n");
            return 1;
        }

        length = floor(log10(field)) + 1;
        fields[index] = field;

        index++;
        fieldString += length+1;
        strptr += length+1;
    }

    for (int i = 0; i < maxLength; i++){
        printf("%d\n", fields[i]);
    }

    return 0;

    // Read file and print characters
    while ((c = fgetwc(fp)) != WEOF) {

        if (foption == 1){
            if ((fieldString - 1) == tabCount){
                printf("%lc", c);
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
}
