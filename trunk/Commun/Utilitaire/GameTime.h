///////////////////////////////////////////////////////////////////////////////
/// @file GameTime.h
/// @author Michael Ferris
/// @date 2011-12-02
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <ctime>

///////////////////////////////////////////////////////////////////////////
/// @class GameTime
/// @brief Contient les m�thodes pour la manipulation du temps dans le programme.
///
/// @author Michael Ferris
/// @date 2011-05-14
///////////////////////////////////////////////////////////////////////////
class GameTime
{
public:
	/// Constructeurs
	GameTime(void);
	GameTime(clock_t currentTime);
	/// Destructeur
	~GameTime(void);

	/// Retourne le temps �couler en milisecondes
	float Elapsed_Time_ms();

	/// Retourne le temps �couler en secondes
	float Elapsed_Time_sec();

	/// Retourne le temps �couler par rapport � l'horloge
	clock_t Elapsed_Time_clock();

	/// ref est en millisecondes
	float Elapsed_Time_percent(clock_t ref);

	/// Remet le compteur � 0 relatif au temps pr�sent
	void reset_Time();
	/// Ajuste le temps du compteur par rapport au temps passer
	void adjustTime(float ms);

	/// Permet de mettre la minuterie en pause
	inline void pause(){pause_time_ = Elapsed_Time_clock();paused_ = true;}
	inline void unPause(){paused_ = false; adjustTime((Elapsed_Time_clock()-pause_time_)/(CLOCKS_PER_SEC/1000.0f));}

private:
	/// Le temps initial
	clock_t begin_time_;
	/// Indique si la minuterie est en pause
	bool paused_;
	/// Conservation en memoire du temps au moment ou on pause la minuterie
	clock_t pause_time_;
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////





