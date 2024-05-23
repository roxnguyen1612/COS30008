#include "List.h"
#include <string>
#include <utility> // for std::swap
#include <iostream> // for debug output

// Constructor
template<typename T>
List<T>::List() noexcept : fHead(nullptr), fTail(nullptr), fSize(0) {
    std::cout << "List constructor called. fHead: " << (fHead ? "not null" : "null") << ", fTail: " << (fTail ? "not null" : "null") << std::endl;
}

// Copy Constructor
template<typename T>
List<T>::List(const List& aOther) : fHead(nullptr), fTail(nullptr), fSize(0) {
    for (auto node = aOther.fHead; node != nullptr; node = node->fNext) {
        push_back(node->fData);
    }
}

// Move Constructor
template<typename T>
List<T>::List(List&& aOther) noexcept : fHead(std::move(aOther.fHead)), fTail(std::move(aOther.fTail)), fSize(aOther.fSize) {
    aOther.fHead = nullptr;
    aOther.fTail = nullptr;
    aOther.fSize = 0;
}

// Copy Assignment Operator
template<typename T>
List<T>& List<T>::operator=(const List& aOther) {
    if (this != &aOther) {
        List temp(aOther); // copy-and-swap idiom
        swap(temp);
    }
    return *this;
}

// Move Assignment Operator
template<typename T>
List<T>& List<T>::operator=(List&& aOther) noexcept {
    if (this != &aOther) {
        fHead = std::move(aOther.fHead);
        fTail = std::move(aOther.fTail);
        fSize = aOther.fSize;
        aOther.fHead = nullptr;
        aOther.fTail = nullptr;
        aOther.fSize = 0;
    }
    return *this;
}

// Swap Function
template<typename T>
void List<T>::swap(List& aOther) noexcept {
    std::swap(fHead, aOther.fHead);
    std::swap(fTail, aOther.fTail);
    std::swap(fSize, aOther.fSize);
}

// Size Accessor
template<typename T>
size_t List<T>::size() const noexcept {
    return fSize;
}

template<typename T>
template<typename U>
void List<T>::push_front(U&& aData) {
    auto newNode = DoublyLinkedList<T>::makeNode(std::forward<U>(aData));
    if (!fHead) { // If list is empty
        fHead = fTail = newNode;
    }
    else { // Non-empty list
        newNode->fNext = fHead;
        fHead->fPrevious = newNode;
        fHead = newNode;
    }
    fSize++;
}

template<typename T>
template<typename U>
void List<T>::push_back(U&& aData) {
    auto newNode = DoublyLinkedList<T>::makeNode(std::forward<U>(aData));
    if (!fTail) { // If list is empty
        fHead = fTail = newNode;
    }
    else { // Non-empty list
        newNode->fPrevious = fTail;
        fTail->fNext = newNode;
        fTail = newNode;
    }
    fSize++;
}

template<typename T>
void List<T>::remove(const T& aElement) noexcept {
    Node current = fHead;
    while (current) {
        if (current->fData == aElement) {
            if (current == fHead) { // Update head
                fHead = fHead->fNext;
                if (fHead) {
                    fHead->fPrevious.reset();
                }
                else { // List is now empty
                    fTail.reset();
                }
            }
            if (current == fTail) { // Update tail
                fTail = fTail->fPrevious.lock();
                if (fTail) {
                    fTail->fNext.reset();
                }
                else { // List is now empty
                    fHead.reset();
                }
            }
            current->isolate();
            fSize--;
            break;
        }
        current = current->fNext;
    }
    // Ensure both fHead and fTail are reset when the list is empty
    if (fSize == 0) {
        fHead.reset();
        fTail.reset();
    }
}

// Access an element by index
template<typename T>
const T& List<T>::operator[](size_t aIndex) const {
    assert(aIndex < fSize && "Index out of bounds.");
    auto node = fHead;
    for (size_t i = 0; i < aIndex; ++i) {
        node = node->fNext;
    }
    return node->fData;
}

template<typename T>
typename List<T>::Iterator List<T>::begin() const noexcept {
    std::cout << "begin() called. fHead: " << (fHead ? "not null" : "null") << ", fTail: " << (fTail ? "not null" : "null") << std::endl;
    if (fHead == nullptr) {
        std::cout << "List is empty. Returning end() iterator." << std::endl;
        return end();  // Return end iterator if the list is empty
    }
    auto it = Iterator(fHead, fTail); // Ensure fCurrent is set to fHead
    std::cout << "Iterator initialized in begin. " << std::endl;
    return it;
}

template<typename T>
typename List<T>::Iterator List<T>::end() const noexcept {
    std::cout << "end() called. fTail: " << (fTail ? "not null" : "null") << std::endl;
    auto it = Iterator(nullptr, nullptr); // Ensure fCurrent is nullptr for the end iterator
    std::cout << "Iterator initialized in end." << std::endl;
    return it;
}



// Define rbegin and rend for reverse iterator support
template<typename T>
typename List<T>::Iterator List<T>::rbegin() const noexcept {
    std::cout << "rbegin() called. fTail: " << (fTail ? "not null" : "null") << ", fHead: " << (fHead ? "not null" : "null") << std::endl;
    if (fTail == nullptr) {
        return rend();  // Return rend iterator if the list is empty
    }
    return Iterator(fTail, fHead);
}

template<typename T>
typename List<T>::Iterator List<T>::rend() const noexcept {
    std::cout << "rend() called. fHead: " << (fHead ? "not null" : "null") << std::endl;
    return Iterator(nullptr, nullptr);
}

// Explicit template instantiation
template class List<int>; // Instantiate for type int, add other types as necessary

// Ensure explicit instantiation is done for each required type and method combination
template class List<std::string>; // For the whole class
template void List<std::string>::push_front<std::string&>(std::string&);  // for lvalue references
template void List<std::string>::push_back<std::string&>(std::string&);   // for lvalue references
template void List<std::string>::push_front<std::string>(std::string&&);  // for rvalue references
template void List<std::string>::push_back<std::string>(std::string&&);   // for rvalue references

// Explicit instantiation for const char(&)[5]
template void List<std::string>::push_front<const char(&)[5]>(const char(&)[5]);
template void List<std::string>::push_back<const char(&)[5]>(const char(&)[5]);
