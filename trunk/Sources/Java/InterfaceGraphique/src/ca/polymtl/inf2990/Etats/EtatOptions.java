package ca.polymtl.inf2990.Etats;

import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.PanneauMenuPrincipal;
/**
 * État qui affiche les choix de menus entre ajouter des profils ou ajouter des canaux de radios.
 * @author Charles Étienne Lalonde
 *
 */
public class EtatOptions extends EtatAbstrait
{

	@Override
	public void initialiser()
	{

	}

	@Override
	public void terminer()
	{

	}

	@Override
	public void mettreAuRepos()
	{
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
		return Etats.MODE_OPTIONS;
	}

	@Override
	protected PanneauAbstrait creerPanneau()
	{
		return PanneauMenuPrincipal.obtenirInstance();
	}

}
