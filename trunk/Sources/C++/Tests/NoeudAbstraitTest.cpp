////////////////////////////////////////////////////////////////////////////////////
/// @file NoeudAbstraitTest.cpp
/// @author Julien Gascon-Samson
/// @date 2011-07-16
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
////////////////////////////////////////////////////////////////////////////////////

#include "NoeudAbstraitTest.h"
#include "Utilitaire.h"

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( NoeudAbstraitTest );

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::setUp()
///
/// Effectue l'initialisation pr�alable � l'ex�cution de l'ensemble des
/// cas de tests de cette suite de tests (si n�cessaire).
/// 
/// Si certains objets doivent �tre construits, il est conseill� de le
/// faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstraitTest::setUp()
{
	noeud = new NoeudAbstrait("TestingNode");
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::tearDown()
///
/// Effectue les op�rations de finalisation n�cessaires suite � l'ex�cution
/// de l'ensemble des cas de tests de cette suite de tests (si n�cessaire).
/// 
/// Si certains objets ont �t� allou�s � l'initialisation, ils doivent �tre
/// d�sallou�s, et il est conseill� de le faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstraitTest::tearDown()
{
	delete noeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::testPositionRelative()
///
/// Cas de test: �criture/lecture de la position relative
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstraitTest::testPositionRelative()
{
	noeud->setPosition(Vecteur3(2.2f, 3.3f, 4.4f));

	Vecteur3 vecteur = noeud->getPosition();

	CPPUNIT_ASSERT( utilitaire::EGAL_ZERO(vecteur[0] - 2.2f) );
	CPPUNIT_ASSERT( utilitaire::EGAL_ZERO(vecteur[1] - 3.3f) );
	CPPUNIT_ASSERT( utilitaire::EGAL_ZERO(vecteur[2] - 4.4f) );
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::testType()
///
/// Cas de test: type de noeud
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstraitTest::testType()
{
	CPPUNIT_ASSERT( noeud->obtenirType() == "TestingNode");
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::testEtat()
///
/// Cas de test: d�finition/obtention de l'�tat (bool�ens) du noeud
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstraitTest::testEtat()
{
	// R�gler tous les �tats (bool�ens) � faux
	noeud->assignerAffiche(false);
	noeud->assignerEstEnregistrable(false);

	// V�rifier que la valeur attribu�e est fausse
	CPPUNIT_ASSERT( noeud->estAffiche() == false );
	CPPUNIT_ASSERT( noeud->estEnregistrable() == false );

	// R�gler tous les �tats (bool�ens) � true
	noeud->assignerAffiche(true);
	noeud->assignerEstEnregistrable(true);

	// V�rifier que la valeur attribu�e est fausse
	CPPUNIT_ASSERT( noeud->estAffiche() == true );
	CPPUNIT_ASSERT( noeud->estEnregistrable() == true );
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::testSelection()
///
/// Cas de test: d�finition/obtention des �tats de s�lection du noeud
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstraitTest::testSelection()
{
	// Cas 1 : non s�lectionnable, non s�lectionn�
	noeud->assignerEstSelectionnable(false);
	noeud->assignerSelection(false);
	CPPUNIT_ASSERT( noeud->estSelectionnable() == false );
	CPPUNIT_ASSERT( noeud->estSelectionne() == false );

	// Cas 2 : non s�lectionnable, s�lectionn�
	noeud->assignerEstSelectionnable(false);
	noeud->assignerSelection(true);
	CPPUNIT_ASSERT( noeud->estSelectionnable() == false );
	CPPUNIT_ASSERT( noeud->estSelectionne() == false );

	// Cas 3 : s�lectionnable, non s�lectionn�
	noeud->assignerEstSelectionnable(true);
	noeud->assignerSelection(false);
	CPPUNIT_ASSERT( noeud->estSelectionnable() == true );
	CPPUNIT_ASSERT( noeud->estSelectionne() == false );

	// Cas 4 : non s�lectionnable, s�lectionn�
	noeud->assignerEstSelectionnable(true);
	noeud->assignerSelection(true);
	CPPUNIT_ASSERT( noeud->estSelectionnable() == true );
	CPPUNIT_ASSERT( noeud->estSelectionne() == true );
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::testEnfants()
///
/// Cas de test: s'assurer que le noeud abstrait n'a pas d'enfant
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstraitTest::testEnfants()
{
	CPPUNIT_ASSERT( noeud->obtenirNombreEnfants() == 0);
	CPPUNIT_ASSERT( noeud->calculerProfondeur() == 1);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::testAjout()
///
/// Cas de test: s'assurer qu'il est impossible d'ajouter un enfant
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstraitTest::testAjout()
{
// 	// Essaie d'ajouter un noeud
// 	NoeudAbstrait* nouveauNoeud = new NoeudConeCube(RazerGameUtilities::NOM_CUBECONE);
// 	bool ajout = noeud->ajouter(nouveauNoeud);
// 
// 	// L'ajout devrait avoir �chou� puisqu'il s'agit d'un noeud abstrait...
// 	CPPUNIT_ASSERT( ajout==false );
// 
// 	// Assurons-nous que le noeud ne poss�de pas d'enfant...
// 	CPPUNIT_ASSERT( noeud->obtenirNombreEnfants() == 0 );
// 
// 	// Nettoyage
// 	delete nouveauNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::testGetTreeRoot()
///
/// Cas de test: s'assurer de bien retrouver la racine du noeud si elle existe
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstraitTest::testGetTreeRoot()
{
    RazerGameTree tree(NULL);

    NoeudComposite* c1 = new NoeudComposite(""),
        * c2 = new NoeudComposite(""),
        * c3 = new NoeudComposite(""),
        * c4 = new NoeudComposite(""),
        * c5 = new NoeudComposite("");

    c1->ajouter(c2);
    c2->ajouter(c3);
    c3->ajouter(c4);
    c4->ajouter(c5);
    c5->ajouter(noeud);

    const ArbreRendu* root = noeud->GetTreeRoot();

    // arbre pas encore ajouter
    CPPUNIT_ASSERT( root == NULL );

    tree.ajouter(c1);
    root = noeud->GetTreeRoot();
    CPPUNIT_ASSERT( root == &tree );


    // pour ne pas liberer 2 fois la memoire
    c5->detacherEnfant(noeud);

    
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
