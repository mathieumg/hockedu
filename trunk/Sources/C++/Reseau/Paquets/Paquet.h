//////////////////////////////////////////////////////////////////////////////
/// @file Paquet.h
/// @author Mathieu Parent
/// @date 2012-12-01
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include "../GestionnaireReseau.h"
class UsinePaquet;


///////////////////////////////////////////////////////////////////////////
/// @class Paquet
/// @brief Classe qui represente un paquet (abstrait)
///
/// @author Mathieu Parent
/// @date 2012-12-01
///////////////////////////////////////////////////////////////////////////
class Paquet
{
public:
	friend class UsinePaquet;

	virtual ~Paquet() = 0;

	static const std::string sequenceIdentification;
    static int CompteurNumeroPaquet;

	virtual PacketTypes getOperation() const = 0;

    inline int getNbAssociatedQueries() const { return mNbAssociatedQueries; }
    // Attention, si la mauvaise valeur est mise, cela peut causer des GROS problemes
    inline void setNbAssociatedQueries(int pNbAssociatedQueries) { mNbAssociatedQueries = pNbAssociatedQueries; }
    // Fonction qui decremente le compteur de queries associees a ce Paquet, Si compteur = 0, on appelle le destructeur
    void removeAssociatedQuery();

    // Execution du Paquet avec un runnable
    inline int run() { return mRunnableFunction(this); }

    inline void setRunnable(PaquetRunnableFunc pRunnable) { mRunnableFunction = pRunnable; }

    int getNumeroPaquet() const;

protected:
	Paquet();
private:
	int mNbAssociatedQueries; // Nb de fois que le paquet doit etre envoyer (different de 1 si on fait un envoie de masse)
    int mNumeroPaquet;

	PaquetRunnableFunc mRunnableFunction;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

