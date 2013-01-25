#include "PaquetString.h"

PaquetString::PaquetString( )
	:PaquetBase("String")
{
    mTaillePaquet += 4;
    message = "";
}


std::string PaquetString::getMessage() const
{
	return message;
}


void PaquetString::setMessage( const std::string& message )
{
    if( this->message != "" )
        mTaillePaquet -= (message.length() + 1);
	this->message = message;
    mTaillePaquet += message.length()+1;
}


PaquetString::~PaquetString()
{
	
}
