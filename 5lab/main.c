#include <stdio.h>
#include "deque.h"

int main(int arcg, char argv[]) {
    deque *deq = create();
    
    push_back(deq, 1);
    push_front(deq, 2);
    push_front(deq, 3);
    
    pop_back(deq);
    pop_front(deq);
    pop_back(deq);
    
    destroy(deq);
}
