#include <stdio.h>
#include <wctype.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <sys/stat.h>


int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "");

    char* fileName = argv[argc - 1];
    struct stat64 buf;

    if (stat64(fileName, &buf) == -1){
        printf("File does not exist\n");
        return 1;
    }

    FILE* fp = fopen(fileName, "r");

    wint_t c;
    char* fields;
    int arg;
    int foption = 0;
    unsigned int tabCount = 0;
    char t;

    while ((arg = getopt(argc, argv, "-f")) != -1){
        switch (arg){
            case 'f':
                foption = 1;
                fields = argv[optind];
                break;
            case '?':
                printf("error\n");
                break;
        }
    }


    while ((c = fgetwc(fp)) != WEOF) {

        if (foption == 1){
            if ((fields - 1) == tabCount){
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
