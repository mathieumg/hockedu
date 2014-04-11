#pragma once
#include "GolPattern.h"
#include "GolUtils.h"
#include <fstream>
#include <regex>

namespace GolUtils
{
    GolPattern* decodeRLEFile( char* filename )
    {
        GolPattern* pattern = NULL;
        std::ifstream file( filename );
        if( !file.fail() )
        {
            pattern = new GolPattern();

            int d = 0, x = 0, y = 0;
            std::smatch sm;
            std::regex reg("x = (\\d+), y = (\\d+).*");
            std::string line;
            do
            {
                getline( file, line );
                if( std::regex_match( line, sm, reg ) )
                {
                    //y = atoi(sm[2].str().c_str());
                    break;
                }
            } while( !file.eof() );


            char c;
            int nextValue = 1;
            do
            {
                {
                    file >> c;
                    switch( c )
                    {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        d *= 10;
                        d += c - '0';
                        nextValue = d;
                        break;
                    case '#':
                        file.ignore( 9999999, '\n' );
                        break;
                    case 'b':
                        x += nextValue;
                        d = 0;
                        nextValue = 1;
                        break;
                    case 'o':
                        for (int i = x; i < x+nextValue ; ++i)
                        {
                            pattern->revive( i, y );
                        }
                        d = 0;
                        nextValue = 1;
                        break;
                    case '$':
                        y++;
                        x = 0;
                        break;
                    default:
                        break;
                    }
                }
                
            } while( !file.eof() );
        }
        file.close();
        return pattern;
    }
};