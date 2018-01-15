#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fileDuplication.h"

int main(int args, char *kwargs[])
{
    int c;
    int fFlag = 0, dFlag = 0;
    char *path = NULL;

    opterr = 0;
    while ((c = getopt(args, kwargs, "f:d:")) != -1)
    {
        switch(c){
            case 'f':
                fFlag = 1;
                path = optarg;
                break;
            case 'd':
                dFlag = 1;
                path = optarg;
                break;
            case '?':
                if (optopt == 'f' || optopt == 'd')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                return 1;
            default:
                abort();
        }
    }
    if(fFlag){
        DuplicationList dupL = findDuplicate(path);
        printf("--------------------------\n");
        printf("| Duplication File Finder |\n");
        printf("--------------------------\n");
        printf("number of duplication found : %d\n\n", dupL.numberOfDuplication);
        listAllDuplication(dupL);
    }
    if(dFlag){
        removeAllFileProperty(path);
    }
    return 1;
}