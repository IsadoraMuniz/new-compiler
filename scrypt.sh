flex Cmenos.l
bison -d Cmenos.y
gcc -c *.c
gcc -o Cmenos *.o -ly -lfl
rm *.o
rm *.yy.c
rm *.tab.c
rm *.tab.h
./Cmenos gcd.Cmenos
