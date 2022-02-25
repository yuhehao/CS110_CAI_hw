gcc -c ringbuffer.c -o ringbuffer.o -Wpedantic -Wall -Wextra -Werror -std=c89 -fPIC 
gcc -c test.c -o test.o -Wpedantic -Wall -Wextra -Werror -std=c89
gcc -shared -o libringbuffer.so ringbuffer.o
gcc test.o -o dynamicringbuffer -L. -lringbuffer -Wpedantic -Wall -Wextra -Werror -std=c89 -Wl,-R./
# gcc test.o libringbuffer.so -o dynamicringbuffer -Wpedantic -Wall -Wextra -Werror -std=c89
# gcc -I/usr/local/include test.o -o dynamicringbuffer -L/usr/local/lib -Wl,-R/usr/local/lib