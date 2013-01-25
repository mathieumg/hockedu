#include "PacketReader.h"
#include "GestionnaireReseau.h"


PacketReader::PacketReader()
    : mArrStart(NULL), mCurrentPosition(0), mSize(0)
{

}

PacketReader::PacketReader( uint8_t* pPacketData)
    : mArrStart(pPacketData), mCurrentPosition(0), mSize(0)
{

}


PacketReader::~PacketReader(void)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Packet::setCurrentByteOrder( ByteOrder pNewByteOrder )
///
/// Sets the byte order to use in future function calls.
///
/// @param[in] ByteOrder pNewByteOrder The new byte order to use.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PacketReader::setCurrentByteOrder( ByteOrder pNewByteOrder )
{
	mCurrentByteOrder = pNewByteOrder;

	if(mCurrentByteOrder != NATIVE && mCurrentByteOrder != GestionnaireReseau::NATIVE_BYTE_ORDER)
	{
		mSwapBytes = true;
		return;
	}

	mSwapBytes = false;
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
int8_t* PacketReader::readData( size_t pDataSize)
{
    int8_t* bytes = new int8_t[pDataSize];
    memset(bytes, 0, pDataSize);
    memcpy_s(bytes, pDataSize, mArrStart+mCurrentPosition, pDataSize);
    mCurrentPosition += pDataSize;
    return bytes;
}


void PacketReader::append ( size_t pNewSize, uint8_t* pDataToAdd)
{
    uint8_t* newArrayStart = new uint8_t[pNewSize];
    memcpy_s(newArrayStart, mSize, mArrStart, mSize);
    memcpy_s(newArrayStart + mSize, pNewSize - mSize, pDataToAdd, pNewSize - mSize);
    mSize = pNewSize;
    delete mArrStart;
    mArrStart = newArrayStart;
}

void PacketReader::setArrayStart( uint8_t* arrayStart, size_t arraySize )
{
    uint8_t* newArrayStart = new uint8_t[arraySize];
    memcpy_s(newArrayStart, arraySize, arrayStart, arraySize);
    mSize = arraySize;
    if (mArrStart != NULL)
        delete mArrStart;
    mArrStart = newArrayStart;
}


void PacketReader::clearBuffer()
{
    mCurrentPosition = 0;
    //mArrStart = 0;
}
