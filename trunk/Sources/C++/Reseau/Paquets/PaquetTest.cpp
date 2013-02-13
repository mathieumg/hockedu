#include "PaquetTest.h"

PaquetTest::PaquetTest(  )
{
    message = "";
}


std::string PaquetTest::getMessage() const
{
	return message;
}


void PaquetTest::setMessage( const std::string& message )
{
	this->message = message;
}


PaquetTest::~PaquetTest()
{
	
}
