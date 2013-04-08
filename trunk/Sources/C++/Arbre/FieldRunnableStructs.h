

#pragma once

class FieldRunnable
{
public:
    virtual ~FieldRunnable(){}
    virtual void execute() = 0;
};


class FieldRunnablePortal : public FieldRunnable
{
public:
    class NoeudPortail* portal;
    class NoeudRondelle* puck;
    virtual void execute();
};

class FieldRunnableGoals : public FieldRunnable
{
public:
    class Partie* game;
    class NoeudRondelle* puck;
    virtual void execute();
};

class FieldRunnableBonus : public FieldRunnable
{
public:
    class NodeBonus* bonus;
    class NoeudRondelle* puck;
    virtual void execute();
};

