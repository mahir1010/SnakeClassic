gcc -c Snake.c
gcc -c main.c 
gcc *.o -o Snake -lSDL2 -lm
rm *.o