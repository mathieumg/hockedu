//////////////////////////////////////////////////////////////////////////////
/// @file PaquetString.h
/// @author Mathieu Parent
/// @date 2013-01-24
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "PaquetBase.h"

class UsinePaquetTest;

///////////////////////////////////////////////////////////////////////////
/// @class PaquetTest
/// @brief Classe qui represente un paquet pour faire des tests
///
/// @author Mathieu Parent
/// @date 2013-01-24
///////////////////////////////////////////////////////////////////////////
class PaquetTest : public PaquetBase {
private:
	friend UsinePaquetTest;
	std::string message;
    int mInt;
    float mFloat;
protected:
	PaquetTest();

public:
	
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

