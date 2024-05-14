#include "ifstream12.h"

#include <iostream>
#include <cstddef>

// Constructor: Initializes the ifstream12 object.
ifstream12::ifstream12(const char* aFileName, size_t aBufferSize)
    : fBuffer(new std::byte[aBufferSize]), fBufferSize(aBufferSize)          // Acquire the necessary buffer memory
{
    // Open the input file
    if (aFileName)
        open(aFileName);

    reset();
}

// Destructor: Clean up the memory
ifstream12::~ifstream12()
{
    // Close the file stream
    close();

    // Free the buffer memory
    delete[] fBuffer;
}


// open method: Open the binary input file
void ifstream12::open(const char* aFileName)
{
    if (aFileName)
        fIStream.open(aFileName, std::ios::binary);
}

// close method: Close the input file
void ifstream12::close()
{
    fIStream.close();
}


// isOpen property: Check whether the file stream is opened or not
bool ifstream12::isOpen() const
{
    return fIStream.is_open();
}

// good property: Check whether the file stream is "good"
bool ifstream12::good() const
{
    return fIStream.good();
}

// eof property: Check whether the file stream reaches the end
bool ifstream12::eof() const
{
    return fByteCount == 0 && fIStream.eof();
}

// reset method: Reset the buffer, and its indices
void ifstream12::reset()
{
    for (size_t i = 0; i < fBufferSize; i++)
        fBuffer[i] = static_cast<std::byte>(0);

    fByteCount = 0;
    fByteIndex = 0;
    fBitIndex = 7;
}


// fetch_data method: Read data into the buffer from the file stream.
void ifstream12::fetch_data()
{
    reset();

    fIStream.read(reinterpret_cast<char*>(fBuffer), fBufferSize);

    fByteCount = fIStream.gcount();
}


// readBit method: Read a bit from the buffer, returning a value of 0, 1, (or End-Of-File).
std::optional<size_t> ifstream12::readBit()
{
    // Trigger fetch_data if necessary (fByteCount = 0)
    if (fByteCount == 0)
    {
        fetch_data();
        if (fByteCount == 0) return std::nullopt; // No more data to read
    }

    // Check if EOF has been reached
    if (eof())
    {
        return std::nullopt;  // Return nullopt if no more data
    }

    // Read the next bit
    std::byte lByte = fBuffer[fByteIndex] & (std::byte{ 1 } << fBitIndex);
    size_t bitValue = std::to_integer<size_t>(lByte) ? 1 : 0;

    // std::cout << bitValue;

    // If fBitIndex is a negative number, then switch to the next byte in the buffer, and reduce the value of fByteCount by 1
    fBitIndex--;
    if (fBitIndex < 0)
    {
        fBitIndex = 7;
        fByteIndex++;
        fByteCount--;

        // std::cout << "|";

        // Fetch the data one more time if the buffer is full
        if (fByteIndex >= fBufferSize)
        {
            fetch_data();
        }
    }

    // The sum of fByteIndex and fByteCount is equal to the current value of fIStream.gcount, 
    // normally, this should be equal to the value of fBufferSize. 
    // However, in the last chunk, the fIStream.gcount value is less than fBufferSize, 
    // due to the number of remaining binary strings is the remainder of dividing the total number of binary strings (input 12-bit number) by fBufferSize

    // Check if it is in the last chunk
    if (fByteIndex + fByteCount < fBufferSize)
    {
        // If so, we need to check if the total number of binary strings (input 12-bit number) is an odd number, 
        // Basically, this means that fByteCount has the value of 1, 
        // as there will be 4 last bits left added to the buffer without triggering the derement of fByteCount
        // Hence, we need to derease it by 1
        if (fByteCount == 1) fByteCount -= 1;
    }

    return bitValue;
}


// Operator >>: Read a value using the readBit function.
ifstream12& ifstream12::operator>>(size_t& aValue)
{
    aValue = 0;

    // The Loop to read the saved 12 bits using readBit.
    for (int i = 0; i < 12; i++)
    {
        auto bit = readBit();

        // If readBit return nothing, break the loop
        if (!bit.has_value()) break;

        else if (bit == 1)
        {
            aValue += (bit.value() << i);
        }
    }

    // std::cout << "; fByteCount: " << fByteCount << " - fByteIndex: " << fByteIndex << " - EOF of fIStream: " << fIStream.eof() << std::endl;

    return *this;
}