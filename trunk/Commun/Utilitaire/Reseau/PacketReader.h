#pragma once

#include <stdint.h>
#include <iostream>
#include <stdio.h>
#ifndef byte
typedef unsigned char byte;
#endif

enum ByteOrder;

class PacketReader
{
public:
    PacketReader();
	PacketReader( uint8_t* pPacketData );
	~PacketReader(void);

	// Sets the current byte order to the specified value.
	void setCurrentByteOrder(ByteOrder pNewByteOrder);
	// Returns current byte order
	ByteOrder getCurrentByteOrder() {return mCurrentByteOrder;}

    //One byte data types - endianness doesn't matter.
    //Reads a character from the packet.
    int8_t readChar() { return *readData( sizeof(int8_t) ); }
    uint8_t readUnsignedChar() { return (uint8_t)readChar(); }
    //Reads a byte from the packet
    byte readByte() { return readUnsignedChar(); }
    //Reads a bool from the packet
    bool readBool(bool pBoolToAdd) { return readByte() == 1; }

    //Two bytes data types.
    //Reads a short from the packet
    int16_t readShort() { return *((int16_t*)readData( sizeof(int16_t) )); }
    uint16_t readUnsignedShort() { return (uint16_t)readShort(); }

    //Four bytes data types.
    //Reads an integer from the packet
    int32_t readInteger() { return *((int32_t*)readData( sizeof(int32_t) )); }
    uint32_t readUnsignedInteger() { return (uint32_t)readInteger(); }

    //Reads a float from the packet.
    float readFloat() { return  *((float*)readData( sizeof(float) )); }

    //Eight bytes data types.
    //Adds a double from the packet.
    double readDouble() { return *((double*)readData( sizeof(double) )); }

    //Reads a 64 bits integer from the packet.
    int64_t read64bInteger() { return *((int64_t*)readData( sizeof(int64_t) )); }
    uint64_t readUnsigned64bInteger() { return (uint64_t) read64bInteger(); }

    //Variable length data types.
    //Reads a string from the packet.
	uint8_t* readString(uint32_t pStringLength) {  return (uint8_t*)readData(pStringLength); }
    uint8_t* readString() { return (uint8_t*)readData( readInteger() ); }

    //Reads padding from the packet.
    uint8_t* readPadding(uint32_t pPaddingLength);

    //Generic function to read data from the packet.
    int8_t* readData( size_t pDataSize);

    void setArrayStart( uint8_t* arrayStart, size_t arraySize);
    void append ( size_t pNewSize, uint8_t* pDataToAdd);
    void setSize (size_t size) { mSize = size; }

    void clearBuffer();

private:
	// Boolean which determines whether data's bytes should be switched or not.
	bool mSwapBytes;
	// Boolean which determines if string length should be read from the packet when reading a string
	bool mReadStringLength;
	// Swaps endianness
	template<typename T> 
	T swapBytes(T& pValueToSwap);
	// Will contain the current byte order 
	ByteOrder mCurrentByteOrder;

	// Pointer to the start of the array
	uint8_t* mArrStart;

    // Stores current position in the array
    uint16_t mCurrentPosition;
    size_t mSize;
};

////////////////////////////////////////////////////////////////////////
///
/// @fn Packet<T>::swapEndianness( T pValueToSwap )
///
/// Swaps the bytes in pValueToSwap - this result in an endianness swapping
///					(little endian -> big endian and vice-versa)
///
/// @param[in] T pValueToSwap
///
/// @return template<typename T> void
///
////////////////////////////////////////////////////////////////////////
template<typename T>
T PacketReader::swapBytes( T& pValueToSwap )
{
	union
	{
		T value;
		byte bytes[sizeof(T)];
	} data;

	data.value = pValueToSwap;

	char temp;
	for(int i = 0 ; i < sizeof(data.bytes) >> 1; ++i)
	{
		temp = data.bytes[i];
		data.bytes[i] = data.bytes[sizeof(data.bytes) - i - 1];
		data.bytes[(sizeof(data.bytes) - i - 1)] = temp;
	}
	
	return data.value;
}

