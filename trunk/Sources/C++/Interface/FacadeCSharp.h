// Test pour l'appel de la DLL depuis le C#
extern "C" 
{
    __declspec(dllexport) int TestCSCall(int i);
}