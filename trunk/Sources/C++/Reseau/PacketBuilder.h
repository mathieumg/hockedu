#pragma once
#include <stdint.h>
#include <iostream>
#include <stdio.h>
#ifndef byte
typedef unsigned char byte;
#endif
#ifdef LINUX
#include <string.h>
#endif

#include "NetworkEnums.h"
#include "Vecteur.h"

class PacketBuilder
{
public:
	PacketBuilder();
	PacketBuilder(PacketBuilder& p);
	~PacketBuilder();
    void setSize()
    {
        checkf(mArrSize >= 16);
        ((int&)mArrStart[12]) = mArrSize;
    }

	//Sets the current byte order to the specified value.
	void setCurrentByteOrder(ByteOrder pNewByteOrder);
	//Returns current byte order
	ByteOrder getCurrentByteOrder() {return mCurrentByteOrder;}
	//Returns packet length
	uint16_t getPacketLength() { return mArrSize; }
	//Returns the string representing the packet
	uint8_t* getPacketString() { return mArrStart; }

	//One byte data types - endianness doesn't matter.
	//Adds a character to the packet.
	void addChar(int8_t pCharToAdd);
	void addChar(uint8_t pCharToAdd) { addChar((int8_t)pCharToAdd); }
	//Adds a byte to the packet
	void addByte(byte pByteToAdd) { addChar((uint8_t)pByteToAdd); }
	//Adds a bool to the packet
	void addBool(bool pBoolToAdd) { addByte((byte)pBoolToAdd); }

	//Two bytes data types.
	//Adds a short to the packet
	void addShort(int16_t pShortToAdd) { addData(pShortToAdd); }
	void addShort(uint16_t pShortToAdd) { addShort((int16_t)pShortToAdd); }

	//Four bytes data types.
	//Adds an integer to the packet
	void addInteger(int32_t pIntToAdd) { addData(pIntToAdd); }
	void addInteger( uint32_t pIntToAdd) { addInteger((int32_t)pIntToAdd); }

	//Adds a float to the packet.
	void addFloat(float pFloatToAdd) { addData(pFloatToAdd); }

	//Eight bytes data types.
	//Adds a double to the packet.
	void addDouble(double pDoubleToAdd) { addData(pDoubleToAdd); }

	//Adds a 64 bits integer to the packet.
	void add64bInteger(int64_t p64bToAdd) { addData(p64bToAdd); }
	void add64bInteger(uint64_t p64bToAdd) { add64bInteger((int64_t)p64bToAdd); }

	//Variable length data types.
	//Adds a string to the packet.
	void addString(const uint8_t* pStringToAdd, uint32_t pStringLength);
	void addString(uint8_t* pStringToAdd, uint32_t pStringLength) { addString((const uint8_t*)pStringToAdd, pStringLength); }
	void addString(std::string& pStringToAdd) { addString((const uint8_t*)pStringToAdd.c_str(), (uint32_t)pStringToAdd.length()); }

	//Adds padding to the packet.
	void addPadding(uint32_t pPaddingLength);

	//Generic function to add data to the packet.
	template<class T>
	void addData(T& pDataToAdd);

	//Operator overloading - <<
	//Changes byte order.
	PacketBuilder& operator<<(ByteOrder pNewByteOrder) { setCurrentByteOrder(pNewByteOrder); return *this; }
	//Integer types
	PacketBuilder& operator<<(int8_t pCharToAdd) { addChar(pCharToAdd); return *this; }
	PacketBuilder& operator<<(uint8_t pCharToAdd) { addChar(pCharToAdd); return *this; }
	PacketBuilder& operator<<(bool pBoolToAdd) { addBool(pBoolToAdd); return *this; }
	PacketBuilder& operator<<(int16_t pShortToAdd) { addShort(pShortToAdd); return *this; }
	PacketBuilder& operator<<(uint16_t pShortToAdd) { addShort(pShortToAdd); return *this; }
	PacketBuilder& operator<<(int32_t pIntToAdd) { addInteger(pIntToAdd); return *this; }
	PacketBuilder& operator<<(uint32_t pIntToAdd) { addInteger(pIntToAdd); return *this; }
	PacketBuilder& operator<<(int64_t p64bIntToAdd) { add64bInteger(p64bIntToAdd); return *this; }
	PacketBuilder& operator<<(uint64_t p64bIntToAdd) { add64bInteger(p64bIntToAdd); return *this; }
	//Float types
	PacketBuilder& operator<<(float pFloatToAdd) { addFloat(pFloatToAdd); return *this; }
	PacketBuilder& operator<<(double pDoubleToAdd) { addDouble(pDoubleToAdd); return *this; }
	//Strings
	PacketBuilder& operator<<(std::string pStringToAdd) { addString(pStringToAdd); return *this; }
    //Vectors
    template<class T,int N>
    PacketBuilder& operator<<(Vecteur<T,N> pVector) { 
        for(int i=0; i<N; ++i)
        {
            (*this )<< pVector[i];
        }
        return *this; 
    }


