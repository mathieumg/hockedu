////////////////////////////////////////////////////////////////////////////////////
/// @file TournoiTest.cpp
/// @author Michael Ferris
/// @date 2012-02-18
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
////////////////////////////////////////////////////////////////////////////////////
#include "TournoiTest.h"
#include "JoueurHumain.h"
#include "JoueurVirtuel.h"
#include "XMLUtils.h"

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( TournoiTest );

////////////////////////////////////////////////////////////////////////
///
/// @fn void TournoiTest::setUp()
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
void TournoiTest::setUp()
{
	tournoi_ = new Tournoi();
	joueurs[0] =  SPJoueurAbstrait(new JoueurHumain("Michael"));
	joueurs[1] =  SPJoueurAbstrait(new JoueurHumain("Samuel"));
	joueurs[2] =  SPJoueurAbstrait(new JoueurHumain("Vincent"));
	joueurs[3] =  SPJoueurAbstrait(new JoueurHumain("Mathieu"));
	joueurs[4] =  SPJoueurAbstrait(new JoueurHumain("Gabriel"));
	joueurs[5] =  SPJoueurAbstrait(new JoueurHumain("Charles Etienne"));
	joueurs[6] =  SPJoueurAbstrait(new JoueurVirtuel("bot1",10,50));
	joueurs[7] =  SPJoueurAbstrait(new JoueurVirtuel("bot2",15,25));
	joueurs[8] =  SPJoueurAbstrait(new JoueurVirtuel("bot3",25,0));
	joueurs[9] =  SPJoueurAbstrait(new JoueurVirtuel("bot4",10,-10));
	joueurs[10] = SPJoueurAbstrait(new JoueurVirtuel("bot5",12,25));
	joueurs[11] = SPJoueurAbstrait(new JoueurVirtuel("bot6",48,75));
	joueurs[12] = SPJoueurAbstrait(new JoueurVirtuel("bot8",95,60));
	joueurs[13] = SPJoueurAbstrait(new JoueurVirtuel("bot9",47,55));
	joueurs[14] = SPJoueurAbstrait(new JoueurVirtuel("bot10",47,55));
	joueurs[15] = SPJoueurAbstrait(new JoueurVirtuel("bot11",47,55));

	JoueursParticipant p1;
	for (int i = 0; i < 16 ; i++)
	{
		p1.push_back(joueurs[i]);
	}
	tournoi_->initialisation(p1,"D:/test_terrain.xml");
	tournoi_->parties_[0].assignerJoueur(joueurs[0]);
	for (int i = 0; i < 7 ; i++)
	{
		tournoi_->parties_[0].incrementerPointsJoueurGauche();		
	}
	tournoi_->modifierNom("awesomeness tournament");
	tournoi_->listeGagnants_.push_back("Roger");
	tournoi_->listeGagnants_.push_back("tamere");
	tournoi_->listeGagnants_.push_back("lolol");
	tournoi_->listeGagnants_.push_back("Roger");
	tournoi_->listeGagnants_.push_back("Roger");
	tournoi_->listeGagnants_.push_back("Roger");
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void TournoiTest::tearDown()
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
void TournoiTest::tearDown()
{
	delete tournoi_;
	// Les tournois s'occupe de liberer la memoire des joueurs
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void TournoiTest::testEcritureXML()
///
/// Test d'écriture d'un tournoi dans un fichier XML
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void TournoiTest::testEcritureXML()
{
	TiXmlDocument document;
	TiXmlDeclaration* declaration = new TiXmlDeclaration( "1.0", "", "" );
	document.LinkEndChild(declaration);
	XmlElement* racine = XMLUtils::createNode("Tournoi");
	document.LinkEndChild(racine);

	racine->LinkEndChild(tournoi_->creerTournoiXML());
	
	document.SaveFile( "tests_xml\\TestEnregistrementTournoi.xml" );
	
	Tournoi tournoi;	
	CPPUNIT_ASSERT(tournoi.initialisationXML(racine->FirstChildElement()));
	TiXmlDocument document2;
	document2.LinkEndChild(tournoi.creerTournoiXML());
	document2.SaveFile( "tests_xml\\TestLectureTournoi.xml" );

    remove("tests_xml\\TestLectureTournoi.xml");
    remove("tests_xml\\TestEnregistrementTournoi.xml");
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void TournoiTest::testReinitialisationTournoi()
///
/// Test pour s'assurer qu'un tournoi revient a son etat orginial suite a une reinitialisation
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void TournoiTest::testReinitialisationTournoi()
{
	while(!tournoi_->estTermine())
	{
		rand()&1 ? tournoi_->obtenirPartieCourante()->incrementerPointsJoueurDroit() : tournoi_->obtenirPartieCourante()->incrementerPointsJoueurGauche();
		tournoi_->miseAJour(false);
	}
	for(int i= 0; i< Tournoi::nbrParties_; ++i)
	{
		CPPUNIT_ASSERT(tournoi_->parties_[i].partieTerminee());
	}
	tournoi_->reinitialiserTournoi();
	for(int i= 0; i< Tournoi::nbrParties_; ++i)
	{
		CPPUNIT_ASSERT(!tournoi_->parties_[i].partieTerminee());
		CPPUNIT_ASSERT(tournoi_->parties_[i].obtenirPointsJoueurDroit() == 0);
		CPPUNIT_ASSERT(tournoi_->parties_[i].obtenirPointsJoueurGauche() == 0);
	}
	for(int i= 0; i< (Tournoi::nbrParties_>>1)-1; ++i)
	{
		CPPUNIT_ASSERT(!tournoi_->parties_[i].obtenirJoueurDroit());
		CPPUNIT_ASSERT(!tournoi_->parties_[i].obtenirJoueurGauche());
	}

}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////