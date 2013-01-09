package ca.polymtl.inf2990.BarresOutils;

import java.awt.Cursor;

/**
 * Classe contenant toutes les constantes utiles à l'exécution des
 * commandes/l'ajout de boutons et autre.
 * 
 * @author Charles Étienne Lalonde
 * 
 */
public class ConstantesOutils
{
	// Images associées à chacun des éléments de l'énum. (En commentaire à
	// côté)
	public static final String	IMAGES[]		= { "pan_main.png", // CAMERA
												"zoom_elastique.png", // ZOOM_ELASTIQUE
												"zoom_proportionnel.png", // ZOOM_PROPORTIONNEL
												"tourner.png", // EDITEUR_ROTATION
												"redimensionner.png", // EDITEUR_ECHELLE
												"selectionner.png", // EDITEUR_SELECTION
												"deplacer.png", // EDITEUR_DÉPLACER
												"ajouter_muret.png", // INSÉRER_MURET
												"ajouter_portail.png", // INSÉRER_PORTAIL
												"ajouter_accelerateur.png", // INSERER_ACCELERATEUR
												"ajouter_objet.png", // INSERER_BUT
												"", // EDITEUR_NOUVEAU
												"", // EDITEUR_SAUVEGARDER
												"", // EDITEUR_CHARGER
												"", // QUITTER
												"", // EDITEUR_SAUVEGARDER_SOUS
												"supprimer.png", // SUPPRIMER
												"ajouter_maillet.png", // INSERER_MAILLET
												"ajouter_rondelle.png", // INSERER_RONDELLE
												"dupliquer.png", // DUPLIQUER
												"", // ALLER_MODE_EDITION
												"", // ALLER_MODE_JEU
												"", // MODIFIER_TOUCHE
												"", // REINITIALISER_PARTIE
												"", // PAUSE_JEU
												"", // REPLAY
												"cameraFixe.png", // CAMERA_FIXE
												"cameraOrbite.png", //CAMERA_ORBITE
												"cameraLibre.png", //CAMERA_LIBRE
												"orbite.png", //ORBITE
												"cameraSplit.png" //CAMERA_SPLIT
												};

	// Noms des menus associés à chaque commande; chaque chaîne est reliée à un
	// élément de l'énum (En commentaire à côté)
	public static final String	NOMS_MENUS[]	= { "Déplacer la caméra", // CAMERA
												"Zoom élastique", // ZOOM_ELASTIQUE
												"Zoom proportionnel", // ZOOM_PROPORTIONNEL
												"Tourner", // EDITEUR_ROTATION
												"Agrandir/Rétrécir", // EDITEUR_ECHELLE
												"Selectionner", // EDITEUR_SELECTION
												"Déplacer un objet", // EDITEUR_DÉPLACER
												"Muret", // INSÉRER_MURET
												"Portail", // INSÉRER_PORTAIL
												"Accélérateur", // INSERER_ACCELERATEUR
												"But", // INSERER_BUT
												"Nouveau", // EDITEUR_NOUVEAU
												"Enregistrer", // EDITEUR_SAUVEGARDER
												"Charger", // EDITEUR_CHARGER
												"Quitter", // QUITTER
												"Enregistrer sous", // EDITEUR_SAUVEGARDER_SOUS
												"Supprimer", // SUPPRIMER
												"Maillet", // INSERER_MAILLET
												"Rondelle", // INSERER_RONDELLE
												"Copier la sélection", // DUPLIQUER
												"Édition", // ALLER_MODE_EDITION
												"Jeu", // ALLER_MODE_JEU
												"Modifier les commandes du jeu", // MODIFIER_TOUCHE
												"Reinitialiser Partie", // REINITIALISER_PARTIE
												"Pause", // PAUSE_JEU
												"Replay", //REPLAY
												"Caméra fixe", // CAMERA_FIXE
												"Caméra orbite", //CAMERA_ORBITE
												"Caméra libre", //CAMERA_LIBRE
												"Outil Orbit", //ORBIT
												"Caméra divisée" //CAMERA_SPLIT
												};

	// Raccourcis associés à chacun des éléments de l'énum. (En commentaire à
	// côté)
	public static final String	RACCOURCIS[][]	= { { "I", "shift I" }, // CAMERA
												{ "R", "shift R" }, // ZOOM_ELASTIQUE
												{ "Z", "shift Z" }, // ZOOM_PROPORTIONNEL
												{ "T", "shift T" }, // EDITEUR_ROTATION
												{ "E", "shift E" }, // EDITEUR_ECHELLE
												{ "F", "shift F" }, // EDITEUR_SELECTION
												{ "D", "shift D" }, // EDITEUR_DEPLACER
												{ "ctrl 1" }, // INSERER_MURET
												{ "ctrl 2" }, // INSERER_PORTAIL
												{ "ctrl 3" }, // INSERER_BOOSTER
												{}, // INSERER_BUT
												{ "ctrl N" }, // EDITEUR_NOUVEAU
												{ "ctrl S" }, // EDITEUR_SAUVEGARDER
												{ "ctrl O" }, // EDITEUR_CHARGER
												{ "ESCAPE", "ctrl Q", "Q", "shift Q" }, // QUITTER
												{}, // EDITEUR_SAUVEGARDER_SOUS
												{ "released DELETE", "released BACKSPACE" }, // SUPPRIMER
												{ "ctrl 4" }, // INSERER_MAILLET
												{ "ctrl 5" }, // INSERER_RONDELLE
												{ "released V", "shift released V", "ctrl released V", "alt released V" }, // DUPLIQUER
												{ "ctrl E" }, // ALLER_MODE_EDITION
												{ "ctrl J" }, // ALLER_MODE_JEU
												{}, // MODIFIER_TOUCHE
												{ "ctrl N" }, // REINITIALISER_PARTIE
												{ "pressed SPACE" }, // PAUSE_JEU
												{ "R" }, // REPLAY
												{ "7" }, // CAMERA_FIXE
												{ "8" }, // CAMERA_ORBITE
												{ "9" }, // CAMERA_LIBRE
												{ "O", "shift O" }, // ORBIT
												{ "6" }, // CAMERA_SPLIT
												};

