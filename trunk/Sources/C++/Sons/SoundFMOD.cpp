//////////////////////////////////////////////////////////////////////////////
/// @file SoundFMOD.h
/// @author Michael Ferris
/// @date 2011-06-08
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SoundFMOD.h"
#include "GestionnaireAnimations.h"
#include <iostream>
#include "RazerGameUtilities.h"

const std::string SOUND_PATH = RazerGameUtilities::NOM_DOSSIER_MEDIA + "Sons/";

////////////////////////////////////////////////////////////////////////
///
/// @fn SoundFMOD::SoundFMOD(void):song_channel(0),effect_channel(0),song_Volume(1),effect_Volume(1),initialised_(false),song_playing_(NO_SONG)
///
/// Constructeur.
///
/// @return
///
////////////////////////////////////////////////////////////////////////
SoundFMOD::SoundFMOD(void):
	song_channel(0),
	effect_channel(0),
	song_Volume(1),
	effect_Volume(1),
	initialised_(false),
	nombreDeMusiques_(0), 
	song_playing_(0), 
	pisteCourante_(0),
	playlistActuelle_(0),
	stopped_(true)
{
	init();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn SoundFMOD::~SoundFMOD(void)
///
/// Destructeur libérant la mémoire.
///
/// @return
///
////////////////////////////////////////////////////////////////////////
SoundFMOD::~SoundFMOD(void)
{	
	clear();
}

/// Déclaration du singleton
SINGLETON_DECLARATION_CPP(SoundFMOD);

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::ERRCHECK( FMOD_RESULT result )
///
/// Gère les erreurs de recherche d'un éléments sonore.
///
/// @param[in] FMOD_RESULT result : Le résultat de la recherche.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////

#define ERRCHECK(r, retour,...) \
if (r != FMOD_OK)\
{\
    checkf(0);\
    std::cerr << "FMOD error! " << result <<" " << FMOD_ErrorString(result)<<std::endl;\
    {__VA_ARGS__;}\
    return retour;\
}




////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::init()
///
/// Initialisation de la librairie.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::init()
{
    // Create a System object and initialize.
    result = FMOD::System_Create(&systemFmod);
    ERRCHECK(result,);

	unsigned int version;
    result = systemFmod->getVersion(&version);
    ERRCHECK(result,);

	result = systemFmod->createSoundGroup("songs",&songs_group);
	ERRCHECK(result,);
	result = systemFmod->createSoundGroup("effects",&effects_group);
	ERRCHECK(result,);
	result = systemFmod->createSoundGroup("voices",&voices_group);
	ERRCHECK(result,);

    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return;
    }
    result = systemFmod->init(32, FMOD_INIT_NORMAL, 0);
    ERRCHECK(result,);

	// Sons par défaut de l'application 
	create_sound("hockey.mp3", FMOD_SOFTWARE, 0, songs_group);
	create_sound("SongoftheCagedBird.mp3", FMOD_LOOP_NORMAL, 0, songs_group);
	create_sound("Diamond_Mine.xm", FMOD_LOOP_NORMAL, 0, songs_group);
	
	// Volume des musiques
	songs_group->setVolume((float)0.25);

	// Effets sonores
 	create_sound("goal.wav", FMOD_SOFTWARE, 0,		effects_group);
 	create_sound("collision_muret.wav", FMOD_SOFTWARE, 0,		effects_group);
 	create_sound("collision_maillet1.wav", FMOD_SOFTWARE, 0,effects_group);
 	create_sound("collision_maillet2.wav", FMOD_SOFTWARE, 0,effects_group);
 	create_sound("collision_maillet3.wav", FMOD_SOFTWARE, 0,effects_group);
 	create_sound("collision_maillet4.wav", FMOD_SOFTWARE, 0,effects_group);
 	create_sound("collision_maillet5.wav", FMOD_SOFTWARE, 0,effects_group);
	create_sound("click.wav", FMOD_SOFTWARE, 0,			effects_group);
	create_sound("beep-3.wav", FMOD_SOFTWARE, 0,		effects_group);
	create_sound("portalenter.wav", FMOD_SOFTWARE, 0,		effects_group);
	create_sound("pause.wav", FMOD_SOFTWARE, 0,		effects_group);
	create_sound("accelerator.mp3", FMOD_SOFTWARE, 0,		effects_group);
	effects_group->setMaxAudible(100);
	effects_group->setMaxAudibleBehavior(FMOD_SOUNDGROUP_BEHAVIOR_MUTE);
	
	// L'initialisation est complète
	initialised_ = true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::clear()
///
/// Désallocations lorsqu'on arrête d'utiliser la librairie.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::clear()
{
	if(!initialised_)
		return;
	
	for(ListeMusique::iterator iter = musiques_.begin(); iter != musiques_.end(); ++iter)
	{
		result = iter->second->release();
		ERRCHECK(result,);
	}

	// Vidage des canaux
	Playlist::iterator iter;
	for(iter = playlists_.begin(); iter != playlists_.end(); ++iter)
		iter->second->release();
	playlists_.clear();
	musiques_.clear();

	// Réinitialisation des paramètres
	song_channel = (0);
	effect_channel = (0);
	initialised_ = (false);
	nombreDeMusiques_ = (0);
	song_playing_ = (0); 
	pisteCourante_ = (0);

	int size;
	songs_group->getNumSounds(&size);
	FMOD::Sound* current_sound;
	for(int i = 0; i<size; i++)
	{
		// On prend la chanson a lindex 0, car suite au release, elle est supprime
		songs_group->getSound(0,&current_sound);
		result = current_sound->release();
		ERRCHECK(result,);
	}
	songs_group->release();

	voices_group->getNumSounds(&size);
	for(int i = 0; i<size; i++)
	{
		// On prend la chanson a lindex 0, car suite au release, elle est supprime
		voices_group->getSound(0,&current_sound);
		result = current_sound->release();
		ERRCHECK(result,);
	}
	voices_group->release();

	effects_group->getNumSounds(&size);
	for(int i = 0; i<size; i++)
	{
		// On prend la chanson a lindex 0, car suite au release, elle est supprime
		effects_group->getSound(0,&current_sound);
		result = current_sound->release();
		ERRCHECK(result,);
	}
	effects_group->release();

	result = systemFmod->close();
	ERRCHECK(result,);
	result = systemFmod->release();
	ERRCHECK(result,);
	initialised_ = false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::create_sound(std::string name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo)
///
/// Création d'un sons.
///
/// @param[in] std::string name_or_data : le nom du son.
/// @param[out] FMOD_CREATESOUNDEXINFO *exinfo : les informations de création.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::create_sound(std::string name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, std::string nomPlaylist)
{
	FMOD::Sound* sound;
	ListeMusique::iterator iter;
	if((iter = musiques_.find(name_or_data)) == musiques_.end())
	{
		result = systemFmod->createSound(name_or_data.c_str(), mode, exinfo, &sound);
        ERRCHECK(result,,std::cerr << "Error loading song : " << name_or_data << std::endl;);
		sound->setSoundGroup(songs_group);
		musiques_[name_or_data] =  sound;
	}
	else
	{
		sound = iter->second;
	}
	creerPlaylist(nomPlaylist);
	ajouterChanson(nomPlaylist, sound);

	ERRCHECK(result,);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::ajouterPlaylist()
///
/// Ajoute une nouvelle playlist au conteneur.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::creerPlaylist(std::string nom)
{
    if(!initialised_)
        return;

	if(playlists_.find(nom) == playlists_.end())
	{
		playlists_[nom] = 0;
		result = systemFmod->createSoundGroup(nom.c_str(),&playlists_[nom]);
		ERRCHECK(result,);
	}

	if(playlistActuelle_ == NULL)
	{
		playlistActuelle_ = playlists_[nom];
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::create_sound(std::string name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo)
///
/// Création d'un sons.
///
/// @param[in] int numeroPlaylist : le numéro de la playlist.
/// @param[out] std::string nomChanson : le nom de la chanson.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::ajouterChanson( std::string nomPlaylist, FMOD::Sound* sound )
{
    if(!initialised_)
        return;

	sound->setSoundGroup(playlists_[nomPlaylist]);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::create_sound(std::string name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, FMOD::SoundGroup* group)
///
/// Création d'un son.
///
/// @param[in] std::string name_or_data : le nom du son.
/// @param[in] FMOD_MODE mode : le mode de création.
/// @param[out] FMOD_CREATESOUNDEXINFO *exinfo : les informations de la création.
/// @param[out] FMOD::SoundGroup* group : le groupe de sons.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::create_sound(std::string name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, FMOD::SoundGroup* group)
{
	name_or_data = SOUND_PATH+name_or_data;
	FMOD::Sound* sound;
	//systemFmod->createStream();
	result = systemFmod->createSound(name_or_data.c_str(), mode, exinfo, &sound);
	sound->setSoundGroup(group);

	ERRCHECK(result,);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::playApplicationSong(Song choice)
///
/// Jouer une musique présente par défaut dans l'application (mode édition et menus).
///
/// @param[in] song choice : le son à jouer.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::playApplicationSong(ApplicationSong choice)
{
    if(!initialised_)
        return;

	if((songIsPlaying() && get_Song_Playing() == choice))
		return;

	song_playing_ = (choice)%NB_APP_SONGS;

	FMOD::Sound* cur;
	songs_group->getSound(song_playing_,&cur);
	result = systemFmod->playSound(FMOD_CHANNEL_REUSE, cur, false, &song_channel);
	ERRCHECK(result,);
	systemFmod->update();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::playSong( song choice )
///
/// Jouer une musique présente dans une playlist (mode jeu).
///
/// @param[in] song choice : le son à jouer.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::playSongInPlaylist( Song choice )
{
    if(!initialised_)
        return;

	int nbSong;
	playlistActuelle_->getNumSounds(&nbSong);
	if((songIsPlaying() && get_Song_Playing() == choice) || playlistActuelle_ == NULL || nbSong == 0 )
		return;

	if(choice < 0)
		song_playing_ = nbSong-1;
	else
		song_playing_ = (choice)%nbSong;

	FMOD::Sound* cur;
	playlistActuelle_->getSound(song_playing_,&cur);
	result = systemFmod->playSound(FMOD_CHANNEL_REUSE, cur, false, &song_channel);
	ERRCHECK(result,);
	systemFmod->update();
	stopped_ = false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::next_Song()
///
/// Changement de musique.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::next_Song()
{
    if(!initialised_)
        return;

// 	if(playlistActuelle_== NULL || playlistActuelle_.second->size() == 0)
// 		return;
// 	++pisteCourante_;
// 	if(pisteCourante_ >= playlistActuelle_.second->size())
// 		pisteCourante_ = 0;

	playSongInPlaylist(song_playing_+1);//musiques_[(*(playlistActuelle_.second))[pisteCourante_]]);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::previous_Song()
///
/// Changement de musique.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::previous_Song()
{
    if(!initialised_)
        return;

	playSongInPlaylist(song_playing_-1);
// 	if(playlistActuelle_.second == NULL || playlistActuelle_.second->size() == 0)
// 		return;
// 	--pisteCourante_;
// 	if(pisteCourante_ < 0)
// 		pisteCourante_ = (int)(playlistActuelle_.second->size() - 1);
// 
// 	playSongInPlaylist(musiques_[(*(playlistActuelle_.second))[pisteCourante_]]);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::playVoice( voices choice )
///
/// Jouer une voix.
///
/// @param[in] voices choice : la voix à jouer.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::playVoice( voices choice )
{
    if(!initialised_)
        return;

	FMOD::Sound* current_song;
	voices_group->getSound(choice,&current_song);
	result = systemFmod->playSound(FMOD_CHANNEL_REUSE, current_song, false, &voices_channel);
	ERRCHECK(result,);
	systemFmod->update();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::playEffect( effect choice )
///
/// Joueur un son.
///
/// @param[in] effect choice : le son à jouer.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::playEffect( effect choice )
{
    if(!initialised_)
        return;

	FMOD::Sound* current_song;
	effects_group->getSound(choice,&current_song);
	result = systemFmod->playSound(FMOD_CHANNEL_FREE, current_song, false, &effect_channel);
	GestionnaireAnimations::obtenirInstance()->saveReplaySound(choice);
	ERRCHECK(result,);
	systemFmod->update();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::setEffectVolume(float new_volume)
///
/// Modifier le volume d'un effet sonore.
///
/// @param[in] float new_volume : le nouveau volume.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::setEffectVolume(float new_volume)
{
    if(!initialised_)
        return;

	effect_Volume = new_volume;
	effects_group->setVolume(new_volume);
	voices_group->setVolume(new_volume);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::setSongVolume(float new_volume)
///
/// Modifier le volume d'une musique.
///
/// @param[in] float new_volume : le nouveau volume.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::setPlaylistVolume(float new_volume)
{
    if(!initialised_)
        return;

	if(playlistActuelle_)
		playlistActuelle_->setVolume(new_volume);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::setSongVolume(float new_volume)
///
/// Modifier le volume d'une musique.
///
/// @param[in] float new_volume : le nouveau volume.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::setAppVolume(float new_volume)
{
    if(!initialised_)
        return;

	songs_group->setVolume(new_volume);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::change_song_if_end()
///
/// Permet le changement automatique des musiques.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
//Pas besoin pour le moment, les .XM ont un loop embeded qui les rendent interminables
void SoundFMOD::change_song_if_end()
{
    if(!initialised_)
        return;

	bool playing;
	song_channel->isPlaying(&playing);
	if(!playing && !stopped_)
	{
		next_Song();
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::repeatAppSong(  )
///
/// Sert à assurer la répétition des musiques par défaut de l'application.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::repeatAppSong()
{
    if(!initialised_)
        return;

	if(!songIsPlaying())
	{	
		if(song_playing_ == STARTUP_SONG)
		{
			playApplicationSong(MENU_MODE_SONG);
		}
		else
		{
			playApplicationSong(ApplicationSong(song_playing_));
		}
	}
}



////////////////////////////////////////////////////////////////////////
///
/// @fn float SoundFMOD::getSongVolume()
///
/// Accesseur au volume de la musique.
///
/// @return : Le volume.
///
////////////////////////////////////////////////////////////////////////
float SoundFMOD::getSongVolume()
{
    if(!initialised_)
        return 0;

	if(playlistActuelle_)
	{
		playlistActuelle_->getVolume(&song_Volume);
	}
	return song_Volume;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn float SoundFMOD::getAppSongVolume(  )
///
/// obtient le volume des sons de l'application
///
///
/// @return float
///
////////////////////////////////////////////////////////////////////////
float SoundFMOD::getAppSongVolume()
{
    if(!initialised_)
        return 0;

	songs_group->getVolume(&song_Volume);
	return song_Volume;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float SoundFMOD::getEffectVolume()
///
/// Accesseur au volume des sons.
///
/// @return : Le volume.
///
////////////////////////////////////////////////////////////////////////
float SoundFMOD::getEffectVolume()
{
    if(!initialised_)
        return 0;

	return effect_Volume;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn song SoundFMOD::get_Song_Playing()
///
/// Accesseur à la musique actuelle.
///
/// @return : La musique qui joue (s'il y en a une).
///
////////////////////////////////////////////////////////////////////////
Song SoundFMOD::get_Song_Playing()
{
    if(!initialised_)
        return 0;

	return song_playing_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::togglePlaying()
///
/// Gestion des musiques qui jouent.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::togglePlaying()
{
    if(!initialised_)
        return;

	bool p;
	song_channel->getPaused(&p);
	song_channel->setPaused(!p);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::restartSong()
///
/// Repete la musique. 
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::restartSong()
{
    if(!initialised_)
        return;

	playSongInPlaylist(song_playing_);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::stop()
///
/// Arrête la musique .
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::stop()
{
    if(!initialised_)
        return;

	song_channel->setPaused(false);
	song_channel->stop();
	stopped_ = true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::stop()
///
/// Arrête la musique .
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
bool SoundFMOD::estEnPause()
{
    if(!initialised_)
        return false;

	bool p;
	song_channel->getPaused(&p);
	return p;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn ListeMusique* SoundFMOD::obtenirNomsCanaux()
///
/// Retourne la mappe .
///
/// @return : La playlist.
///
////////////////////////////////////////////////////////////////////////
std::vector<std::string> SoundFMOD::obtenirCanaux()
{
	Playlist::iterator iter;
	std::vector<std::string> valeurDeRetour;

    if(initialised_)
    {
        for(iter = playlists_.begin(); iter != playlists_.end(); iter++)
        {
            char name[500];
            result = iter->second->getName(name,500);
            ERRCHECK(result,valeurDeRetour);
            valeurDeRetour.push_back(name);
        }
    }
		
	return valeurDeRetour;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::next_playlist()
///
/// Va au prochain canal.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::next_playlist()
{
    if(!initialised_)
        return;

	Playlist::iterator iter;
	iter = playlists_.find(obtenirNomCanalCourant());
	++iter;

	if(iter == playlists_.end())
	{
		iter = playlists_.begin();
	}

	playlistActuelle_ = iter->second;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::previous_playlist()
///
/// Revient au canal précédent.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::previous_playlist()
{
	/*Playlist::iterator iter;
	iter = playlists_.find(obtenirNomCanalCourant());
	--iter;

	if(iter == playlists_.begin())
	{
		iter = playlists_.end();
	}

	playlistActuelle_ = iter->second;*/
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void modifierPlaylistActuelle(std::string)
///
/// Change de playlist.
///
/// @param[in] std::string nomCanal : le nom du canal.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::modifierPlaylistActuelle(std::string nomCanal)
{
    if(!initialised_)
        return;

	if(playlists_.find(nomCanal) != playlists_.end())
		playlistActuelle_ = playlists_[nomCanal];
}

////////////////////////////////////////////////////////////////////////
///
/// @fn std::string SoundFMOD::obtenirCanalCourant() 
///
/// Obtient le canal courant.
///
/// @return : Le nom de la playlist courante.
///
////////////////////////////////////////////////////////////////////////
std::string SoundFMOD::obtenirNomCanalCourant() 
{ 
	if(initialised_ && playlistActuelle_)
	{
		char name[500];
		result = playlistActuelle_->getName(name,500);
		ERRCHECK(result,"");
		return name;
	}
	return "";
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool SoundFMOD::songIsPlaying(  )
///
/// Indique s'il y a une chanson qui joue sur le canal en ce moment
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool SoundFMOD::songIsPlaying()
{
    if(!initialised_)
        return false;

	bool playing;
	song_channel->isPlaying(&playing);
	return playing;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::supprimerPlaylist( std::string nom )
///
/// Suppression d'un canal
///
/// @param[in] std::string nom : nom du canal a supprimer
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::supprimerPlaylist( std::string nom )
{
    if(!initialised_)
        return;

	Playlist::iterator iter = playlists_.find(nom);
	if(iter != playlists_.end())
	{
		if(playlistActuelle_ == iter->second)
		{
			if(playlists_.size() > 1)
			{
				next_playlist();
			}
			else
			{
				playlistActuelle_ = 0;
			}
		}
		result = iter->second->release();
		ERRCHECK(result,);
		playlists_.erase(iter);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SoundFMOD::supprimerChanson( std::string nomPlaylist, std::string nomChanson )
///
/// Suppression d'une chanson dans un canal, la chanson n'est pas libere de FMOD
///
/// @param[in] std::string nomPlaylist
/// @param[in] std::string nomChanson
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SoundFMOD::supprimerChanson( std::string nomPlaylist, std::string nomChanson )
{
    if(!initialised_)
        return;

	Playlist::iterator iter = playlists_.find(nomPlaylist);
	if(iter != playlists_.end())
	{
		int size;
		iter->second->getNumSounds(&size);
		FMOD::Sound* current_sound;
		for(int i = 0; i<size; i++)
		{
			iter->second->getSound(i,&current_sound);
			if(obtenirNomChanson(current_sound) == nomChanson)
			{
				result = current_sound->release();
				ERRCHECK(result,);
				return;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn std::string SoundFMOD::obtenirNomChanson( FMOD::Sound* sound )
///
/// Obtenir le nom d'une chanson FMOD
///
/// @param[in] FMOD::Sound * sound
///
/// @return std::string
///
////////////////////////////////////////////////////////////////////////
std::string SoundFMOD::obtenirNomChanson( FMOD::Sound* sound )
{
	if(initialised_ && sound)
	{
		char name[500];
		result = sound->getName(name,500);
		ERRCHECK(result,"");
		return name;
	}
	return "";
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

