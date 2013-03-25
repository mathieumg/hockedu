///////////////////////////////////////////////////////////////////////////
/// @file NoeudMuretRelatif.h
/// @author Michael Ferris
/// @date 2013-02-16
/// @version 2.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once

#include "PositionObserver.h"
#include "NodeWallAbstract.h"


///////////////////////////////////////////////////////////////////////////
/// @class NoeudMuretRelatif
/// @brief Classe qui représente un muret relatif, donc ceux aux extrimites 
/// gauche et droite de la table.
///
/// @author Michael Ferris
/// @date 2013-02-16
///////////////////////////////////////////////////////////////////////////
class NoeudMuretRelatif :
	public NodeWallAbstract, public PositionObserver
{
public:
    typedef NodeWallAbstract Super;

    NoeudMuretRelatif(const std::string& type);
    virtual ~NoeudMuretRelatif() = 0;

    /// fonction de comparaison de 2 noeuds
    virtual bool equals(NoeudAbstrait* n)
    {
        auto n2 = dynamic_cast<NoeudMuretRelatif*>(n);
        return !!n2 && 
            (*coins_[0] - *n2->coins_[0]).norme2() < 1 && 
            (*coins_[1] - *n2->coins_[1]).norme2() < 1 && 
            Super::equals(n);
    }
	/// Accesseur du coin1
	virtual const Vecteur3& obtenirCoin1() const;
	/// Accesseur du coin2
	virtual const Vecteur3& obtenirCoin2() const;

    virtual void updateObserver( const  PositionSubject* pSubject );


protected:
    /// flag used to ignore PositionSubject signal.
    /// since we are the one setting his position
    /// the callback would create an infinite loop
    bool mUpdatingCornerPosition;


    void init( const Vecteur3& pCorner1, const Vecteur3& pCorner2,PositionSubject* s1,PositionSubject* s2);
	const Vecteur3* coins_[2];
};