	// Tooltip des éléments associés à chaque commande; chaque chaîne est reliée
	// à un élément de l'énum (En commentaire à côté)
	public static final String	TOOLTIPS[]		= { "Déplace la caméra", // CAMERA
												"Zoom élastique", // ZOOM_ELASTIQUE
												"Zoom proportionnel", // ZOOM_PROPORTIONNEL
												"Faire tourner un objet", // EDITEUR_ROTATION
												"Agrandir ou Rétrécir un objet sélectionné", // EDITEUR_ECHELLE
												"Selectionne un ou des objets", // EDITEUR_SELECTION
												"Déplacer un objet", // EDITEUR_DÉPLACER
												"Insérer un muret", // INSÉRER_MURET
												"Insérer un portail", // INSÉRER_PORTAIL
												"Insérer un accélérateur", // INSERER_ACCELERATEUR
												"Insérer des buts", // INSERER_BUT
												"Créer une nouvelle table à éditer", // EDITEUR_NOUVEAU
												"Sauvegarder la table actuelle", // EDITEUR_SAUVEGARDER
												"Charger une table", // EDITEUR_CHARGER
												"Quitter le programme", // QUITTER
												"Sauvegarder la table actuelle dans un nouveau fichier", // EDITEUR_SAUVEGARDER_SOUS
												"Supprimer le ou les objets sélectionnés", // SUPPRIMER
												"Insérer un maillet", // INSERER_MAILLET
												"Insérer une rondelle", // INSERER_RONDELLE
												"Dupliquer la sélection", // DUPLIQUER
												"Aller au mode édition", // ALLER_MODE_EDITION
												"Aller au mode jeu", // ALLER_MODE_JEU
												"Modifier les commandes du joueur 2 (qui devra utiliser le clavier)", // MODIFIER_TOUCHE,
												"Reinitialisation de la partie a zero", // REINITIALISER_PARTIE
												"Permet de pauser et resumer la partie", // PAUSE_JEU
												"Regarder une reprise instantanée des 10 dernières secondes", //REPLAY
												"Utilise une caméra fixe", // CAMERA_FIXE
												"Utilise une caméra qui tourne autour d'un point", //CAMERA_ORBITE
												"Utilise une caméra libre", //CAMERA_LIBRE
												"Effectue une rotation de la caméra", //ORBIT
												"Utilise une caméra qui divise l'écran en deux parties" //CAMERA_SPLIT
												};

	// Curseurs des éléments associés à chaque commande; chaque curseur est
	// relié à un élément de l'énum (En commentaire à côté)
	public static final Cursor	CURSEURS[]		= { null, // CAMERA
												null, // ZOOM_ELASTIQUE
												null, // ZOOM_PROPORTIONNEL
												Cursor.getPredefinedCursor(Cursor.N_RESIZE_CURSOR), // EDITEUR_ROTATION
												Cursor.getPredefinedCursor(Cursor.N_RESIZE_CURSOR), // EDITEUR_ECHELLE
												Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR), // EDITEUR_SELECTION
												Cursor.getPredefinedCursor(Cursor.MOVE_CURSOR), // EDITEUR_DÉPLACER
												Cursor.getPredefinedCursor(Cursor.HAND_CURSOR), // INSÉRER_MURET
												Cursor.getPredefinedCursor(Cursor.HAND_CURSOR), // INSÉRER_PORTAIL
												Cursor.getPredefinedCursor(Cursor.HAND_CURSOR), // INSERER_ACCELERATEUR
												Cursor.getPredefinedCursor(Cursor.HAND_CURSOR), // INSERER_BUT
												null, // EDITEUR_NOUVEAU
												null, // EDITEUR_SAUVEGARDER
												null, // EDITEUR_CHARGER
												null, // QUITTER
												null, // EDITEUR_SAUVEGARDER_SOUS
												null, // SUPPRIMER
												Cursor.getPredefinedCursor(Cursor.HAND_CURSOR), // INSERER_MAILLET
												Cursor.getPredefinedCursor(Cursor.HAND_CURSOR), // INSERER_RONDELLE
												null, // DUPLIQUER
												null, // ALLER_MODE_EDITION
												null, // ALLER_MODE_JEU
												null, // MODIFIER_TOUCHE,
												null, // REINITIALISER_PARTIE
												null, // PAUSE_JEU
												null, // REPLAY
												Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR), // CAMERA_FIXE
												Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR), // CAMERA_ORBITE
												Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR), // CAMERA_LIBRE
												null, // ORBIT
												Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR), // CAMERA_LIBRE
												};
}
