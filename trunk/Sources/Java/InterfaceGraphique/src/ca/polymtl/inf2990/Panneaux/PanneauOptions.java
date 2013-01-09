package ca.polymtl.inf2990.Panneaux;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;

import javax.swing.JPanel;

import ca.polymtl.inf2990.GestionnaireImages.NomImage;
import ca.polymtl.inf2990.Composants.BoutonHockey;
import ca.polymtl.inf2990.Etats.EtatListeCanaux;
import ca.polymtl.inf2990.Etats.EtatListeProfils;
import ca.polymtl.inf2990.Etats.GestionnaireEtats;

/**
 * Panneau qui affiche les options : canal de radio ou profil de joueurs.
 * @author Charles Étienne Lalonde
 *
 */
public class PanneauOptions extends PanneauAbstrait
{

	@Override
	protected void creer()
	{
		setLayout(new GridLayout(4, 1, 0, (int) (0.050673611 * getHeight())));
		setOpaque(false);
		BoutonHockey boutonProfils = new BoutonHockey(NomImage.BOUTON_NORMAL, NomImage.BOUTON_ENFONCE, NomImage.BOUTON_SURVOLE, "Profils");
		boutonProfils.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(final ActionEvent e)
			{

				GestionnaireEtats.obtenirInstance().changerEtat(new EtatListeProfils());

			}
		});
		add(boutonProfils);

		BoutonHockey boutonListeCanaux = new BoutonHockey(NomImage.BOUTON_NORMAL, NomImage.BOUTON_ENFONCE, NomImage.BOUTON_SURVOLE, "Canaux radio");
		boutonListeCanaux.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent e)
			{

				GestionnaireEtats.obtenirInstance().changerEtat(new EtatListeCanaux());

			}
		});
		add(boutonListeCanaux);

		BoutonHockey boutonRetour = new BoutonHockey(NomImage.BOUTON_NORMAL, NomImage.BOUTON_ENFONCE, NomImage.BOUTON_SURVOLE, "Retour");
		boutonRetour.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent e)
			{
				GestionnaireEtats.obtenirInstance().allerEtatPrecedent();
			}
		});

		JPanel panneauRemplissage = new JPanel();
		panneauRemplissage.setOpaque(false);
		add(panneauRemplissage);

		panneauRemplissage = new JPanel();
		panneauRemplissage.setOpaque(false);
		add(panneauRemplissage);

		add(boutonRetour);
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

	}

}