	//Generic overload
	template<typename T>
	PacketBuilder& operator<<(T pDataToAdd);
	//PacketBuilder
	PacketBuilder& operator<<(PacketBuilder& pPacketToAdd) { addString(pPacketToAdd.getPacketString(), pPacketToAdd.getPacketLength()); return *this; }

	//Operator overloading - +
	//Changes byte order.
	PacketBuilder& operator+(ByteOrder pNewByteOrder) { setCurrentByteOrder(pNewByteOrder); return *this; }

	//Integer types
	PacketBuilder& operator+(int8_t pCharToAdd) { addChar(pCharToAdd); return *this; }
	PacketBuilder& operator+(uint8_t pCharToAdd) { addChar(pCharToAdd); return *this; }
	PacketBuilder& operator+(bool pBoolToAdd) { addBool(pBoolToAdd); return *this; }
	PacketBuilder& operator+(int16_t pShortToAdd) { addShort(pShortToAdd); return *this; }
	PacketBuilder& operator+(uint16_t pShortToAdd) { addShort(pShortToAdd); return *this; }
	PacketBuilder& operator+(int32_t pIntToAdd) { addInteger(pIntToAdd); return *this; }
	PacketBuilder& operator+(uint32_t pIntToAdd) { addInteger(pIntToAdd); return *this; }
	PacketBuilder& operator+(int64_t p64bToAdd) { add64bInteger(p64bToAdd); return *this; }
	PacketBuilder& operator+(uint64_t p64bToAdd) { add64bInteger(p64bToAdd); return *this; }
	//Float types
	PacketBuilder& operator+(float pFloatToAdd) { addFloat(pFloatToAdd); return *this; }
	PacketBuilder& operator+(double pDoubleToAdd) { addDouble(pDoubleToAdd); return *this; }
	//Strings
	PacketBuilder& operator+(std::string& pStringToAdd) { addString(pStringToAdd); return *this; }
	//Generic overload.
	template<typename T>
	PacketBuilder& operator+(T pDataToAdd);
	//PacketBuilder
	PacketBuilder& operator+(PacketBuilder& pPacketToAdd) { addString(pPacketToAdd.getPacketString(), pPacketToAdd.getPacketLength()); return *this; }

