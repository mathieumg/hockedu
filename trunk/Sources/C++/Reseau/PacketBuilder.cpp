#include "PacketBuilder.h"
#include "GestionnaireReseau.h"

PacketBuilder::PacketBuilder()
:mCurrentByteOrder(NATIVE), mSwapBytes(false), mAddStringLength(true)
{
	mArrStart = NULL;
	mArrSize = new uint16_t(0);
}

PacketBuilder::~PacketBuilder(void)
{
	delete mArrSize;
	free(mArrStart);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn PacketBuilder::setCurrentByteOrder( ByteOrder pNewByteOrder )
///
/// Sets the byte order to use in future function calls.
///
/// @param[in] ByteOrder pNewByteOrder The new byte order to use.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PacketBuilder::setCurrentByteOrder( ByteOrder pNewByteOrder )
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
/// @fn PacketBuilder::addPadding( uint32_t& pPaddingLength )
///
/// Adds padding to the packet.
///
/// @param[in] uint32_t & pPaddingLength The number of padding bytes to add.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PacketBuilder::addPadding( uint32_t pPaddingLength )
{
	uint8_t* paddingArray = new uint8_t[pPaddingLength];
	for(uint32_t i = 0; i < pPaddingLength; ++i)
		paddingArray[i] = 0x00;
	addString(paddingArray, pPaddingLength-1);
	delete paddingArray;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn PacketBuilder::operator=( PacketBuilder& packet )
///
/// /*Description*/
///
/// @param[in] PacketBuilder & pPacket
///
/// @return PacketBuilder&
///
////////////////////////////////////////////////////////////////////////
PacketBuilder& PacketBuilder::operator=( PacketBuilder& pPacket )
{
	if(this != &pPacket)
	{
		free(mArrStart);
		mArrStart = (uint8_t*)realloc(mArrStart, pPacket.getPacketLength());
		*mArrSize = pPacket.getPacketLength();
		memcpy_s(getPacketString(), getPacketLength(), pPacket.getPacketString(), pPacket.getPacketLength());
		includeStringLength(pPacket.mAddStringLength);
		setCurrentByteOrder(pPacket.getCurrentByteOrder());
	}
	return *this;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn PacketBuilder::addString( const char* pStringToAdd, uint32_t pStringLength )
///
/// Adds a string to the packet.
///
/// @param[in] const char * pStringToAdd The string to add to the packet
/// @param[in] uint32_t pStringLength The length of the string
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PacketBuilder::addString( const uint8_t* pStringToAdd, uint32_t pStringLength )
{
    ++pStringLength;
	if(mAddStringLength)
		addInteger(pStringLength);
	mArrStart = (uint8_t*)realloc(mArrStart, *mArrSize + pStringLength);
	memcpy_s(mArrStart + *mArrSize, pStringLength, pStringToAdd, pStringLength);
    *mArrSize = *mArrSize + pStringLength;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn PacketBuilder::addChar( int8_t pCharToAdd )
///
/// Adds a character to the packet
///
/// @param[in] int8_t pCharToAdd The character to add
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PacketBuilder::addChar( int8_t pCharToAdd )
{
	mArrStart = (uint8_t*)realloc(mArrStart, *mArrSize + 1);
	memcpy_s(mArrStart + *mArrSize, 1, &pCharToAdd, 1);
	*mArrSize = *mArrSize + 1;
}

void PacketBuilder::clearBuffer()
{
    delete mArrSize;
    free(mArrStart);
    mArrSize = new uint16_t(0);
    mArrStart = NULL;
}


















