package ca.polymtl.inf2990.Panneaux;

import java.awt.BorderLayout;

import ca.polymtl.inf2990.Fenetre;

/**
 * Le panneau abstrait contenant le canevas openGL.
 * @author Charles Étienne Lalonde
 *
 */
public class PanneauCanevas extends PanneauAbstrait
{
	private static PanneauCanevas	instance_;

	private PanneauCanevas()
	{
		setLayout(new BorderLayout());
		add(Fenetre.obtenirInstance().obtenirCanevas(), BorderLayout.CENTER);
	}

	public static PanneauCanevas obtenirInstance()
	{
		if (instance_ == null)
		{
			instance_ = new PanneauCanevas();
		}
		return instance_;
	}

	@Override
	protected void creer()
	{

	}

	@Override
	public void rafraichir()
	{
	}

}
