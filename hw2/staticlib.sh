gcc -c ringbuffer.c -o ringbuffer.o -Wpedantic -Wall -Wextra -Werror -std=c89
gcc -c test.c -o test.o -Wpedantic -Wall -Wextra -Werror -std=c89
ar rcs libringbuffer.a ringbuffer.o
ld test.o -o staticringbuffer -lc -L. -l:libringbuffer.a
