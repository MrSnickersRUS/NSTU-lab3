#ifndef SINGLELIST_CPP
#define SINGLELIST_CPP

template<typename T>
SingleList<T>::Node::Node(const T& value) : data(value), next(nullptr) {}

template<typename T>
SingleList<T>::SingleList() : head(nullptr), length(0) {}

template<typename T>
SingleList<T>::~SingleList() {
    clear();
}

template<typename T>
void SingleList<T>::addHead(const T& value) {
    Node* newNode = new Node(value);
    newNode->next = head;
    head = newNode;
    length++;
}

template<typename T>
void SingleList<T>::addTail(const T& value) {
    Node* newNode = new Node(value);
    if (head == nullptr) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
    length++;
}

template<typename T>
bool SingleList<T>::remove(const T& value) {
    if (head == nullptr) return false;
    
    if (head->data == value) {
        Node* temp = head;
        head = head->next;
        delete temp;
        length--;
        return true;
    }
    
    Node* current = head;
    while (current->next != nullptr) {
        if (current->next->data == value) {
            Node* temp = current->next;
            current->next = current->next->next;
            delete temp;
            length--;
            return true;
        }
        current = current->next;
    }
    return false;
}

template<typename T>
bool SingleList<T>::contains(const T& value) const {
    Node* current = head;
    while (current != nullptr) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

template<typename T>
void SingleList<T>::clear() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    length = 0;
}

template<typename T>
size_t SingleList<T>::size() const {
    return length;
}

template<typename T>
bool SingleList<T>::empty() const {
    return head == nullptr;
}

template<typename T>
void SingleList<T>::print(std::ostream& os) const {
    Node* current = head;
    os << "[";
    while (current != nullptr) {
        os << current->data;
        if (current->next != nullptr) os << " -> ";
        current = current->next;
    }
    os << "]";
}

#endif
