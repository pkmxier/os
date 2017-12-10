#ifndef DEQUE_H
#define DEQUE_H
#include <stdlib.h>

struct node{
    struct node *next;
    struct node *prev;
    double val;
};

typedef struct node node;

typedef struct{
    struct node *head;
    struct node *tail;
} deque;

extern int is_empty(deque* q);
extern void destroy(deque* q);
extern deque* create();
extern int push_back(deque* q, double v);
extern int push_front(deque* q, double v);
extern double pop_back(deque* q);
extern double pop_front(deque* q);

#endif //DEQUE_H
