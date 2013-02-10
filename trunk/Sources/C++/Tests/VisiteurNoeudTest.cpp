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
#include "..\Arbre\RazerGameTree.h"

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
	// Nous pourrions initialiser l'objet.
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
	// Nous pourrions libérer l'objet.
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

	NoeudAbstrait* n = new NoeudMaillet(RazerGameUtilities::NOM_MAILLET);
	n->assignerPositionRelative(Vecteur3(0.0,0.0));
	n->assignerSelection(true);
	VisiteurDeplacement v(Vecteur2(25.0,-10.0));
	n->acceptVisitor(v);
	CPPUNIT_ASSERT(n->obtenirPositionRelative() == Vecteur3(-25.0,-10.0));


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
	RazerGameTree* arbre = new RazerGameTree();
	NoeudAbstrait	*noeud1 = arbre->creerNoeud(RazerGameUtilities::NOM_MAILLET),
					*noeud2 = arbre->creerNoeud(RazerGameUtilities::NOM_PORTAIL),
					*noeud3 = arbre->creerNoeud(RazerGameUtilities::NOM_PORTAIL),
					*noeud4 = arbre->creerNoeud(RazerGameUtilities::NOM_MAILLET),
					*noeud5 = arbre->creerNoeud(RazerGameUtilities::NOM_RONDELLE);
	arbre->ajouter(noeud1);
	arbre->ajouter(noeud2);
	arbre->ajouter(noeud3);
	arbre->ajouter(noeud4);
	arbre->ajouter(noeud5);
	arbre->assignerEstSelectionnable(false);
	noeud1->assignerEstSelectionnable(true);
	noeud2->assignerEstSelectionnable(true);
	noeud3->assignerEstSelectionnable(true);
	noeud4->assignerEstSelectionnable(true);
	noeud5->assignerEstSelectionnable(true);
	noeud1->assignerSelection(false);
	noeud2->assignerSelection(false);
	noeud3->assignerSelection(false);
	noeud4->assignerSelection(false);
	noeud5->assignerSelection(false);

	// assignation de position tres eloigné pour éviter que les boites de collisions se touchent
	noeud1->assignerPositionRelative(Vecteur3(-500.0,500.0,0.0));
	noeud2->assignerPositionRelative(Vecteur3(500.0,500.0,0));
	noeud3->assignerPositionRelative(Vecteur3(0.0,0.0,0.0));
	noeud4->assignerPositionRelative(Vecteur3(-500.0,-500.0,0.0));
	noeud5->assignerPositionRelative(Vecteur3(500.0,-500.0,0.0));

	VisiteurSelection v(Vecteur2(-500.0,500.0),Vecteur2(-500.0,500.0));
	CPPUNIT_ASSERT(v.avecRectangle == false);

	arbre->acceptVisitor(v);
	v.faireSelection();

	//CPPUNIT_ASSERT(noeud1->estSelectionne() == true);
	CPPUNIT_ASSERT(noeud2->estSelectionne() == false);
	CPPUNIT_ASSERT(noeud3->estSelectionne() == false);
	CPPUNIT_ASSERT(noeud4->estSelectionne() == false);
	CPPUNIT_ASSERT(noeud5->estSelectionne() == false);
	noeud1->assignerSelection(false);

	VisiteurSelection v2(Vecteur2(-550.0,450.0),Vecteur2(550.0,550.0));
	CPPUNIT_ASSERT(v2.avecRectangle == true);

	arbre->acceptVisitor(v2);
	v2.faireSelection();
	CPPUNIT_ASSERT(noeud1->estSelectionne() == true);
	CPPUNIT_ASSERT(noeud2->estSelectionne() == true);
	CPPUNIT_ASSERT(noeud3->estSelectionne() == false);
	CPPUNIT_ASSERT(noeud4->estSelectionne() == false);
	CPPUNIT_ASSERT(noeud5->estSelectionne() == false);

	arbre->vider();
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
	RazerGameTree* arbre = new RazerGameTree();
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

	arbre->ajouter(n1);
	arbre->ajouter(n2);
	arbre->ajouter(nC1);
	arbre->ajouter(nC2);
	nC1->ajouter(n3);
	nC1->ajouter(n4);
	nC2->ajouter(n5);
	nC2->ajouter(n6);
	nC2->ajouter(nC3);
	nC3->ajouter(n7);

	// S'assure que tous est déselectionné
	arbre->deselectionnerTout();

	// On selectionne 2 noeud
	n2->assignerSelection(true);
	n3->assignerSelection(true);
	nC3->assignerSelection(true);


	VisiteurSuppression v;
	arbre->acceptVisitor(v);

	CPPUNIT_ASSERT(arbre->obtenirNombreEnfants() == 3);
	CPPUNIT_ASSERT(arbre->calculerProfondeur() == 3);
	CPPUNIT_ASSERT(nC1->calculerProfondeur() == 2);
	CPPUNIT_ASSERT(nC1->obtenirNombreEnfants() == 1);
	CPPUNIT_ASSERT(nC2->calculerProfondeur() == 2);
	CPPUNIT_ASSERT(nC2->obtenirNombreEnfants() == 2);

	arbre->vider();
	delete arbre;

}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


