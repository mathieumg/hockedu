///////////////////////////////////////////////////////////////////////////////
/// @file NoeudPoint.cpp
/// @author Samuel Ledoux, Michael Ferris
/// @date 2012-02-7
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "NoeudPoint.h"
#include "Utilitaire.h"
#include "ZoneEdition.h"
#include "Terrain.h"
#include "NoeudTable.h"
#include "XMLUtils.h"
#include "VisiteurNoeud.h"


const float NoeudPoint::DEFAULT_RADIUS = 22;

CreateListDelegateImplementation(TableControlPoint)
{
    return RazerGameUtilities::CreateListSphereDefault(pModel,NoeudPoint::DEFAULT_RADIUS);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudPoint::NoeudPoint( const std::string& typeNoeud, 
/// float coordX, float coordY, int typePosNoeud )
///
/// Constructeur assignant les paramètres donner aux attributs de la 
/// classe.
///
/// @param[in] const std::string & typeNoeud : le type de noeud.
/// @param[in] float coordX : la coordonne en X du noeud.
/// @param[in] float coordY : la coordonne en Y du noeud.
/// @param[in] int typePosNoeud : un int représentant le enum du 
///									type de position du noeud, i.e. HG.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudPoint::NoeudPoint( const std::string& typeNoeud, float coordX, float coordY, TypePosPoint typePosNoeud)
	: NoeudComposite(typeNoeud) , longueurCote_(2.0f), typePosNoeud_(typePosNoeud)
{
    // Assigner le rayon par défaut le plus tot possible car la suite peut en avoir besoin
    setDefaultRadius(DEFAULT_RADIUS);
    // Il ne faut aps utiliser le modificateur de position relative, car il ne faut pas affecter le modele 3D a la construction des points
    NoeudAbstrait::setPosition(Vecteur3(coordX,coordY, 0));
	modifierPositionInitiale(mPosition);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::~NoeudPoint()
///
/// Destructeur de la classe
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudPoint::~NoeudPoint()
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::afficherConcret() const
///
/// Fait le reel rendu de l'objet.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPoint::afficherConcret() const
{
	// Appel à la version de la classe de base pour l'affichage des enfants.
	NoeudComposite::afficherConcret();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::animer( const float& temps)
///
/// Permet l'animation du point.
///
/// @param[in] float temps :  la duree de l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPoint::animer( const float& temps )
{
	mAngle = (float)((int)(mAngle+temps*500.0f)%360);
	updateMatrice();

#if WIN32
	glPushMatrix();
	glLoadMatrixd(matrice_); // Chargement de la matrice du noeud
	glRotated(90, 1.0, 0.0, 0.0);
	glGetDoublev(GL_MODELVIEW_MATRIX, matrice_); // Savegarde de la matrice courante dans le noeud
	glPopMatrix(); // Recuperation de la matrice d'origine
#endif

}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::acceptVisitor(VisiteurNoeud& v)
///
/// Permet d'accueillir un visiteur pour faire un opération qui 
///	vient de l'extérieur.
///
/// @param[in] VisiteurNoeud & v : le visiteur contenant l'operation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPoint::acceptVisitor( VisiteurNoeud& v )
{
	v.visiterNoeudPoint(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::obtenirLongueur() const
///
/// Retourne la longueur d'un cote du carre représentant le point.
///
///
/// @return float : la longueur du cote d'un point.
///
////////////////////////////////////////////////////////////////////////
float NoeudPoint::obtenirLongueur() const
{
	return longueurCote_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::obtenirTypePosNoeud() const
///
/// Retourne le type de position du noeud selon le enum, i.e. HG = hautGauche_
///
///
/// @return int : le type de position du noeud.
///
////////////////////////////////////////////////////////////////////////
TypePosPoint NoeudPoint::obtenirTypePosNoeud() const
{
	return typePosNoeud_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::getPosition() const  
///
/// Retourne la position absolue du noeud.
///
///
/// @return Vecteur3 : position du noeud.
///
////////////////////////////////////////////////////////////////////////
Vecteur3 NoeudPoint::getPosition() const 
{
	return Vecteur3(mPosition);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::obtenirPointSym() const
///
/// Retourne un pointeur sur le noeud symétrique.
///
///
/// @return NoeudPoint* : pointeur sur un noeud point symétrique.
///
////////////////////////////////////////////////////////////////////////
NoeudPoint* NoeudPoint::obtenirPointSym() const
{
	return pointSym_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::modifierPointSym( NoeudPoint* pointSym )
///
/// Modifie le pointeur sur le noeud symétrique.
///
/// @param[in] NoeudPoint * pointSym : le point symetrique au point courant.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPoint::modifierPointSym( NoeudPoint* pointSym )
{
	pointSym_=pointSym;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::validerDeplacement( Vecteur3 pos, Vecteur2 deplace, int axe )
///
/// Valide le déplacement selon la limite in ou out et selon l'axe ( x ou y )
///
/// @param[in] Vecteur3 pos : la position a valider.
/// @param[in] Vecteur3 deplace : le vecteur de deplacement desire.
/// @param[in] int axe : l'axe a verifier.
///
/// @return bool : si le deplacement est valide ou non.
///
////////////////////////////////////////////////////////////////////////
bool NoeudPoint::validerDeplacement( const Vecteur3& pos, const Vecteur2& deplace, int axe )
{
	Terrain* t = GetTerrain();
	if( t )
	{
		ZoneEdition* zone = t->getZoneEdition();
        checkf(zone,"Tentative de déplacer un point sans zone édition, s'assurer qu'on est en mode édition");
        if(zone)
        {
            Vecteur3 deplace2(deplace[VX],deplace[VY],0);
            Vecteur3 cible=pos+deplace2;
            float valeurLimiteInt = (axe == VX) ? zone->obtenirLimiteIntLongueur() : zone->obtenirLimiteIntLargeur();
            if(abs(cible[axe]) < valeurLimiteInt)
                return false;

            float valeurLimiteExt = (axe == VX) ? zone->obtenirLimiteExtLongueur() : zone->obtenirLimiteExtLargeur();
            if(abs(cible[axe]) > valeurLimiteExt)
                return false;

            if(pos[axe]*cible[axe]<0)
                return false;
        }
	}
	return true;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPoint::obtenirRayon() const
///
/// Retourne le rayon du noeud.
///
/// @param[in]	void
///
/// @return float : rayon du noeud.
///
////////////////////////////////////////////////////////////////////////
float NoeudPoint::obtenirRayon() const
{
	return longueurCote_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur3 NoeudPoint::calculerDeplacementMax( Vecteur3 posAbsActuel, Vecteur3 deplacement )
///
/// Calcule et retourne le deplacement maximal d'un point de contrôle dans la direction du vecteur de déplacement
/// Le point retourner est donc un 
///
/// @param[in] Vecteur3 posAbsActuel
/// @param[in] Vecteur3 deplacement
///
/// @return Vecteur3
///
////////////////////////////////////////////////////////////////////////
Vecteur3 NoeudPoint::calculerDeplacementMax( Vecteur3 posAbsActuel, Vecteur3 deplacement )
{
	return Vecteur3(0,0,0);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* NoeudPoint::creerNoeudXML()
///
/// Creation du noeud XML d'un point
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NoeudPoint::creerNoeudXML()
{
	XmlElement* elementNoeud = NoeudComposite::creerNoeudXML();

    XMLUtils::writeAttribute<int>(elementNoeud,"typePosNoeud",typePosNoeud_);

	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudPoint::initialiser( const XmlElement* element )
///
/// Initialisation du NoeudPoint à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudPoint::initialiser( const XmlElement* element )
{
    // faire l'initialisaiton des attribut concernant le point en premier pour que la suite puisse les utiliser
	int intElem;
    if(!XMLUtils::readAttribute(element,"typePosNoeud",intElem))
		return false;
	typePosNoeud_ = TypePosPoint(intElem);

    if(!NoeudComposite::initialiser(element))
        return false;

	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn const GroupeTripleAdresseFloat* NoeudPoint::obtenirListePointsAChanger() const
///
/// Accesseur de la liste de point du modele de la table associe a ce point d'ancrage
///
///
/// @return GroupeTripleAdresseFloat
///
////////////////////////////////////////////////////////////////////////
const GroupeTripleAdresseFloat* NoeudPoint::obtenirListePointsAChanger() const
{
	ListeIndexPoints::iterator iter = NoeudTable::obtenirListeIndexPointsModeleTable().find(typePosNoeud_);
	if(iter != NoeudTable::obtenirListeIndexPointsModeleTable().end())
		return &iter->second;
	return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudPoint::assignerPositionRelative( const Vecteur3& positionRelative )
///
/// Assigne la position relative du noeud et deplace le modele 3D de la table
///
/// @param[in] const Vecteur3 & positionRelative
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPoint::assignerPositionRelative( const Vecteur3& positionRelative )
{
	const GroupeTripleAdresseFloat* liste = obtenirListePointsAChanger();
	if(liste)
	{
		Vecteur3 deplacement(positionRelative-mPosition);
		for(unsigned int i=0; i<liste->size(); i++)
		{
			*(liste->get(i)[VX]) += (float)deplacement[VX];
			*(liste->get(i)[VY]) += (float)deplacement[VY];
		}
	}

    // assigner la position du point en premier pour que la table puisse l'utiliser à sa mise a jour
    NoeudAbstrait::setPosition(positionRelative);
    PositionSubject::signalObservers();
}





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

