#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fileDuplication.h"
#include <getopt.h>

static int verbose_flag;
int main(int args, char *kwargs[])
{
    int c;
    int fFlag = 0, cFlag = 0;
    char *path = NULL;
    int option_index = 0;

    opterr = 0;

    static struct option long_options[] =
        {
            /* These options set a flag. */
            {"verbose", no_argument, &verbose_flag, 1},
            {"brief", no_argument, &verbose_flag, 0},
            /* These options don’t set a flag.
             We distinguish them by their indices. */
            {"find", required_argument, 0, 'f'},
            {"clean", required_argument, 0, 'c'},
            {0, 0, 0, 0}};
    while ((c = getopt_long(args, kwargs, "f:c:", long_options, &option_index)) != -1)
    {
        switch(c){
            case 'f':
                fFlag = 1;
                path = optarg;
                break;
            case 'c':
                cFlag = 1;
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
    if(cFlag){
        removeAllFileProperty(path);
    }
    return 1;
}