#include "KeyProvider.h"
#include <cassert>
#include <cctype>

std::string KeyProvider::preprocessString(const std::string& aString) noexcept
{
    std::string lResult;
    for (char ch : aString)
    {
        if (std::isalpha(static_cast<unsigned char>(ch))) //static_cast<unsigned char>() to prevent out of scope later
        {
            lResult += std::toupper(static_cast<unsigned char>(ch));
        }
    }
    return lResult;
}

KeyProvider::KeyProvider(const std::string& aKeyword, const std::string& aSource) noexcept
{
    fIndex = 0;

    std::string lProcessedSource = preprocessString(aSource);
    std::string lProcessedKeyword = preprocessString(aKeyword);
    fKeys = "";

    while (fKeys.length() < lProcessedSource.length())  // to make sure that the fkey >= source
    {
        fKeys += lProcessedKeyword;
    }
    fKeys = fKeys.substr(0, lProcessedSource.length());  // then trim to make key = source length
    assert(fKeys.length() == lProcessedSource.length());
}

char KeyProvider::operator*() const noexcept
{
    if (fIndex < fKeys.size())
    {
        return fKeys[fIndex];
    }
    // if index is out of range -> reset
    return fKeys[fIndex % fKeys.size()];
}

// prefix
KeyProvider& KeyProvider::operator++() noexcept
{
    ++fIndex;
    fIndex %= fKeys.size();  // to double ensure it reset
    return *this;
}

// postfix
KeyProvider KeyProvider::operator++(int) noexcept
{
    KeyProvider temp = *this;
    ++(*this);
    return temp;
}

bool KeyProvider::operator==(const KeyProvider& aOther) const noexcept
{
    return fKeys == aOther.fKeys && fIndex == aOther.fIndex;
}

bool KeyProvider::operator!=(const KeyProvider& aOther) const noexcept
{
    return !(*this == aOther);
}

KeyProvider KeyProvider::begin() const noexcept
{
    KeyProvider temp(*this);
    temp.fIndex = 0;
    return temp;
}

KeyProvider KeyProvider::end() const noexcept
{
    KeyProvider temp(*this);
    temp.fIndex = fKeys.size();
    return temp;
}
