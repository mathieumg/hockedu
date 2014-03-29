#pragma once
#include <map>
#include "Vecteur.h"


typedef std::map<Vecteur2i, int> Cells;
class GolPattern
{
public:
    void revive( const int x, const int y );
    void revive( const Vecteur2i& pos);
    void kill( const int x, const int y );
    void kill( const Vecteur2i& pos );
    void clear();
    void evolve();
    const Cells& getAliveCells() const
    {
        return mAlives;
    }

private:
    Cells mAlives;
};