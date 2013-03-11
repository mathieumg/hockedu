///////////////////////////////////////////////////////////////////////////////
/// @file ArbreRendu.h
/// @author Martin Bisson
/// @date 2007-01-28
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_ARBRERENDU_H__
#define __ARBRE_ARBRERENDU_H__


#include "NoeudComposite.h"


#ifndef __APPLE__
#include <hash_map>
#define HashMap std::hash_map

#else

#include <ext/hash_map>
#define HashMap __gnu_cxx::hash_map
namespace __gnu_cxx
{
    template<> struct hash< std::string >
    {
        size_t operator()( const std::string& x ) const
        {
            return hash< const char* >()( x.c_str() );
        }
    };
}
#endif

class NoeudAbstrait;
class UsineNoeud;
class TerrainTest;

///////////////////////////////////////////////////////////////////////////
/// @class ArbreRendu
/// @brief Classe d'arbre de rendu qui contient la racine de l'arbre de
///        rendu avec les usines qui permettent d'ajouter des noeuds à
///        cet arbre.
///
///        La profondeur de cet arbre est limitée par la taille de la pile
///        des matrices et la taille de la pile des noms pour la sélection
///        OpenGL, étant donné que chaque niveau de l'arbre effectue un
///        "push" sur chacune de ces piles lors du rendu.  L'arbre ne
///        vérifie pas que la profondeur reste sous la limite, mais il
///        offre des fonctions permettant de le vérifier aisément.
///
/// @author Martin Bisson
/// @date 2007-01-28
///////////////////////////////////////////////////////////////////////////
class ArbreRendu : public NoeudComposite
{
public:
    friend TerrainTest;
   /// Constructeur par défaut.
   ArbreRendu(class Terrain* pField);
   /// Destructeur.
   virtual ~ArbreRendu();

   /// Ajoute une usine associée à un type de noeud.
   inline void ajouterUsine(
      const std::string& type, const UsineNoeud* usine
      );

   /// Crée un nouveau noeud.
   NoeudAbstrait* creerNoeud(
      const std::string& typeNouveauNoeud
      ) const;

   /// Crée et ajoute un nouveau noeud à l'arbre.
   NoeudAbstrait* ajouterNouveauNoeud(
      const std::string& nomParent,
      const std::string& typeNouveauNoeud
      );

   /// Calcule la profondeur maximale possible pour l'arbre de rendu.
   static unsigned int calculerProfondeurMaximale();

   virtual const ArbreRendu* GetTreeRoot() const {return this;}

   /// Initialisation du NoeudAbstrait à partir d'un element XML
   virtual bool initialiser(const XmlElement* element);

   /// Creation du noeud XML du Noeud
   virtual XmlElement* creerNoeudXML();

private:
   /// Définition du type pour l'association du nom d'un type vers l'usine
   /// correspondante.
    typedef HashMap< std::string, const UsineNoeud* > RegistreUsines;
   /// Association du nom d'un type vers l'usine correspondante.
   RegistreUsines usines_;
};


////////////////////////////////////////////////////////////////////////
///
/// @fn inline void ArbreRendu::ajouterUsine(const std::string& type, const UsineNoeud* usine)
///
/// Cette fonction permet d'ajouter une usine qui sera ensuite utilisée
/// pour créer de nouveaux noeuds.
///
/// @param[in] type  : La chaîne qui identifie le type de noeuds à créer
///                    avec cette usine.
/// @param[in] usine : L'usine ajoutée.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
inline void ArbreRendu::ajouterUsine(
   const std::string& type, const UsineNoeud* usine
   )
{
   usines_[type] = usine;
}


#endif // __ARBRE_ARBRERENDU_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