	//Operator overloading - +=
	//Changes byte order.
	PacketBuilder& operator+=(ByteOrder pNewByteOrder) { setCurrentByteOrder(pNewByteOrder); return *this; }
	//Integer types
	PacketBuilder& operator+=(int8_t pCharToAdd) { addChar(pCharToAdd); return *this; }
	PacketBuilder& operator+=(uint8_t pCharToAdd) { addChar(pCharToAdd); return *this; }
	PacketBuilder& operator+=(bool pBoolToAdd) { addBool(pBoolToAdd); return *this; }
	PacketBuilder& operator+=(int16_t pShortToAdd) { addShort(pShortToAdd); return *this; }
	PacketBuilder& operator+=(uint16_t pShortToAdd) { addShort(pShortToAdd); return *this; }
	PacketBuilder& operator+=(int32_t pIntToAdd) { addInteger(pIntToAdd); return *this; }
	PacketBuilder& operator+=(uint32_t pIntToAdd) { addInteger(pIntToAdd); return *this; }
	PacketBuilder& operator+=(int64_t p64bIntToAdd) { add64bInteger(p64bIntToAdd); return *this; }
	PacketBuilder& operator+=(uint64_t p64bIntToAdd) { add64bInteger(p64bIntToAdd); return *this; }
	//Float types
	PacketBuilder& operator+=(float pFloatToAdd) { addFloat(pFloatToAdd); return *this; }
	PacketBuilder& operator+=(double pDoubleToAdd) { addDouble(pDoubleToAdd); return *this; }
	//Strings
	PacketBuilder& operator+=(std::string& pStringToAdd) { addString(pStringToAdd); return *this; }
	//Generic overload.
	template<typename T>
	PacketBuilder& operator+=(T pDataToAdd);
	//PacketBuilder
	PacketBuilder& operator+=(PacketBuilder& pPacketToAdd) { addString(pPacketToAdd.getPacketString(), pPacketToAdd.getPacketLength()); return *this; }

	//Operator overloading - =
	PacketBuilder& operator=(PacketBuilder& pPacket) ;

	//Indicates if the string length should be included when adding strings.
	void includeStringLength(bool pAddStringLength) { mAddStringLength = pAddStringLength;}

    void clearBuffer();



private:
	//Boolean which determines whether data's bytes should be switched or not.
	bool mSwapBytes;
	//Boolean which determines if string length should be added to the packet when adding a string
	bool mAddStringLength;
	//Swaps endianness
	template<typename T>
	T swapBytes(T& pValueToSwap);
	//Will contain the current byte order
	ByteOrder mCurrentByteOrder;

	//Variables used to stock the bytes.
	uint8_t* mArrStart;
	int mArrSize;
};


////////////////////////////////////////////////////////////////////////
///
/// @fn PacketBuilder<T>::operator+=( T pDataToAdd )
///
/// Generic overload for operator +=.
///
/// @param[in] T pDataToAdd The data to add
///
/// @return template<typename T> PacketBuilder&
///
////////////////////////////////////////////////////////////////////////
template<typename T>
PacketBuilder& PacketBuilder::operator+=( T pDataToAdd )
{
	addData(pDataToAdd);
	return *this;
}



////////////////////////////////////////////////////////////////////////
///
/// @fn PacketBuilder<T>::operator+( T pDataToAdd )
///
/// Generic overload for operator +.
///
/// @param[in] T pDataToAdd Data to add to the packet.
///
/// @return template<typename T> PacketBuilder& Reference to the current packet.
///
////////////////////////////////////////////////////////////////////////
template<typename T>
PacketBuilder& PacketBuilder::operator+( T pDataToAdd )
{
	addData(pDataToAdd);
	return *this;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn PacketBuilder::operator<<( T pDataToAdd )
///
/// Generic overloading for operator <<.
///
/// @param[in] T  pDataToAdd The data to add.
///
/// @return PacketBuilder& Reference to the current packet.
///
////////////////////////////////////////////////////////////////////////
template<typename T>
PacketBuilder& PacketBuilder::operator<<( T pDataToAdd )
{
	addData(pDataToAdd);
	return *this;
}



////////////////////////////////////////////////////////////////////////
///
/// @fn PacketBuilder<T>::addValue( T& pDataToAdd )
///
/// Generic function to add data to the packet.
///
/// @param[in] T & pDataToAdd The data to add to the packet
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
template<typename T>
void PacketBuilder::addData( T& pDataToAdd )
{
	mArrStart = (uint8_t*)realloc(mArrStart, mArrSize + sizeof(T));
	if(mSwapBytes)
	{
		T result = swapBytes(pDataToAdd);

        memcpy_s(mArrStart+ mArrSize, sizeof(T), &result, sizeof(T));

		mArrSize += sizeof(T);
		return;
	}

	memcpy_s(mArrStart+ mArrSize, sizeof(T), &pDataToAdd, sizeof(T));

	mArrSize += sizeof(T);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn PacketBuilder<T>::swapEndianness( T pValueToSwap )
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
T PacketBuilder::swapBytes( T& pValueToSwap )
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









