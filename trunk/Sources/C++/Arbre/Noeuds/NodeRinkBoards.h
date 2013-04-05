///////////////////////////////////////////////////////////////////////////
/// @file NodeRinkBoards.h
/// @author Michael Ferris
/// @date 2013-02-16
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once
#include "NoeudMuretRelatif.h"

class NoeudBut;
class NoeudPoint;

///////////////////////////////////////////////////////////////////////////
/// @class NodeRinkBoards
/// @brief Classe qui représente une bande extérieur de la patinoire, 
///         donc ceux aux extrimites de la table.
///
/// @author Michael Ferris
/// @date 2013-02-16
///////////////////////////////////////////////////////////////////////////
class NodeRinkBoards : public NoeudMuretRelatif
{
public:
    typedef NoeudMuretRelatif Super;
	/// Constructeurs par paramètres
	NodeRinkBoards(NoeudPoint* n1, NoeudPoint* n2);
	NodeRinkBoards(NoeudPoint* n, NoeudBut* but, bool haut);
	NodeRinkBoards(NoeudBut* but, NoeudPoint* n, bool haut);
    virtual ~NodeRinkBoards();

    virtual void updatePhysicBody();
    virtual void updateCornerPosition();

private:
    void initBase();
    /// un des pointeurs peut etre null s'il est ratacher a un but
    NoeudPoint* mPoint1,*mPoint2;

};

