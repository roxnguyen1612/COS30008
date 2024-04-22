constexpr long long MAX_FIBONACCI = 92; //to have a fixed end limit

#include "FibonacciSequenceIterator.h"
#include <iostream>

// iterator constructor
FibonacciSequenceIterator::FibonacciSequenceIterator(const FibonacciSequenceGenerator& aSequenceObject, long long aStart) noexcept
    : fSequenceObject(aSequenceObject), fIndex(aStart)
{
    if (aStart == 1)
    { //index 1 = fCurrent = 1
        fSequenceObject.reset();
    }
}

// return current Fibonacci number
const long long& FibonacciSequenceIterator::operator*() const noexcept
{
    return *fSequenceObject;
}

// prefix, next Fibonacci number
FibonacciSequenceIterator& FibonacciSequenceIterator::operator++() noexcept
{
    //if has next number -> move on
    if (fSequenceObject.hasNext())
    {
        fSequenceObject.next();
        ++fIndex;
    }
    else
    {
        // Set fIndex out-of-scope to signal the end -> break the loop
        fIndex = MAX_FIBONACCI + 1; 
    }
    return *this;
}

// postfix (extra unused argument)
FibonacciSequenceIterator FibonacciSequenceIterator::operator++(int) noexcept
{
    FibonacciSequenceIterator temp = *this;
    ++(*this);
    return temp;
}

bool FibonacciSequenceIterator::operator==(const FibonacciSequenceIterator& aOther) const noexcept
{
    return this->fIndex == aOther.fIndex && this->fSequenceObject.id() == aOther.fSequenceObject.id();
}

bool FibonacciSequenceIterator::operator!=(const FibonacciSequenceIterator& aOther) const noexcept
{
    return !(*this == aOther);
}

// return new iterator positioned at start
FibonacciSequenceIterator FibonacciSequenceIterator::begin() const noexcept {
    return FibonacciSequenceIterator(fSequenceObject, 1);
}

// return new iterator positioned at limit
FibonacciSequenceIterator FibonacciSequenceIterator::end() const noexcept {
    return FibonacciSequenceIterator(fSequenceObject, MAX_FIBONACCI + 1); 
    //Indicate the end based on limits
}
