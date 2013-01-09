package ca.polymtl.inf2990.Etats;

import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.PanneauMenuPrincipal;

/**
 * État initial, dans le menu principal.
 * @author Charles Étienne Lalonde
 *
 */
public class EtatMenuPrincipal extends EtatAbstrait
{

	@Override
	public void initialiser()
	{
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
		return Etats.MENU_PRINCIPAL;
	}

	@Override
	protected PanneauAbstrait creerPanneau()
	{
		return PanneauMenuPrincipal.obtenirInstance();
	}

}
