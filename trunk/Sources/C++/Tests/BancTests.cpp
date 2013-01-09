////////////////////////////////////////////////////////////////////////////////////
/// @file BancTests.cpp
/// @author Julien Gascon-Samson
/// @date 2011-07-16
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
////////////////////////////////////////////////////////////////////////////////////

#include "BancTests.h"

// Inclusions cppunit pour l'ex�cution des tests
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

SINGLETON_DECLARATION_CPP(BancTests);

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BancTests::executer()
///
/// Cette fonction ex�cute l'ensemble des tests unitaires d�finis.
/// La sortie de l'ex�cution des tests se fait dans la console standard
/// d'erreurs 'cerr'. Cette fonction ajuste �galement le format de
/// sortie pour correspondre � celui de Visual Studio afin d'int�grer
/// l'ex�cution des tests au processus de compilation ("Post Build Event").
///
/// @return true si l'ex�cution de tous les tests a r�ussi, sinon false.
///
////////////////////////////////////////////////////////////////////////
bool BancTests::executer()
{
  // Obtenir la suite de tests � haut niveau
  CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

  // Obtient un environnement d'ex�cution de tests qui imprime les r�sultats
  // dans une console (cout, cerr, fichier, etc.) et ajoute la suite de tests
  // de base � l'environnement.
  // (Notez qu'il est aussi possible d'obtenir un environnement qui affiche
  // les r�sultats des tests au sein d'une interface graphique QT ou MFC.
  // Consultez la documentation cppunit pour plus d'informations)
  CppUnit::TextUi::TestRunner runner;
  runner.addTest( suite );

  // Indique que nous souhaitons formatter la sortie selon un format qui
  // s'apparente � la sortie d'un compilateur (MSVC++), et que nous
  // souhaitons que la sortie soit r�alis�e dans le canal standard cerr.
  // Cela permettra � Visual Studio d'interpr�ter la sortie de cppunit,
  // d'indiquer les erreurs trouv�es par les tests et leur num�ro de ligne
  // en tant qu'erreurs survenant au niveau du processus de compilation.
  runner.setOutputter( new CppUnit::CompilerOutputter( &runner.result(),
                                                       std::cerr ) );
  // Ex�cuter les tests
  return runner.run();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
