//////////////////////////////////////////////////////////////////////////////
/// @file SocketObserver.h
/// @author Mathieu Parent
/// @date 2013-01-28
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>
#include "ObserverPaternMacros.h"
#include "Reseau\Socket.h"


///////////////////////////////////////////////////////////////////////////
/// @class SocketObserver
/// @brief Classe qui sert d'observer pour les sockets et qui devra relayer les infos de connections aux differents client. 
///			Par exemple, le Chat l'utilise pour dire aux client connectes que quelqu'un d'autre vient de se connecter ou que quelqu'un est deconnecte
///
/// @author Mathieu Parent
/// @date 2013-01-28
///////////////////////////////////////////////////////////////////////////
class SocketObserverServerChat //:public Observer<Socket>
{

public:
	/// Constructeur par defaut
	SocketObserverServerChat();
	/// Destructeur
	~SocketObserverServerChat();

	virtual void updateObserver( SPSocket pSubject );
    

private:
	


};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




