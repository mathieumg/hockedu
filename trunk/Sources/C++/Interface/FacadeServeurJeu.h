
#ifdef CREATING_DLL
#define DLLEXPORT_SERVEUR_JEU __declspec(dllexport)
#elif defined(LOADING_DLL)
#define DLLEXPORT_SERVEUR_JEU __declspec(dllimport)
#else
#define DLLEXPORT_SERVEUR_JEU
#endif

#include <string>
#include "../../C#/UIHeavyClient/UIHeavyClient/Enum_Declarations.h"


extern "C" 
{
    DLLEXPORT_SERVEUR_JEU void InitDLLServeurJeu(std::string& pMasterServerIP);




    class ControllerInterface;
    class Paquet;
   
}



//////////////////////////////////////////////////////////////////////////////
/// @file FacadeServeurJeu.h
/// @author Mathieu Parent
/// @date 2013-03-18
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#ifndef __APPLICATION_FACADESERVEURJEU_H__
#define __APPLICATION_FACADESERVEURJEU_H__



///////////////////////////////////////////////////////////////////////////
/// @class FacadeServeurJeu
/// @brief Classe qui constitue une interface (une fa�ade) sur l'ensemble
///        du mod�le du serveur de jeu
///
/// @author Mathieu Parent
/// @date 2013-03-18
///////////////////////////////////////////////////////////////////////////
class FacadeServeurJeu
{
public:


    /// Obtient l'instance unique de la classe.
    static FacadeServeurJeu* getInstance();
    /// Lib�re l'instance unique de la classe.
    static void libererInstance();
    /// Checks if the instance exists
    static bool Exists(){return !!instance_;}



    /// Anime la sc�ne.
    void animer( const float& temps);


    /// Send event to the controller
    static void transmitEvent( EventCodes pCode, ... );

private:
    /// Constructeur par d�faut.
    FacadeServeurJeu();
    /// Destructeur.
    ~FacadeServeurJeu();
    /// Constructeur copie.  D�clar� mais non d�fini pour �viter qu'il soit
    /// g�n�r� par le compilateur.
    FacadeServeurJeu(const FacadeServeurJeu&);
    /// Op�rateur d'assignation.  D�clar� mais non d�fini pour �viter qu'il
    /// soit g�n�r� par le compilateur.
    FacadeServeurJeu& operator =(const FacadeServeurJeu&);
    /// Liberation de la m�moire
    void libererMemoire();

    /// Pointeur vers l'instance unique de la classe.
    static FacadeServeurJeu* instance_;

    // interval entre les ticks (en ms)
    int mTickInterval;

    /// Accesseurs
public:
    inline int getTickInterval() const { return mTickInterval; }
};








#endif // __APPLICATION_FACADESERVEURJEU_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
