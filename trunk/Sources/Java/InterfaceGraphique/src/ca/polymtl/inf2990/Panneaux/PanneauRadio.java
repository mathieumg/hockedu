package ca.polymtl.inf2990.Panneaux;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.Jeu.OperationsRadioJNI;

/**
 * Panneau de la radio.
 * 
 * @author Gabriel Couvrette
 * 
 */
public class PanneauRadio extends PanneauAbstrait
{

	//Composants de l'interface
	private BoutonRadio	boutonTogglePlayPause_;
	private BoutonRadio	boutonStop_;
	private BoutonRadio	boutonSuivant_;
	private BoutonRadio	boutonPrecedent_;
	private JSlider		glissoirVolume_;
	private JComboBox	champTexte_;

	enum ActionsRadio
	{
		JOUER, ARRETER, PAUSE, SUIVANT, PRECEDENT, CHARGER_DOSSIER
	};

	@Override
	protected void creer()
	{
		ecouteurRadio ecouteurRadio = new ecouteurRadio();
		setLayout(new BorderLayout());
		JPanel panneauBoutons_ = new JPanel(new FlowLayout(FlowLayout.CENTER));

		boutonTogglePlayPause_ = new BoutonRadio("||");
		boutonTogglePlayPause_.choisirAction(ActionsRadio.PAUSE);
		boutonTogglePlayPause_.addActionListener(ecouteurRadio);

		boutonStop_ = new BoutonRadio("STOP");
		boutonStop_.choisirAction(ActionsRadio.ARRETER);
		boutonStop_.addActionListener(ecouteurRadio);

		boutonSuivant_ = new BoutonRadio(">>>");
		boutonSuivant_.choisirAction(ActionsRadio.SUIVANT);
		boutonSuivant_.addActionListener(ecouteurRadio);

		boutonPrecedent_ = new BoutonRadio("<<<");
		boutonPrecedent_.choisirAction(ActionsRadio.PRECEDENT);
		boutonPrecedent_.addActionListener(ecouteurRadio);

		glissoirVolume_ = new JSlider(0, 100);
		glissoirVolume_.addChangeListener(ecouteurRadio);
		champTexte_ = new JComboBox();
		champTexte_.addActionListener(new ecouteurZoneTexte());

		panneauBoutons_.add(boutonPrecedent_);
		panneauBoutons_.add(boutonTogglePlayPause_);
		panneauBoutons_.add(boutonStop_);
		panneauBoutons_.add(boutonSuivant_);
		panneauBoutons_.add(glissoirVolume_);

		add(champTexte_, BorderLayout.NORTH);
		add(panneauBoutons_, BorderLayout.SOUTH);
	}

	@Override
	public void rafraichir()
	{
		ArrayList<Object> canaux = OperationsRadioJNI.obtenirCanaux();
		champTexte_.removeAllItems();
		for (int i = 0; i < canaux.size(); ++i)
		{
			champTexte_.addItem(canaux.get(i));
		}
		champTexte_.setSelectedItem(OperationsRadioJNI.obtenirCanalCourant());
	}

	/**
	 * Change la radio entre le mode jouer / pauser.
	 */
	private void togglePlayPause()
	{
		if (boutonTogglePlayPause_.obtenirAction() == ActionsRadio.JOUER)
		{
			boutonTogglePlayPause_.setText("||");
			boutonTogglePlayPause_.choisirAction(ActionsRadio.PAUSE);
		}
		else
		{
			boutonTogglePlayPause_.setText(">");
			boutonTogglePlayPause_.choisirAction(ActionsRadio.JOUER);
		}
	}

	/**
	 * Écouteur sur la liste des canaux.
	 * 
	 * @author Gabriel Couvrette
	 * 
	 */
	class ecouteurZoneTexte implements ActionListener
	{

		@Override
		public void actionPerformed(final ActionEvent e)
		{
			if (champTexte_.getSelectedIndex() >= 0)
			{
				OperationsRadioJNI.arreterMusiqueRadio();
				OperationsRadioJNI.chargerCanal((String) champTexte_.getSelectedItem());
				OperationsRadioJNI.jouerMusiqueRadio();
			}
		}

	}

	/**
	 * Écouteur utile pour les boutons de la radio.
	 * 
	 * @author Gabriel Couvrette
	 * 
	 */
	class ecouteurRadio implements ActionListener, ChangeListener
	{
		/**
		 * appelé lorsque le volume change.
		 */
		@Override
		public void stateChanged(final ChangeEvent arg0)
		{
			OperationsRadioJNI.placerVolumeRadio(glissoirVolume_.getValue());
		}

		/**
		 * Appelé lorsqu'un des boutons de la radio est cliqué.
		 */
		@Override
		public void actionPerformed(final ActionEvent e)
		{
			ActionsRadio action = ((BoutonRadio) e.getSource()).obtenirAction();
			switch (action)
			{
				case JOUER:
					OperationsRadioJNI.jouerMusiqueRadio();
					togglePlayPause();
					break;
				case ARRETER:
					OperationsRadioJNI.arreterMusiqueRadio();
					boutonTogglePlayPause_.setText(">");
					boutonTogglePlayPause_.choisirAction(ActionsRadio.JOUER);
					break;
				case PAUSE:
					OperationsRadioJNI.pauserMusiqueRadio();
					togglePlayPause();
					break;
				case PRECEDENT:
					OperationsRadioJNI.reculerChansonRadio();
					break;
				case SUIVANT:
					OperationsRadioJNI.avancerChansonRadio();
					break;
			}
			Fenetre.obtenirInstance().giveFocusToCanevas();
		}
	}

	/**
	 * Un bouton de la radio.
	 * 
	 * @author Gabriel Couvrette
	 * 
	 */
	class BoutonRadio extends JButton
	{
		BoutonRadio(final String s)
		{
			super(s);
		}

		private ActionsRadio	action_;

		public ActionsRadio obtenirAction()
		{
			return action_;
		}

		public void choisirAction(final ActionsRadio action)
		{
			action_ = action;
		}
	}

}
