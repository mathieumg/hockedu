/*
 * To change this template, choose Tools | Templates and open the template in
 * the editor.
 */

/*
 * BarreEdition.java
 * 
 * Created on Feb 17, 2012, 12:52:06 PM
 */
package ca.polymtl.inf2990.BarresOutils;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dialog;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.text.NumberFormat;
import java.text.ParseException;
import java.util.Locale;

import javax.swing.BorderFactory;
import javax.swing.InputVerifier;
import javax.swing.JComponent;
import javax.swing.JFormattedTextField;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.SwingUtilities;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;

import ca.polymtl.inf2990.GestionnaireImages;
import ca.polymtl.inf2990.GestionnaireImages.NomImage;
import ca.polymtl.inf2990.Jeu.ModificateurProprieteNoeud;

/**
 * 
 * @author Mathieu Parent
 */
public class BarreEdition extends javax.swing.JDialog
{
	public static native ModificateurProprieteNoeud obtenirValeursEdition();

	enum Panneaux
	{
		ZONE_EDITION, POSITION, ROTATION, ECHELLE, REBOND, ACCELERATION,
		FRICTION, REBOND_BANDES
	};

	/** Creates new form BarreEdition */
	public BarreEdition()
	{
		super(new JFrame(), "Modification des propriétés", Dialog.ModalityType.APPLICATION_MODAL);
		initComponents();
		fenetreImageBandes_ = new JFrame();
		JPanel p = new JPanel();
		p.add(new JLabel(GestionnaireImages.obtenirInstance().obtenirImageIcon(NomImage.BANDES)));
		fenetreImageBandes_.setContentPane(p);
		fenetreImageBandes_.setAlwaysOnTop(true);
		fenetreImageBandes_.pack();
		proprieteChangee_ = new boolean[Panneaux.values().length];

		addComponentListener(new ComponentAdapter() {
			@Override
			public void componentResized(final ComponentEvent e)
			{
				fenetreImageBandes_.setLocation(getX() + getWidth() + 5, getY() - 5);
			}

			@Override
			public void componentMoved(final ComponentEvent e)
			{
				fenetreImageBandes_.setLocation(getX() + getWidth() + 5, getY() - 5);
			}
		});
	}

	@Override
	public void setVisible(final boolean b)
	{
		if (b)
		{
			fenetreImageBandes_.setLocation(getX() + getWidth() + 5, getY() - 5);
			resetValeurs();
		}
		fenetreImageBandes_.setVisible(b);
		super.setVisible(b);

	}

	/**
	 * Replace toute les valeurs à des valeurs par défaut.
	 */
	private void resetValeurs()
	{
		toutAfficher();
		ModificateurProprieteNoeud modificateur = obtenirValeursEdition();
		if (modificateur == null)
		{
			toutCacher();
			modifierAffichage(Panneaux.ZONE_EDITION, true);
			modifierAffichage(Panneaux.FRICTION, true);
			modifierAffichage(Panneaux.REBOND_BANDES, true);
		}
		else
		{
			if (modificateur.obtenirEchelle() == -1)
			{
				modifierAffichage(Panneaux.ECHELLE, false);
			}
			if (modificateur.obtenirCoefRebond() == -1)
			{
				modifierAffichage(Panneaux.REBOND, false);
			}
			if (modificateur.obtenirBonusAccel() == -1)
			{
				modifierAffichage(Panneaux.ACCELERATION, false);
			}
			if (modificateur.obtenirRotation() == -1)
			{
				modifierAffichage(Panneaux.ROTATION, false);
			}
			if (modificateur.obtenirPositionX() == Double.MAX_VALUE)
			{
				modifierAffichage(Panneaux.POSITION, false);
			}

			double nouv = 0;

			// Valeur par defaut de la position y
			champPositionX_.setValue(modificateur.obtenirPositionX());

			// Valeur par defaut de la position x
			champPositionY_.setValue(modificateur.obtenirPositionY());

			// Valeur par defaut de la rotation
			sliderRotation_.setValue(modificateur.obtenirRotation());
			champRotation_.setValue(modificateur.obtenirRotation());

			// Valeur par defaut de l'echelle
			nouv = modificateur.obtenirEchelle();
			nouv = Math.round(nouv * 100.0) / 100.0;
			sliderEchelle_.setValue(convertirIntervalle(nouv));
			champEchelle_.setValue(nouv);

			// Valeur par defaut du rebond
			nouv = modificateur.obtenirCoefRebond();
			nouv = Math.round(nouv * 100.0) / 100.0;
			sliderRebond_.setValue(convertirIntervalle(nouv));
			champRebond_.setValue(nouv);

			// Valeur par defaut de la friction
			nouv = modificateur.obtenirCoefFriction();
			nouv = Math.round(nouv * 100.0) / 100.0;
			sliderFriction_.setValue((int) (nouv * 10));
			champFriction_.setValue(nouv);

			// Valeur par defaut de l'acceleration
			nouv = modificateur.obtenirBonusAccel();
			nouv = Math.round(nouv * 100.0) / 100.0;
			sliderAcceleration_.setValue(convertirIntervalle(nouv));
			champAcceleration_.setValue(nouv);

			// Valeur par defaut de la zone edition en x
			champLargeurZoneEdition_.setValue(modificateur.obtenirZoneEditionX());
			verificateurEntreePosition_.modifierValeurHauteur((Double) champLargeurZoneEdition_.getValue());

			// Valeur par defaut de la zone edition en y
			champLongueurZoneEdition_.setValue(modificateur.obtenirZoneEditionY());
			verificateurEntreePosition_.modifierValeurHauteur((Double) champLongueurZoneEdition_.getValue());

			for (int i = 0; i < ModificateurProprieteNoeud.NOMBRE_BANDES; ++i)
			{
				nouv = modificateur.obtenirCoefRebondBandes(i);
				nouv = Math.round(nouv * 100.0) / 100.0;
				slidersRebondBandes_[i].setValue(convertirIntervalle(nouv));
				champsRebondBandes_[i].setValue(nouv);
			}
		}

	}

	/**
	 * Vérificateur d'entrée qui va servir à vérifier la position.
	 * 
	 * @author Charles Étienne Lalonde
	 * 
	 */
	private class VerificateurEntreePosition extends InputVerifier
	{

		private double	valLargeur	= 200;
		private double	valHauteur	= 150;

		private double	valMin		= 0;
		private double	valMax		= 800;

		@Override
		public boolean verify(final JComponent entree)
		{
			valMin = (entree == champPositionX_ ? -valLargeur : -valHauteur);
			valMax = (entree == champPositionX_ ? valLargeur : valHauteur);
			JFormattedTextField champEntree = (JFormattedTextField) entree;
			double valeurChamp;
			Object o = champEntree.getValue();
			if (o instanceof Long)
			{
				long l = (Long) o;
				valeurChamp = l;
			}
			else
			{
				valeurChamp = (Double) (o);
			}
			if (valeurChamp < valMin)
			{
				champEntree.setValue(valMin);
				return false;
			}
			else if (valeurChamp > valMax)
			{
				champEntree.setValue(valMax);
				return false;
			}
			return true;
		}

