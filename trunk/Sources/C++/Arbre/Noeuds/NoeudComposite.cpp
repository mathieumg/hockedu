////////////////////////////////////////////////
/// @file   NoeudComposite.cpp
/// @author DGI-2990
/// @date   2007-01-25
///
/// @addtogroup razergame RazerGame
/// @{
////////////////////////////////////////////////

#include "NoeudComposite.h"
#include <algorithm>
#include "VisiteurNoeud.h"

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
	empty();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn unsigned int NoeudComposite::treeDepth(  )
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
unsigned int NoeudComposite::treeDepth() const
{
	unsigned int profondeurEnfantMax = 0;

	for (ConteneurNoeuds::const_iterator it = enfants_.begin();
		it != enfants_.end();
		it++) {
			const unsigned int profondeurEnfant = (*it)->treeDepth();

			if (profondeurEnfantMax < profondeurEnfant)
				profondeurEnfantMax = profondeurEnfant;
	}

	return profondeurEnfantMax + 1;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::empty(  )
///
/// Cette fonction vide le noeud de tous ses enfants.  Elle effectue une
/// itération prudente sur les enfants afin d'être assez robuste pour
/// supporter la possibilité qu'un enfant en efface un autre dans son
/// destructeur, par exemple si deux objets ne peuvent pas exister l'un
/// sans l'autre.  Elle peut toutefois entrer en boucle infinie si un
/// enfant ajoute un nouveau noeud lorsqu'il se fait erase.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::empty()
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
/// @fn void NoeudComposite::erase( const NoeudAbstrait* noeud )
///
/// Efface un noeud qui est un enfant ou qui est contenu dans un des
/// enfants.
///
/// @param[in] const NoeudAbstrait * noeud : Le noeud à erase.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::erase( const NoeudAbstrait* noeud )
{
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		it++) {
			if (*it == noeud) {
				// On a trouvé le noeud à erase
				NoeudAbstrait* noeudAEffacer = (*it);
				enfants_.erase(it);
				delete noeudAEffacer;
				noeud = 0;
				return;
			}
			else {
				// On cherche dans les enfants.
				(*it)->erase( noeud );
			}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn const NoeudAbstrait* NoeudComposite::find( const std::string& typeNoeud )
///
/// Recherche un noeud d'un type donné parmi le noeud courant et ses
/// enfants.  Version constante de la fonction.
///
/// @param[in] const std::string & typeNoeud : Le type du noeud cherché.
///
/// @return const NoeudAbstrait* : Noeud recherché ou 0 si le noeud n'est pas trouvé.
///
////////////////////////////////////////////////////////////////////////
const NoeudAbstrait* NoeudComposite::find(
	const std::string& typeNoeud
	) const
{
	if ( typeNoeud == mType ) {
		return this;
	}
	else {
		for (ConteneurNoeuds::const_iterator it = enfants_.begin();
			it != enfants_.end();
			++it) {
				NoeudAbstrait* noeud = (*it)->find( typeNoeud );
				if ( noeud != 0 ) {
					return noeud;
				}
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* NoeudComposite::find( const std::string& typeNoeud )
///
/// Recherche un noeud d'un type donné parmi le noeud courant et ses
/// enfants.
///
/// @param[in] const std::string & typeNoeud : Le type du noeud cherché.
///
/// @return NoeudAbstrait* : Noeud recherché ou 0 si le noeud n'est pas trouvé.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait* NoeudComposite::find(const std::string& typeNoeud)
{
	if ( typeNoeud == mType ) {
		return this;
	}
	else {
		for (ConteneurNoeuds::iterator it = enfants_.begin();
			it != enfants_.end();
			++it) {
				NoeudAbstrait* noeud = (*it)->find( typeNoeud );
				if ( noeud != 0 ) {
					return noeud;
				}
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn const NoeudAbstrait* NoeudComposite::find( unsigned int indice )
///
/// Retourne le i-ème enfant, où i est l'indice passé à la fonction.
/// Version constante de la fonction.
///
/// @param[in] unsigned int indice : L'indice de l'enfant cherché.
///
/// @return const NoeudAbstrait* : Noeud recherché ou 0 si le noeud n'est pas trouvé.
///
////////////////////////////////////////////////////////////////////////
const NoeudAbstrait* NoeudComposite::find( unsigned int indice ) const
{
    if ( indice < (unsigned int)enfants_.size() ) {
		return enfants_[indice];
	}
	else {
		return 0;
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* NoeudComposite::find( unsigned int indice )
///
/// Retourne le i-ème enfant, où i est l'indice passé à la fonction.
///
/// @param[in] indice : L'indice de l'enfant cherché.
///
/// @return Noeud recherché ou 0 si le noeud n'est pas trouvé.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait* NoeudComposite::find( unsigned int indice )
{
	if ( (indice < enfants_.size()) ) {
		return enfants_[indice];
	}
	else {
		return 0;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudComposite::add( NoeudAbstrait* enfant )
///
/// Ajoute un noeud enfant au noeud courant.
///
/// @param[in] NoeudAbstrait * enfant : Noeud à add.
///
/// @return bool : Vrai si l'ajout a réussi, donc en tout temps pour cette classe.
///
////////////////////////////////////////////////////////////////////////
bool NoeudComposite::add( NoeudAbstrait* enfant )
{
	// un noeud ne peut s'avoir comme parent
	if(this == enfant)
		return false;
	// Ce noeud appartient dejà à un autre noeud, alors on le détache au préalable
	if(enfant->getParent() != NULL)
	{
		if( enfant->getParent() == this)
			return true;
		enfant->getParent()->unlinkChild(enfant);
	}

    enfant->setField(getField());
    enfant->setParent(this);
	enfants_.push_back( enfant );
	return true;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn unsigned int NoeudComposite::childCount(  )
///
/// Retourne le nombre d'enfants directement sous ce noeud.  Elle ne
/// donne pas le nombre total de descendants, mais bien le nombre de
/// ceux qui sont directement sous ce noeud.
///
///
/// @return unsigned int : Le nombre d'enfants directement sous ce noeud.
///
////////////////////////////////////////////////////////////////////////
unsigned int NoeudComposite::childCount() const
{
	return static_cast<unsigned int> (enfants_.size());
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::selectAll(  )
///
/// Sélectionne tous les noeuds qui sont sélectionnés parmis les
/// les descendants de ce noeud, lui-même étant inclus.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::selectAll()
{
	NoeudAbstrait::selectAll();

	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->selectAll();
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::deselectAll()
///
/// Désélectionne tous les noeuds qui sont sélectionnés parmis les
/// les descendants de ce noeud, lui-même étant inclus.
///
/// @return Aucune
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::deselectAll()
{
    setSelection(false);

	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->deselectAll();
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
	toggleSelection();

	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->toggleSelection();
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudComposite::selectionPresent() const
///
/// Vérifie si le noeud ou un de ses descendants est sélectionné en
/// s'appelant de manière récursive sur les enfants du noeud.
///
/// @return : Vrai s'il existe un noeud sélectionné, faux autrement.
///
////////////////////////////////////////////////////////////////////////
bool NoeudComposite::selectionPresent() const
{
	if ( IsSelected()) {
		return true;
	}

	for (ConteneurNoeuds::const_iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			if ( (*it)->selectionPresent() )
				return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::cyclePolygonMode( bool estForce )
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
void NoeudComposite::cyclePolygonMode( bool estForce )
{
	NoeudAbstrait::cyclePolygonMode( estForce );
	const bool forceEnfant = ( ( estForce ) || ( IsSelected() ) );

	// Applique le changement récursivement aux enfants.
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->cyclePolygonMode(forceEnfant);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::setPolygonMode( GLenum modePolygones )
///
/// Cette fonction assigne le mode de rendu des polygones du noeud et
/// de ses enfants.
///
/// @param[in] GLenum modePolygones : Le mode de polygone.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::setPolygonMode( GLenum modePolygones )
{
	// Appel à la version de la classe de base.
	NoeudAbstrait::setPolygonMode(modePolygones);

	// Applique le changement récursivement aux enfants.
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->setPolygonMode(modePolygones);
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::renderReal() const
///
/// Cette fonction effectue le véritable rendu de l'objet.  Elle est
/// appelée par la template method (dans le sens du patron de conception,
/// et non des template C++) render() de la classe de base.
///
/// Pour cette classe, elle affiche chacun des enfants du noeud.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::renderReal() const
{
    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT | GL_POLYGON_BIT);

    // Assignation du mode d'affichage des polygones
    glPolygonMode( GL_FRONT_AND_BACK, mModePolygones );

    // Affichage concret
    NoeudAbstrait::renderReal();

    // Restauration
    glPopAttrib();
    glPopMatrix();

    DrawChild();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::tick( const float& dt )
///
/// Anime tous les enfants de ce noeud.
///
/// @param[in] float dt : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::tick( const float& dt )
{
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->tick(dt);
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
/// @fn void NoeudComposite::unlinkChild( NoeudAbstrait* noeud )
///
/// Suppression d'un noeud comme enfant du noeud courant
/// On ne supprime ni vide cet enfant pour qu'il puisse être ré-utilisé.
///
/// @param[in] NoeudAbstrait * noeud : Un pointeur sur le noeud a enlever.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::unlinkChild( const NoeudAbstrait* noeud )
{
	ConteneurNoeuds::iterator iter = std::find(enfants_.begin(),enfants_.end(),noeud);
	if(iter != enfants_.end())
	{
		(*iter)->setParent(NULL);
		(*iter)->setField(NULL);
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
void NoeudComposite::getSelectedNodes( ConteneurNoeuds& pSelectedNodes ) const
{
	if(IsSelected() && canBeSelected())
	{
		pSelectedNodes.push_back((NoeudAbstrait*)this);
	}
	else
	{
		for (ConteneurNoeuds::const_iterator it = enfants_.begin(); it != enfants_.end(); ++it)
		{
			NoeudComposite* compositeChild = dynamic_cast<NoeudComposite*> (*it);
			if(compositeChild)
            {
				compositeChild->getSelectedNodes(pSelectedNodes);
            }
			else if((*it)->IsSelected())
			{
				pSelectedNodes.push_back(*it);
			}
		}
	}
}

#if MANUAL_PHYSICS_DETECTION
////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::collisionDetection( const float& temps )
///
/// Application de la physique des noeuds enfants
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::collisionDetection( const float& temps )
{
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->collisionDetection(temps);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::positionUpdate( const float& temps )
///
/// /*Description*/
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::positionUpdate( const float& temps )
{
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->positionUpdate(temps);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::fixOverlap()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::fixOverlap()
{
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->fixOverlap();
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::fixSpeed( const float& temps )
///
/// /*Description*/
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::fixSpeed( const float& temps )
{
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->fixSpeed(temps);
	}
}
#endif

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudComposite::setField( Terrain* val )
///
/// Modificateur de terrain_
///
/// @param[in] Terrain * val
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudComposite::setField( Terrain* val )
{
	NoeudAbstrait::setField(val);
	// Modification du pointeur de terrain de tous les enfants du noeuds. 
	for (ConteneurNoeuds::iterator it = enfants_.begin();
		it != enfants_.end();
		++it) {
			(*it)->setField(val);
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
        (*it)->render();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudComposite::equals( NoeudAbstrait* )
///
/// fonction de comparaison de 2 noeuds
///
/// @param[in] NoeudAbstrait *
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudComposite::equals( NoeudAbstrait* n)
{
    NoeudComposite* nc = dynamic_cast<NoeudComposite*>(n);
    if(!nc || !Super::equals(n))
        return false;

    if(enfants_.size() != nc->enfants_.size())
    {
        return false;
    }

    for(int i=0; i<(int)enfants_.size(); ++i)
    {
        auto child1 = enfants_[i];
        auto child2 = nc->enfants_[i];
        if(child1)
        {
            if(!child1->equals(child2))
            {
                return false;
            }
        }
        // les 2 enfants doivent etre null si un l'est
        else if(!!child2)
        {
            return false;
        }
    }
    return true;
}







////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////
