//////////////////////////////////////////////////////////////////////////////
/// @file SoundFMOD.h
/// @author Michael Ferris, Vincent Lemire
/// @date 2012-04-02
/// @version 2.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>
#include <string>
#include <map>
#include <vector>
#include "Singleton.h"

/// Typedef
typedef int Song;
typedef std::map<std::string, FMOD::Sound*> ListeMusique;
typedef std::vector<std::string> NomsPlaylists;

/// Enum des sons de l'application
enum ApplicationSong
{
	STARTUP_SONG,
	EDITION_MODE_SONG,
	MENU_MODE_SONG,
	NB_APP_SONGS
};

/// Enum des effets
enum effect
{
	GOAL_EFFECT,
	COLLISION_MURET_EFFECT, 
	COLLISION_MAILLET_EFFECT1,
	COLLISION_MAILLET_EFFECT2,
	COLLISION_MAILLET_EFFECT3,
	COLLISION_MAILLET_EFFECT4,
	COLLISION_MAILLET_EFFECT5,
	CLICK_EFFECT, 
	BEEP_EFFECT,
	PORTAL_EFFECT,
	PAUSE_EFFECT,
	ACCELERATOR_EFFECT,
};

/// Enum des voix (pas utile dans ce projet)
enum voices
{
	/*
	WELCOME_VOICE,
	GAME_OVER_VOICE,
	NO_MORE_MOVES_VOICE,
	*/
};

///////////////////////////////////////////////////////////////////////////
/// @class SoundFMOD
/// @brief Classe qui gère les sons grâce à la librairie FMOD.
///
/// @author Michael Ferris, Vincent Lemire
/// @date 2012-04-02
///////////////////////////////////////////////////////////////////////////
class SoundFMOD:public Singleton<SoundFMOD>
{
	SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(SoundFMOD);
	/// Constructeur privé
	SoundFMOD(void);
	/// Destructeur
	~SoundFMOD(void);
public:
	/// Initialisation de FMOD
	void init();
	/// Désallocation de FMOD
	void clear();
	
	/// Création de sons
	void create_sound(std::string name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, std::string nomPlaylist);

	/// Ajouter un canal
	void creerPlaylist(std::string nom);
	/// Suppression d'un canal
	void supprimerPlaylist(std::string nom);
	/// Ajouter une chanson dans un canal
	void ajouterChanson(std::string nomPlaylist, FMOD::Sound* sound);
	/// Suppression d'une chanson dans un canal, la chanson n'est pas libere de FMOD
	void supprimerChanson(std::string nomPlaylist, std::string nomChanson);
	/// Obtenir la liste des noms des canaux existants
	NomsPlaylists obtenirCanaux();
	/// Obtenir le nom du canal courant
	std::string obtenirNomCanalCourant();
	/// Obtenir le nom d'une chanson FMOD
	std::string obtenirNomChanson(FMOD::Sound* sound);

	/// Changement de canal
	void next_playlist();
	void previous_playlist();
	void modifierPlaylistActuelle(std::string nomCanal);

	/// Changement de chanson
	void next_Song();
	void previous_Song();
	void repeatAppSong();

	/// Méthodes pour jouer des sons
	void playApplicationSong(ApplicationSong choice);
	void playSongInPlaylist(Song choice);
	void playEffect(effect choice);
	void playVoice( voices choice );

	/// Accesseurs et modificateurs des volumes
	void setEffectVolume(float new_volume);
	void setPlaylistVolume(float new_volume);
	void setAppVolume(float new_volume);
	float getSongVolume();
	float getAppSongVolume();
	float getEffectVolume();

	/// Méthodes pour gérer la musiques qui joue
	Song get_Song_Playing();
	void togglePlaying();
	void change_song_if_end();
	void restartSong();
	void stop();
	bool estEnPause();
	bool songIsPlaying();

private:
	/// Vérifications des erreurs
	void ERRCHECK(FMOD_RESULT result);

	/// Méthode de création de sons
	void create_sound(std::string name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, FMOD::SoundGroup* group);
	/// Variables de configurations de FMOD
	FMOD::System *systemFmod;
	FMOD::Channel *song_channel,*effect_channel,*voices_channel;
	FMOD::SoundGroup *songs_group, *effects_group, *voices_group;
	FMOD_RESULT result;

	/// Attributs de volume
	float effect_Volume;
	float song_Volume;

	/// Musique actuelle (0 = aucune, -1 = random)
	Song song_playing_;

	/// La liste des sons enregistrés dans FMOD
	ListeMusique musiques_;
	/// Les canaux (noms + listes de leurs chansons)
	typedef std::map<std::string, FMOD::SoundGroup *> Playlist;
	Playlist playlists_;
	/// Le canal actuel (nom + liste de ses chansons)
	FMOD::SoundGroup * playlistActuelle_;

	/// Indique si initialisé
	bool initialised_;
	bool stopped_;
	/// Compteur de musiques
	Song nombreDeMusiques_;
	
	/// La piste courante
	int pisteCourante_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




