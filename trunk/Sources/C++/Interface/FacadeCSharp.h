// Summary:
//     Specifies constants that define which mouse button was pressed.
enum MouseButtons
{
    // Summary:
    //     No mouse button was pressed.
    None = 0,
    //
    // Summary:
    //     The left mouse button was pressed.
    Left = 1048576,  //2^20
    //
    // Summary:
    //     The right mouse button was pressed.
    Right = 2097152,
    //
    // Summary:
    //     The middle mouse button was pressed.
    Middle = 4194304,
    //
    // Summary:
    //     The first XButton was pressed.
    XButton1 = 8388608,
    //
    // Summary:
    //     The second XButton was pressed.
    XButton2 = 16777216,
};

// Test pour l'appel de la DLL depuis le C#
extern "C" 
{
    __declspec(dllexport) void InitOpenGL(HWND hWnd);
    __declspec(dllexport) void RenderOpenGL();
    __declspec(dllexport) void WindowResized(int largeur, int hauteur);
    __declspec(dllexport) void LogicUpdate(float time);
    __declspec(dllexport) int ExecuteUnitTest();
    __declspec(dllexport) void InitDLL();
    __declspec(dllexport) void RequestLogin( char* pUsername, char* pIpAdress );
    __declspec(dllexport) void CancelConnection( char* pUsername );
    __declspec(dllexport) void SendMessageDLL( char* pUsername, char * pMessage );

    __declspec(dllexport) bool ActionPerformed( char* action );
    __declspec(dllexport) bool IsGamePaused();
    __declspec(dllexport) void PauseGame(bool doPause);
    __declspec(dllexport) void GenerateDefaultField();
    __declspec(dllexport) bool ValidateField();

    

    

    ///////////////////////////////////////////////////////////////////////////////
    // User mouse and keyboard events
    __declspec(dllexport) void OnKeyPressed(int key);
    __declspec(dllexport) void OnKeyReleased(int key);
    __declspec(dllexport) void OnMousePressed( int x, int y, MouseButtons button);
    __declspec(dllexport) void OnMouseReleased( int x, int y, MouseButtons button);
    __declspec(dllexport) void OnMouseMoved( int x, int y, MouseButtons button );
    __declspec(dllexport) void OnMouseWheelMoved( int deltaRotation );
    ///////////////////////////////////////////////////////////////////////////////



    //////////////////////////////////////////////////////////////////////////
    /// Chat functions
    /// declare the callback prototype
    typedef BOOL (__stdcall *MessageReceivedCallBack)( char* pUsername, char* pMessage );
    /// Enregistre la callback pour mettre a jour la vue lors de nouveau message
    __declspec(dllexport) void SetMessageCallback( MessageReceivedCallBack callback );

    /// declare the callback prototype
    typedef BOOL (__stdcall *EventReceivedCallBack)( int pId, char* pMessage );
    /// Enregistre la callback pour mettre a jour la vue lors d'événement
    __declspec(dllexport) void SetEventCallback( EventReceivedCallBack callback );

    __declspec(dllexport) void DisconnectUser( char* pUsername );
    //////////////////////////////////////////////////////////////////////////
}