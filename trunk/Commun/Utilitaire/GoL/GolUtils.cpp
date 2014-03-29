#pragma once
#include "GolPattern.h"
#include "GolUtils.h"
#include <fstream>
#include <regex>

namespace GolUtils
{
    GolPattern* decodeRLEFile( char* filename )
    {
        GolPattern* pattern = new GolPattern();
        std::ifstream file( filename );
        if( !file.fail() )
        {
            std::regex reg("x = (\\d+), y = (\\d+)");
            std::string line;
            do
            {
                getline( file, line );

            } while( !file.eof() );



            char currentChar;
            file >> currentChar;
            while( !file.eof() )
            {
                switch( currentChar )
                {
                default:
                    break;
                }
                if( currentChar == '#' )
                {
                    file.ignore( 9999999, '\n' );
                }

            }
        }
        file.close();
    }
};