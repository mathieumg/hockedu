//////////////////////////////////////////////////////////////////////////////
/// @file PaquetString.h
/// @author Mathieu Parent
/// @date 2013-01-24
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Paquet.h"

class UsinePaquetTest;

///////////////////////////////////////////////////////////////////////////
/// @class PaquetTest
/// @brief Classe qui represente un paquet pour faire des tests
///
/// @author Mathieu Parent
/// @date 2013-01-24
///////////////////////////////////////////////////////////////////////////
class PaquetTest : public Paquet {
private:
	friend class UsinePaquetTest;
	std::string message;
    int mInt;
    float mFloat;
protected:
	PaquetTest();

public:
	virtual PacketTypes getOperation() const { return TEST; }

	std::string getMessage() const;
	void setMessage(const std::string& message);

    int getInt() const { return mInt; }
    void setInt(int val) { mInt = val; }

    float getFloat() const { return mFloat; }
    void setFloat(float val) { mFloat = val; }

	~PaquetTest();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

