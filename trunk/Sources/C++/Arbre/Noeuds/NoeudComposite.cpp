////////////////////////////////////////////////
/// @file   NoeudComposite.cpp
/// @author DGI-2990
/// @date   2007-01-25
///
/// @addtogroup inf2990 INF2990
/// @{
////////////////////////////////////////////////

#include "NoeudComposite.h"
#include <algorithm>

extern HANDLE mutexNoeuds;


////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudComposite::NoeudComposite( const std::string& type /*= std::string( "" ) */ )
///
/// Ne fait qu'appeler la version de la classe de base.
///
/// @param[in] const std::string & type : Le type du noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudComposite::NoeudComposite(
	const std::string& type //= std::string( "" )
	) :
NoeudAbstrait(type)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudComposite::~NoeudComposite(  )
///
/// Destructeur qui détruit tous les enfants du noeud.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudComposite::~NoeudComposite()
{
	vider();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn unsigned int NoeudComposite::calculerProfondeur(  )
///
/// Cette fonction calcule la profondeur de l'arbre incluant le noeud
/// courant ainsi que tous ses enfants.
///
/// Cette fonction retourne toujours 1 de plus que la profondeur de son
/// enfants le plus profond.
///
///
/// @return unsigned int : La profondeur de l'arbre sous ce noeud.
///
////////////////////////////////////////////////////////////////////////
unsigned int NoeudComposite::calculerProfondeur() const
{
	unsigned int profondeurEnfantMax = 0;

	for (ConteneurNoeuds::const_iterator it = enfants_.begin();
		it != enfants_.end();
		it++) {
			const unsigned int profondeurEnfant = (*it)->calculerProfondeur();

			if (profondeurEnfantMax < profondeurEnfant)
				profondeurEnfantMax = profondeurEnfant;
	}

	return profondeurEnfantMax + 1;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::vider(  )
///
/// Cette fonction vide le noeud de tous ses enfants.  Elle effectue une
/// itération prudente sur les enfants afin d'être assez robuste pour
/// supporter la possibilité qu'un enfant en efface un autre dans son
/// destructeur, par exemple si deux objets ne peuvent pas exister l'un
/// sans l'autre.  Elle peut toutefois entrer en boucle infinie si un
/// enfant ajoute un nouveau noeud lorsqu'il se fait effacer.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::vider()
{
	// L'itération doit être faite ainsi pour éviter les problèmes lorsque
	// le desctructeur d'un noeud modifie l'arbre, par exemple en retirant
	// d'autres noeuds.  Il pourrait y avoir une boucle infinie si la
	// desctruction d'un enfant entraînerait l'ajout d'un autre.
	while (!enfants_.empty()) {
		NoeudAbstrait* enfantAEffacer = enfants_.front();
		enfants_.erase(enfants_.begin());
		delete enfantAEffacer;
		enfantAEffacer = 0;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::effacer( const NoeudAbstrait* noeud )
///
/// Efface un noeud qui est un enfant ou qui est contenu dans un des
/// enfants.
///
/// @param[in] const NoeudAbstrait * noeud : Le noeud à effacer.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::effacer( const NoeudAbstrait* noeud )
{
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		it++) {
			if (*it == noeud) {
				// On a trouvé le noeud à effacer
				NoeudAbstrait* noeudAEffacer = (*it);
				enfants_.erase(it);
				delete noeudAEffacer;
				noeud = 0;
				return;
			}
			else {
				// On cherche dans les enfants.
				(*it)->effacer( noeud );
			}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn const NoeudAbstrait* NoeudComposite::chercher( const std::string& typeNoeud )
///
/// Recherche un noeud d'un type donné parmi le noeud courant et ses
/// enfants.  Version constante de la fonction.
///
/// @param[in] const std::string & typeNoeud : Le type du noeud cherché.
///
/// @return const NoeudAbstrait* : Noeud recherché ou 0 si le noeud n'est pas trouvé.
///
////////////////////////////////////////////////////////////////////////
const NoeudAbstrait* NoeudComposite::chercher(
	const std::string& typeNoeud
	) const
{
	if ( typeNoeud == type_ ) {
		return this;
	}
	else {
		for (ConteneurNoeuds::const_iterator it = enfants_.begin();
			it != enfants_.end();
			++it) {
				NoeudAbstrait* noeud = (*it)->chercher( typeNoeud );
				if ( noeud != 0 ) {
					return noeud;
				}
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* NoeudComposite::chercher( const std::string& typeNoeud )
///
/// Recherche un noeud d'un type donné parmi le noeud courant et ses
/// enfants.
///
/// @param[in] const std::string & typeNoeud : Le type du noeud cherché.
///
/// @return NoeudAbstrait* : Noeud recherché ou 0 si le noeud n'est pas trouvé.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait* NoeudComposite::chercher(const std::string& typeNoeud)
{
	if ( typeNoeud == type_ ) {
		return this;
	}
	else {
		for (ConteneurNoeuds::iterator it = enfants_.begin();
			it != enfants_.end();
			++it) {
				NoeudAbstrait* noeud = (*it)->chercher( typeNoeud );
				if ( noeud != 0 ) {
					return noeud;
				}
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn const NoeudAbstrait* NoeudComposite::chercher( unsigned int indice )
///
/// Retourne le i-ème enfant, où i est l'indice passé à la fonction.
/// Version constante de la fonction.
///
/// @param[in] unsigned int indice : L'indice de l'enfant cherché.
///
/// @return const NoeudAbstrait* : Noeud recherché ou 0 si le noeud n'est pas trouvé.
///
////////////////////////////////////////////////////////////////////////
const NoeudAbstrait* NoeudComposite::chercher( unsigned int indice ) const
{
	if ( (indice >= 0) && (indice < enfants_.size()) ) {
		return enfants_[indice];
	}
	else {
		return 0;
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* NoeudComposite::chercher( unsigned int indice )
///
/// Retourne le i-ème enfant, où i est l'indice passé à la fonction.
///
/// @param[in] indice : L'indice de l'enfant cherché.
///
/// @return Noeud recherché ou 0 si le noeud n'est pas trouvé.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait* NoeudComposite::chercher( unsigned int indice )
{
	if ( (indice >= 0) && (indice < enfants_.size()) ) {
		return enfants_[indice];
	}
	else {
		return 0;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudComposite::ajouter( NoeudAbstrait* enfant )
///
/// Ajoute un noeud enfant au noeud courant.
///
/// @param[in] NoeudAbstrait * enfant : Noeud à ajouter.
///
/// @return bool : Vrai si l'ajout a réussi, donc en tout temps pour cette classe.
///
////////////////////////////////////////////////////////////////////////
bool NoeudComposite::ajouter( NoeudAbstrait* enfant )
{
	// un noeud ne peut s'avoir comme parent
	if(this == enfant)
		return false;
	// Ce noeud appartient dejà à un autre noeud, alors on le détache au préalable
	if(enfant->obtenirParent() != NULL)
	{
		if( enfant->obtenirParent() == this)
			return true;
		enfant->obtenirParent()->detacherEnfant(enfant);
	}

	enfant->assignerParent(this);
	enfants_.push_back( enfant );
	enfant->modifierTerrain(GetTerrain());
	return true;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn unsigned int NoeudComposite::obtenirNombreEnfants(  )
///
/// Retourne le nombre d'enfants directement sous ce noeud.  Elle ne
/// donne pas le nombre total de descendants, mais bien le nombre de
/// ceux qui sont directement sous ce noeud.
///
///
/// @return unsigned int : Le nombre d'enfants directement sous ce noeud.
///
////////////////////////////////////////////////////////////////////////
unsigned int NoeudComposite::obtenirNombreEnfants() const
{
	return static_cast<unsigned int> (enfants_.size());
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::selectionnerTout(  )
///
/// Sélectionne tous les noeuds qui sont sélectionnés parmis les
/// les descendants de ce noeud, lui-même étant inclus.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::selectionnerTout()
{
	NoeudAbstrait::selectionnerTout();

	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->selectionnerTout();
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::deselectionnerTout()
///
/// Désélectionne tous les noeuds qui sont sélectionnés parmis les
/// les descendants de ce noeud, lui-même étant inclus.
///
/// @return Aucune
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::deselectionnerTout()
{
	selectionne_ = false;

	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->deselectionnerTout();
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::inverserSelectionTout()
///
/// Inverse la selection du noeud et de ses enfants
///
/// @return Aucune
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::inverserSelectionTout()
{
	inverserSelection();

	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->inverserSelection();
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudComposite::selectionExiste() const
///
/// Vérifie si le noeud ou un de ses descendants est sélectionné en
/// s'appelant de manière récursive sur les enfants du noeud.
///
/// @return : Vrai s'il existe un noeud sélectionné, faux autrement.
///
////////////////////////////////////////////////////////////////////////
bool NoeudComposite::selectionExiste() const
{
	if ( selectionne_ ) {
		return true;
	}

	for (ConteneurNoeuds::const_iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			if ( (*it)->selectionExiste() )
				return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::changerModePolygones( bool estForce )
///
/// Change le mode d'affichage des polygones pour ce noeud et ses
/// enfants.
///
/// @param[in] bool estForce : Si vrai, le mode est changé pour ce noeud et
///                       tous ses descendants.  Sinon, seuls les noeuds
///                       sélectionnés verront leur mode changer.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::changerModePolygones( bool estForce )
{
	NoeudAbstrait::changerModePolygones( estForce );
	const bool forceEnfant = ( ( estForce ) || ( estSelectionne() ) );

	// Applique le changement récursivement aux enfants.
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->changerModePolygones(forceEnfant);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::assignerModePolygones( GLenum modePolygones )
///
/// Cette fonction assigne le mode de rendu des polygones du noeud et
/// de ses enfants.
///
/// @param[in] GLenum modePolygones : Le mode de polygone.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::assignerModePolygones( GLenum modePolygones )
{
	// Appel à la version de la classe de base.
	NoeudAbstrait::assignerModePolygones(modePolygones);

	// Applique le changement récursivement aux enfants.
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->assignerModePolygones(modePolygones);
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::afficherConcret() const
///
/// Cette fonction effectue le véritable rendu de l'objet.  Elle est
/// appelée par la template method (dans le sens du patron de conception,
/// et non des template C++) afficher() de la classe de base.
///
/// Pour cette classe, elle affiche chacun des enfants du noeud.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::afficherConcret() const
{
    NoeudAbstrait::afficherConcret();

    DrawChild();

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::animer( const float& dt )
///
/// Anime tous les enfants de ce noeud.
///
/// @param[in] float dt : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::animer( const float& dt )
{
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->animer(dt);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudComposite::acceptVisitor( const VisiteurNoeud& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant
///
/// @param[in] VisiteurNoeud & v : Le visiteur du noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::acceptVisitor( VisiteurNoeud& v )
{
	v.visiterNoeudComposite(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur3 NoeudComposite::obtenirPositionAbsolue(  )
///
/// Retourne la position absolue du noeud.
///
///
/// @return Vecteur3 : le vecteur contenant la position absolue.
///
////////////////////////////////////////////////////////////////////////
Vecteur3 NoeudComposite::obtenirPositionAbsolue() const
{

	Vecteur3 positionAbsolue = this->obtenirPositionRelative();
	if(parent_ != 0 && parent_->obtenirType()!="racine")
	{
		positionAbsolue+=parent_->obtenirPositionAbsolue();
	}
	return positionAbsolue;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::detacherEnfant( NoeudAbstrait* noeud )
///
/// Suppression d'un noeud comme enfant du noeud courant
/// On ne supprime ni vide cet enfant pour qu'il puisse être ré-utilisé.
///
/// @param[in] NoeudAbstrait * noeud : Un pointeur sur le noeud a enlever.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::detacherEnfant( const NoeudAbstrait* noeud )
{
	ConteneurNoeuds::iterator iter = find(enfants_.begin(),enfants_.end(),noeud);
	if(iter != enfants_.end())
	{
		(*iter)->assignerParent(NULL);
		(*iter)->modifierTerrain(NULL);
		enfants_.erase(iter);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn ConteneurNoeuds NoeudComposite::obtenirNoeudsSelectionnes(  )
///
/// Retourne les racines des sous-arbres selectionnees
///
///
/// @return ConteneurNoeuds : un conteneur avec tous les noeuds selectionnes
///
////////////////////////////////////////////////////////////////////////
ConteneurNoeuds NoeudComposite::obtenirNoeudsSelectionnes() const
{
	static ConteneurNoeuds liste;

	if(estSelectionne() && estSelectionnable())
	{
		liste.push_back((NoeudAbstrait*)this);
	}
	else
	{
		for (ConteneurNoeuds::const_iterator it = enfants_.begin(); it != enfants_.end(); ++it)
		{
			NoeudComposite* enfant = dynamic_cast<NoeudComposite*> (*it);
			if(enfant != 0)
				enfant->obtenirNoeudsSelectionnes();
			else if((*it)->estSelectionne())
			{
				liste.push_back(*it);
			}
		}
	}

	ConteneurNoeuds listeTemp = ConteneurNoeuds(liste);
	if(type_=="racine")
		liste.clear();
	return listeTemp;

}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudComposite::possedeSelection()
///
/// Retourne true si un des enfants du noeud est selectionne
///
/// @param[in]	void
///
/// @return bool	: un enfant est selectionne
///
////////////////////////////////////////////////////////////////////////
bool NoeudComposite::possedeSelection()
{
	if(NoeudAbstrait::possedeSelection())
		return true;
	else
	{
		bool retour = false;
		for(unsigned int i=0; i<obtenirNombreEnfants(); i++)
		{
			retour = retour | chercher(i)->possedeSelection();
		}
		return retour;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::gestionCollision( const float& temps )
///
/// Application de la physique des noeuds enfants
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::gestionCollision( const float& temps )
{
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->gestionCollision(temps);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::majPosition( const float& temps )
///
/// /*Description*/
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::majPosition( const float& temps )
{
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->majPosition(temps);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::ajusterEnfoncement()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::ajusterEnfoncement()
{
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->ajusterEnfoncement();
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::ajusterVitesse( const float& temps )
///
/// /*Description*/
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::ajusterVitesse( const float& temps )
{
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->ajusterVitesse(temps);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::modifierTerrain( Terrain* val )
///
/// Modificateur de terrain_
///
/// @param[in] Terrain * val
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::modifierTerrain( Terrain* val )
{
	NoeudAbstrait::modifierTerrain(val);
	// Modification du pointeur de terrain de tous les enfants du noeuds. 
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->modifierTerrain(val);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::DrawChild()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::DrawChild() const
{
    for (ConteneurNoeuds::const_iterator it = enfants_.begin(); it != enfants_.end(); ++it)
    {
        (*it)->afficherConcret();
    }
}







////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////
