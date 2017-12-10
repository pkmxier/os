#include "deque.h"

int is_empty(deque* q) {
    return q->head == NULL;
}

void destroy(deque* q) {
    free(q);
}

deque* create(){
    deque* q = (deque*) malloc(sizeof(deque));
    if (q != NULL) {
        q->head = q->tail = NULL;
    }
    return q;
}

int push_back(deque* q, double v) {
    node* n = (node*) malloc(sizeof(node));
    if (n == NULL)
        return 0;
    n->val = v;
    n->prev = q->tail;
    n->next = NULL;
    if (q->head == NULL){
        q->head = q->tail = n;
    } else{
        q->tail->next = n;
        q->tail = n;
    }
    return 1;
}

int push_front(deque* q, double v) {
    node* n = (node*) malloc(sizeof(node));
    if (n == NULL)
        return 0;
    n->val = v;
    n->next = q->head;
    n->prev = NULL;
    if (q->tail == NULL) {
        q->tail = q->head = NULL;
    } else {
        q->head->prev = n;
        q->head = n;
    }
    return 1;
}

double pop_back(deque* q) {
    double res = q->tail->val;
    struct node* n = q->tail;
    if (q->head == q->tail) {
        q->head = q->tail = NULL;
    } else {
        q->tail = n->prev;
    }
    free(n);
    return res;
}

double pop_front(deque* q) {
    double res = q->head->val;
    struct node* n = q->head;
    if (q->head == q->tail) {
        q->head = q->tail = NULL;
    } else {
        q->head = n->next;
    }
    free(n);
    return res;
}
