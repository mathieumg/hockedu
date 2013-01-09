package ca.polymtl.inf2990.Panneaux;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.Jeu.OperationsJoueursJNI;

/**
 * Panneau pour changer les touches qui contrôleront le maillet du joueur au clavier.
 * 
 * @author Vincent Lemire/Gabriel Couvrette
 * 
 */
public class PanneauModificationTouches extends PanneauAbstrait
{
	private static JPanel		panneauModificationTouche_;
	private EcouteurDeTouches	keyboardListener_;
	private ChampTouche			textFieldSelectionne_;

	private ChampTouche			toucheHaut_;
	private ChampTouche			toucheBas_;
	private ChampTouche			toucheGauche_;
	private ChampTouche			toucheDroite_;

	private JButton				boutonOk_;
	private JButton				boutonReset_;
	private JButton				boutonCancel_;

	@Override
	public void creer()
	{

		if (panneauModificationTouche_ == null)
		{
			int[] touches = OperationsJoueursJNI.obtenirTouches();

			JPanel panneauPrincipal = new JPanel(new BorderLayout());
			EcouteurZoneTexteFocus ecouteurDuFocus = new EcouteurZoneTexteFocus();
			keyboardListener_ = new EcouteurDeTouches();
			panneauModificationTouche_ = new JPanel();
			panneauModificationTouche_.setLayout(new GridLayout(4, 1));
			panneauModificationTouche_.addKeyListener(keyboardListener_);

			JPanel panneauHaut = new JPanel();
			JLabel labelHaut = new JLabel("Haut : ");
			toucheHaut_ = new ChampTouche();
			toucheHaut_.addFocusListener(ecouteurDuFocus);
			panneauModificationTouche_.add(panneauHaut);
			toucheHaut_.setColumns(15);
			toucheHaut_.setEditable(false);
			toucheHaut_.addKeyListener(keyboardListener_);
			panneauHaut.add(labelHaut);
			toucheHaut_.setFocusable(false);
			panneauHaut.add(toucheHaut_);
			panneauModificationTouche_.add(panneauHaut);

			JPanel panneauBas = new JPanel();
			JLabel labelBas = new JLabel("Bas : ");
			toucheBas_ = new ChampTouche();
			toucheBas_.addFocusListener(ecouteurDuFocus);
			panneauModificationTouche_.add(panneauBas);
			toucheBas_.setColumns(15);
			toucheBas_.setEditable(false);
			toucheBas_.addKeyListener(keyboardListener_);
			toucheBas_.setFocusable(false);
			panneauBas.add(labelBas);
			panneauBas.add(toucheBas_);
			panneauModificationTouche_.add(panneauBas);

			JPanel panneauGauche = new JPanel();
			JLabel labelGauche = new JLabel("Gauche : ");
			toucheGauche_ = new ChampTouche();
			toucheGauche_.addFocusListener(ecouteurDuFocus);
			panneauModificationTouche_.add(panneauGauche);
			toucheGauche_.setColumns(14);
			toucheGauche_.setEditable(false);
			toucheGauche_.addKeyListener(keyboardListener_);
			panneauGauche.add(labelGauche);
			toucheGauche_.setFocusable(false);
			panneauGauche.add(toucheGauche_);
			panneauModificationTouche_.add(panneauGauche);

			JPanel panneauDroite = new JPanel();
			JLabel labelDroite = new JLabel("Droite : ");
			toucheDroite_ = new ChampTouche();
			toucheDroite_.addFocusListener(ecouteurDuFocus);
			toucheDroite_.setColumns(15);
			toucheDroite_.setEditable(false);
			toucheDroite_.addKeyListener(keyboardListener_);
			toucheDroite_.setFocusable(false);

			panneauDroite.add(labelDroite);
			panneauDroite.add(toucheDroite_);
			panneauModificationTouche_.add(panneauDroite);

			panneauPrincipal.add(panneauModificationTouche_, BorderLayout.CENTER);

			JPanel panneauBoutons = new JPanel(new FlowLayout(FlowLayout.TRAILING));

			boutonReset_ = new JButton("Réinitialiser");
			boutonReset_.addActionListener(new ActionListener() {

				@Override
				public void actionPerformed(final ActionEvent e)
				{
					actionReset();
				}
			});
			panneauBoutons.add(boutonReset_);

			boutonOk_ = new JButton("Ok");
			boutonOk_.addActionListener(new ActionListener() {

				@Override
				public void actionPerformed(final ActionEvent e)
				{
					actionOk();
				}
			});
			panneauBoutons.add(boutonOk_);

			boutonCancel_ = new JButton("Annuler");
			boutonCancel_.addActionListener(new ActionListener() {

				@Override
				public void actionPerformed(final ActionEvent e)
				{
					actionCancel();
				}
			});
			panneauBoutons.add(boutonCancel_);

			toucheHaut_.lierTouche(touches[0]);
			toucheBas_.lierTouche(touches[1]);
			toucheGauche_.lierTouche(touches[2]);
			toucheDroite_.lierTouche(touches[3]);

			panneauPrincipal.add(panneauBoutons, BorderLayout.SOUTH);
			add(panneauPrincipal);
		}
		boutonCancel_.requestFocusInWindow();
	}

