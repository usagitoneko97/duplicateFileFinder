# Duplicate File Finder

Find duplicate files by compute and compare each cyclic redundancy check (crc) of the file.

## Code implementation
include the header file
```c
#include "fileDuplication.h"
```
obtained a list of duplication by :
```c
DuplicationList dupL = findDuplicate(path_for_searching_duplication);
```
prettyprint the result by:
```c
listAllDuplication(dupL);
```

## Command line usage
```
duplicateFileFinder [-f P] [-d P] 

optional arguments:
    -f P, --find P      find the duplicated file in path (P) specified
    -c P, --clean P     clean the cache file (.property.json) used for temporary
                        storing the property of file
```
