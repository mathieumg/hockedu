//////////////////////////////////////////////////////////////////////////////
/// @file UsinePaquet.h
/// @author Mathieu Parent
/// @date 2012-12-30
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>

///////////////////////////////////////////////////////////////////////////
/// @class UsinePaquet
/// @brief Classe Factory (Usine) de base (abstraite) qui fournit l'interface aux autres Usines de Paquets
///
/// @author Mathieu Parent
/// @date 2012-12-30
///////////////////////////////////////////////////////////////////////////
class UsinePaquet
{
public:
	/// Destructeur vide déclaré virtuel pour les classes dérivées.
	inline virtual ~UsinePaquet() {};

	/// Fonction à surcharger pour la création d'un paquet.
	virtual Paquet* creerPaquet() const = 0;

protected:
	/// Constructeur qui prend le nom associé à l'usine.
	UsinePaquet()  {}

};


#define PAQUET_FACTORY_DECLARATION(FactoryName)      \
class UsinePaquet## FactoryName : public UsinePaquet   \
{                                                           \
public:                                                     \
    virtual Paquet* creerPaquet() const;     \
};

#define PAQUET_FACTORY_IMPLEMENTATION(FactoryName)                \
    Paquet* UsinePaquet## FactoryName::creerPaquet() const    \
{  return new FactoryName();  }


PAQUET_FACTORY_DECLARATION(ChatMessage    );
PAQUET_FACTORY_DECLARATION(ConnAutomatique);
PAQUET_FACTORY_DECLARATION(Event          );
PAQUET_FACTORY_DECLARATION(LoginInfo      );
PAQUET_FACTORY_DECLARATION(ChatMessage    );
PAQUET_FACTORY_DECLARATION(UserStatus     );
PAQUET_FACTORY_DECLARATION(GameStatus     );
PAQUET_FACTORY_DECLARATION(GameCreation   );
PAQUET_FACTORY_DECLARATION(GameConnection );
PAQUET_FACTORY_DECLARATION(Maillet        );
PAQUET_FACTORY_DECLARATION(Rondelle       );
PAQUET_FACTORY_DECLARATION(GameEvent      );
PAQUET_FACTORY_DECLARATION(Portal         );
PAQUET_FACTORY_DECLARATION(Bonus          );

// extern pour s'assurer d'allouer la memoire a 1 seule place et une seule fois
extern const std::string PaquetNamesArray[NB_PACKET_TYPES];
extern UsinePaquet* PaquetFactories[NB_PACKET_TYPES];


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




