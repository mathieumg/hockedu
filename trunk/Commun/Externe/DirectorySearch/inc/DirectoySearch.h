#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <strsafe.h>
#include <map>

void PrintErrorMessage(LPTSTR lpszFunction);
int SearchDirectory(std::vector<std::string> &refvecFiles,
    const std::string &refcstrRootDirectory,
    const std::map<std::string,bool>& extensions,
    //const std::string &refcstrExtension,
    bool bSearchSubdirectories = true);
int printSearchDocument(int argc, char** argv);

