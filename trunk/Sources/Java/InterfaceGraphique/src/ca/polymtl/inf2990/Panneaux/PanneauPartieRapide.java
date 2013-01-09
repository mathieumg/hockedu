package ca.polymtl.inf2990.Panneaux;

import java.awt.Dialog;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.util.ArrayList;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.GestionnaireImages.NomImage;
import ca.polymtl.inf2990.Composants.BoutonHockey;
import ca.polymtl.inf2990.Etats.EtatPartieRapide;
import ca.polymtl.inf2990.Etats.GestionnaireEtats;
import ca.polymtl.inf2990.Jeu.JoueurHumain;
import ca.polymtl.inf2990.Jeu.JoueurVirtuel;
import ca.polymtl.inf2990.Jeu.OperationsJoueursJNI;

/**
 * Panneau simple qui demande à l'usager si il veut joueur sa partie rapide
 * contre un joueur virtuel ou un joueur humain.
 * 
 * @author Gabriel Couvrette
 * 
 */
public class PanneauPartieRapide extends PanneauAbstrait
{
	private static JDialog	fenetre_;

	@Override
	protected void creer()
	{
		setLayout(new GridLayout(4, 1, 0, (int) (0.050673611 * getHeight())));
		BoutonHockey boutonVirtuel = new BoutonHockey(NomImage.BOUTON_NORMAL, NomImage.BOUTON_ENFONCE, NomImage.BOUTON_SURVOLE, "Joueur virtuel");
		boutonVirtuel.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent e)
			{
				int larg = 500;
				int haut = 400;
				FenetreChoixJoueurVirtuel fenetre = new FenetreChoixJoueurVirtuel();
				Rectangle dimensionsFenetre = Fenetre.obtenirInstance().getBounds();
				fenetre.setBounds((int) (dimensionsFenetre.getX() + (dimensionsFenetre.getWidth() / 2) - (larg / 2)), (int) (dimensionsFenetre.getY() + (dimensionsFenetre.getHeight() / 2) - (haut / 2)), larg, haut);
				fenetre.pack();
				fenetre.setVisible(true);
			}
		});

		BoutonHockey boutonHumain = new BoutonHockey(NomImage.BOUTON_NORMAL, NomImage.BOUTON_ENFONCE, NomImage.BOUTON_SURVOLE, "Joueur humain");
		boutonHumain.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(final ActionEvent e)
			{
				EtatPartieRapide etat = new EtatPartieRapide();
				etat.changerJoueur(new JoueurHumain("Joueur Droit"));
				GestionnaireEtats.obtenirInstance().changerEtat(etat);
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

		add(boutonVirtuel);
		add(boutonHumain);

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
		setLayout(new GridLayout(4, 1, 0, (int) (0.050673611 * getHeight())));
	}

	/**
	 * Popup qui demande à l'usager contre quel profil de joueur jouer cette
	 * partie rapide.
	 * 
	 * @author Charles Etienne Lalonde
	 * 
	 */
	private class FenetreChoixJoueurVirtuel extends JDialog
	{
		JComboBox	componentSource_;
		JButton		boutonOk_, boutonCancel_;

		public FenetreChoixJoueurVirtuel()
		{
			super(new JFrame(), "", Dialog.ModalityType.APPLICATION_MODAL);
			// Chargement des profils
			Font police = new Font("Game Over", 0, 40);

			ArrayList<Object> listeDesJoueurs = OperationsJoueursJNI.obtenirJoueurs();
			componentSource_ = new JComboBox();
			componentSource_.setFont(police);
			for (int j = 0; j < listeDesJoueurs.size(); j++)
			{
				componentSource_.addItem(listeDesJoueurs.get(j));
			}
			if (listeDesJoueurs.isEmpty())
			{
				componentSource_.addItem("IA normale");
			}

			boutonOk_ = new JButton("Ok");
			boutonOk_.setFont(police);
			boutonOk_.addActionListener(new ActionListener() {

				@Override
				public void actionPerformed(final ActionEvent e)
				{
					actionOk();
				}
			});

			boutonCancel_ = new JButton("Annuler");
			boutonCancel_.setFont(police);
			boutonCancel_.addActionListener(new ActionListener() {

				@Override
				public void actionPerformed(final ActionEvent e)
				{
					componentSource_.setSelectedIndex(-1);
					dispose();
				}
			});

			JPanel p = new JPanel(new FlowLayout());
			JLabel label = new JLabel("Joueur : ");
			label.setFont(police);
			p.add(label);
			p.add(componentSource_);
			p.add(boutonOk_);
			p.add(boutonCancel_);
			add(p);
		}

		/**
		 * Que faire quand on appuie sur ok...
		 */
		private void actionOk()
		{
			JoueurVirtuel joueur;

			joueur = (JoueurVirtuel) (OperationsJoueursJNI.obtenirJoueur(componentSource_.getSelectedItem().toString()));

			if (joueur == null)
			{
				joueur = new JoueurVirtuel("IA normale", 50, 50);// si aucun joueur n'est selectionne, on en choisi un par defaut.
			}
			EtatPartieRapide etat = new EtatPartieRapide();
			etat.changerJoueur(joueur);
			GestionnaireEtats.obtenirInstance().changerEtat(etat);
			dispose();
		}
	}

	@Override
	public void rafraichir()
	{
		// TODO Auto-generated method stub

	}

}
