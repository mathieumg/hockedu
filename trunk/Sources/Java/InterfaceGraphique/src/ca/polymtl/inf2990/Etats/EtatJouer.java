package ca.polymtl.inf2990.Etats;

import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.PanneauMenuPrincipal;

/**
 * État d'interface utilisée lorsqu'on joue.
 * @author Gabriel Couvrette
 *
 */
public class EtatJouer extends EtatAbstrait
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
		return Etats.MODE_JOUER;
	}

	@Override
	protected PanneauAbstrait creerPanneau()
	{
		// TODO Auto-generated method stub
		return PanneauMenuPrincipal.obtenirInstance();
	}

}
