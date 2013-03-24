////////////////////////////////////////////////////////////////////////////////////
/// @file ConfigSceneTest.cpp
/// @author Michael Ferris
/// @date 2012-02-02
/// @version 2.0
///
/// @addtogroup razergame RazerGame
/// @{
////////////////////////////////////////////////////////////////////////////////////

#include "ConfigSceneTest.h"
#include "JoueurVirtuel.h"
#include "JoueurHumain.h"
#include "Utilitaire.h"
#include <algorithm>
#include "RazerGameTree.h"
#include "VisiteurEcrireXML.h"
#include "Terrain.h"
#include "NoeudMaillet.h"

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( ConfigSceneTest );

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
void ConfigSceneTest::setUp()
{
	// On initialise un arbre arbitraire pour les test
	arbre = new RazerGameTree(NULL,999,999);
	noeuds_[0] = arbre->creerNoeud(RazerGameUtilities::NOM_MURET);
	noeuds_[1] = arbre->creerNoeud(RazerGameUtilities::NOM_PORTAIL); 
	noeuds_[2] = arbre->creerNoeud(RazerGameUtilities::NOM_RONDELLE); 
	noeuds_[3] = arbre->creerNoeud(RazerGameUtilities::NOM_MAILLET);
	if(noeuds_[0] != 0)
	{
		if(noeuds_[1] != 0)
		{
			noeuds_[0]->add(noeuds_[1]);
			if(noeuds_[3] != 0)
			{
				noeuds_[1]->add(noeuds_[3]); //ne s'ajoutera pas car un portail est un noeudAbstrait
			}
		}
		if(noeuds_[2] != 0)
		{
			noeuds_[0]->add(noeuds_[2]);
		}
		arbre->add(noeuds_[0]);
	}

#ifdef WIN32
	CreateDirectoryA("tests_xml",NULL);
#else
	// creation d'un dossier avec un autre API, dunno which yet
#endif

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
void ConfigSceneTest::tearDown()
{
	arbre->empty();
	delete arbre;

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::testEcritureArbreRenduXML()
///
/// Cas de test: Écriture dans un fichier XML.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void ConfigSceneTest::testEcritureArbreRenduXML() 
{
	arbre->empty();

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
	NoeudComposite* nC4 = new NoeudComposite("groupe4");

	arbre->add(n1);
	arbre->add(n2);
	arbre->add(nC1);
    {
        nC1->add(n3);
        nC1->add(n4);
    }
    arbre->add(nC2);
    {
        nC2->add(nC4);
        {
            nC4->add(n5);
        }
        nC2->add(n6);
        nC2->add(nC3);
        {
            nC3->add(n7);
        }
    }

	nC4->setRecordable(false);

	// Écriture initiale du document
	XmlDocument document ;
    XMLUtils::CreateDocument(document,"","","");
    VisiteurEcrireXML v;
    arbre->acceptVisitor(v);

    XmlElement* elem = v.obtenirRacine();
    XMLUtils::LinkEndChild(document,elem);

    XMLUtils::SaveDocument(document,"tests_xml\\TestEnregistrement.xml" );
	
	// Lecture manuelle du document et vérification des noeuds lus (et de la non-existence de noeuds qui ne sont pas supposer exister
	const XmlNode* elementConfiguration = XMLUtils::FirstChild(document.GetElem(),ConfigScene::ETIQUETTE_ARBRE), *child, *grantChild, *grantGrantChild;//, *grantGrantGrantChild;
	CPPUNIT_ASSERT (elementConfiguration != NULL);
    std::string name;
    {
        // n1
        child = elementConfiguration->FirstChild();
        CPPUNIT_ASSERT (child != NULL);
        name = child->Value();
        CPPUNIT_ASSERT(name == RazerGameUtilities::NOM_MAILLET);

        // n2
        child = child->NextSibling();
        CPPUNIT_ASSERT (child != NULL);
        name = child->Value();
        CPPUNIT_ASSERT(name == RazerGameUtilities::NOM_MAILLET);

        // nC1
        child = child->NextSibling();
        CPPUNIT_ASSERT (child != NULL);
        name = child->Value();
        CPPUNIT_ASSERT(name == "groupe1");
        {
            // n3
            grantChild = child->FirstChild();
            CPPUNIT_ASSERT (grantChild != NULL);
            name = grantChild->Value();
            CPPUNIT_ASSERT(name == RazerGameUtilities::NOM_PORTAIL);

            // n4
            grantChild = grantChild->NextSibling();
            CPPUNIT_ASSERT (grantChild != NULL);
            name = grantChild->Value();
            CPPUNIT_ASSERT(name == RazerGameUtilities::NOM_PORTAIL);

            // n'existe pas
            grantChild = grantChild->NextSibling();
            CPPUNIT_ASSERT (grantChild == NULL);
        }
        // nC2
        child = child->NextSibling();
        CPPUNIT_ASSERT (child != NULL);
        name = child->Value();
        CPPUNIT_ASSERT(name == "groupe2");
        {
            {
                // n5, car nC4 n'est pas enregistrable
                grantChild = child->FirstChild();
                CPPUNIT_ASSERT (grantChild != NULL);
                name = grantChild->Value();
                CPPUNIT_ASSERT(name == RazerGameUtilities::NOM_RONDELLE);
            }

            // n6
            grantChild = grantChild->NextSibling();
            CPPUNIT_ASSERT (grantChild != NULL);
            name = grantChild->Value();
            CPPUNIT_ASSERT(name == RazerGameUtilities::NOM_RONDELLE);

            // nC3
            grantChild = grantChild->NextSibling();
            CPPUNIT_ASSERT (grantChild != NULL);
            name = grantChild->Value();
            CPPUNIT_ASSERT(name == "groupe3");
            {
                // n7
                grantGrantChild = grantChild->FirstChild();
                CPPUNIT_ASSERT (grantGrantChild != NULL);
                name = grantGrantChild->Value();
                CPPUNIT_ASSERT(name == RazerGameUtilities::NOM_ACCELERATEUR);

                // n'existe pas
                grantGrantChild = grantGrantChild->NextSibling();
                CPPUNIT_ASSERT (grantGrantChild == NULL);
            }

            // n'existe pas
            grantChild = grantChild->NextSibling();
            CPPUNIT_ASSERT (grantChild == NULL);
        }
        child = child->NextSibling();
        CPPUNIT_ASSERT (child == NULL);
    }
	// Parcours de l'arbre complet

    remove("tests_xml\\TestEnregistrement.xml");
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::testLectureArbreRenduXML()
///
/// Cas de test: Lecture dans un fichier XML.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void ConfigSceneTest::testLectureArbreRenduXML() 
{
    Terrain terrainWrite(NULL),terrainRead(NULL);
    terrainWrite.createRandomField("test");
    XmlElement* racine = XMLUtils::createNode("racine");
    XmlElement* elem = terrainWrite.creerNoeudXML();
    XMLUtils::LinkEndChild(racine,elem);
    try
    {
        terrainRead.initialiserXml(racine);
    }
    catch (ExceptionJeu&)
    {
    	CPPUNIT_ASSERT(false);
    }

    CPPUNIT_ASSERT(terrainRead.getLogicTree()->equals(terrainWrite.getLogicTree()));
    auto m = terrainWrite.getLeftMallet();
    CPPUNIT_ASSERT(!!m);
    if(m)
    {
        m->getParent()->empty();
    }
    CPPUNIT_ASSERT(!terrainRead.getLogicTree()->equals(terrainWrite.getLogicTree()));


    delete racine;
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::testLectureArbreRenduXMLCorrompu()
///
/// Cas de test: On tente de lire un fichier XML de format invalide
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void ConfigSceneTest::testLectureArbreRenduXMLCorrompu()
{
	// Chargement du fichier
	TiXmlDocument document;
	document.LoadFile( "tests_xml\\xmlCorrompu.xml" );
	
	// On tente de le lire
	try
	{
		ConfigScene::obtenirInstance()->lireDOM(document, arbre);
		CPPUNIT_ASSERT(false);
	}
	catch(...)
	{
		// La méthode doit lancer une exception
		CPPUNIT_ASSERT(true);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::testLectureArbreRenduXMLInvalide()
///
/// Cas de test: On tente de lire un fichier XML possédant un attribut non-reconnu
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void ConfigSceneTest::testLectureArbreRenduXMLInvalide()
{
	// Chargement du document
	TiXmlDocument document;
	document.LoadFile( "tests_xml\\xmlInvalideAttributInexistant.xml" );
	
	// On tente de le lire
	try
	{
		ConfigScene::obtenirInstance()->lireDOM(document, arbre);
		CPPUNIT_ASSERT(false);
	}
	catch (ExceptionJeu&)	
	{
		// La méthode doit lancer une exception
		CPPUNIT_ASSERT(true);
	}

	// On répète avec un autre document
	document.LoadFile( "tests_xml\\xmlInvalideNoeudInexistant.xml" );
	try
	{
		ConfigScene::obtenirInstance()->lireDOM(document, arbre);
		CPPUNIT_ASSERT(false);
	}
	catch (ExceptionJeu&)	
	{
		CPPUNIT_ASSERT(true);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::testLectureEcritureJoueurXML()
///
/// Test de lecture d'un fichier XML
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ConfigSceneTest::testLectureEcritureJoueurXML()
{
	ConteneurJoueur conteneurJoueur;
	SPJoueurAbstrait joueurs[15];
	joueurs[0] =  SPJoueurAbstrait(new JoueurHumain("Michaël"		  ));
	joueurs[1] =  SPJoueurAbstrait(new JoueurHumain("Samuel"		  ));
	joueurs[2] =  SPJoueurAbstrait(new JoueurHumain("Vincent"		  ));
	joueurs[3] =  SPJoueurAbstrait(new JoueurHumain("Mathieu"		  ));
	joueurs[4] =  SPJoueurAbstrait(new JoueurHumain("Gabriel"		  ));
	joueurs[5] =  SPJoueurAbstrait(new JoueurHumain("Charles Etienne" ));
	joueurs[6] =  SPJoueurAbstrait(new JoueurVirtuel("bot1",10,50)	  );
	joueurs[7] =  SPJoueurAbstrait(new JoueurVirtuel("bot2",15,25)	  );
	joueurs[8] =  SPJoueurAbstrait(new JoueurVirtuel("bot3",25,0)	  );
	joueurs[9] =  SPJoueurAbstrait(new JoueurVirtuel("bot4",10,-10)   );
	joueurs[10] = SPJoueurAbstrait(new JoueurVirtuel("bot5",12,25)	  );
	joueurs[11] = SPJoueurAbstrait(new JoueurVirtuel("bot6",48,75)	  );
	joueurs[12] = SPJoueurAbstrait(new JoueurVirtuel("bot8",95,60)	  );
	joueurs[13] = SPJoueurAbstrait(new JoueurVirtuel("bot9",47,55)	  );
	joueurs[14].reset();

	for (int i = 0; i < 14 ; i++)
	{
		conteneurJoueur[joueurs[i]->obtenirNom()] = joueurs[i];
	}
	conteneurJoueur["bidons"] = joueurs[14];

	// Écriture initiale du document
	TiXmlDocument document;
	ConfigScene::obtenirInstance()->creerDOM(document, conteneurJoueur);
	document.SaveFile( "tests_xml\\TestEnregistrementJoueur.xml" );
	
	// Chargement du document
	ConteneurJoueur conteneurJoueur2;
	ConfigScene::obtenirInstance()->lireDOM(document, conteneurJoueur2);
	CPPUNIT_ASSERT(conteneurJoueur2.find("Michaël") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("Samuel") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("Vincent") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("Mathieu") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("Gabriel") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("Charles Etienne") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("bot1") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("bot2") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("bot3") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("bot4") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("bot5") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("bot6") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("bot8") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("bot9") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("random") == conteneurJoueur2.end());

    remove("tests_xml\\TestEnregistrementJoueur.xml");
	//for_each(conteneurJoueur.begin(),conteneurJoueur.end(),utilitaire::LibererMappe());
	//for_each(conteneurJoueur2.begin(),conteneurJoueur2.end(),utilitaire::LibererMappe());

}




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


