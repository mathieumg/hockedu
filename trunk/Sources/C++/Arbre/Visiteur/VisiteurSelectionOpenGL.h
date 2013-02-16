#pragma once
#include "VisiteurNoeud.h"
#include "RazerGameTypeDef.h"


class VisiteurSelectionOpenGL :
	public VisiteurNoeud
{
public:
	VisiteurSelectionOpenGL(const IdNoeuds*, bool ctrlOn);
	~VisiteurSelectionOpenGL(void);

	/// Visitation d'un noeud abstrait
	virtual void visiterNoeudAbstrait( NoeudAbstrait* noeud );
	/// Visitation d'un noeud composite
	virtual void visiterNoeudComposite( NoeudComposite* noeud );
	/// Algortihme de selection des noeuds
	void visiterNoeudAffichable( NoeudAbstrait* noeud );

private:
	const IdNoeuds* noeudsAselectioner_;
	bool ctrlOn_;

};

