package ca.polymtl.inf2990.Panneaux;

import java.awt.Color;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;

import javax.swing.JPanel;

import ca.polymtl.inf2990.GestionnaireImages.NomImage;
import ca.polymtl.inf2990.Composants.BoutonHockey;
import ca.polymtl.inf2990.Etats.EtatListeTournois;
import ca.polymtl.inf2990.Etats.EtatNouveauTournoi;
import ca.polymtl.inf2990.Etats.GestionnaireEtats;

/**
 * Panneau simple demandant à l'usager si il veut créer ou poursuivre un
 * tournoi.
 * 
 * @author Gabriel Couvrette
 * 
 */
public class PanneauOptionsTournoi extends PanneauAbstrait
{

	@Override
	protected void creer()
	{
		setLayout(new GridLayout(4, 1, 0, (int) (0.050673611 * getHeight())));
		BoutonHockey boutonNouveau = new BoutonHockey(NomImage.BOUTON_NORMAL, NomImage.BOUTON_ENFONCE, NomImage.BOUTON_SURVOLE, "Nouveau tournoi");
		boutonNouveau.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent e)
			{

				GestionnaireEtats.obtenirInstance().changerEtat(new EtatNouveauTournoi());

			}
		});

		BoutonHockey boutonContinuer = new BoutonHockey(NomImage.BOUTON_NORMAL, NomImage.BOUTON_ENFONCE, NomImage.BOUTON_SURVOLE, "Continuer tournoi");
		boutonContinuer.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent e)
			{
				GestionnaireEtats.obtenirInstance().changerEtat(new EtatListeTournois());
			}
		});

		BoutonHockey boutonCancel = new BoutonHockey(NomImage.BOUTON_NORMAL, NomImage.BOUTON_ENFONCE, NomImage.BOUTON_SURVOLE, "Retour");
		boutonCancel.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent e)
			{
				GestionnaireEtats.obtenirInstance().allerEtatPrecedent();
			}
		});

		add(boutonNouveau);
		add(boutonContinuer);

		JPanel panneauRemplissage = new JPanel();
		panneauRemplissage.setOpaque(false);
		add(panneauRemplissage);

		add(boutonCancel);
		setOpaque(false);

		addComponentListener(new ComponentAdapter() {
			@Override
			public void componentResized(final ComponentEvent e)
			{
				setLayout(new GridLayout(getComponentCount(), 1, 0, (int) (0.050673611 * getHeight())));
			}
		});
	}

	@Override
	public void rafraichir()
	{
		// TODO Auto-generated method stub

	}

}
