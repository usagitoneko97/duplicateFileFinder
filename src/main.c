#include "fileDuplication.h"

int main(int args, char *kwargs[])
{
    printf("input : %s\n", kwargs[1]);
    char resultPath[256];
    if (args < 2)
    {
        printf("error! Path need to be specified!");
    }
    
    DuplicationList dupL = findDuplicate(kwargs[1]);
    printf("--------------------------\n");
    printf("| Duplication File Finder |\n");
    printf("--------------------------\n");
    printf("number of duplication found : %d\n\n", dupL.numberOfDuplication);
    listAllDuplication(dupL);
    return 0;
}

