// COS30008, Problem Set 4, 2024

#pragma once

#include "DoublyLinkedList.h"
#include "DoublyLinkedListIterator.h"

template<typename T>
class List
{
private:
    using Node = typename DoublyLinkedList<T>::Node;

    Node fHead;		// first element
    Node fTail;		// last element
    size_t fSize;	// number of elements

public:

    using Iterator = DoublyLinkedListIterator<T>;

    // default constructor	(2)
    List() noexcept : fHead(nullptr), fTail(nullptr), fSize(0)
    {}

    // copy semantics
    List(const List& aOther) : fSize(aOther.fSize)
    {
        fHead = fTail = nullptr;
        Node lNode = aOther.fHead;
        while (lNode != nullptr)
        {
            push_back(lNode->fData);
            lNode = lNode->fNext;
        }
    }

    List& operator=(const List& aOther)
    {
        if (this != &aOther)
        {
            List lTemp(aOther);
            Node lNode = fHead;
            while (lNode)
                //List lTemp(aOther); // copy-and-swap idiom -> swap(lTemp)
            {
                Node lNext = lNode->fNext;
                lNode = lNext;
            }
        }
        return *this;
    }

    // move semantics
    List(List&& aOther) noexcept : fHead(std::move(aOther.fHead)), fTail(std::move(aOther.fTail)), fSize(std::move(aOther.fSize))
    {
        aOther.fHead = nullptr;
        aOther.fTail = nullptr;
        aOther.fSize = 0;
    }

    List& operator=(List&& aOther) noexcept
    {
        if (this != &aOther)
        {
            fHead = std::move(aOther.fHead);
            fTail = std::move(aOther.fTail);
            fSize = aOther.fSize;
            aOther.fHead = nullptr;
            aOther.fTail = nullptr;
            aOther.fSize = 0;
        }
        return *this;
    }

    void swap(List& aOther) noexcept
    {
        std::swap(fHead, aOther.fHead);
        std::swap(fTail, aOther.fTail);
        std::swap(fSize, aOther.fSize);
    }

    // basic operations
    size_t size() const noexcept
    {
        return fSize;
    }

    // add element at front	(24)
    template<typename U>
    void push_front(U&& aData)
    {
        Node lNode = DoublyLinkedList<T>::makeNode(std::forward<U>(aData));
        if (!fHead)
        { // If list is empty
            fHead = fTail = lNode;
        }
        else
        { // Non-empty list
            lNode->fNext = fHead;
            fHead->fPrevious = lNode;
            fHead = lNode;
        }
        fSize++;
    }

    // add element at back	(24)
    template<typename U>
    void push_back(U&& aData)
    {
        Node lNode = DoublyLinkedList<T>::makeNode(std::forward<U>(aData));
        if (!fTail)
        { // If list is empty
            fHead = fTail = lNode;
        }
        else
        {
            lNode->fPrevious = fTail;
            fTail->fNext = lNode;
            fTail = lNode;
        }
        fSize++;
    }

    // remove element		(36)
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
        fSize--;
    }

    // list indexer			(14)
    const T& operator[](size_t aIndex) const
    {
        Node lNode = fHead;
        for (size_t i = 0; i < aIndex; i++)
        {
            lNode = lNode->fNext;
        }
        return lNode->fData;
    }

    // iterator interface
    Iterator begin() const noexcept
    {
        return Iterator(fHead, fTail);
    }

    Iterator end() const noexcept
    {
        return Iterator(fHead, fTail).end();
    }

    Iterator rbegin() const noexcept
    {
        return Iterator(fHead, fTail).rbegin();
    }

    Iterator rend() const noexcept
    {
        return Iterator(fHead, fTail).rend();
    }
};
