///////////////////////////////////////////////////////////////////////////
/// @file NodeWallEdition.h
/// @author Michael Ferris
/// @date 2013-02-16
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once
#include "NoeudMuretRelatif.h"
#include "ControlPointMutableAbstract.h"

class NodeControlPoint;
///////////////////////////////////////////////////////////////////////////
/// @class NodeWallEdition
/// @brief Classe qui représente un muret relatif qui peut être manipuler
///         pour jouer sur le terrain
///
/// @author Michael Ferris
/// @date 2013-02-16
///////////////////////////////////////////////////////////////////////////
class NodeWallEdition : public NoeudMuretRelatif, public ControlPointMutableAbstract
{
public:
    typedef NoeudMuretRelatif Super;
	/// Constructeurs par paramètres
	NodeWallEdition(const std::string& type);
    ~NodeWallEdition();
    /// Ajoute un noeud enfant.
    virtual bool ajouter( NoeudAbstrait* enfant );
    virtual void detacherEnfant( const NoeudAbstrait* enfant );
    virtual void afficherConcret()const;

    ///////////////////////////////////////////////////////////////////////////
    /// Implementation interface ControlPointMutableAbstract
    virtual void modifierEchelle( float facteur );
protected:
    virtual bool onAddControlPoint(NodeControlPoint* point);
    virtual void onRemoveControlPoint(NodeControlPoint* point);
    ///////////////////////////////////////////////////////////////////////////

};

