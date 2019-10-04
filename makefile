run : sudo.o fill.o num.o main.o
	gcc -o run sudo.o fill.o num.o main.o
sudo.o : sudo.c
	gcc -c -o sudo.o sudo.c
fill.o : fill.c
	gcc -c -o fill.o fill.c
num.o : num.c
	gcc -c -o num.o num.c
main.o : main.c
	gcc -c -o main.o main.c