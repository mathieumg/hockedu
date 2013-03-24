#pragma once
#include "VisiteurNoeud.h"
#include "RazerGameTypeDef.h"


class VisiteurSelectionOpenGL :
	public VisiteurNoeud
{
public:
	VisiteurSelectionOpenGL(const IdNoeuds*, bool ctrlOn);
	virtual ~VisiteurSelectionOpenGL(void);

	/// Visitation d'un noeud abstrait
	virtual void visiterNoeudAbstrait( NoeudAbstrait* noeud );
	/// Visitation d'un noeud composite
	virtual void visiterNoeudComposite( NoeudComposite* noeud );
	/// Algortihme de selection des noeuds
	void visiterNoeudAffichable( NoeudAbstrait* noeud );
    inline int getNbSelected() const{return mNbSelected;}
private:
	const IdNoeuds* noeudsAselectioner_;
	bool ctrlOn_;
    int mNbSelected;
};

