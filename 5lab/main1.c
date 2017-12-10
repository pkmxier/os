#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "deque.h"

int main(int arcg, char argv[]) {
	int (* is_empty)(deque *q);
	void (* destroy)(deque* q);
	deque * (* create)();
	int (* push_back)(deque* q, double v);
	int (* push_front)(deque* q, double v);
	double (* pop_back)(deque* q);
	double (* pop_front)(deque* q);
	char *error;
	
	void *lib;
	lib = dlopen("libdeque.so", RTLD_LAZY);
	if (!lib) {
		printf("%s\n", dlerror());
		exit(1);
	}
	
	is_empty = dlsym(lib, "is_empty");
	destroy = dlsym(lib, "destroy");
	create = dlsym(lib, "create");
	push_back = dlsym(lib, "push_back");
	push_front = dlsym(lib, "push_front");
	pop_back = dlsym(lib, "pop_back");
	pop_front = dlsym(lib, "pop_front");
	
	if (error = dlerror()) {
		printf("%s\n", error);
		exit(1);
	}
	
	deque *deq = (*create)();
	
	(*push_back)(deq, 1);
	(*push_front)(deq, 2);
	(*push_back)(deq, 3);
	
	int a = (*pop_front)(deq);
	int b = (*pop_back)(deq);
	int c = (*pop_back)(deq);
	
	printf("%d , %d, %d\n", a, b, c);
	
	(*destroy)(deq);
}
