#pragma once

#include <stdint.h>
#include <iostream>
#include <stdio.h>

#if PACKET_DECRYPTOR
#include <fstream>
#endif

#ifndef byte
typedef unsigned char byte;
#endif


#ifdef LINUX
#include <string.h>
#endif

#include "NetworkEnums.h"
#include "Vecteur.h"


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
    int8_t readChar() { return readData<int8_t>(); }
    uint8_t readUnsignedChar() { return (uint8_t)readChar(); }
    //Reads a byte from the packet
    byte readByte() { return readUnsignedChar(); }
    //Reads a bool from the packet
    bool readBool() { return readByte() == 1; }

    //Two bytes data types.
    //Reads a short from the packet
    int16_t readShort() { return readData<int16_t>(); }
    uint16_t readUnsignedShort() { return (uint16_t)readShort(); }

    //Four bytes data types.
    //Reads an integer from the packet
    int32_t readInteger() { return readData<int32_t>(); }
    uint32_t readUnsignedInteger() { return (uint32_t)readInteger(); }

    //Reads a float from the packet.
    float readFloat() { return  readData<float>(); }

    //Eight bytes data types.
    //Adds a double from the packet.
    double readDouble() { return readData<double>(); }

    //Reads a 64 bits integer from the packet.
    int64_t read64bInteger() { return readData<int64_t>(); }
    uint64_t readUnsigned64bInteger() { return (uint64_t) read64bInteger(); }

    //Variable length data types.
    //Reads a string from the packet.
	void readString(uint8_t* pReturnString, uint32_t pStringLength);

    //Reads padding from the packet.
    uint8_t* readPadding(uint32_t pPaddingLength);

    //Generic function to read data from the packet.
    template<class T> T readData( );

    void setArrayStart( uint8_t* arrayStart, size_t arraySize);
    void append ( size_t pNewSize, uint8_t* pDataToAdd);
    void setSize (size_t size) { mSize = size; }
    /// Accessors of mArrStart
    inline const uint8_t* getBuffer() const { return mArrStart; }

    void clearBuffer();

    PacketReader& operator>>(int32_t& val){ val = readInteger(); return *this; }
    PacketReader& operator>>(std::string& val)
    { 
        int s = readInteger(); 
        val.resize(s);
        readString((uint8_t*)val.c_str(),s);
        return *this; 
    }
    //Vectors
    template<typename T,int N>
    PacketReader& operator>>(Vecteur<T,N>& pVector) { 
        for(int i=0; i<N; ++i)
        {
            (*this )>> pVector[i];
        }
        return *this; 
    }

    template<typename T>
    PacketReader& operator>>(T& val){ val = readData<T>(); return *this; }
    

#if PACKET_DECRYPTOR
    bool mPrintingToOut;
    std::ifstream mInputFile;
    std::ofstream* mOutputFile;
#endif

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
    size_t mCurrentPosition; // Doit etre de type au moins aussi grand que mSize pour pouvoir donner la position
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
	for(unsigned int i = 0 ; i < sizeof(data.bytes) >> 1; ++i)
	{
		temp = data.bytes[i];
		data.bytes[i] = data.bytes[sizeof(data.bytes) - i - 1];
		data.bytes[(sizeof(data.bytes) - i - 1)] = temp;
	}

	return data.value;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Packet::setCurrentByteOrder( ByteOrder pNewByteOrder )
///
/// Reads the next pDataSize bytes from the packet.
///
/// @param[in] ByteOrder pDataSize The number of bytes to read.
///
/// @return int8_t The data read from the packet.
///
////////////////////////////////////////////////////////////////////////
template<class T>
T PacketReader::readData( )
{
    T wDataRead = (T)0;
#if PACKET_DECRYPTOR
    mInputFile.read((char*)&wDataRead,sizeof(T));
    std::cout << mInputFile.tellg() << std::endl;
    if(mPrintingToOut && mOutputFile)(*mOutputFile )<< " " << wDataRead;
#else
    memcpy_s(&wDataRead, sizeof(T), mArrStart+mCurrentPosition, sizeof(T));
    mCurrentPosition += sizeof(T);
#endif

    return mSwapBytes ? swapBytes(wDataRead) : wDataRead;
}

