#include <iostream>
#include <memory>

template <class T>
class TDequeElement {
private:
    TDequeElement<T> *prev;
    TDequeElement<T> *next;
    T val;
public:
    TDequeElement();
    
    void set_next(TDequeElement *n);
    void set_prev(TDequeElement *p);
    TDequeElement *& get_prev();
    TDequeElement *& get_next();
    const T & get_val();
    void set_val(const T &val);
    
    virtual ~TDequeElement();
};

template <class T>
class TDeque {
private:
    TDequeElement<T> *head;
    TDequeElement<T> *tail;
    int size;
public:
    TDeque();
    
    void push_back(const T &val);
    void pop_back();
    const T & back();   
     
    void push_front(const T &val);
    void pop_front();
    const T & front();
    
    int length();
    
    virtual ~TDeque();  
};

template <class T>
TDequeElement<T>::TDequeElement() {
    prev = next = nullptr;
}

template <class T>
void TDequeElement<T>::set_next(TDequeElement<T> *next) {
    this->next = next;
}

template <class T>
void TDequeElement<T>::set_prev(TDequeElement<T> *prev) {
    this->prev = prev;
}

template <class T>
TDequeElement<T> *& TDequeElement<T>::get_next() {
    return next;
}

template <class T>
TDequeElement<T> *& TDequeElement<T>::get_prev() {
    return prev;
}

template <class T>
const T & TDequeElement<T>::get_val() {
    return val;
}

template <class T>
void TDequeElement<T>::set_val(const T &val) {
    this->val = val;
}

template <class T>
TDequeElement<T>::~TDequeElement() {
    delete next;
}

template <class T>
TDeque<T>::TDeque() {
    head = tail = nullptr;
    size = 0;
}

template <class T>
const T & TDeque<T>::back() {
    return tail->get_val();
}

template <class T>
void TDeque<T>::push_back(const T &val) {
    TDequeElement<T> *tmp = new TDequeElement<T>;
    tmp->set_val(val);
    tmp->set_next(nullptr);
    tmp->set_prev(tail);
    
    if (tail != nullptr) {
        tail->set_next(tmp);
        tail = tmp;
    } else {
        tail = head = tmp;
    }
    
    ++size;
}

template <class T>
void TDeque<T>::pop_back() {
    if (tail != nullptr) { 
        if (tail != head) {
            TDequeElement<T> *tmp = tail;
            tail = tail->get_prev();
            tail->set_next(nullptr);
            delete tmp;
        } else {
            delete tail;
            head = tail = nullptr;
        }
        
        --size;
    }
}

template <class T>
const T & TDeque<T>::front() {
    return head->get_val();
}

template <class T>
void TDeque<T>::push_front(const T &val) {
    TDequeElement<T> *tmp = new TDequeElement<T>;
    tmp->set_val(val);
    tmp->set_prev(nullptr);
    tmp->set_next(head);
    
    if (head != nullptr) {
        head->set_prev(tmp);
        head = tmp;
    } else {
        head = tail = tmp;
    }
    
    ++size;
}

template <class T>
void TDeque<T>::pop_front() {
    if (tail != nullptr) { 
        if (head != tail) {
            TDequeElement<T> *tmp = head;
            head = head->get_next();
            head->set_prev(nullptr);
            delete tmp;
        } else {
            head = tail = nullptr;
        }
        
        --size;
    }
}

template <class T>
int TDeque<T>::length() {
    return size;
}

template <class T>
TDeque<T>::~TDeque() {
    delete head;
}

int main(int argc, char *argv[]) {
    TDeque<int> deq;
    deq.push_front(1);
    deq.push_front(2);
    std::cout << deq.front() << std::endl;
    deq.pop_front();
    std::cout << deq.front() << std::endl;
    deq.pop_front();
    deq.push_back(3);
    deq.push_front(4);
    std::cout << "Length = " << deq.length() << std::endl;
    std::cout << deq.front() << " " << deq.back() << std::endl;
}
