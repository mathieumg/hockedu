package ca.polymtl.inf2990.Etats;

import ca.polymtl.inf2990.BarresOutils.EvenementsSelectionneurFichier;
import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.PanneauCreerTournoi;

/**
 * État de création d'un nouveau tournoi.
 * @author Gabriel Couvrette
 *
 */
public class EtatNouveauTournoi extends EtatAbstrait
{

	@Override
	public void initialiser()
	{
	}

	@Override
	public void terminer()
	{
		((PanneauCreerTournoi)obtenirPanneau()).reinitialiser();
	}

	@Override
	public void mettreAuRepos()
	{
		// TODO Auto-generated method stub

	}

	@Override
	public void reveiller()
	{
		// TODO Auto-generated method stub

	}

	@Override
	public Etats obtenirNom()
	{
		return Etats.CREATION_TOURNOI;
	}

	@Override
	protected PanneauAbstrait creerPanneau()
	{
		return new PanneauCreerTournoi(16);
	}

}
