
// COS30008, Problem Set 4, 2024
// Chi Cuong Nguyen, 104222057

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
    
    List() noexcept :
        fHead(nullptr), 
        fTail(nullptr), 
        fSize(0) 
    {}

    // copy semantics
    List( const List& aOther ) :
        fSize(aOther.fSize)
    {
        fHead = fTail = nullptr;
        Node lNode = aOther.fHead;
	    while (lNode)
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
		    {
			    Node lNext = lNode->fNext;
			  
			    lNode = lNext;
		    }
	    }
	    return *this;
    }

    // move semantics
    List( List&& aOther ) noexcept :
        // std::move is used to move the pointer to the new object and set the old pointer to nullptr to avoid double deletion
        fHead(std::move(aOther.fHead)), 
        fTail(std::move(aOther.fTail)),
        fSize(std::move(aOther.fSize))
	{
		aOther.fHead = aOther.fTail = nullptr;
		aOther.fSize = 0;
	}

    List& operator=(List&& aOther) noexcept
    {
        if (this != &aOther) {
            fHead = std::move(aOther.fHead);
            fTail = std::move(aOther.fTail);
            fSize = aOther.fSize;
            aOther.fHead = aOther.fTail = nullptr;
            aOther.fSize = 0;
        }
        return *this;
    }

    void swap( List& aOther ) noexcept
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
    
    template<typename U>
    void push_front( U&& aData )
    {
        Node lNode = DoublyLinkedList<T>::makeNode(std::forward<U>(aData));
        if (fHead) { // if there is a head, add the new node to the front of the list
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
    
    template<typename U>
    void push_back(U&& aData)
    {
        Node lNode = DoublyLinkedList<T>::makeNode(std::forward<U>(aData));
        if (fTail) { // if there is a tail, add the new node to the end of the list
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
