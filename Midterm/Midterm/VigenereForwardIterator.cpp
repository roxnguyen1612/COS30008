#include "VigenereForwardIterator.h"
#include <cctype> 
#include <cassert>
#include <iostream> //for debug purpose

VigenereForwardIterator::VigenereForwardIterator(const std::string& aKeyword, const std::string& aSource, EVigenereMode aMode) noexcept
    : fKeys(KeyProvider(aKeyword, aSource)), fSource(aSource), fMode(aMode), fIndex(0)
{
    initializeTable();
    if (!fSource.empty())
    {
        //fMode switch
        if (fMode == EVigenereMode::Encode)
        {
            encodeCurrentChar();
        }
        else if (fMode == EVigenereMode::Decode)
        {
            decodeCurrentChar();
        }
    }
}

//the mode part
void VigenereForwardIterator::encodeCurrentChar() noexcept
{
    // if non-alpha -> not changed
    if (fIndex >= fSource.length() || !isalpha(fSource[fIndex]))
    {
        fCurrentChar = fSource[fIndex];
    }
    else
    {
        char keyChar = toupper(static_cast<unsigned char>(*fKeys));
        char sourceChar = fSource[fIndex];
        int row = keyChar - 'A'; // the row index from the key character
        int column = toupper(static_cast<unsigned char>(sourceChar)) - 'A';  //the col index from the source character

        fCurrentChar = isupper(sourceChar) ? fMappingTable[row][column] : tolower(fMappingTable[row][column]);

        assert(row >= 0 && row < CHARACTERS); //for debug purpose
        assert(column >= 0 && column < CHARACTERS);
    }
}

void VigenereForwardIterator::decodeCurrentChar() noexcept
{
    if (fIndex >= fSource.length() || !isalpha(fSource[fIndex]))
    {
        fCurrentChar = fSource[fIndex];
    }
    else
    {
        char keyChar = toupper(static_cast<unsigned char>(*fKeys));
        char sourceChar = fSource[fIndex];
        int row = keyChar - 'A';

        // iterates over the column in the specified row
        int column = 0;
        while (column < CHARACTERS && fMappingTable[row][column] != toupper(static_cast<unsigned char>(sourceChar)))
        {
            ++column;
        }

        //compare to source char -> upper || lower
        fCurrentChar = isupper(sourceChar) ? ('A' + column) : tolower('A' + column);
    }
}

char VigenereForwardIterator::operator*() const noexcept
{
    return fCurrentChar;
}

//prefix
VigenereForwardIterator& VigenereForwardIterator::operator++() noexcept
{
    ++fIndex;
    ++fKeys;
    if (fIndex < fSource.size())
    {
        if (fMode == EVigenereMode::Encode)
        {
            encodeCurrentChar();
        }
        else
        {
            decodeCurrentChar();
        }
    }
    return *this;
}

//postfix
VigenereForwardIterator VigenereForwardIterator::operator++(int) noexcept
{
    VigenereForwardIterator temp = *this;
    ++(*this);
    return temp;
}

bool VigenereForwardIterator::operator==(const VigenereForwardIterator& aOther) const noexcept
{
    return fIndex == aOther.fIndex && fSource == aOther.fSource;
}

// Inequality comparison operator
bool VigenereForwardIterator::operator!=(const VigenereForwardIterator& aOther) const noexcept
{
    return !(*this == aOther);
}

VigenereForwardIterator VigenereForwardIterator::begin() const noexcept
{
    return VigenereForwardIterator(*this);
}

VigenereForwardIterator VigenereForwardIterator::end() const noexcept
{
    VigenereForwardIterator temp = *this;
    temp.fIndex = fSource.size();
    return temp;
}