// Test pour l'appel de la DLL depuis le C#
extern "C" 
{
    __declspec(dllexport) int TestCSCall(int i);
    __declspec(dllexport) int ExecuteUnitTest();
    __declspec(dllexport) void InitDLL(char * pName);
    __declspec(dllexport) void SendMessageDLL(char * pMessage);
    __declspec(dllexport) void GetMessageDLL(char * pMessage , int* pBufferSize);
}