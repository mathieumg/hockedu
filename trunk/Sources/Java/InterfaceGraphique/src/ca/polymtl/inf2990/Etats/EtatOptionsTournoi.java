package ca.polymtl.inf2990.Etats;

import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.PanneauMenuPrincipal;

/**
 * Etat qui donne le choix entre aller dans le mode nouveau tournoi ou continuer tournoi.
 * @author écharles Étienne Lalonde
 *
 */
public class EtatOptionsTournoi extends EtatAbstrait
{

	@Override
	public void initialiser()
	{
		// TODO Auto-generated method stub
	}

	@Override
	public void terminer()
	{
		// TODO Auto-generated method stub

	}

	@Override
	public void mettreAuRepos()
	{
		// TODO Auto-generated method stub

	}

	@Override
	public void reveiller()
	{
		((PanneauMenuPrincipal) obtenirPanneau()).afficher(obtenirNom());
		obtenirPanneau().getComponentListeners()[0].componentResized(null);
	}

	@Override
	public Etats obtenirNom()
	{
		return Etats.MODE_OPTION_TOURNOI;
	}

	@Override
	protected PanneauAbstrait creerPanneau()
	{
		return PanneauMenuPrincipal.obtenirInstance();
	}

}
