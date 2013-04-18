///////////////////////////////////////////////////////////////////////////////
/// @file AideCollision.cpp
/// @brief Ce fichier contient l'implantation de l'espace de nom aidecollision.
///
/// Il contient les implantations de fonctions utiles pour le calcul des forces
/// causées par les collisions.
///
/// @author Martin Bisson
/// @date 2007-01-10
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "AideCollision.h"
#include "Utilitaire.h"
#include <stdlib.h>
namespace aidecollision {


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn DetailsCollision calculerCollisionSegment(
	///        const Vecteur3& point1,
	///        const Vecteur3& point2,
	///        const Vecteur3& position,
	///        float          rayon,
	///        bool            collisionAvecPoints
	///        );
	///
	/// Cette fonction calcule l'intersection d'une sphère, identifiée par son
	/// centre et son rayon, avec un segment de droite, identifié par les 
	/// coordonnées de chacune de ses extrémités.
	///
	/// @param[in] point1              : Première extrémité du segment de
	///                                  droite.
	/// @param[in] point2              : Seconde extrémité du segment de
	///                                  droite.
	/// @param[in] position            : Centre de la sphère.
	/// @param[in] rayon               : Rayon de la sphère.
	/// @param[in] collisionAvecPoints : Vrai si on veut les collisions avec
	///                                  les extrémités.
	///
	/// @return Structure contenant de l'information sur la collision.
	///
	////////////////////////////////////////////////////////////////////////
	DetailsCollision calculerCollisionSegment(
		const Vecteur3& point1,
		const Vecteur3& point2,
		const Vecteur3& position,
		float          rayon,
		bool            collisionAvecPoints //= true
		)
	{
		// Valeur de retour.
		DetailsCollision detailsCollision;
		detailsCollision.type = COLLISION_AUCUNE;

		// On calcule la projection de la position de la sphère sur le vecteur
		// du segment de droite.
		const Vecteur3 segment = point2 - point1;
		const float ratio = produitScalaire(position - point1, segment) /
			segment.norme2();

		if (collisionAvecPoints) {
			if (ratio < 0.0) {
				// On est du côté du premier point.
				const Vecteur3 directionCollision = position - point1;
				const float distance = directionCollision.norme();
				if (distance < rayon) {
					detailsCollision.type = COLLISION_SEGMENT_PREMIERPOINT;
					detailsCollision.direction = directionCollision / distance;
					detailsCollision.enfoncement = rayon - distance;
				}
			}
			else if (ratio > 1.0) {
				// On est du côté du second point.
				const Vecteur3 directionCollision = position - point2;
				const float distance = directionCollision.norme();
				if (distance < rayon) {
					detailsCollision.type = COLLISION_SEGMENT_DEUXIEMEPOINT;
					detailsCollision.direction = directionCollision / distance;
					detailsCollision.enfoncement = rayon - distance;
				}
			}
		}

		if (ratio >= 0.0 && ratio <= 1.0) {
			// On est sur le segment de droite.
			Vecteur3f pointPerpendiculaire = (1 - ratio) * point1 + ratio * point2;
			const Vecteur3 directionCollision = position - pointPerpendiculaire;
			const float distance = directionCollision.norme();
			if (distance < rayon) {
				detailsCollision.type = COLLISION_SEGMENT;
				detailsCollision.direction = directionCollision / distance;
				detailsCollision.enfoncement = rayon - distance;
			}
		}

		return detailsCollision;
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn DetailsCollision calculerCollisionCercle(
	///        const Vecteur2& centreCercle,
	///        float          rayonCercle,
	///        const Vecteur2& positionObjet,
	///        float          rayonObjet
	///        );
	///
	/// Cette fonction calcule l'intersection d'un cercle avec un autre
	/// cercle, chacun identifié par son centre et son rayon.
	///
	/// Elle ne fait qu'appeler la version 3D de cette même fonction.
	///
	/// @param[in] centreCercle  : Centre du cercle avec lequel on teste la
	///                            collision.
	/// @param[in] rayonCercle   : Rayon du cercle avec lequel on teste la
	///                            collision.
	/// @param[in] positionObjet : Position de l'objet peut-être en collision.
	/// @param[in] rayonObjet    : Rayon de l'objet peut-être en collision.
	///
	/// @return Structure contenant de l'information sur la collision.
	///
	////////////////////////////////////////////////////////////////////////
	DetailsCollision calculerCollisionCercle(
		const Vecteur2& centreCercle,
		float          rayonCercle,
		const Vecteur2& positionObjet,
		float          rayonObjet
		)
	{
		return calculerCollisionSphere(
			centreCercle.convertir<3>(),
			rayonCercle,
			positionObjet.convertir<3>(),
			rayonObjet
			);
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn DetailsCollision calculerCollisionArc(
	///        const Vecteur2& centreCercle,
	///        const Vecteur2& pointArc1,
	///        const Vecteur2& pointArc2,
	///        const Vecteur2& positionObjet,
	///        float          rayonObjet
	///        );
	///
	/// Cette fonction calcule l'intersection d'un cercle identifié par
	/// son centre et son rayon, avec un arc de cercle identifié par ses
	/// deux extrémités et son centre.
	///
	/// @param[in] centreCercle  : Centre de l'arc avec lequel on teste
	///                            la collision.
	/// @param[in] pointArc1     : Première extrémité de l'arc avec lequel on
	///                            teste la collision.
	/// @param[in] pointArc2     : Seconde extrémité de l'arc avec lequel on
	///                            teste la collision.
	/// @param[in] positionObjet : Position de l'objet peut-être en collision.
	/// @param[in] rayonObjet    : Rayon de l'objet peut-être en collision.
	///
	/// @return Structure contenant de l'information sur la collision.
	///
	////////////////////////////////////////////////////////////////////////
	DetailsCollision calculerCollisionArc(
		const Vecteur2& centreCercle,
		const Vecteur2& pointArc1,
		const Vecteur2& pointArc2,
		const Vecteur2& positionObjet,
		float          rayonObjet
		)
	{
		// Valeur de retour.
		DetailsCollision detailsCollision;
		detailsCollision.type = COLLISION_AUCUNE;

		// Rayon de l'arc de cercle.
		float rayonCercle = (pointArc1 - centreCercle).norme();

		detailsCollision.direction = (positionObjet - centreCercle).convertir<3>();
		detailsCollision.enfoncement = rayonCercle + rayonObjet -
			detailsCollision.direction.norme();

		if (detailsCollision.enfoncement > 0) {
			Vecteur3 direction1 = (pointArc1 - centreCercle).convertir<3>();
			Vecteur3 direction2 = (pointArc2 - centreCercle).convertir<3>();

			// On vérifie si la direction est située à l'intérieur de l'arc à
			// l'aide de produits vectoriels.
			float sensExtremites = produitVectoriel(direction1, direction2)[2];
			float sens1          = produitVectoriel(direction1, detailsCollision.direction)[2];
			float sens2          = produitVectoriel(detailsCollision.direction, direction2)[2];

			if ( ((sens1 > 0.0) == (sensExtremites > 0.0)) &&
				((sens1 > 0.0) == (sens2 > 0.0)) ) {
					detailsCollision.type = COLLISION_ARC;
			}
		}

		return detailsCollision;      
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn DetailsCollision calculerCollisionSphere( const Vecteur3& centreSphere, float rayonSphere, const Vecteur3& positionObjet, float rayonObjet );
	///
	/// Cette fonction calcule l'intersection d'une sphère avec une autre
	/// sphère, chacune identifiée par son centre et son rayon.
	///
	/// @param[in] centreSphere  : Centre de la sphère avec laquelle on teste la
	///                            collision.
	/// @param[in] rayonSphere   : Rayon de la sphère avec laquelle on teste la
	///                            collision.
	/// @param[in] positionObjet : Position de l'objet peut-être en collision.
	/// @param[in] rayonObjet    : Rayon de l'objet peut-être en collision.
	///
	/// @return Structure contenant de l'information sur la collision.
	///
	////////////////////////////////////////////////////////////////////////
	DetailsCollision calculerCollisionSphere(
		const Vecteur3& centreSphere,
		float          rayonSphere,
		const Vecteur3& positionObjet,
		float          rayonObjet
		)
	{
		// Valeur de retour
		DetailsCollision detailsCollision;

		detailsCollision.direction = positionObjet - centreSphere;
		detailsCollision.enfoncement = rayonSphere + rayonObjet -
			detailsCollision.direction.norme();

		if (detailsCollision.enfoncement > 0)
			detailsCollision.type = COLLISION_SPHERE;
		else
			detailsCollision.type = COLLISION_AUCUNE;

		return detailsCollision;
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn Vecteur2 calculerForceRebondissement2D(
	///        const DetailsCollision& details,
	///        float constanteRebondissement
	///        );
	///
	/// Cette fonction calcule la force causée par une collision à partir
	/// de la loi des ressorts qui stipule que la force est proportionnelle
	/// à une constante caractérisant le ressort (dans ce cas l'objet sur
	/// lequel on rebondit) et à l'étirement ou la compression du ressort
	/// (dans ce cas l'enfoncement).
	///
	///
	/// @param[in] details                 : Détails de la collision.
	/// @param[in] constanteRebondissement : Constante de rebondissement de
	///                                      l'objet.
	///
	/// @return Force causée par la collision, en deux dimensions.
	///
	////////////////////////////////////////////////////////////////////////
	Vecteur2 calculerForceRebondissement2D(
		const DetailsCollision& details,
		float constanteRebondissement
		)
	{
		if (details.type == COLLISION_AUCUNE) {
			// Pas de collision, donc pas de force
			return Vecteur2(0, 0);
		}
		else {
			// Collision: application de la loi des ressorts
			return details.enfoncement * constanteRebondissement *
				details.direction.convertir<2>();
		}
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn Vecteur3 calculerForceRebondissement3D(
	///        const DetailsCollision& details,
	///        float constanteRebondissement
	///        );
	///
	/// Cette fonction calcule la force causée par une collision à partir
	/// de la loi des ressorts qui stipule que la force est proportionnelle
	/// à une constante caractérisant le ressort (dans ce cas l'objet sur
	/// lequel on rebondit) et à l'étirement ou la compression du ressort
	/// (dans ce cas l'enfoncement).
	///
	/// @param[in] details                 : Détails de la collision.
	/// @param[in] constanteRebondissement : Constante de rebondissement de
	///                                      l'objet.
	///
	/// @return Force causée par la collision, en trois dimensions.
	///
	////////////////////////////////////////////////////////////////////////
	Vecteur3 calculerForceRebondissement3D(
		const DetailsCollision& details,
		float constanteRebondissement
		)
	{
		if (details.type == COLLISION_AUCUNE) {
			// Pas de collision, donc pas de force.
			return Vecteur3(0, 0, 0);
		}
		else {
			// Collision: application de la loi des ressorts.
			return (float) (details.enfoncement * constanteRebondissement) *
				details.direction;
		}
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn Vecteur2 calculerForceAmortissement2D(
	///        const DetailsCollision& details,
	///        const Vecteur2& vitesse,
	///        float constanteAmortissement
	///        );
	///
	/// Cette fonction calcule une force d'amortissement proportionnelle à
	/// la vitesse et dans sa direction, causée par une collision.
	///
	/// Elle ne fait qu'appeler la version 3D de cette même fonction.
	///
	/// @param[in] details                : Détails de la collision.
	/// @param[in] vitesse                : Vitesse de l'objet en collision.
	/// @param[in] constanteAmortissement : Constante d'amortissement de
	///                                     l'objet.
	///
	/// @return Force causée par la collision, en deux dimensions.
	///
	////////////////////////////////////////////////////////////////////////
	Vecteur2 calculerForceAmortissement2D(
		const DetailsCollision& details,
		const Vecteur2& vitesse,
		float constanteAmortissement
		)
	{
		return calculerForceAmortissement3D(
			details, vitesse.convertir<3>(), constanteAmortissement
			).convertir<2>();
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn Vecteur3 calculerForceAmortissement3D(
	///        const DetailsCollision& details,
	///        const Vecteur3& vitesse,
	///        float constanteAmortissement
	///        );
	///
	/// Cette fonction calcule une force d'amortissement proportionnelle à
	/// la vitesse et dans sa direction, causée par une collision.
	///
	/// @param[in] details                : Détails de la collision.
	/// @param[in] vitesse                : Vitesse de l'objet en collision.
	/// @param[in] constanteAmortissement : Constante d'amortissement de
	///                                     l'objet.
	///
	/// @return Force causée par la collision, en trois dimensions.
	///
	////////////////////////////////////////////////////////////////////////
	Vecteur3 calculerForceAmortissement3D(
		const DetailsCollision& details,
		const Vecteur3& vitesse,
		float constanteAmortissement
		)
	{
		if (details.type == COLLISION_AUCUNE) {
			// Pas de collision, donc pas de force.
			return Vecteur3(0, 0, 0);
		}
		else {
			// Collision: application d'un amortissement.
			const Vecteur3 vitesseCollision = vitesse >> details.direction;
			return vitesseCollision * -constanteAmortissement;
		}
	}

	////////////////////////////////////////////////////////////////////////
	///
	/// @fn Vecteur3 calculerCollisionSegment(
	///        const Vecteur3&   point1,
	///        const Vecteur3&   point2,
	///        const Vecteur3&   position,
	///        float            rayon,
	///        bool              collisionAvecPoints,
	///        float            constanteRebondissement,
	///        float            constanteAmortissement,
	///        const Vecteur3&   vitesse,
	///        DetailsCollision* retourDetails = 0
	///        );
	///
	/// Cette fonction calcule la force causée par la collision d'un cercle,
	/// identifié par son centre et son rayon, avec un segment de droite,
	/// identifié par les coordonnées de chacune de ses extrémités.  Elle
	/// tient compte d'une force de rebondissement ainsi qu'une force
	/// d'amortissement.
	///
	/// @param[in]  point1                  : Première extrémité du segment de
	///                                       droite.
	/// @param[in]  point2                  : Seconde extrémité du segment de
	///                                       droite.
	/// @param[in]  position                : Centre du cercle.
	/// @param[in]  rayon                   : Rayon du cercle.
	/// @param[in]  collisionAvecPoints     : Vrai si on veut les collisions
	///                                       avec les extrémités.
	/// @param[in]  constanteRebondissement : Constante de rebondissement de
	///                                       l'objet.
	/// @param[in]  constanteAmortissement  : Constante d'amortissement de
	///                                       l'objet.
	/// @param[in]  vitesse                 : Vitesse de l'objet en collision.
	/// @param[out] retourDetails           : Pointeur pour retourner les
	///                                       détails de la collision.
	///
	/// @return Force causée par la collision, en trois dimensions.
	///
	////////////////////////////////////////////////////////////////////////
	Vecteur3 calculerCollisionSegment(
		const Vecteur3&   point1,
		const Vecteur3&   point2,
		const Vecteur3&   position,
		float            rayon,
		bool              collisionAvecPoints,
		float            constanteRebondissement,
		float            constanteAmortissement,
		const Vecteur3&   vitesse,
		DetailsCollision* retourDetails //= 0
		)
	{
		DetailsCollision details = calculerCollisionSegment(
			point1, point2,
			position, rayon,
			collisionAvecPoints
			);

		Vecteur3 force = calculerForceRebondissement3D(
			details, constanteRebondissement
			);
		force += calculerForceAmortissement3D(
			details, vitesse, constanteAmortissement
			);

		if (retourDetails)
			*retourDetails = details;

		return force;
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn Vecteur2 calculerCollisionCercle(
	///        const Vecteur2&   centreCercle,
	///        float            rayonCercle,
	///        const Vecteur2&   positionObjet,
	///        float            rayonObjet,
	///        float            constanteRebondissement,
	///        float            constanteAmortissement,
	///        const Vecteur2&   vitesse,
	///        DetailsCollision* retourDetails = 0
	///        );
	///
	/// Cette fonction calcule la force causée par la collision d'un
	/// cercle avec un autre cercle, chacun identifié par son centre
	/// et son rayon.  Elle tient compte d'une force de rebondissement
	/// ainsi qu'une force d'amortissement.
	///
	/// Elle ne fait qu'appeler la version 3D de cette même fonction.
	///
	/// @param[in]  centreCercle            : Centre du cercle avec lequel on
	///                                       teste la collision.
	/// @param[in]  rayonCercle             : Rayon du cercle avec lequel on
	///                                       teste la collision.
	/// @param[in]  positionObjet           : Position de l'objet peut-être en
	///                                       collision.
	/// @param[in]  rayonObjet              : Rayon de l'objet peut-être en
	///                                       collision.
	/// @param[in]  constanteRebondissement : Constante de rebondissement de
	///                                       l'objet.
	/// @param[in]  constanteAmortissement  : Constante d'amortissement de
	///                                       l'objet.
	/// @param[in]  vitesse                 : Vitesse de l'objet en collision.
	/// @param[out] retourDetails           : Pointeur pour retourner les
	///                                       détails de la collision.
	///
	/// @return Force causée par la collision, en deux dimensions.
	///
	////////////////////////////////////////////////////////////////////////
	Vecteur2 calculerCollisionCercle(
		const Vecteur2&   centreCercle,
		float            rayonCercle,
		const Vecteur2&   positionObjet,
		float            rayonObjet,
		float            constanteRebondissement,
		float            constanteAmortissement,
		const Vecteur2&   vitesse,
		DetailsCollision* retourDetails //= 0
		)
	{
		return calculerCollisionSphere(
			centreCercle.convertir<3>(), rayonCercle,
			positionObjet.convertir<3>(), rayonObjet,
			constanteRebondissement,
			constanteAmortissement,
			vitesse.convertir<3>(),
			retourDetails
			).convertir<2>();
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn Vecteur3 calculerCollisionSphere(
	///        const Vecteur3&   centreSphere,
	///        float            rayonSphere,
	///        const Vecteur3&   positionObjet,
	///        float            rayonObjet,
	///        float            constanteRebondissement,
	///        float            constanteAmortissement,
	///        const Vecteur3&   vitesse,
	///        DetailsCollision* retourDetails = 0
	///        );
	///
	/// Cette fonction calcule la force causée par la collision d'une
	/// sphère avec une autre sphère, chacune identifiée par son centre
	/// et son rayon.  Elle tient compte d'une force de rebondissement
	/// ainsi qu'une force d'amortissement.
	///
	/// @param[in]  centreSphere            : Centre de la sphère avec laquelle
	///                                       on teste la collision.
	/// @param[in]  rayonSphere             : Rayon de la sphère avec laquelle
	///                                       on teste la collision.
	/// @param[in]  positionObjet           : Position de l'objet peut-être en
	///                                       collision.
	/// @param[in]  rayonObjet              : Rayon de l'objet peut-être en
	///                                       collision.
	/// @param[in]  constanteRebondissement : Constante de rebondissement de
	///                                       l'objet.
	/// @param[in]  constanteAmortissement  : Constante d'amortissement de
	///                                       l'objet.
	/// @param[in]  vitesse                 : Vitesse de l'objet en collision.
	/// @param[out] retourDetails           : Pointeur pour retourner les
	///                                       détails de la collision.
	///
	/// @return Force causée par la collision, en trois dimensions.
	///
	////////////////////////////////////////////////////////////////////////
	Vecteur3 calculerCollisionSphere(
		const Vecteur3&   centreSphere,
		float            rayonSphere,
		const Vecteur3&   positionObjet,
		float            rayonObjet,
		float            constanteRebondissement,
		float            constanteAmortissement,
		const Vecteur3&   vitesse,
		DetailsCollision* retourDetails //= 0
		)
	{
		DetailsCollision details = calculerCollisionSphere(
			centreSphere, rayonSphere,
			positionObjet, rayonObjet
			);

		Vecteur3 force = calculerForceRebondissement3D(
			details, constanteRebondissement
			);
		force += calculerForceAmortissement3D(
			details, vitesse, constanteAmortissement
			);

		if (retourDetails)
			*retourDetails = details;

		return force;
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn float calculerCombinaisonRebondissement(
	///        float constante1, float constante2
	///        );
	///
	/// Cette fonction calcule la constante résultant de la combinaison de deux
	/// corps en collision possédant chacun leur constante de rebondissement.
	///
	/// Elle utilise le modèle de ressort en série qui se compriment l'un
	/// l'autre.
	///
	/// @param[in] constante1 : La constante du premier objet.
	/// @param[in] constante2 : La constante du second objet.
	///
	/// @return Constante résultante pour les deux objets.
	///
	////////////////////////////////////////////////////////////////////////
	float calculerCombinaisonRebondissement(
		float constante1, float constante2
		)
	{
		return constante1 * constante2 / (constante1 + constante2);
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn float calculerCombinaisonAmortissement(
	///        float constante1, float constante2
	///        );
	///
	/// Cette fonction calcule la constante résultant de la combinaison de deux
	/// corps en collision possédant chacun leur constante d'amortissement.
	///
	/// @param[in] constante1 : La constante du premier objet.
	/// @param[in] constante2 : La constante du second objet.
	///
	/// @return Constante résultante pour les deux objets.
	///
	////////////////////////////////////////////////////////////////////////
	float calculerCombinaisonAmortissement(
		float constante1, float constante2
		)
	{
		return constante1 + constante2;
	}



	////////////////////////////////////////////////////////////////////////
	///
	/// @fn bool calculerIntersection2Droites( const Vecteur2& d1P1, const Vecteur2& d1P2, const Vecteur2& d2P1, const Vecteur2& d2P2, const Vecteur2& intersection )
	///
	/// determine le point d'intersection entre 2 droites s'il existe. Les points decrivant la droites doivent etre different
	///
	/// @param[in] const Vecteur2 & d1P1 : premier point de la droite1
	/// @param[in] const Vecteur2 & d1P2 : deuxieme point de la droite1
	/// @param[in] const Vecteur2 & d2P1 : premier point de la droite2
	/// @param[in] const Vecteur2 & d2P2 : deuxieme point de la droite2
	/// @param[out]  Vecteur2 & intersection : resultat du calcul
	///
	/// @return bool : true si le point d'intersection existe
	///
	////////////////////////////////////////////////////////////////////////
	bool calculerIntersection2Droites( const Vecteur2& d1P1, const Vecteur2& d1P2, const Vecteur2& d2P1, const Vecteur2& d2P2, Vecteur2& intersection )
	{
		// Un des murets est nul, il ne peut avoir de collision ou plutot c'est tellement peu probable qu'on ignore ce cas
		if(d1P1 == d1P2 || d2P1 == d2P2)
			return false;

		float deltaX1 = (d1P2[VX]-d1P1[VX]);
		float deltaX2 = (d2P2[VX]-d2P1[VX]);
		bool droite1Vertical = utilitaire::EGAL_ZERO(deltaX1), droite2Vertical = utilitaire::EGAL_ZERO(deltaX2);

		float a1,a2,b1,b2;
		// les 2 droites sont verticales
		if(droite1Vertical && droite2Vertical)
		{
			// infinité de possibilité
			return false;
		}
		else if(droite1Vertical)
		{
			// une droite verticale, calcul l'intersection a partir de cette droite
			intersection[VX] = d1P1[VX];
			a2 = (d2P2[VY]-d2P1[VY])/deltaX2;
			b2 = d2P1[VY]-a2*d2P1[VX];
			intersection[VY] = a2*intersection[VX]+b2;
		}
		else if(droite2Vertical)
		{
			// une droite verticale, calcul l'intersection a partir de cette droite
			intersection[VX] = d2P1[VX];
			a1 = (d1P2[VY]-d1P1[VY])/deltaX1;
			b1 = d1P1[VY]-a1*d1P1[VX];
			intersection[VY] = a1*intersection[VX]+b1;
		}
		else
		{
			// Les 2 droites ne sont pas verticales
			a1 = (d1P2[VY]-d1P1[VY])/deltaX1;
			a2 = (d2P2[VY]-d2P1[VY])/deltaX2;
			b1 = d1P1[VY]-a1*d1P1[VX];
			b2 = d2P1[VY]-a2*d2P1[VX];

			// Droite parallele
			if(utilitaire::EGAL_ZERO(a1-a2))
			{
				// infinité de possibilité
				return false;
			}
			intersection[VX] = (b2-b1)/(a1-a2);
			intersection[VY] = a1*intersection[VX]+b1;
		}

		return true;
	}



	////////////////////////////////////////////////////////////////////////
	///
	/// @fn bool calculerCollisionSegmentSegment( const Vecteur2& d1P1, const Vecteur2& d1P2, const Vecteur2& d2P1, const Vecteur2& d2P2, const Vecteur2& intersection )
	///
	/// 
	///
	/// @param[in] const Vecteur2 & d1P1
	/// @param[in] const Vecteur2 & d1P2
	/// @param[in] const Vecteur2 & d2P1
	/// @param[in] const Vecteur2 & d2P2
	/// @param[out]  Vecteur2 & intersection
	///
	/// @return bool
	///
	////////////////////////////////////////////////////////////////////////
	aidecollision::DetailsCollision calculerCollisionSegmentSegment( const Vecteur2& d1P1, const Vecteur2& d1P2, const Vecteur2& d2P1, const Vecteur2& d2P2, Vecteur2& intersection )
	{
		
		DetailsCollision details;
		details.type = COLLISION_AUCUNE;
		
		// Un des murets est nul, il ne peut avoir de collision ou plutot c'est tellement peu probable qu'on ignore ce cas
		if(d1P1 == d1P2 || d2P1 == d2P2)
			return details;

		details.direction = ((d1P1+d1P2)/2.0-(d2P1+d2P2)/2.0).convertir<3>();
		details.enfoncement = details.direction.norme();
		details.direction /= details.enfoncement;

		float deltaX1 = (d1P2[VX]-d1P1[VX]);
		float deltaX2 = (d2P2[VX]-d2P1[VX]);
		bool droite1Vertical = utilitaire::EGAL_ZERO(deltaX1), droite2Vertical = utilitaire::EGAL_ZERO(deltaX2);

		float a1,a2,b1,b2;
		// les 2 droites sont verticales
		if(droite1Vertical && droite2Vertical)
		{
			// Verifie si les 2 droites sont une par dessus lautre
			if(!utilitaire::EGAL_ZERO(d1P2[VX]-d2P2[VX]))
			{
				return details;
			}

			// Droites verticales superpose
			float distanceCarre = (d1P1-d1P2).norme2();
			float distanceCarre1 = (d2P1-d1P1).norme2();
			float distanceCarre2 = (d2P1-d1P2).norme2();
			float distanceCarre3 = (d2P2-d1P1).norme2();
			float distanceCarre4 = (d2P2-d1P2).norme2();
			// Regarde si l'une des extremite d'un segment fait parti de l'autre segment
			if((distanceCarre1 <= distanceCarre && distanceCarre2 <= distanceCarre) || (distanceCarre3 <= distanceCarre && distanceCarre4 <= distanceCarre))
			{
				details.type = COLLISION_SEGMENT;
			}
			return details;
		}
		else if(droite1Vertical)
		{
			// une droite verticale, calcul l'intersection a partir de cette droite
			intersection[VX] = d1P1[VX];
			a2 = (d2P2[VY]-d2P1[VY])/deltaX2;
			b2 = d2P1[VY]-a2*d2P1[VX];
			intersection[VY] = a2*intersection[VX]+b2;
		}
		else if(droite2Vertical)
		{
			// une droite verticale, calcul l'intersection a partir de cette droite
			intersection[VX] = d2P1[VX];
			a1 = (d1P2[VY]-d1P1[VY])/deltaX1;
			b1 = d1P1[VY]-a1*d1P1[VX];
			intersection[VY] = a1*intersection[VX]+b1;
		}
		else
		{
			// Les 2 droites ne sont pas verticales
			a1 = (d1P2[VY]-d1P1[VY])/deltaX1;
			a2 = (d2P2[VY]-d2P1[VY])/deltaX2;
			b1 = d1P1[VY]-a1*d1P1[VX];
			b2 = d2P1[VY]-a2*d2P1[VX];

			// Droite parallele
			if(utilitaire::EGAL_ZERO(a1-a2))
			{
				// Droite superpose
				if(utilitaire::EGAL_ZERO(b1-b2))
				{
					float distanceCarre = (d1P1-d1P2).norme2();
					float distanceCarre1 = (d2P1-d1P1).norme2();
					float distanceCarre2 = (d2P1-d1P2).norme2();
					float distanceCarre3 = (d2P2-d1P1).norme2();
					float distanceCarre4 = (d2P2-d1P2).norme2();

					// Regarde si l'une des extremite d'un segment fait parti de l'autre segment
					if((distanceCarre1 <= distanceCarre && distanceCarre2 <= distanceCarre) || (distanceCarre3 <= distanceCarre && distanceCarre4 <= distanceCarre))
					{
						details.type = COLLISION_SEGMENT;
					}
					return details;
				}
				else
				{
					// Les 2 droites n'ont pas la meme ordonnees a l'origine alors ils ne peuvent pas avoir d'intersection
					return details;
				}
			}
			intersection[VX] = (b2-b1)/(a1-a2);
			intersection[VY] = a1*intersection[VX]+b1;
		}

		float distanceCarreP1 = (d1P1-d1P2).norme2();
		float distanceCarreP2 = (d2P1-d2P2).norme2();
		float distanceCarre1 = (intersection-d1P1).norme2();
		float distanceCarre2 = (intersection-d1P2).norme2();
		float distanceCarre3 = (intersection-d2P1).norme2();
		float distanceCarre4 = (intersection-d2P2).norme2();
		// Regarde si le point d'intersection appartient au 2 segments
		if((distanceCarre1 <= distanceCarreP1 && distanceCarre2 <= distanceCarreP1) && (distanceCarre3 <= distanceCarreP2 && distanceCarre4 <= distanceCarreP2))
		{
			details.type = COLLISION_SEGMENT;
		}
		return details;
	}

	DetailsCollision calculerCollisionCercleSegmentGrandeVitesse(
		const Vecteur2& anciennePos, 
		const Vecteur2& nouvellePos, 
		const float& rayon,
		const Vecteur2& coinMuret1,
		const Vecteur2& coinMuret2,
		Vecteur2& intersection
		)
	{
		return DetailsCollision();
		Vecteur2 directeurObjet(nouvellePos-anciennePos);
		directeurObjet.normaliser();
		// Vecteur pointant vers la gauche du vecteur directeur
		Vecteur2 normale(-directeurObjet[VY], directeurObjet[VX]);
		normale*=rayon;
		Vecteur2 coinHautGauche(nouvellePos+normale),
				 coinHautDroite(nouvellePos+normale),
				 coinBasGauche(anciennePos+normale),
				 coinBasDroite(anciennePos-normale),
				 coinHautMilieu(nouvellePos+directeurObjet*rayon);
		Vecteur2 intersectionTemp;
		float distance = 9999999;
		bool intersectionGauche = false, intersectionDroite = false, intersectionMilieu = false, intersectionFront = false;
		if(calculerCollisionSegmentSegment(coinBasGauche,coinHautGauche,coinMuret1,coinMuret2,intersectionTemp).type != COLLISION_AUCUNE)
		{
			intersectionGauche = true;
			intersection = intersectionTemp;
			distance = produitScalaire(intersection-anciennePos,directeurObjet);
		}
		if(calculerCollisionSegmentSegment(coinBasDroite,coinHautDroite,coinMuret1,coinMuret2,intersectionTemp).type != COLLISION_AUCUNE)
		{
			intersectionDroite = true;
			float distTemp = produitScalaire(intersection-anciennePos,directeurObjet);
			if(distTemp < distance)
			{
				intersection = intersectionTemp;
				distance = distTemp;
			}
		}

		if(calculerCollisionSegmentSegment(anciennePos,coinHautMilieu,coinMuret1,coinMuret2,intersectionTemp).type != COLLISION_AUCUNE)
		{
			intersectionMilieu = true;
			float distTemp = produitScalaire(intersection-anciennePos,directeurObjet)-rayon;
			if(distTemp < distance)
			{
				intersection = intersectionTemp;
				distance = distTemp;
			}
		}

		if(!intersectionDroite && !intersectionGauche)
		{
			float a;
			float b;
			float ac4; 
			float deltaX = (coinMuret2[VX]-coinMuret1[VX]);

			if(utilitaire::EGAL_ZERO(deltaX))
			{
				a = 1;
				b = nouvellePos[VY]*2;
				float lambda = coinMuret1[VX]-nouvellePos[VX];
				ac4 = ((nouvellePos[VY]*nouvellePos[VY]+lambda*lambda-rayon*rayon)*4);
				intersectionTemp[VX] = coinMuret1[VX];
				/*if(utilitaire::EGAL_ZERO(b*b-ac4))
				{
					float distTemp1 = produitScalaire(Vecteur2(coinMuret1[VX],b/2.0)-anciennePos,directeurObjet);
					if(distTemp1 < distanceDeplacement)
						enfoncement = true;					
					intersectionFront = true;
				}
				else*/ if(b*b >= ac4)
				{
					/*float y1 = (b+sqrt(b*b-ac4))/2.0;
					float distTemp1 = produitScalaire(Vecteur2(intersectionTemp[VX],y1)-anciennePos,directeurObjet);
					float y2 = (b-sqrt(b*b-ac4))/2.0;
					float distTemp2 = produitScalaire(Vecteur2(intersectionTemp[VX],y2)-anciennePos,directeurObjet);
					if(distTemp1 < distanceDeplacement || distTemp2 < distanceDeplacement)
						enfoncement = true;*/
					intersectionFront = true;
				}
			}
			else
			{
				float aDroite = (coinMuret2[VX]-coinMuret1[VX])/deltaX;
				if(utilitaire::EGAL_ZERO(aDroite))
					aDroite = 0;
				float bDroite = coinMuret1[VY]-aDroite*coinMuret1[VX];
				float lambda = bDroite-nouvellePos[VY];
				a = aDroite*aDroite+1;
				b = 2*(nouvellePos[VX]+lambda);
				float ac4 = 4*a*((nouvellePos[VX]*nouvellePos[VX]+lambda*lambda-rayon*rayon));

				/*if(utilitaire::EGAL_ZERO(b*b-ac4))
				{
					float x1 = b/(2.0*a);
					float distTemp1 = produitScalaire(Vecteur2(x1,aDroite*x1+bDroite)-anciennePos,directeurObjet);
					if(distTemp1 < distanceDeplacement)
						enfoncement = true;	
					intersectionFront = true;
				}
				else*/ if(b*b >= ac4)
				{
					/*float x1 = (b+sqrt(b*b-ac4))/(2.0*a);
					float y1 = aDroite*x1+bDroite;
					float distTemp1 = produitScalaire(Vecteur2(x1,y1)-anciennePos,directeurObjet);
					float x2 = (b-sqrt(b*b-ac4))/(2.0*a);
					float y2 = aDroite*x2+bDroite;
					float distTemp2 = produitScalaire(Vecteur2(x2,y2)-anciennePos,directeurObjet);
					if(distTemp1 < distanceDeplacement || distTemp2 < distanceDeplacement)
						enfoncement = true;*/
					intersectionFront = true;
				}
			}
		}
		


		DetailsCollision retour;
		
		if(!intersectionDroite && !intersectionFront && !intersectionGauche && !intersectionMilieu)
		{
			retour.type = COLLISION_AUCUNE;
			return retour;
		}
		if(!intersectionDroite && !intersectionGauche)
		{
			float distcoin1 = produitScalaire(coinMuret1-nouvellePos,directeurObjet);
			float distcoin2 = produitScalaire(coinMuret2-nouvellePos,directeurObjet);
			// Si les 2 superieur a 1 alors il ne sagit pas d<enfoncement
			if(distcoin1 < 0 || distcoin2 < 0)
			{
				// c'est le coin1 qui est le plus eloigne du la nouvelle position en direction de l'ancienne
				if(distcoin1 < distcoin2)
				{
					//retour.direction = 0;
				}
			}
		}
		

		//rayon*1.415 pour incertitude un peu moins pour que ca paraisse pas trop


	}



	/// Calculate the time of closest approach of two moving circles.  Also determine if the circles collide.
	/// 
	/// Input:
	/// Pa - Position of circle A.
	/// Pb - Position of circle B.
	/// Va - Velocity of circle A.
	/// Vb - Velocity of circle B.
	/// Ra - Radius of circle A.
	/// Rb - Radius of circle B.
	/// 
	/// Returns:
	/// collision - Returns True if a collision occured, else False.
	/// The method returns the time to impact if collision=true, else it returns the time of closest approach.
	/// 
	/// Notes:
	/// This algorithm will work in any dimension.  Simply change the Vector2's to Vector3's to make this work
	/// for spheres.  You can also set the radii to 0 to work with points/rays.
	/// 
	float TimeOfClosestApproach( const Vecteur2& Pa, const Vecteur2& Pb, const Vecteur2& Va, const Vecteur2& Vb, const float& Ra, const float& Rb, bool& collision )
	{
		Vecteur2 Pab = Pa - Pb;
		Vecteur2 Vab = Va - Vb;
		float a = produitScalaire(Vab, Vab);
		float b = 2 * produitScalaire(Pab, Vab);
		float c = produitScalaire(Pab, Pab) - (Ra + Rb) * (Ra + Rb);

		// The quadratic discriminant.
		float discriminant = b * b - 4 * a * c;

		// Case 1:
		// If the discriminant is negative, then there are no real roots, so there is no collision.  The time of
		// closest approach is then given by the average of the imaginary roots, which is:  t = -b / 2a
		float t;
		if (discriminant < 0)
		{
			t = -b / (2 * a);
			collision = false;
		}
		else
		{
			// Case 2 and 3:
			// If the discriminant is zero, then there is exactly one real root, meaning that the circles just grazed each other.  If the 
			// discriminant is positive, then there are two real roots, meaning that the circles penetrate each other.  In that case, the
			// smallest of the two roots is the initial time of impact.  We handle these two cases identically.
			float t0 = (-b + sqrt(discriminant)) / (2 * a);
			float t1 = (-b - sqrt(discriminant)) / (2 * a);
			t = (t0 < t1) ? t0:t1;
			
			// We also have to check if the time to impact is negative.  If it is negative, then that means that the collision
			// occured in the past.  Since we're only concerned about future events, we say that no collision occurs if t < 0.
			if (t < 0)
				collision = false;
			else
				collision = true;
		}

		// Finally, if the time is negative, then set it to zero, because, again, we want this function to respond only to future events.
		if (t < 0)
			t = 0;

		return t;
	}



} // Fin de l'espace de nom aidecollision.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
