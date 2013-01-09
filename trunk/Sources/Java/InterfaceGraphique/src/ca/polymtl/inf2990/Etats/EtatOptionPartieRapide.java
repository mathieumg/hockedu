package ca.polymtl.inf2990.Etats;

import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.PanneauMenuPrincipal;

/**
 * État de sous-menu affichant le choix entre jouer contre un adversaire humain ou virtuel.
 * @author Charles Étienne Lalonde
 *
 */
public class EtatOptionPartieRapide extends EtatAbstrait
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
		return Etats.MODE_OPTION_PARTIE_RAPIDE;
	}

	@Override
	protected PanneauAbstrait creerPanneau()
	{
		return PanneauMenuPrincipal.obtenirInstance();
	}

}
