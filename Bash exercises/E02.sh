#!/bin/bash

# Search all files in current directory with extension .c and that include "POSIX" inside them
find . -maxdepth 1 -name "*.c" -exec grep -l "POSIX" '{}' \;

# OR
# grep -l "POSIX" "*.c"  
