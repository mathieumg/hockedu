#include "GolPattern.h"


void GolPattern::revive( const int x, const int y )
{
    revive( Vecteur2i( x, y ) );
}

void GolPattern::revive( const Vecteur2i& pos )
{
    mAlives[pos] = 0;
}

void GolPattern::kill( const int x, const int y )
{
    kill(Vecteur2i( x, y ));
}

void GolPattern::kill( const Vecteur2i& pos )
{
    auto it = mAlives.find( pos );
    if( it != mAlives.end() )
    {
        mAlives.erase( it );
    }
}

void GolPattern::clear()
{
    mAlives.clear();
}

const Vecteur2i deltaPosAround[8] = {
    Vecteur2i(1,0),
    Vecteur2i(1,1),
    Vecteur2i(0,1),
    Vecteur2i(-1,1),
    Vecteur2i(-1,0),
    Vecteur2i(-1,-1),
    Vecteur2i(0,-1),
    Vecteur2i(1,-1),
};

void GolPattern::evolve( )
{
    Cells maybes;
    // Update adjacence
    for( Cells::iterator it = mAlives.begin(); it != mAlives.end(); ++it )
    {
        for( int i = 0; i < 8; ++i )
        {
            Vecteur2i c = it->first + deltaPosAround[i];
            if( mAlives.find( c ) != mAlives.end() )
            {
                ++it->second;
            }
            else
            {
                // update maybes
                Cells::iterator it2 = maybes.find( c );
                if( it2 == maybes.end() )
                {
                    maybes[c] = 1;
                }
                else
                {
                    ++it2->second;
                }
            }
        }
    }

    // kill cells
    for( Cells::iterator it = mAlives.begin(); it != mAlives.end(); /* no increment */ )
    {
        const int adj = it->second;
        if( (unsigned int)( adj - 2 ) > 1 ) // only if adj is 2 or 3
        {
            mAlives.erase( it++ );
        }
        else
        {
            // reset
            it->second = 0;
            ++it;
        }
    }

    // resume life
    for( Cells::const_iterator it = maybes.begin(); it != maybes.end(); ++it )
    {
        const int adj = it->second;
        if( adj == 3 ) // only if adj is 3
        {
            mAlives[it->first] = 0;
        }
    }
    maybes.clear();
}

