all: main.o main1.o libdeque.so
	gcc main.o -L. -ldeque -Wl,-rpath,. -o static
	gcc main1.o -ldl -o dynamic
main_static.o: main.c
	gcc -c main.c
main_dynamic.o: main1.c
	gcc -c main1.c
libdeque.so: deque.o
	gcc -shared deque.o -o libdeque.so
deque.o: deque.c
	gcc -c -fPIC deque.c
clean:
	rm -rf *.o *.so a.out

