
// COS30008, Problem Set 4, 2024

#pragma once

#include "DoublyLinkedList.h"
#include "DoublyLinkedListIterator.h"

template<typename T>
class List
{
private:
    using Node = typename DoublyLinkedList<T>::Node;

    Node fHead;
    Node fTail;
    size_t fSize;

public:

    using Iterator = DoublyLinkedListIterator<T>;

    // Default constructor initializes an empty list
    List() noexcept : fHead(nullptr), fTail(nullptr), fSize(0)
    {}

    // Copy constructor for deep copying of list
    List(const List& aOther) : fSize(aOther.fSize)
    {
        fHead = fTail = nullptr;
        Node lNode = aOther.fHead;
        while (lNode)
        {
            push_back(lNode->fData);
            lNode = lNode->fNext;
        }
    }

    // Copy assigning operator using copy-and-swap idiom for exception safety
    List& operator=(const List& aOther)
    {
        if (this != &aOther)
        {
            List lTemp(aOther);

            Node lNode = fHead;
            while (lNode)
            {
                Node lNext = lNode->fNext;

                lNode = lNext;
            }
        }
        return *this;
    }

    // Move constructor transfers ownership of resources
    List(List&& aOther) noexcept :
        // To prevent double deletion, std::move is used to relocate the pointer to the new object and set the old pointer to nullptr.
        fHead(std::move(aOther.fHead)),
        fTail(std::move(aOther.fTail)),
        fSize(std::move(aOther.fSize))
    {
        aOther.fHead = aOther.fTail = nullptr;
        aOther.fSize = 0;
    }

    // Move assigning operator for transferring resources
    List& operator=(List&& aOther) noexcept
    {
        if (this != &aOther) 
        {
            fHead = std::move(aOther.fHead);
            fTail = std::move(aOther.fTail);
            fSize = aOther.fSize;

            aOther.fHead = aOther.fTail = nullptr;
            aOther.fSize = 0;
        }
        return *this;
    }

    // Swap function for Swapping the content of this list with another list
    void swap(List& aOther) noexcept
    {
        std::swap(fHead, aOther.fHead);
        std::swap(fTail, aOther.fTail);
        std::swap(fSize, aOther.fSize);
    }

    // Size property return the number of elements in the list
    size_t size() const noexcept
    {
        return fSize;
    }

    // Push_front function adds an element to the beginning of the list
    template<typename U>
    void push_front(U&& aData)
    {
        Node lNode = DoublyLinkedList<T>::makeNode(std::forward<U>(aData));

        // Add the new element to the beginning of the list if there is a head.
        if (fHead)
        {
            lNode->fNext = fHead;
            fHead->fPrevious = lNode;
            fHead = lNode;
        }
        else
        {
            fHead = fTail = lNode;
        }

        ++fSize;
    }

    // Push_back function adds an element to the end of the list
    template<typename U>
    void push_back(U&& aData)
    {
        Node lNode = DoublyLinkedList<T>::makeNode(std::forward<U>(aData));

        // Add the new element to the end of the list if there is a tail.
        if (fTail)
        { 
            fTail->fNext = lNode;
            lNode->fPrevious = fTail;
            fTail = lNode;
        }
        else
        {
            fHead = fTail = lNode;
        }

        ++fSize;
    }

    // Remove function to removes the first occurrence of aElement from the list
    void remove(const T& aElement) noexcept
    {
        Node lNode = fHead;

        while (lNode)
        {
            if (lNode->fData == aElement)
            {
                lNode->isolate();
            }
            lNode = lNode->fNext;
        }
        --fSize;
    }

    // Subscript operator: Provide access to the specific element in the list using an index
    const T& operator[](size_t aIndex) const
    {
        Node lNode = fHead;
        for (size_t i = 0; i < aIndex; i++)
        {
            lNode = lNode->fNext;
        }
        return lNode->fData;
    }

    // Iterator interface
   
    //  1. Return an iterator to the beginning of the list 
    Iterator begin() const noexcept
    {
        return Iterator(fHead, fTail);
    }
    //  2. Return an iterator to the end of the list
    Iterator end() const noexcept
    {
        return Iterator(fHead, fTail).end();
    }

    //  3. Return a reverse iterator to the beginning (end of the original list)
    Iterator rbegin() const noexcept
    {
        return Iterator(fHead, fTail).rbegin();
    }
    //  4. Return a reverse iterator to the end (beginning of the original list)
    Iterator rend() const noexcept
    {
        return Iterator(fHead, fTail).rend();
    }
};
