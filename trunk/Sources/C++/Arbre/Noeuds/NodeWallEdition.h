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
    virtual bool add( NoeudAbstrait* enfant );
    virtual void unlinkChild( const NoeudAbstrait* enfant );
    virtual void renderReal()const;

    /// Creation du noeud XML du muret
    virtual XmlElement* createXmlNode();
    /// Initialisation du NoeudMuret à partir d'un element XML
    virtual bool initFromXml(const XmlElement* element);
    /// Accueil un visiteur
    virtual void acceptVisitor( VisiteurNoeud& v );

    virtual void modifierEchelle( float facteur );
    ///////////////////////////////////////////////////////////////////////////
    /// Implementation interface ControlPointMutableAbstract
protected:
    virtual bool onAddControlPoint(NodeControlPoint* point);
    virtual void onRemoveControlPoint(NodeControlPoint* point);
    ///////////////////////////////////////////////////////////////////////////

};

