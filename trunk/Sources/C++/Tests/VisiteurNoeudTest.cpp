//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurNoeudTest.cpp
/// @author Vincent Lemire
/// @date 2012-02-03
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "VisiteurNoeudTest.h"
#include "NoeudComposite.h"
#include "NoeudMaillet.h"
#include "NoeudPortail.h"
#include "NoeudAccelerateur.h"
#include "NoeudMuret.h"
#include "NoeudRondelle.h"
#include "VisiteurSelection.h"
#include "Vecteur.h"
#include "VisiteurDeplacement.h"
#include "ConfigScene.h"
#include "VisiteurSuppression.h"
#include "NoeudBut.h"
#include "NoeudPoint.h"
#include "NoeudTable.h"
#include "NoeudAbstrait.h"
#include "VisiteurRotation.h"
#include "VisiteurDupliquer.h"
#include "FacadeModele.h"
#include "ArbreRendu.h"
#include "RazerGameTree.h"

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( VisiteurNoeudTest );

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::setUp()
///
/// Effectue l'initialisation préalable à l'exécution de l'ensemble des
/// cas de tests de cette suite de tests (si nécessaire).
/// 
/// Si certains objets doivent être construits, il est conseillé de le
/// faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void VisiteurNoeudTest::setUp()
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::tearDown()
///
/// Effectue les opérations de finalisation nécessaires suite à l'exécution
/// de l'ensemble des cas de tests de cette suite de tests (si nécessaire).
/// 
/// Si certains objets ont été alloués à l'initialisation, ils doivent être
/// désalloués, et il est conseillé de le faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void VisiteurNoeudTest::tearDown()
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurNoeudTest::testDeplacerObjet()
///
/// Cas de test: Déplacement d'objets.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void VisiteurNoeudTest::testDeplacerObjet()
{

	NoeudAbstrait* n = new NoeudAbstrait("");
	n->setPosition(Vecteur3(0.0,0.0));
	n->setSelection(true);
	VisiteurDeplacement v(Vecteur2(25.0,-10.0));
	n->acceptVisitor(v);
	CPPUNIT_ASSERT(n->getPosition() == Vecteur3(25.0,-10.0));
}


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurNoeudTest::testSelectionObjet()
///
/// Test pour la sélection d'objets
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurNoeudTest::testSelectionObjet()
{
	RazerGameTree* arbre = new RazerGameTree(NULL,999,999);
	NoeudAbstrait	*noeud1 = arbre->creerNoeud(RazerGameUtilities::NOM_MAILLET),
					*noeud2 = arbre->creerNoeud(RazerGameUtilities::NOM_PORTAIL),
					*noeud3 = arbre->creerNoeud(RazerGameUtilities::NOM_PORTAIL),
					*noeud4 = arbre->creerNoeud(RazerGameUtilities::NOM_MAILLET),
					*noeud5 = arbre->creerNoeud(RazerGameUtilities::NOM_RONDELLE);
	arbre->add(noeud1);
	arbre->add(noeud2);
	arbre->add(noeud3);
	arbre->add(noeud4);
	arbre->add(noeud5);
	arbre->setCanBeSelected(false);
	noeud1->setCanBeSelected(true);
	noeud2->setCanBeSelected(true);
	noeud3->setCanBeSelected(true);
	noeud4->setCanBeSelected(true);
	noeud5->setCanBeSelected(true);
	noeud1->setSelection(false);
	noeud2->setSelection(false);
	noeud3->setSelection(false);
	noeud4->setSelection(false);
	noeud5->setSelection(false);

	// assignation de position tres eloigné pour éviter que les boites de collisions se touchent
	noeud1->setPosition(Vecteur3(-500.0,500.0,0.0));
	noeud2->setPosition(Vecteur3(500.0,500.0,0));
	noeud3->setPosition(Vecteur3(0.0,0.0,0.0));
	noeud4->setPosition(Vecteur3(-500.0,-500.0,0.0));
	noeud5->setPosition(Vecteur3(500.0,-500.0,0.0));

	VisiteurSelection v(Vecteur2(-500.0,500.0),Vecteur2(-500.0,500.0));
	CPPUNIT_ASSERT(v.avecRectangle == false);

	arbre->acceptVisitor(v);
	v.faireSelection();

	CPPUNIT_ASSERT(noeud1->IsSelected() == true);
	CPPUNIT_ASSERT(noeud2->IsSelected() == false);
	CPPUNIT_ASSERT(noeud3->IsSelected() == false);
	CPPUNIT_ASSERT(noeud4->IsSelected() == false);
	CPPUNIT_ASSERT(noeud5->IsSelected() == false);
	noeud1->setSelection(false);

	VisiteurSelection v2(Vecteur2(-550.0,450.0),Vecteur2(550.0,550.0));
	CPPUNIT_ASSERT(v2.avecRectangle == true);

	arbre->acceptVisitor(v2);
	v2.faireSelection();
	CPPUNIT_ASSERT(noeud1->IsSelected() == true);
	CPPUNIT_ASSERT(noeud2->IsSelected() == true);
	CPPUNIT_ASSERT(noeud3->IsSelected() == false);
	CPPUNIT_ASSERT(noeud4->IsSelected() == false);
	CPPUNIT_ASSERT(noeud5->IsSelected() == false);

	arbre->empty();
	delete arbre;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurNoeudTest::testSuppressionObjet()
///
/// Test de suppression de noeuds
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurNoeudTest::testSuppressionObjet()
{
	// Construction d'un arbre de test
	RazerGameTree* arbre = new RazerGameTree(NULL,999,999);
	NoeudAbstrait* n1 = arbre->creerNoeud(RazerGameUtilities::NOM_MAILLET);
	NoeudAbstrait* n2 = arbre->creerNoeud(RazerGameUtilities::NOM_MAILLET);
	NoeudAbstrait* n3 = arbre->creerNoeud(RazerGameUtilities::NOM_PORTAIL);
	NoeudAbstrait* n4 = arbre->creerNoeud(RazerGameUtilities::NOM_PORTAIL);
	NoeudAbstrait* n5 = arbre->creerNoeud(RazerGameUtilities::NOM_RONDELLE);
	NoeudAbstrait* n6 = arbre->creerNoeud(RazerGameUtilities::NOM_RONDELLE);
	NoeudAbstrait* n7 = arbre->creerNoeud(RazerGameUtilities::NOM_ACCELERATEUR);
	NoeudComposite* nC1 = new NoeudComposite("groupe1");
	NoeudComposite* nC2 = new NoeudComposite("groupe2");
	NoeudComposite* nC3 = new NoeudComposite("groupe3");

	arbre->add(n1);
	arbre->add(n2);
	arbre->add(nC1);
	arbre->add(nC2);
	nC1->add(n3);
	nC1->add(n4);
	nC2->add(n5);
	nC2->add(n6);
	nC2->add(nC3);
	nC3->add(n7);

	// S'assure que tous est déselectionné
	arbre->deselectAll();

	// On selectionne 2 noeud
	n2->setSelection(true);
	n3->setSelection(true);
	nC3->setSelection(true);


	VisiteurSuppression v;
	arbre->acceptVisitor(v);

	CPPUNIT_ASSERT(arbre->childCount() == 3);
	CPPUNIT_ASSERT(arbre->treeDepth() == 3);
	CPPUNIT_ASSERT(nC1->treeDepth() == 2);
	CPPUNIT_ASSERT(nC1->childCount() == 1);
	CPPUNIT_ASSERT(nC2->treeDepth() == 2);
	CPPUNIT_ASSERT(nC2->childCount() == 2);

	arbre->empty();
	delete arbre;

}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


