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
/// @brief Classe qui repr�sente un muret relatif qui peut �tre manipuler
///         pour jouer sur le terrain
///
/// @author Michael Ferris
/// @date 2013-02-16
///////////////////////////////////////////////////////////////////////////
class NodeWallEdition : public NoeudMuretRelatif, public ControlPointMutableAbstract
{
public:
    typedef NoeudMuretRelatif Super;
	/// Constructeurs par param�tres
	NodeWallEdition(const std::string& type);
    ~NodeWallEdition();
    /// Ajoute un noeud enfant.
    virtual bool ajouter( NoeudAbstrait* enfant );
    virtual void detacherEnfant( const NoeudAbstrait* enfant );
    virtual void afficherConcret()const;

    /// Creation du noeud XML du muret
    virtual XmlElement* creerNoeudXML();
    /// Initialisation du NoeudMuret � partir d'un element XML
    virtual bool initialiser(const XmlElement* element);


    virtual void modifierEchelle( float facteur );
    ///////////////////////////////////////////////////////////////////////////
    /// Implementation interface ControlPointMutableAbstract
protected:
    virtual bool onAddControlPoint(NodeControlPoint* point);
    virtual void onRemoveControlPoint(NodeControlPoint* point);
    ///////////////////////////////////////////////////////////////////////////

};

