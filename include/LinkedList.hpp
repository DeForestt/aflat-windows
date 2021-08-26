/* This linked list class will be nessisary for carrying and
manipulating the strings being built.*/
#ifndef LINK
#define LINK

namespace links{
    
    /*The Node is a genaric class used to hold values
    in the linked list.  It is generic in case I want to
    use it for somthing else in the futer.*/
    
    template<typename T>
    class Node{
        public:
            T data;
            Node * next;
    };

    template<typename T>
    class LinkedList{
        public:
            int count;

            Node<T> * head;
            LinkedList();

            /*Push a new value to the top of the list*/
            void push(T value);

            /*Reverses the direction of the list*/
            void invert();

            /*Sattatches the head of another linked list to the tail of this one*/
            void stitch(links::LinkedList<T> l);
            
            /*stitches the head of this linked list to the tail of another one and sets the head of the other as the new head*/
            void istitch(links::LinkedList<T> l);

            /*clears the list and sets the head to nullptr*/
            void clear();

            /*Pop removes the top eleent for the list
            Behaves likle a stack*/ 
            T pop();

            /*Observes the first node on the list and returns its value withou
            popping it.*/
            T peek();

            /*search acording to passed in logic*/
            template <typename Z>
            T* search(bool(* foo)(T, Z), Z input);
    };
}


template <typename T>
template <typename Z>
T* links::LinkedList<T>::search(bool(* foo)(T, Z), Z input){
        links::Node<T> * pointer = this->head;
        if(foo(this->head, input)) return &this->head->data;
        while (pointer->next != nullptr){
            pointer = pointer->next;
            if(foo(pointer->data, input)){
                return &pointer->data;
            }
        }
        return nullptr;
}

template <typename T>
links::LinkedList<T>::LinkedList(){
    this->count = 0;
    head = nullptr;
}

template <typename T>
void links::LinkedList<T>::clear(){
    if (this->count > 0){
        //this->head = nullptr;
        while (this->count > 0)
        {
            this->pop();
        }
        
    }
    
}

template <typename T>
void links::LinkedList<T>::push(T value){
    this->count += 1;
    Node<T>* push = new Node<T>();
    push->next = this->head;
    push->data = value;
    this->head = push;
}

template <typename T>
void links::LinkedList<T>::invert(){
    Node<T>* prev = nullptr;
    Node<T>* curr = this->head;
    Node<T>* next = nullptr;

    while (curr!= nullptr){
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    this->head = prev;
}

template <typename T>
T links::LinkedList<T>::pop(){
    this->count -= 1;
    T data = this->head->data;
    Node<T>* poper = this->head;
    this->head = this->head->next;
    delete poper;
    return data;
}

template<typename T>
T links::LinkedList<T>::peek(){
    return this->head->data;
}

template<typename T>
void links::LinkedList<T>::stitch(LinkedList<T> l){
    links::Node<T> * pointer = head;
    this->count += l.count;
    if (pointer == nullptr){
        this->head = l.head;
    }
    else
    {
        while (pointer->next != nullptr){
            pointer = pointer->next;
        }
        pointer->next = l.head;
    }
}

template<typename T>
void links::LinkedList<T>::istitch(LinkedList<T> l){
    links::Node<T> * pointer = l.head;
    this->count += l.count;
    if (pointer == nullptr){
        this->head = l.head;
    }
    else
    {
        while (pointer->next != nullptr){
            pointer = pointer->next;
        }
        pointer->next = head;
        this->head = l.head;
    }
}

#endif