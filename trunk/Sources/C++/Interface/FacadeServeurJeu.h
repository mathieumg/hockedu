
// Test pour l'appel de la DLL depuis le C#
extern "C" 
{
    __declspec(dllexport) void InitDLLServeurJeu();

    class ControllerInterface;
    class Paquet;
    //////////////////////////////////////////////////////////////////////////
    /// Network functions
    __declspec(dllexport) void envoyerPaquet( Paquet* pPaquet );
    //__declspec(dllexport) void recevoirPaquet( Paquet* pPaquet );
    /// 
    //////////////////////////////////////////////////////////////////////////
   
}