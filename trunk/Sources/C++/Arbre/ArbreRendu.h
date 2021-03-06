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
#include <unordered_map>
#define HashMap std::unordered_map

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
///        rendu avec les usines qui permettent d'ajouter des noeuds �
///        cet arbre.
///
///        La profondeur de cet arbre est limit�e par la taille de la pile
///        des matrices et la taille de la pile des noms pour la s�lection
///        OpenGL, �tant donn� que chaque niveau de l'arbre effectue un
///        "push" sur chacune de ces piles lors du rendu.  L'arbre ne
///        v�rifie pas que la profondeur reste sous la limite, mais il
///        offre des fonctions permettant de le v�rifier ais�ment.
///
/// @author Martin Bisson
/// @date 2007-01-28
///////////////////////////////////////////////////////////////////////////
class ArbreRendu : public NoeudComposite
{
public:
    friend TerrainTest;
   /// Constructeur par d�faut.
   ArbreRendu(class Terrain* pField);
   /// Destructeur.
   virtual ~ArbreRendu();

   /// Ajoute une usine associ�e � un type de noeud.
   inline void ajouterUsine(
       const RazerKey type, const UsineNoeud* usine
      );

   /// Cr�e un nouveau noeud.
   NoeudAbstrait* creerNoeud(
       const RazerKey typeNouveauNoeud
      ) const;

   /// Cr�e et ajoute un nouveau noeud � l'arbre.
   NoeudAbstrait* ajouterNouveauNoeud(
       const RazerKey nomParent,
       const RazerKey typeNouveauNoeud
      );

   /// Calcule la profondeur maximale possible pour l'arbre de rendu.
   static unsigned int calculerProfondeurMaximale();

   virtual const ArbreRendu* GetTreeRoot() const {return this;}

   /// Initialisation du NoeudAbstrait � partir d'un element XML
   virtual bool initFromXml(const XmlElement* element);

   /// Creation du noeud XML du Noeud
   virtual XmlElement* createXmlNode();

private:
   /// D�finition du type pour l'association du nom d'un type vers l'usine
   /// correspondante.
    typedef HashMap< RazerKey, const UsineNoeud* > RegistreUsines;
   /// Association du nom d'un type vers l'usine correspondante.
   RegistreUsines usines_;
};


////////////////////////////////////////////////////////////////////////
///
/// @fn inline void ArbreRendu::ajouterUsine(const std::string& type, const UsineNoeud* usine)
///
/// Cette fonction permet d'ajouter une usine qui sera ensuite utilis�e
/// pour cr�er de nouveaux noeuds.
///
/// @param[in] type  : La cha�ne qui identifie le type de noeuds � cr�er
///                    avec cette usine.
/// @param[in] usine : L'usine ajout�e.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
inline void ArbreRendu::ajouterUsine(
    const RazerKey type, const UsineNoeud* usine
   )
{
   usines_[type] = usine;
}


#endif // __ARBRE_ARBRERENDU_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
