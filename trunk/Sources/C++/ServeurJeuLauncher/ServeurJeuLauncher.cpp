// ServeurJeuLauncher.cpp : main project file.

#include "stdafx.h"
#include "mainServeurJeu.h"
#include <vcclr.h>
#include "..\Interface\FacadeServeurJeu.h"

using namespace System;
using namespace HttpHockeduRequests;
using namespace System::Runtime::InteropServices;

CallbackDone mCallbackMapDownloadedDLL;


// Callback une fois que le HttpManager a terminer de telecharger la map (pont entre le C# et la DLL)
void CallbackMapDownloadedManaged(System::String^ pOutputPath, int pMapId, System::String^ pName, System::String^ pDescription, bool pIsPublic)
{
    Console::WriteLine(L"Download finished");
    char* str = (char*) Marshal::StringToHGlobalAnsi(pOutputPath).ToPointer();
    std::string ret(str);
    Marshal::FreeHGlobal(IntPtr(str));
    mCallbackMapDownloadedDLL(ret);

}

// Fonction qui appelle le download dans le HttpManager (pont entre la DLL et le C#)
void FonctionAppelDownload(int pUserId, int pMapId, CallbackDone pMapDownloadedCallback)
{
    HttpManager^ wManager = gcnew HttpManager;
    mCallbackMapDownloadedDLL = pMapDownloadedCallback;

    MapDownloadedCallBack ^delegate1 = gcnew MapDownloadedCallBack(&CallbackMapDownloadedManaged);
    wManager->downloadMap(pUserId, pMapId, "", delegate1);
}

int main(array<System::String ^> ^args)
{
    for(int i=0; i<(args->Length); i++)
    {
        Console::WriteLine(args[i]);
    }

    ManagedStartMapDownload delegateDownload = &FonctionAppelDownload;
    SetStartMapDownloadCallback(delegateDownload);

    if(args->Length > 0)
    {
        char* str = (char*) Marshal::StringToHGlobalAnsi(args[0]).ToPointer();
        std::string ret(str);
        Marshal::FreeHGlobal(IntPtr(str));
        mainServeurJeu(ret.c_str());
    }
    else
    {
        mainServeurJeu("");
    }
    return 0;
}
