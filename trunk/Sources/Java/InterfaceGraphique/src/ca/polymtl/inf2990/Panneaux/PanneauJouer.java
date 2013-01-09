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
import ca.polymtl.inf2990.Etats.EtatOptionPartieRapide;
import ca.polymtl.inf2990.Etats.EtatOptionsTournoi;
import ca.polymtl.inf2990.Etats.GestionnaireEtats;

/**
 * Sous-panneau offrant les options : Partie rapide ou tournoi.
 * @author Charles Étienne Lalonde
 *
 */
public class PanneauJouer extends PanneauAbstrait
{

	@Override
	protected void creer()
	{
		setLayout(new GridLayout(4, 1, 0, (int) (0.050673611 * getHeight())));
		setOpaque(false);

		BoutonHockey boutonPartieRapide = new BoutonHockey(NomImage.BOUTON_NORMAL, NomImage.BOUTON_ENFONCE, NomImage.BOUTON_SURVOLE, "Partie rapide");
		boutonPartieRapide.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent e)
			{
				GestionnaireEtats.obtenirInstance().changerEtat(new EtatOptionPartieRapide());
			}
		});
		add(boutonPartieRapide);

		BoutonHockey boutonTournois = new BoutonHockey(NomImage.BOUTON_NORMAL, NomImage.BOUTON_ENFONCE, NomImage.BOUTON_SURVOLE, "Tournoi");
		boutonTournois.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent e)
			{

				GestionnaireEtats.obtenirInstance().changerEtat(new EtatOptionsTournoi());

			}
		});
		add(boutonTournois);

		JPanel panneauRemplissage = new JPanel();
		panneauRemplissage.setOpaque(false);
		add(panneauRemplissage);

		BoutonHockey boutonRetour = new BoutonHockey(NomImage.BOUTON_NORMAL, NomImage.BOUTON_ENFONCE, NomImage.BOUTON_SURVOLE, "Retour");
		boutonRetour.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent e)
			{
				GestionnaireEtats.obtenirInstance().allerEtatPrecedent();
			}
		});

		add(boutonRetour);

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
