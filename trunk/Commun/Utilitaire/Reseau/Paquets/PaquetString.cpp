#include "PaquetString.h"

PaquetString::PaquetString( )
	:PaquetBase("String")
{
    message = "";
}


std::string PaquetString::getMessage() const
{
	return message;
}


void PaquetString::setMessage( const std::string& message )
{
	this->message = message;
}


PaquetString::~PaquetString()
{
	
}
