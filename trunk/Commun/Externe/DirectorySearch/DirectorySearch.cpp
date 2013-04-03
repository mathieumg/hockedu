#include "DirectoySearch.h"

void PrintErrorMessage(LPTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw); 
}

int SearchDirectory(std::vector<std::string> &refvecFiles,
    const std::string &refcstrRootDirectory,
    const std::map<std::string,bool>& extensions,
    bool bSearchSubdirectories /*= true*/)
{
    std::string strFilePath; // Filepath
    std::string strPattern; // Pattern
    std::string strExtension; // Extension
    HANDLE hFile; // Handle to file
    WIN32_FIND_DATAA FileInformation; // File information

    strPattern = refcstrRootDirectory + "\\*.*";

    hFile = ::FindFirstFileA(strPattern.c_str(), &FileInformation);
    if(hFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if(FileInformation.cFileName[0] != '.')
            {
                strFilePath.erase();
                strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;

                if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    if(bSearchSubdirectories)
                    {
                        // Search subdirectory
                        int iRC = SearchDirectory(refvecFiles,
                            strFilePath,
                            extensions,
                            bSearchSubdirectories);
                        if(iRC)
                            return iRC;
                    }
                }
                else
                {
                    // Check extension
                    strExtension = FileInformation.cFileName;
                    strExtension = strExtension.substr(strExtension.rfind(".") + 1);
                    std::map<std::string,bool>::const_iterator itExtension = extensions.find(strExtension);
                    if( itExtension != extensions.end() && itExtension->second)
                    //if(strExtension == refcstrExtension)
                    {
                        // Save filename
                        refvecFiles.push_back(strFilePath);
                    }
                }
            }
        } while(::FindNextFileA(hFile, &FileInformation) == TRUE);

        // Close handle
        ::FindClose(hFile);

        DWORD dwError = ::GetLastError();
        if(dwError != ERROR_NO_MORE_FILES)
            return dwError;
    }

    return 0;
}

// first arg is the path, the nexts are the desired extensions to search
int printSearchDocument(int argc, char** argv)
{
    int iRC = 0;
    std::vector<std::string> vecFiles;
    std::vector<std::string> vecTxtFiles;
    const std::string defaultPath("D:");
    const std::string defaultExtension("jpg");
    
    
    std::string directoryPath = defaultPath;
    //std::string extension = defaultExtension;
    std::map<std::string,bool> extensions;

    if(argc > 1)
    {
        directoryPath = argv[1];
    }
    //extensions
    if(argc <= 2)
    {
        extensions[defaultExtension] = true;
    }
    else
    {
        for(int iExtensions = 2; iExtensions<argc; ++iExtensions)
        {
            extensions[argv[iExtensions]] = true;
        }
    }
    
    // Search 'C:/Program Files/BitComet/Downloads' for '.avi' files including subdirectories
    iRC = SearchDirectory(vecFiles, directoryPath, extensions);
    if(iRC)
    {
        PrintErrorMessage(TEXT("SearchDirectory"));
        std::cout << "Error " << iRC << std::endl;
        return -1;
    }

    // Print results
    for(std::vector<std::string>::iterator iterFiles = vecFiles.begin();
        iterFiles != vecFiles.end();
        ++iterFiles)
    {
        std::cout << *iterFiles << std::endl;
    }

    // Wait for keystroke
    _getch();

    return 0;
}