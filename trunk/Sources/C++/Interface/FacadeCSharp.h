// Test pour l'appel de la DLL depuis le C#
extern "C" 
{
    __declspec(dllexport) void initialiserOpenGL(HWND hWnd);
    __declspec(dllexport) void renderOpenGL();
    __declspec(dllexport) void WindowResized(int largeur, int hauteur);

    __declspec(dllexport) int ExecuteUnitTest();
    __declspec(dllexport) void InitDLL();
    __declspec(dllexport) void RequestLogin( char* pUsername, char* pIpAdress );
    __declspec(dllexport) void CancelConnection( char* pUsername );

    __declspec(dllexport) void SendMessageDLL( char* pUsername, char * pMessage );

    

    /// declare the callback prototype
    typedef BOOL (__stdcall *MessageReceivedCallBack)( char* pUsername, char* pMessage );
    /// Enregistre la callback pour mettre a jour la vue lors de nouveau message
    __declspec(dllexport) void SetMessageCallback( MessageReceivedCallBack callback );

    /// declare the callback prototype
    typedef BOOL (__stdcall *EventReceivedCallBack)( int pId, char* pMessage );
    /// Enregistre la callback pour mettre a jour la vue lors d'événement
    __declspec(dllexport) void SetEventCallback( EventReceivedCallBack callback );

    __declspec(dllexport) void DisconnectUser( char* pUsername );
}