# Duplicate File Finder


## Code implementation
include the header file
```c
#include "fileDuplication.h"
```
obtained a list of duplication by :
```
DuplicationList dupL = findDuplicate(path_for_searching_duplication);
```
prettyprint the result by:
```
listAllDuplication(dupL);
```

## usage
```
duplicateFileFinder [-f P] [-d P] 

optional arguments:
    -f P, --find P      find the duplicated file in path (P) specified
    -c P, --clean P     clean the cache file (.property.json) used for temporary
                        storing the property of file
```