		public void modifierValeurLargeur(final double nouvelleValeur)
		{
			valLargeur = nouvelleValeur;
		}

		public void modifierValeurHauteur(final double nouvelleValeur)
		{
			valHauteur = nouvelleValeur;
		}
	}

	/**
	 * This method is called from within the constructor to initialize the form.
	 * WARNING: Do NOT modify this code. The content of this method is always
	 * regenerated by the Form Editor.
	 */
	private void initComponents()
	{
		// Création des formats pour les champs textes formattés.
		NumberFormat formatReel = NumberFormat.getInstance(Locale.US);
		NumberFormat formatEntier = NumberFormat.getIntegerInstance(Locale.US);

		// Vérification d'entrée pour les champs pour les réels
		InputVerifier verifReels = new InputVerifier() {
			private final double	valMin	= 0;
			private final double	valMax	= 10;

			@Override
			public boolean verify(final JComponent entree)
			{
				JFormattedTextField champEntree = (JFormattedTextField) entree;
				double valeurChamp;
				Object o = champEntree.getValue();
				if (o instanceof Long)
				{
					long l = (Long) o;
					valeurChamp = l;
				}
				else
				{
					valeurChamp = (Double) (o);
				}

				if (valeurChamp < valMin)
				{
					champEntree.setValue(valMin);
					return false;
				}
				else if (valeurChamp > valMax)
				{
					champEntree.setValue(valMax);
					return false;
				}

				return true;
			}
		};

		// Vérification d'entrée pour le champ pour la rotation
		InputVerifier verifRotation = new InputVerifier() {
			private final int	valMin	= 0;
			private final int	valMax	= 359;

			@Override
			public boolean verify(final JComponent entree)
			{
				JFormattedTextField champEntree = (JFormattedTextField) entree;
				Object o = champEntree.getValue();
				int valeurChamp;
				if (o instanceof Long)
				{
					long l = (Long) o;
					valeurChamp = (int) l;
				}
				else
				{
					valeurChamp = (Integer) (o);
				}
				if (valeurChamp < valMin)
				{
					champEntree.setValue(valMin);
					return false;
				}
				else if (valeurChamp > valMax)
				{
					champEntree.setValue((valeurChamp % (valMax - valMin + 1)) + valMin);
					return false;
				}
				return true;
			}
		};

		// Vérification d'entrée pour les champs pour la position
		verificateurEntreePosition_ = new VerificateurEntreePosition();

		// Vérification d'entrée pour le champ pour la largeur de la zone
		// d'édition
		InputVerifier verifLargeurZoneEdition = new InputVerifier() {
			/******/
			private final double	valMin	= 75;
			private final double	valMax	= 400;

			@Override
			public boolean verify(final JComponent entree)
			{
				JFormattedTextField champEntree = (JFormattedTextField) entree;
				double valeurChamp;
				Object o = champEntree.getValue();
				if (o instanceof Long)
				{
					long l = (Long) o;
					valeurChamp = l;
				}
				else
				{
					valeurChamp = (Double) (o);
				}

				if (valeurChamp < valMin)
				{
					champEntree.setValue(valMin);
					verificateurEntreePosition_.modifierValeurLargeur(valMin);
					return false;
				}
				else if (valeurChamp > valMax)
				{
					champEntree.setValue(valMax);
					verificateurEntreePosition_.modifierValeurLargeur(valMax);
					return false;
				}
				verificateurEntreePosition_.modifierValeurLargeur(valeurChamp);
				return true;
			}
		};

		// Vérification d'entrée pour le champ pour la longueur de la zone
		// d'édition
		InputVerifier verifLongueurZoneEdition = new InputVerifier() {
			private final double	valMin	= 50;
			/*****/
			private final double	valMax	= 300;

			@Override
			public boolean verify(final JComponent entree)
			{
				JFormattedTextField champEntree = (JFormattedTextField) entree;
				double valeurChamp;
				Object o = champEntree.getValue();
				if (o instanceof Long)
				{
					long l = (Long) o;
					valeurChamp = l;
				}
				else
				{
					valeurChamp = (Double) (o);
				}

				if (valeurChamp < valMin)
				{
					champEntree.setValue(valMin);
					verificateurEntreePosition_.modifierValeurHauteur(valMin);
					return false;
				}
				else if (valeurChamp > valMax)
				{
					champEntree.setValue(valMax);
					verificateurEntreePosition_.modifierValeurHauteur(valMax);
					return false;
				}
				verificateurEntreePosition_.modifierValeurHauteur(valeurChamp);
				return true;
			}
		};

		// Création de l'écouteur pour la perte de focus des champs
		FocusListener ecouteurFocus = new FocusAdapter() {
			@Override
			public void focusGained(final FocusEvent e)
			{
				SwingUtilities.invokeLater(new Runnable() {

					@Override
					public void run()
					{
						((JFormattedTextField) e.getComponent()).selectAll();
					}
				});
			}

			@Override
			public void focusLost(final FocusEvent e)
			{
				try
				{
					((JFormattedTextField) e.getComponent()).commitEdit();
				}
				catch (ParseException e1)
				{
					((JFormattedTextField) e.getComponent()).setValue(((JFormattedTextField) e.getComponent()).getValue());
				}
				((JFormattedTextField) e.getComponent()).getInputVerifier().verify((JFormattedTextField) e.getComponent());
			}
		};

		// Creation des objets
		labelTitre_ = new javax.swing.JLabel(); // Titre de la Barre
		panelPosition_ = new javax.swing.JPanel(); // Conteneur pour la position
		labelPosition_ = new javax.swing.JLabel(); // Titre pour la position
		champPositionY_ = new javax.swing.JFormattedTextField(formatReel); // Champ
																			// pour
																			// la
																			// modification
																			// de
																			// la
																			// position
																			// en
																			// y
		separateurPosition_ = new javax.swing.JSeparator(); // Separateur au bas
															// de la section
															// position
		labelPositionY_ = new javax.swing.JLabel(); // Indicateur x
		labelPositionX_ = new javax.swing.JLabel(); // Indicateur y
		champPositionX_ = new javax.swing.JFormattedTextField(formatReel); // Champ
																			// pour
																			// la
																			// modification
																			// de
																			// la
																			// position
																			// en
																			// x
		panelRotation_ = new javax.swing.JPanel(); // Conteneur pour la rotation
		champRotation_ = new javax.swing.JFormattedTextField(formatEntier); // Champ
																			// pour
																			// la
																			// modification
																			// de
																			// l'angle
																			// de
																			// rotation
		sliderRotation_ = new javax.swing.JSlider(); // Slider pour la
														// modification de la
														// rotation
		labelRotation_ = new javax.swing.JLabel(); // Titre pour la rotation
		panelEchelle_ = new javax.swing.JPanel(); // Conteneur pour l'echelle
		labelEchelle_ = new javax.swing.JLabel(); // Titre pour la mise a
													// l'echelle
		champEchelle_ = new javax.swing.JFormattedTextField(formatReel); // Champ
																			// pour
																			// la
																			// modification
																			// de
																			// l'echelle
		sliderEchelle_ = new javax.swing.JSlider(); // Slider pour la
													// modification de l'echelle
		panelRebond_ = new javax.swing.JPanel(); // Conteneur pour les rebonds
		champRebond_ = new javax.swing.JFormattedTextField(formatReel); // Champ
																		// pour
																		// la
																		// modification
																		// du
																		// rebond
		sliderRebond_ = new javax.swing.JSlider(); // Slider pour la
													// modification du rebond
		labelRebond_ = new javax.swing.JLabel(); // Titre pour le rebond
		panelAcceleration_ = new javax.swing.JPanel(); // Conteneur pour
														// l'acceleration
		sliderAcceleration_ = new javax.swing.JSlider(); // Slider pour la
															// mdification de
															// l'acceleration
		champAcceleration_ = new javax.swing.JFormattedTextField(formatReel); // Champ
																				// pour
																				// la
																				// modification
																				// de
																				// l'acceleration
		labelAcceleration_ = new javax.swing.JLabel(); // Titre pour
														// l'acceleration
		panelFriction_ = new javax.swing.JPanel(); // Conteneur pour la friction
		sliderFriction_ = new javax.swing.JSlider(); // Slider pour la
														// modification de la
														// friction
		champFriction_ = new javax.swing.JFormattedTextField(formatReel); // Champ
																			// pour
																			// la
																			// modification
																			// de
																			// la
																			// friction
		labelFriction_ = new javax.swing.JLabel(); // Titre pour la friction
		separateurFriction_ = new javax.swing.JSeparator();

		panelZoneEdition_ = new javax.swing.JPanel(); // Conteneur pour la zone
														// d'edition
		separateurZoneEdition_ = new javax.swing.JSeparator(); // Separateur au
																// bas de la
																// section Zone
																// d'edition
		labelZoneEdition_ = new javax.swing.JLabel(); // Affichage du X pour la
														// zone d'edition

		labelLargeurZoneEdition_ = new javax.swing.JLabel(); // Affichage du X
																// pour la zone
																// d'edition
		champLargeurZoneEdition_ = new javax.swing.JFormattedTextField(formatReel); // Champ
																					// X
																					// de
																					// la
																					// zone
																					// d'edition

		labelLongueurZoneEdition_ = new javax.swing.JLabel(); // Affichage du Y
																// pour la zone
																// d'edition
		champLongueurZoneEdition_ = new javax.swing.JFormattedTextField(formatReel); // Champ
																						// Y
																						// de
																						// la
																						// zone
																						// d'edition

		// Changement de la police des champs de texte
		Font police = new Font("Game Over", 0, 32);
		java.awt.Cursor curseur = new java.awt.Cursor(java.awt.Cursor.MOVE_CURSOR);

		champsRebondBandes_ = new JFormattedTextField[ModificateurProprieteNoeud.NOMBRE_BANDES];
		labelsRebondBandes_ = new JLabel[ModificateurProprieteNoeud.NOMBRE_BANDES];
		slidersRebondBandes_ = new JSlider[ModificateurProprieteNoeud.NOMBRE_BANDES];
		panelsRebondBandes_ = new JPanel[ModificateurProprieteNoeud.NOMBRE_BANDES];
		panneauCommun_ = new JPanel();
		Border bordurePanneauCommun = BorderFactory.createLineBorder(champLongueurZoneEdition_.getForeground(), 2);
		bordurePanneauCommun = BorderFactory.createTitledBorder(bordurePanneauCommun, "Propriétés de la table", TitledBorder.LEADING, TitledBorder.DEFAULT_POSITION, police.deriveFont(Font.ITALIC, 42));
		panneauCommun_.setBorder(bordurePanneauCommun);
		panneauDePanneaux_ = new JPanel();
		Border bordurePanneauObjet = BorderFactory.createLineBorder(champLongueurZoneEdition_.getForeground(), 2);
		bordurePanneauObjet = BorderFactory.createTitledBorder(bordurePanneauObjet, "Propriétés de l'objet (des objets) sélectionné(s)", TitledBorder.LEADING, TitledBorder.DEFAULT_POSITION, police.deriveFont(Font.ITALIC, 42));
		panneauDePanneaux_.setBorder(bordurePanneauObjet);
		panneauDeBoutons_ = new JPanel();

		for (int i = 0; i < ModificateurProprieteNoeud.NOMBRE_BANDES; ++i)
		{
			police = police.deriveFont(Font.PLAIN, 32);
			champsRebondBandes_[i] = new JFormattedTextField(formatReel);
			champsRebondBandes_[i].setInputVerifier(verifReels);
			champsRebondBandes_[i].addFocusListener(ecouteurFocus);
			champsRebondBandes_[i].setFont(police);
			police = police.deriveFont(Font.PLAIN, 42);
			labelsRebondBandes_[i] = new JLabel();
			labelsRebondBandes_[i].setFont(police);
			labelsRebondBandes_[i].setText("Coefficient de rebond bande " + (i + 1) + ": ");
			slidersRebondBandes_[i] = new JSlider();
			slidersRebondBandes_[i].setCursor(curseur);
			panelsRebondBandes_[i] = new JPanel();
		}

		boutonAnnuler_ = new javax.swing.JButton();
		boutonAppliquer_ = new javax.swing.JButton();

		panneauDeBoutons_.add(boutonAppliquer_);
		panneauDeBoutons_.add(boutonAnnuler_);

		add(panneauDeBoutons_, BorderLayout.SOUTH);

		// Application des vérificateurs d'entrée
		champEchelle_.setInputVerifier(verifReels);
		champAcceleration_.setInputVerifier(verifReels);
		champFriction_.setInputVerifier(verifReels);
		champRebond_.setInputVerifier(verifReels);
		champRotation_.setInputVerifier(verifRotation);
		champPositionX_.setInputVerifier(verificateurEntreePosition_);
		champPositionY_.setInputVerifier(verificateurEntreePosition_);
		champLargeurZoneEdition_.setInputVerifier(verifLargeurZoneEdition);
		champLongueurZoneEdition_.setInputVerifier(verifLongueurZoneEdition);

		// Application de l'écouteur de focus
		champEchelle_.addFocusListener(ecouteurFocus);
		champAcceleration_.addFocusListener(ecouteurFocus);
		champFriction_.addFocusListener(ecouteurFocus);
		champRebond_.addFocusListener(ecouteurFocus);
		champRotation_.addFocusListener(ecouteurFocus);
		champPositionX_.addFocusListener(ecouteurFocus);
		champPositionY_.addFocusListener(ecouteurFocus);
		champLargeurZoneEdition_.addFocusListener(ecouteurFocus);
		champLongueurZoneEdition_.addFocusListener(ecouteurFocus);

		police = police.deriveFont(Font.PLAIN, 32);
		champEchelle_.setFont(police);
		champAcceleration_.setFont(police);
		champFriction_.setFont(police);
		champRebond_.setFont(police);
		champRotation_.setFont(police);
		champPositionX_.setFont(police);
		champPositionY_.setFont(police);
		champLargeurZoneEdition_.setFont(police);
		champLongueurZoneEdition_.setFont(police);

		setDefaultCloseOperation(javax.swing.WindowConstants.HIDE_ON_CLOSE); // Cache
																				// seulement
		setResizable(false);

		// Application du titre du frame
		labelTitre_.setFont(police.deriveFont(Font.BOLD, 54));
		labelTitre_.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
		labelTitre_.setText("Édition");

		police = police.deriveFont(Font.PLAIN, 42);
		// Application du titre de position
		labelPosition_.setFont(police);
		labelPosition_.setText("Position");
		labelPosition_.setToolTipText("Position de l'objet sur la zone de jeu (Le centre correspond au centre de l'aire de jeu)");

		// Application du titre de rotation
		labelRotation_.setFont(police);
		labelRotation_.setText("Rotation");
		labelRotation_.setToolTipText("Angle de rotation à appliquer à l'objet (En degrés)");

		// Application du titre de echelle
		labelEchelle_.setFont(police);
		labelEchelle_.setText("Échelle");
		labelEchelle_.setToolTipText("Facteur d'agrandissement de l'objet (1 correspond à une échelle normale)");

		// Application du titre de rebond
		labelRebond_.setFont(police);
		labelRebond_.setText("Rebond");
		labelRebond_.setToolTipText("Facteur avec lequel l'objet fera rebondir la rondelle (1 correspond à un rebond normal sans accélération)");

		// Application du titre de acceleration
		labelAcceleration_.setFont(police);
		labelAcceleration_.setText("Accélération");
		labelAcceleration_.setToolTipText("Facteur de différence de vitesse qui influencera la rondelle (1 correspond à une accélération nulle)");

		// Application du titre de friction
		labelFriction_.setFont(police);
		labelFriction_.setText("Friction");
		labelFriction_.setToolTipText("Coefficient de frottement de la table (1 correspond à aucune friction)");

		// Config des etiquettes
		labelPositionY_.setFont(police);
		labelPositionY_.setText("y");
		labelPositionX_.setFont(police);
		labelPositionX_.setText("x");

		// Config des etiquettes de la zone d'edition
		labelZoneEdition_.setFont(police);
		labelZoneEdition_.setText("Zone d'edition");
		labelLargeurZoneEdition_.setFont(police);
		labelLargeurZoneEdition_.setText("x");
		labelLongueurZoneEdition_.setFont(police);
		labelLongueurZoneEdition_.setText("y");

		boutonAppliquer_.setFont(police);
		boutonAppliquer_.setText("Appliquer");
		boutonAppliquer_.addActionListener(new java.awt.event.ActionListener() {
			@Override
			public void actionPerformed(final java.awt.event.ActionEvent evt)
			{
				boutonAppliquer_ActionPerformed(evt);
			}
		});

		boutonAnnuler_.setFont(police);
		boutonAnnuler_.setText("Annuler");
		boutonAnnuler_.addActionListener(new java.awt.event.ActionListener() {
			@Override
			public void actionPerformed(final java.awt.event.ActionEvent evt)
			{
				boutonAnnuler_ActionPerformed(evt);
			}
		});
		sliderRotation_.setMaximum(359);

		// Pour tous les autres sliders, l'intervalle est 0, 100 et la valeur de
		// depart est 50 (a utiliser comme pourcentage)

		// Assignation des curseurs de deplacement pour les sliders
		sliderRotation_.setCursor(curseur);
		sliderEchelle_.setCursor(curseur);
		sliderRebond_.setCursor(curseur);
		sliderFriction_.setCursor(curseur);
		sliderAcceleration_.setCursor(curseur);

		// Config du panneau de position
		javax.swing.GroupLayout panelPosition_Layout = new javax.swing.GroupLayout(panelPosition_);
		panelPosition_.setLayout(panelPosition_Layout);
		panelPosition_Layout.setHorizontalGroup(panelPosition_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelPosition_Layout.createSequentialGroup().addContainerGap().addGroup(panelPosition_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelPosition_Layout.createSequentialGroup().addComponent(labelPosition_, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addGap(18, 18, 18).addComponent(labelPositionX_).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED).addComponent(champPositionX_, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED).addComponent(labelPositionY_).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED).addComponent(champPositionY_, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE))).addContainerGap()));
		panelPosition_Layout.setVerticalGroup(panelPosition_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelPosition_Layout.createSequentialGroup().addContainerGap().addGroup(panelPosition_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE).addComponent(labelPosition_).addComponent(champPositionY_, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE).addComponent(labelPositionY_).addComponent(champPositionX_, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE).addComponent(labelPositionX_))));

		javax.swing.GroupLayout panelZoneEdition_Layout = new javax.swing.GroupLayout(panelZoneEdition_);
		panelZoneEdition_.setLayout(panelZoneEdition_Layout);
		panelZoneEdition_Layout.setHorizontalGroup(panelZoneEdition_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelZoneEdition_Layout.createSequentialGroup().addContainerGap().addGroup(panelZoneEdition_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelZoneEdition_Layout.createSequentialGroup().addComponent(labelZoneEdition_, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addGap(18, 18, 18).addComponent(labelLargeurZoneEdition_).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED).addComponent(champLargeurZoneEdition_, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED).addComponent(labelLongueurZoneEdition_).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED).addComponent(champLongueurZoneEdition_, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE))).addContainerGap()));
		panelZoneEdition_Layout.setVerticalGroup(panelZoneEdition_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelZoneEdition_Layout.createSequentialGroup().addContainerGap().addGroup(panelZoneEdition_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE).addComponent(labelZoneEdition_).addComponent(champLongueurZoneEdition_, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE).addComponent(labelLongueurZoneEdition_).addComponent(champLargeurZoneEdition_, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE).addComponent(labelLargeurZoneEdition_))));

		javax.swing.GroupLayout panelRotation_Layout = new javax.swing.GroupLayout(panelRotation_);
		panelRotation_.setLayout(panelRotation_Layout);
		panelRotation_Layout.setHorizontalGroup(panelRotation_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelRotation_Layout.createSequentialGroup().addContainerGap().addGroup(panelRotation_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelRotation_Layout.createSequentialGroup().addComponent(labelRotation_).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 71, Short.MAX_VALUE).addComponent(champRotation_, javax.swing.GroupLayout.PREFERRED_SIZE, 33, javax.swing.GroupLayout.PREFERRED_SIZE)).addComponent(sliderRotation_, javax.swing.GroupLayout.DEFAULT_SIZE, 152, Short.MAX_VALUE)).addContainerGap()));
		panelRotation_Layout.setVerticalGroup(panelRotation_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelRotation_Layout.createSequentialGroup().addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addGroup(panelRotation_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE).addComponent(labelRotation_).addComponent(champRotation_, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED).addComponent(sliderRotation_, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)));

		javax.swing.GroupLayout panelEchelle_Layout = new javax.swing.GroupLayout(panelEchelle_);
		panelEchelle_.setLayout(panelEchelle_Layout);
		panelEchelle_Layout.setHorizontalGroup(panelEchelle_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelEchelle_Layout.createSequentialGroup().addContainerGap().addGroup(panelEchelle_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelEchelle_Layout.createSequentialGroup().addComponent(labelEchelle_).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 71, Short.MAX_VALUE).addComponent(champEchelle_, javax.swing.GroupLayout.PREFERRED_SIZE, 33, javax.swing.GroupLayout.PREFERRED_SIZE)).addComponent(sliderEchelle_, javax.swing.GroupLayout.DEFAULT_SIZE, 152, Short.MAX_VALUE)).addContainerGap()));
		panelEchelle_Layout.setVerticalGroup(panelEchelle_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelEchelle_Layout.createSequentialGroup().addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addGroup(panelEchelle_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE).addComponent(labelEchelle_).addComponent(champEchelle_, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED).addComponent(sliderEchelle_, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)));

		javax.swing.GroupLayout panelRebond_Layout = new javax.swing.GroupLayout(panelRebond_);
		panelRebond_.setLayout(panelRebond_Layout);
		panelRebond_Layout.setHorizontalGroup(panelRebond_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelRebond_Layout.createSequentialGroup().addContainerGap().addGroup(panelRebond_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelRebond_Layout.createSequentialGroup().addComponent(labelRebond_).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 71, Short.MAX_VALUE).addComponent(champRebond_, javax.swing.GroupLayout.PREFERRED_SIZE, 33, javax.swing.GroupLayout.PREFERRED_SIZE)).addComponent(sliderRebond_, javax.swing.GroupLayout.DEFAULT_SIZE, 152, Short.MAX_VALUE)).addContainerGap()));
		panelRebond_Layout.setVerticalGroup(panelRebond_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelRebond_Layout.createSequentialGroup().addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addGroup(panelRebond_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE).addComponent(labelRebond_).addComponent(champRebond_, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED).addComponent(sliderRebond_, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)));

		javax.swing.GroupLayout panelAcceleration_Layout = new javax.swing.GroupLayout(panelAcceleration_);
		panelAcceleration_.setLayout(panelAcceleration_Layout);
		panelAcceleration_Layout.setHorizontalGroup(panelAcceleration_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelAcceleration_Layout.createSequentialGroup().addContainerGap().addGroup(panelAcceleration_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelAcceleration_Layout.createSequentialGroup().addComponent(labelAcceleration_).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 71, Short.MAX_VALUE).addComponent(champAcceleration_, javax.swing.GroupLayout.PREFERRED_SIZE, 33, javax.swing.GroupLayout.PREFERRED_SIZE)).addComponent(sliderAcceleration_, javax.swing.GroupLayout.DEFAULT_SIZE, 152, Short.MAX_VALUE)).addContainerGap()));
		panelAcceleration_Layout.setVerticalGroup(panelAcceleration_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelAcceleration_Layout.createSequentialGroup().addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addGroup(panelAcceleration_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE).addComponent(labelAcceleration_).addComponent(champAcceleration_, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED).addComponent(sliderAcceleration_, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)));

		javax.swing.GroupLayout panelFriction_Layout = new javax.swing.GroupLayout(panelFriction_);
		panelFriction_.setLayout(panelFriction_Layout);
		panelFriction_Layout.setHorizontalGroup(panelFriction_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelFriction_Layout.createSequentialGroup().addContainerGap().addGroup(panelFriction_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelFriction_Layout.createSequentialGroup().addComponent(labelFriction_).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 71, Short.MAX_VALUE).addComponent(champFriction_, javax.swing.GroupLayout.PREFERRED_SIZE, 33, javax.swing.GroupLayout.PREFERRED_SIZE)).addComponent(sliderFriction_, javax.swing.GroupLayout.DEFAULT_SIZE, 152, Short.MAX_VALUE)).addContainerGap()));
		panelFriction_Layout.setVerticalGroup(panelFriction_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panelFriction_Layout.createSequentialGroup().addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addGroup(panelFriction_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE).addComponent(labelFriction_).addComponent(champFriction_, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED).addComponent(sliderFriction_, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)));

		for (int i = 0; i < ModificateurProprieteNoeud.NOMBRE_BANDES; ++i)
		{
			javax.swing.GroupLayout panneau_Layout = new javax.swing.GroupLayout(panelsRebondBandes_[i]);
			panelsRebondBandes_[i].setLayout(panneau_Layout);
			panneau_Layout.setHorizontalGroup(panneau_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panneau_Layout.createSequentialGroup().addContainerGap().addGroup(panneau_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addComponent(slidersRebondBandes_[i], javax.swing.GroupLayout.DEFAULT_SIZE, 152, Short.MAX_VALUE).addGroup(panneau_Layout.createSequentialGroup().addComponent(labelsRebondBandes_[i]).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 78, Short.MAX_VALUE).addComponent(champsRebondBandes_[i], javax.swing.GroupLayout.PREFERRED_SIZE, 33, javax.swing.GroupLayout.PREFERRED_SIZE))).addContainerGap()));
			panneau_Layout.setVerticalGroup(panneau_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(panneau_Layout.createSequentialGroup().addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addGroup(panneau_Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE).addComponent(labelsRebondBandes_[i]).addComponent(champsRebondBandes_[i], javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)).addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED).addComponent(slidersRebondBandes_[i], javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)));
		}

		panneauDePanneaux_.setLayout(new GridLayout(4, 2, 5, 5));
		panneauCommun_.setLayout(new GridLayout((int) Math.ceil((ModificateurProprieteNoeud.NOMBRE_BANDES + 2) / 2.0), 2));
		for (int i = 0; i < ModificateurProprieteNoeud.NOMBRE_BANDES; ++i)
		{
			panneauCommun_.add(panelsRebondBandes_[i]);
		}

		add(panneauDePanneaux_);

		sliderEchelle_.addChangeListener(new javax.swing.event.ChangeListener() {
			@Override
			public void stateChanged(final javax.swing.event.ChangeEvent evt)
			{
				sliderEchelle_StateChanged(evt);
			}
		});

		sliderFriction_.addChangeListener(new javax.swing.event.ChangeListener() {
			@Override
			public void stateChanged(final javax.swing.event.ChangeEvent evt)
			{
				sliderFriction_StateChanged(evt);
			}
		});

		sliderRebond_.addChangeListener(new javax.swing.event.ChangeListener() {
			@Override
			public void stateChanged(final javax.swing.event.ChangeEvent evt)
			{
				sliderRebond_StateChanged(evt);
			}
		});

		sliderAcceleration_.addChangeListener(new javax.swing.event.ChangeListener() {
			@Override
			public void stateChanged(final javax.swing.event.ChangeEvent evt)
			{
				sliderAcceleration_StateChanged(evt);
			}
		});

		sliderRotation_.addChangeListener(new javax.swing.event.ChangeListener() {
			@Override
			public void stateChanged(final javax.swing.event.ChangeEvent evt)
			{
				sliderRotation_StateChanged(evt);
			}
		});

		champEchelle_.addActionListener(new java.awt.event.ActionListener() {
			@Override
			public void actionPerformed(final java.awt.event.ActionEvent evt)
			{
				champEchelle_ActionPerformed(evt);
			}
		});
		champFriction_.addActionListener(new java.awt.event.ActionListener() {
			@Override
			public void actionPerformed(final java.awt.event.ActionEvent evt)
			{

				champFriction_ActionPerformed(evt);
			}
		});
		champRotation_.addActionListener(new java.awt.event.ActionListener() {
			@Override
			public void actionPerformed(final java.awt.event.ActionEvent evt)
			{
				champRotation_ActionPerformed(evt);
			}
		});
		champRebond_.addActionListener(new java.awt.event.ActionListener() {
			@Override
			public void actionPerformed(final java.awt.event.ActionEvent evt)
			{
				champRebond_ActionPerformed(evt);
			}
		});
		champAcceleration_.addActionListener(new java.awt.event.ActionListener() {
			@Override
			public void actionPerformed(final java.awt.event.ActionEvent evt)
			{
				champAcceleration_ActionPerformed(evt);
			}
		});
		champPositionX_.addActionListener(new java.awt.event.ActionListener() {
			@Override
			public void actionPerformed(final java.awt.event.ActionEvent evt)
			{
				champPositionX_ActionPerformed(evt);
			}
		});
		champPositionY_.addActionListener(new java.awt.event.ActionListener() {
			@Override
			public void actionPerformed(final java.awt.event.ActionEvent evt)
			{
				champPositionY_ActionPerformed(evt);
			}
		});
		champLargeurZoneEdition_.addActionListener(new java.awt.event.ActionListener() {
			@Override
			public void actionPerformed(final java.awt.event.ActionEvent evt)
			{
				champLargeurZoneEdition_ActionPerformed(evt);
			}
		});
		champLongueurZoneEdition_.addActionListener(new java.awt.event.ActionListener() {
			@Override
			public void actionPerformed(final java.awt.event.ActionEvent evt)
			{
				champLongueurZoneEdition_ActionPerformed(evt);
			}
		});

		for (int i = 0; i < ModificateurProprieteNoeud.NOMBRE_BANDES; ++i)
		{
			final int j = i;
			champsRebondBandes_[i].addActionListener(new java.awt.event.ActionListener() {
				@Override
				public void actionPerformed(final java.awt.event.ActionEvent evt)
				{
					champsRebondBandes_ActionPerformed(evt, j);
				}
			});

			slidersRebondBandes_[i].addChangeListener(new javax.swing.event.ChangeListener() {
				@Override
				public void stateChanged(final javax.swing.event.ChangeEvent evt)
				{
					slidersRebondBandes_StateChanged(evt, j);
				}
			});
		}

		pack();

	}

	private void champEchelle_ActionPerformed(final java.awt.event.ActionEvent evt)
	{
		// Force le vérificateur d'entrée à vérifier le champ.
		modifierEchelle(obtenirEchelle());
	}

	private void champRotation_ActionPerformed(final java.awt.event.ActionEvent evt)
	{
		// Force le vérificateur d'entrée à vérifier le champ.
		modifierRotation(obtenirRotation());
	}

	private void champRebond_ActionPerformed(final java.awt.event.ActionEvent evt)
	{
		// Force le vérificateur d'entrée à vérifier le champ.
		modifierRebond(obtenirCoefRebond());
	}

	private void champAcceleration_ActionPerformed(final java.awt.event.ActionEvent evt)
	{
		// Force le vérificateur d'entrée à vérifier le champ.
		modifierAcceleration(obtenirCoefAcceleration());
	}

	private void champFriction_ActionPerformed(final java.awt.event.ActionEvent evt)
	{
		// Force le vérificateur d'entrée à vérifier le champ.
		modifierFriction(obtenirCoefFriction());
	}

	private void champPositionX_ActionPerformed(final java.awt.event.ActionEvent evt)
	{
		// Force le vérificateur d'entrée à vérifier le champ.
		modifierPositionX(obtenirPositionX());
	}

	private void champPositionY_ActionPerformed(final java.awt.event.ActionEvent evt)
	{
		// Force le vérificateur d'entrée à vérifier le champ.
		modifierPositionY(obtenirPositionY());
	}

	private void champLargeurZoneEdition_ActionPerformed(final java.awt.event.ActionEvent evt)
	{
		// Force le vérificateur d'entrée à vérifier le champ.
		modifierLargeurZoneEdition(obtenirLargeurZoneEdition());
	}

	private void champLongueurZoneEdition_ActionPerformed(final java.awt.event.ActionEvent evt)
	{
		// Force le vérificateur d'entrée à vérifier le champ.
		modifierLongueurZoneEdition(obtenirLongueurZoneEdition());
	}

	private void champsRebondBandes_ActionPerformed(final java.awt.event.ActionEvent evt, final int index)
	{
		// Force le vérificateur d'entrée à vérifier le champ.
		modifierRebondBande(obtenirRebondBande(index), index);
	}

	private void sliderEchelle_StateChanged(final javax.swing.event.ChangeEvent evt)
	{
		int nouvelleValeurInt = sliderEchelle_.getValue();
		double nouv;
		if (nouvelleValeurInt <= 50)
		{
			nouv = nouvelleValeurInt / 50.0;
		}
		else
		{
			nouv = ((nouvelleValeurInt - 50.0) * 9.0 / 50.0) + 1.0;
		}
		nouv = Math.round(nouv * 100.0) / 100.0;
		modifierEchelle(nouv);
	}

	private void sliderAcceleration_StateChanged(final javax.swing.event.ChangeEvent evt)
	{
		int nouvelleValeurInt = sliderAcceleration_.getValue();
		double nouv;
		if (nouvelleValeurInt <= 50)
		{
			nouv = nouvelleValeurInt / 50.0;
		}
		else
		{
			nouv = ((nouvelleValeurInt - 50.0) * 9.0 / 50.0) + 1.0;
		}
		nouv = Math.round(nouv * 100.0) / 100.0;
		modifierAcceleration(nouv);
	}

	private void sliderFriction_StateChanged(final javax.swing.event.ChangeEvent evt)
	{
		int nouvelleValeurInt = sliderFriction_.getValue() / 10;
		modifierFriction(nouvelleValeurInt);
	}

	private void sliderRebond_StateChanged(final javax.swing.event.ChangeEvent evt)
	{
		int nouvelleValeurInt = sliderRebond_.getValue();
		double nouv;
		if (nouvelleValeurInt <= 50)
		{
			nouv = nouvelleValeurInt / 50.0;
		}
		else
		{
			nouv = ((nouvelleValeurInt - 50.0) * 9.0 / 50.0) + 1.0;
		}
		nouv = Math.round(nouv * 100.0) / 100.0;
		modifierRebond(nouv);
	}

	private void sliderRotation_StateChanged(final javax.swing.event.ChangeEvent evt)
	{
		modifierRotation(sliderRotation_.getValue());
	}

	private void slidersRebondBandes_StateChanged(final javax.swing.event.ChangeEvent evt, final int index)
	{
		int nouvelleValeurInt = slidersRebondBandes_[index].getValue();
		double nouv;
		if (nouvelleValeurInt <= 50)
		{
			nouv = nouvelleValeurInt / 50.0;
		}
		else
		{
			nouv = ((nouvelleValeurInt - 50.0) * 9.0 / 50.0) + 1.0;
		}
		nouv = Math.round(nouv * 100.0) / 100.0;
		modifierRebondBande(nouv, index);
	}

	private void boutonAppliquer_ActionPerformed(final java.awt.event.ActionEvent evt)
	{
		/*
		 * new ModificateurProprieteNoeud(obtenirPositionX(),
		 * obtenirPositionY(), obtenirCoefFriction(), obtenirCoefRebond(),
		 * obtenirCoefAcceleration(), obtenirEchelle(), obtenirRotation(),
		 * obtenirLargeurZoneEdition(),
		 * obtenirLongueurZoneEdition()).envoyerModificateurPropriete();
		 */
		new ModificateurProprieteNoeud(obtenirPositionX(), obtenirPositionY(), obtenirCoefFriction(), obtenirCoefRebond(), obtenirCoefAcceleration(), obtenirEchelle(), obtenirRotation(), obtenirLargeurZoneEdition(), obtenirLongueurZoneEdition(), obtenirCoefsRebondBandes()).envoyerModificateurPropriete();
		setVisible(false);
	}

	private double obtenirPositionX()
	{
		Object o = champPositionX_.getValue();
		if (o instanceof Long)
		{
			long l = (Long) (o);
			return l;
		}
		else
		{
			return (Double) (o);
		}

	}

	private double obtenirPositionY()
	{
		Object o = champPositionY_.getValue();
		if (o instanceof Long)
		{
			long l = (Long) (o);
			return l;
		}
		else
		{
			return (Double) (o);
		}
	}

	private double obtenirCoefFriction()
	{
		Object o = champFriction_.getValue();
		if (o instanceof Long)
		{
			long l = (Long) (o);
			return l;
		}
		else
		{
			return (Double) (o);
		}
	}

	private double obtenirCoefRebond()
	{
		Object o = champRebond_.getValue();
		if (o instanceof Long)
		{
			long l = (Long) (o);
			return l;
		}
		else
		{
			return (Double) (o);
		}
	}

	private double obtenirCoefAcceleration()
	{
		Object o = champAcceleration_.getValue();
		if (o instanceof Long)
		{
			long l = (Long) (o);
			return l;
		}
		else
		{
			return (Double) (o);
		}
	}

	private double obtenirEchelle()
	{
		Object o = champEchelle_.getValue();
		if (o instanceof Long)
		{
			long l = (Long) (o);
			return l;
		}
		else
		{
			return (Double) (o);
		}
	}

	private double obtenirLargeurZoneEdition()
	{
		Object o = champLargeurZoneEdition_.getValue();
		if (o instanceof Long)
		{
			long l = (Long) (o);
			return l;
		}
		else
		{
			return (Double) (o);
		}
	}

	private double obtenirLongueurZoneEdition()
	{
		Object o = champLongueurZoneEdition_.getValue();
		if (o instanceof Long)
		{
			long l = (Long) (o);
			return l;
		}
		else
		{
			return (Double) (o);
		}
	}

	private int obtenirRotation()
	{
		Object o = champRotation_.getValue();
		if (o instanceof Long)
		{
			long l = (Long) (o);
			return (int) l;
		}
		else
		{
			return (Integer) (o);
		}
	}

	private double obtenirRebondBande(final int index)
	{
		Object o = champsRebondBandes_[index].getValue();
		if (o instanceof Long)
		{
			long l = (Long) (o);
			return l;
		}
		else
		{
			return (Double) (o);
		}
	}

	private double[] obtenirCoefsRebondBandes()
	{
		double[] coefsRebondBandes = new double[ModificateurProprieteNoeud.NOMBRE_BANDES];
		for (int i = 0; i < ModificateurProprieteNoeud.NOMBRE_BANDES; ++i)
		{
			Object o = champsRebondBandes_[i].getValue();
			if (o instanceof Long)
			{
				long l = (Long) (o);
				coefsRebondBandes[i] = l;
			}
			else
			{
				coefsRebondBandes[i] = (Double) (o);
			}
		}
		return coefsRebondBandes;
	}

	private void boutonAnnuler_ActionPerformed(final java.awt.event.ActionEvent evt)
	{
		setVisible(false);
		sliderAcceleration_.setValue(50);
		sliderFriction_.setValue(50);
		sliderRotation_.setValue(50);
		sliderEchelle_.setValue(50);
		sliderRebond_.setValue(50);
		for (Panneaux p : Panneaux.values())
		{
			proprieteChangee_[p.ordinal()] = false;
		}
	}

	private void modifierFriction(final double nouvelleValeur)
	{
		sliderFriction_.setValue((int) nouvelleValeur * 10);
		champFriction_.setValue(nouvelleValeur);
		champFriction_.setBackground(Color.WHITE);
		proprieteChangee_[Panneaux.FRICTION.ordinal()] = true;
		champFriction_.setFocusable(false);
		champFriction_.setFocusable(true);
		champFriction_.requestFocus();
	}

	private void modifierEchelle(final double nouvelleValeur)
	{
		sliderEchelle_.setValue(convertirIntervalle(nouvelleValeur));
		champEchelle_.setValue(nouvelleValeur);
		proprieteChangee_[Panneaux.ECHELLE.ordinal()] = true;
		champEchelle_.requestFocus();
		champEchelle_.setFocusable(false);
		champEchelle_.setFocusable(true);
	}

	private void modifierLargeurZoneEdition(final double nouvelleValeur)
	{
		// sliderEchelle_.setValue(convertirIntervalle(nouvelleValeur));
		champLargeurZoneEdition_.setValue(nouvelleValeur);
		proprieteChangee_[Panneaux.ZONE_EDITION.ordinal()] = true;
		champLargeurZoneEdition_.requestFocus();
		champLargeurZoneEdition_.setFocusable(false);
		champLargeurZoneEdition_.setFocusable(true);
	}

	private void modifierLongueurZoneEdition(final double nouvelleValeur)
	{
		// sliderEchelle_.setValue(convertirIntervalle(nouvelleValeur));
		champLongueurZoneEdition_.setValue(nouvelleValeur);
		proprieteChangee_[Panneaux.ZONE_EDITION.ordinal()] = true;
		champLongueurZoneEdition_.requestFocus();
		champLongueurZoneEdition_.setFocusable(false);
		champLongueurZoneEdition_.setFocusable(true);

	}

	private void modifierAcceleration(final double nouvelleValeur)
	{
		champAcceleration_.setValue(nouvelleValeur);
		sliderAcceleration_.setValue(convertirIntervalle(nouvelleValeur));
		proprieteChangee_[Panneaux.ACCELERATION.ordinal()] = true;
		champAcceleration_.requestFocus();
		champAcceleration_.setFocusable(false);
		champAcceleration_.setFocusable(true);

	}

	private void modifierRebond(final double nouvelleValeur)
	{
		sliderRebond_.setValue(convertirIntervalle(nouvelleValeur));
		champRebond_.setValue(nouvelleValeur);
		proprieteChangee_[Panneaux.REBOND.ordinal()] = true;
		champRebond_.requestFocus();
		champRebond_.setFocusable(false);
		champRebond_.setFocusable(true);

	}

	private void modifierPositionX(final double nouvelleValeur)
	{
		champPositionX_.setValue(nouvelleValeur);
		proprieteChangee_[Panneaux.POSITION.ordinal()] = true;
		champPositionX_.requestFocus();
		champPositionX_.setFocusable(false);
		champPositionX_.setFocusable(true);
	}

	private void modifierPositionY(final double nouvelleValeur)
	{
		champPositionY_.setValue(nouvelleValeur);
		proprieteChangee_[Panneaux.POSITION.ordinal()] = true;
		champPositionY_.requestFocus();
		champPositionY_.setFocusable(false);
		champPositionY_.setFocusable(true);

	}

	private void modifierRotation(final int nouvelleValeur)
	{
		champRotation_.setValue(nouvelleValeur);
		sliderRotation_.setValue(nouvelleValeur);
		champRotation_.setBackground(Color.WHITE);
		proprieteChangee_[Panneaux.ROTATION.ordinal()] = true;
		champRotation_.requestFocus();
		champRotation_.setFocusable(false);
		champRotation_.setFocusable(true);

	}

	private void modifierRebondBande(final double nouvelleValeur, final int index)
	{
		slidersRebondBandes_[index].setValue(convertirIntervalle(nouvelleValeur));
		champsRebondBandes_[index].setValue(nouvelleValeur);
		proprieteChangee_[Panneaux.REBOND_BANDES.ordinal()] = true;
		champsRebondBandes_[index].requestFocus();
		champsRebondBandes_[index].setFocusable(false);
		champsRebondBandes_[index].setFocusable(true);
	}

	public void toutAfficher()
	{
		for (Panneaux p : Panneaux.values())
		{
			modifierAffichage(p, true);
		}
	}

	public void toutCacher()
	{
		for (Panneaux p : Panneaux.values())
		{
			modifierAffichage(p, false);
		}
	}

	public void modifierAffichage(final Panneaux quelPanneau, final boolean afficher)
	{
		switch (quelPanneau)
		{
			case ACCELERATION:
				if (afficher)
				{
					panneauDePanneaux_.add(panelAcceleration_);
				}
				else
				{
					panneauDePanneaux_.remove(panelAcceleration_);
				}
				break;
			case FRICTION:
				if (afficher)
				{
					panneauCommun_.add(panelFriction_);
				}
				else
				{
					panneauCommun_.remove(panelFriction_);
				}
				break;
			case POSITION:
				if (afficher)
				{
					panneauDePanneaux_.add(panelPosition_);
				}
				else
				{
					panneauDePanneaux_.remove(panelPosition_);
				}
				break;
			case ROTATION:
				if (afficher)
				{
					panneauDePanneaux_.add(panelRotation_);
				}
				else
				{
					panneauDePanneaux_.remove(panelRotation_);
				}
				break;
			case ECHELLE:
				if (afficher)
				{
					panneauDePanneaux_.add(panelEchelle_);
				}
				else
				{
					panneauDePanneaux_.remove(panelEchelle_);
				}
				panelEchelle_.setVisible(afficher);
				break;
			case REBOND:
				if (afficher)
				{
					panneauDePanneaux_.add(panelRebond_);
				}
				else
				{
					panneauDePanneaux_.remove(panelRebond_);
				}
				break;
			case ZONE_EDITION:
				if (afficher)
				{
					panneauCommun_.add(panelZoneEdition_);
				}
				else
				{
					panneauCommun_.remove(panelZoneEdition_);
				}
				break;
			case REBOND_BANDES:
				if (afficher)
				{
					add(panneauCommun_, BorderLayout.NORTH);
				}
				else
				{
					remove(panneauCommun_);
				}
				break;
			default:
				break;
		}
		panneauDePanneaux_.setLayout(new GridLayout((int) Math.ceil(panneauDePanneaux_.getComponentCount() / 2.0), 2));
		pack();
	}

	private int convertirIntervalle(final double valeurAConvertir)
	{
		if (valeurAConvertir <= 1.0)
		{
			return (int) (valeurAConvertir * 50.0);
		}
		else
		{
			return (int) (((valeurAConvertir - 1) * 50.0 / 9.0) + 50.0);
		}
	}

	// Variables declaration
	private final JFrame						fenetreImageBandes_;
	private VerificateurEntreePosition			verificateurEntreePosition_;
	private final boolean[]						proprieteChangee_;
	private javax.swing.JButton					boutonAnnuler_;
	private javax.swing.JButton					boutonAppliquer_;
	public javax.swing.JFormattedTextField		champAcceleration_;
	public javax.swing.JFormattedTextField		champEchelle_;
	public javax.swing.JFormattedTextField		champFriction_;
	public javax.swing.JFormattedTextField		champPositionX_;
	public javax.swing.JFormattedTextField		champPositionY_;
	public javax.swing.JFormattedTextField		champRebond_;
	public javax.swing.JFormattedTextField		champRotation_;
	public javax.swing.JFormattedTextField		champLargeurZoneEdition_;
	public javax.swing.JFormattedTextField		champLongueurZoneEdition_;
	public javax.swing.JFormattedTextField[]	champsRebondBandes_;
	private javax.swing.JLabel					labelPositionX_;
	private javax.swing.JLabel					labelPositionY_;
	private javax.swing.JLabel					labelLargeurZoneEdition_;
	private javax.swing.JLabel					labelLongueurZoneEdition_;
	private javax.swing.JLabel					labelZoneEdition_;
	private javax.swing.JLabel					labelAcceleration_;
	private javax.swing.JLabel					labelEchelle_;
	private javax.swing.JLabel					labelFriction_;
	private javax.swing.JLabel					labelPosition_;
	private javax.swing.JLabel					labelRebond_;
	private javax.swing.JLabel					labelRotation_;
	private javax.swing.JLabel					labelTitre_;
	private javax.swing.JLabel[]				labelsRebondBandes_;
	private javax.swing.JPanel					panelAcceleration_;
	private javax.swing.JPanel					panelEchelle_;
	private javax.swing.JPanel					panelFriction_;
	private javax.swing.JPanel					panelPosition_;
	private javax.swing.JPanel					panelRebond_;
	private javax.swing.JPanel					panelRotation_;
	private javax.swing.JPanel					panelZoneEdition_;
	private javax.swing.JPanel[]				panelsRebondBandes_;
	private javax.swing.JPanel					panneauDePanneaux_;
	private javax.swing.JPanel					panneauCommun_;
	private javax.swing.JPanel					panneauDeBoutons_;
	private javax.swing.JSeparator				separateurPosition_;
	private javax.swing.JSeparator				separateurZoneEdition_;
	private javax.swing.JSeparator				separateurFriction_;
	private javax.swing.JSlider					sliderAcceleration_;
	private javax.swing.JSlider					sliderEchelle_;
	private javax.swing.JSlider					sliderFriction_;
	private javax.swing.JSlider					sliderRebond_;
	private javax.swing.JSlider					sliderRotation_;
	private javax.swing.JSlider[]				slidersRebondBandes_;
	// End of variables declaration
}
