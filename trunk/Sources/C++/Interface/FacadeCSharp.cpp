#include "FacadeCSharp.h"
#include "UsineNoeudMaillet.h"
#include "UsineNoeudRondelle.h"
#include "BancTests.h"

// Test pour l'appel de la DLL depuis le C#
int TestCSCall(int i)
{
    return i*2;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ExecuteUnitTest()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
int ExecuteUnitTest()
{
    UsineNoeudMaillet::bypassLimitePourTest = true;
    UsineNoeudRondelle::bypassLimitePourTest = true;
    bool reussite = BancTests::obtenirInstance()->executer();

    UsineNoeudMaillet::bypassLimitePourTest = false;
    UsineNoeudRondelle::bypassLimitePourTest = false;
    // Visual Studio interpr�te le code de retour 0 comme une r�ussite et le code
    // de retour 1 comme un �chec. Nous transmettons le code de retour � Java
    // qui le transmet directement comme code de sortie du programme.
    system("pause");
    return reussite ? 0 : 1;
}