	public void activerPanneau()
	{
		toucheHaut_.setFocusable(true);
		toucheBas_.setFocusable(true);
		toucheGauche_.setFocusable(true);
		toucheDroite_.setFocusable(true);
		boutonCancel_.requestFocusInWindow();
	}

	/**
	 * Comportement du bouton DEFAUT
	 */
	public void actionReset()
	{
		toucheHaut_.lierTouche(KeyEvent.VK_W);
		toucheBas_.lierTouche(KeyEvent.VK_S);
		toucheGauche_.lierTouche(KeyEvent.VK_A);
		toucheDroite_.lierTouche(KeyEvent.VK_D);
	}

	/**
	 * Comportement du bouton OK
	 */
	public void actionOk()
	{
		if (valeursValide())
		{
			OperationsJoueursJNI.modifierTouches(toucheHaut_.obtenirEvenementLie(), toucheBas_.obtenirEvenementLie(), toucheGauche_.obtenirEvenementLie(), toucheDroite_.obtenirEvenementLie());

			actionCancel();
		}
		else
		{
			JOptionPane.showMessageDialog(Fenetre.obtenirInstance(), "Certaines touches sont invalides!");
		}
	}

	/**
	 * Comportement du bouton ANNULER
	 */
	public void actionCancel()
	{
		Fenetre.obtenirInstance().cacherPanneauModificationTouches();
	}

	/**
	 * Valide les entrées des touches.
	 * @return Si les touches demandées sont valides.
	 */
	public boolean valeursValide()
	{
		ChampTouche[] touches = { toucheHaut_, toucheBas_, toucheGauche_, toucheDroite_ };
		for (int i = 0; i < 4; i++)
		{
			if(
					touches[i].obtenirEvenementLie()==KeyEvent.VK_ESCAPE ||
					touches[i].obtenirEvenementLie()==KeyEvent.VK_SPACE  ||
					touches[i].obtenirEvenementLie()==KeyEvent.VK_CONTROL||
					touches[i].obtenirEvenementLie()==KeyEvent.VK_ALT	 ||
					touches[i].obtenirEvenementLie()==KeyEvent.VK_ALT_GRAPH					
			  )
				return false;
			for (int j = i; j < 4; j++)
			{
				if (i != j)
				{
					if (touches[i].obtenirEvenementLie() == touches[j].obtenirEvenementLie())
					{
						return false;
					}
				}
			}
		}
		

		return true;
	}

	@Override
	public void rafraichir()
	{
		// Rien?
	}

	/**
	 * Écouteur servant à gérer l'entrée de touches dans les zones de texte.
	 * @author Gabriel Couvrette
	 *
	 */
	private class EcouteurDeTouches extends KeyAdapter
	{
		@Override
		public void keyPressed(final KeyEvent e)
		{
			if (textFieldSelectionne_ != null)
			{
				textFieldSelectionne_.lierEvenement(e);
				textFieldSelectionne_ = null;
				boutonOk_.requestFocus();
			}
		}
	}

	/**
	 * Écouteur servant à gérer l'accès aux zones de texte. Travaille conjointement avec EcouteurDeTouches afin de gérer adéquatement
	 * l'entrées de touches dans les zones de texte.
	 * @author Gabriel Couvrette
	 *
	 */
	private class EcouteurZoneTexteFocus implements FocusListener
	{

		@Override
		public void focusGained(final FocusEvent e)
		{
			textFieldSelectionne_ = ((ChampTouche) e.getSource());
			textFieldSelectionne_.setText("Appuyez sur une touche...");
		}

		@Override
		public void focusLost(final FocusEvent e)
		{
			ChampTouche champ = ((ChampTouche) e.getSource());
			if (champ.obtenirEvenementLie() != 0)
			{
				champ.setText(KeyEvent.getKeyText(champ.obtenirEvenementLie()));
			}
			else
			{
				champ.setText("null");
			}
		}

	}

	/**
	 * Champ de texte adapté afin de mémoriser le code de touche qu'on lui associe ainsi que d'afficher correctement son nom.
	 * @author Gabriel Couvrette
	 *
	 */
	private class ChampTouche extends JTextField
	{
		int	evenementLie_;

		/**
		 * Retourne l'événement lié à cette zone de texte.
		 * @return le code de la touche liée.
		 */
		public int obtenirEvenementLie()
		{
			return evenementLie_;
		}

		/**
		 * Lie une touche à cet élément selon le code de la touche.
		 * @param codeTouche Le code de la touche selon la classe KeyEvent.
		 */
		public void lierTouche(final int codeTouche)
		{
			evenementLie_ = codeTouche;
			setText(KeyEvent.getKeyText(codeTouche));
		}

		/**
		 * Lie la touche directement avec l'événement de touche saisie par EcouteurDeTouches.
		 * @param evt L'événement de touche.
		 */
		public void lierEvenement(final KeyEvent evt)
		{
			evenementLie_ = evt.getKeyCode();
			setText(KeyEvent.getKeyText(evt.getKeyCode()));
		}
	}
}
