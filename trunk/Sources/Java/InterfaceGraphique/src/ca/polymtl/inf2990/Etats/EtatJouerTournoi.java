package ca.polymtl.inf2990.Etats;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.Jeu.OperationsRadioJNI;
import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.PanneauCanevas;

/**
 * État java lorsqu'on joue un tournoi.
 * @author Gabriel Couvrette
 *
 */
public class EtatJouerTournoi extends EtatAbstrait
{

	@Override
	public void initialiser()
	{

	}

	@Override
	public void terminer()
	{
		Fenetre.obtenirInstance().terminerRadio();

	}

	@Override
	public void mettreAuRepos()
	{

	}

	@Override
	public void reveiller()
	{
		OperationsRadioJNI.arreterMusiqueRadio();
		Fenetre.obtenirInstance().afficherRadio();
	}

	@Override
	public Etats obtenirNom()
	{
		return Etats.MODE_TOURNOI;
	}

	@Override
	protected PanneauAbstrait creerPanneau()
	{
		return PanneauCanevas.obtenirInstance();
	}

}
