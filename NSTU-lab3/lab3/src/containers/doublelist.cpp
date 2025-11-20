#ifndef DOUBLELIST_CPP
#define DOUBLELIST_CPP

template<typename T>
DoubleList<T>::Node::Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}

template<typename T>
DoubleList<T>::DoubleList() : head(nullptr), tail(nullptr), length(0) {}

template<typename T>
DoubleList<T>::~DoubleList() {
    clear();
}

template<typename T>
void DoubleList<T>::addHead(const T& value) {
    Node* newNode = new Node(value);
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    length++;
}

template<typename T>
void DoubleList<T>::addTail(const T& value) {
    Node* newNode = new Node(value);
    if (tail == nullptr) {
        head = tail = newNode;
    } else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
    length++;
}

template<typename T>
bool DoubleList<T>::remove(const T& value) {
    Node* current = head;
    while (current != nullptr) {
        if (current->data == value) {
            if (current->prev != nullptr) {
                current->prev->next = current->next;
            } else {
                head = current->next;
            }
            if (current->next != nullptr) {
                current->next->prev = current->prev;
            } else {
                tail = current->prev;
            }
            delete current;
            length--;
            return true;
        }
        current = current->next;
    }
    return false;
}

template<typename T>
void DoubleList<T>::clear() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    length = 0;
}

template<typename T>
size_t DoubleList<T>::size() const {
    return length;
}

template<typename T>
bool DoubleList<T>::empty() const {
    return head == nullptr;
}

template<typename T>
void DoubleList<T>::print(std::ostream& os) const {
    Node* current = head;
    os << "[";
    while (current != nullptr) {
        os << current->data;
        if (current->next != nullptr) os << " <-> ";
        current = current->next;
    }
    os << "]";
}

#endif
