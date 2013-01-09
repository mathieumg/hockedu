package ca.polymtl.inf2990.Panneaux;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.JTextField;
import javax.swing.SwingConstants;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.Etats.GestionnaireEtats;
import ca.polymtl.inf2990.Jeu.JoueurAbstrait;
import ca.polymtl.inf2990.Jeu.JoueurHumain;
import ca.polymtl.inf2990.Jeu.JoueurVirtuel;
import ca.polymtl.inf2990.Jeu.OperationsJoueursJNI;

/**
 * Sous-panneau généré par le visiteurGenererPanneau. C'est le panneau à utiliser dans PanneauGererElement si on a un joueur virtuel.
 * @author Gabriel Couvrette
 *
 */
public class SousPanneauJoueurVirtuel extends PanneauAbstrait {
	
	
	enum ActionBouton
	{
		ACTION_OK, ACTION_RESET, ACTION_CANCEL
	};
	
	
	private JoueurVirtuel joueur_;
	private static final int	EPAISSEUR_SEPARATEUR	= 2;
	
	
	// la barre contenant le nom du joueur
	private JTextField		componentNom_;

	// les deux glissoires contenant la vitesse et le pourcentage de coup
	// reussis par le AI.
	private JSlider			componentVitesse_, componentPrecision_;

	//les boutons.
	private JButton			boutonOk_, boutonReset_, boutonCancel_;
	
	// Couleurs pour la fenetre.
	private final Color		couleurPanneau		= Color.white,
			couleurLignesEtTexte = new Color(0, 0, 0),
			couleurErreur=Color.WHITE;
	
	
	private EcouteurBoutons ecouteur_;
	
public SousPanneauJoueurVirtuel(JoueurVirtuel joueur) {
	joueur_=joueur;
	ecouteur_=new EcouteurBoutons();
	initialiserFenetre();
}


/**
 * Methode qui initialise les valeurs importantes pour la fenetre.
 */
private void initialiserFenetre()
{
	// on place le layout.
	setLayout(new BorderLayout(0, EPAISSEUR_SEPARATEUR));

	
	// creation du panneau du centre
	JPanel panneauDuCentre = new JPanel();
	panneauDuCentre.setLayout(new BorderLayout());

	// creation du champ nom :
	JPanel champNom = new JPanel();
	champNom.setLayout(new FlowLayout(FlowLayout.CENTER));
	JLabel labelnom=new JLabel("Nom : ");
	champNom.add(labelnom);
	componentNom_ = new JTextField(joueur_.obtenirNom());		
	componentNom_.setPreferredSize(new Dimension(167,  30));
	champNom.add(componentNom_);
	
	panneauDuCentre.add(champNom, BorderLayout.NORTH);
	

	// ajout du panneau du centre dans la fenetre.
	add(panneauDuCentre, BorderLayout.CENTER);

	
	// Creation de la zone du bas :
	// ---------------------------------------------------------------------------//
	JPanel zoneDuBas = new JPanel(new BorderLayout(0, EPAISSEUR_SEPARATEUR));

	JPanel panneauSliders = new JPanel(new BorderLayout(0, 0));
	// on cree le champ vitesse
	JPanel champVitesse = new JPanel(new BorderLayout());
	JLabel labelvitesse=new JLabel("Vitesse");
	champVitesse.add(labelvitesse,BorderLayout.NORTH);
	componentVitesse_ = new JSlider(0, 100,joueur_.obtenirVitesse());
	componentVitesse_.setPreferredSize(new Dimension(167, 30));
	champVitesse.add(componentVitesse_, BorderLayout.CENTER);
	panneauSliders.add(champVitesse, BorderLayout.NORTH);

	// on cree le champ precision:
	JPanel champPrecision = new JPanel(new BorderLayout());
	JLabel labelprecision=new JLabel("Précision");
	champPrecision.add(labelprecision, BorderLayout.NORTH);
	componentPrecision_ = new JSlider(0, 100,joueur_.obtenirProbabiliteSucces());
	componentPrecision_.setPreferredSize(new Dimension(167, 30));
	champPrecision.add(componentPrecision_, BorderLayout.CENTER);
	panneauSliders.add(champPrecision, BorderLayout.CENTER);

	zoneDuBas.add(panneauSliders, BorderLayout.CENTER);

	// on ajoute les boutons au bas de la fenetre.
	JPanel panneauBoutons = new JPanel(new FlowLayout(FlowLayout.TRAILING));
	// on crée les boutons
	boutonOk_ = new JButton("Ok");
	panneauBoutons.add(boutonOk_);
	boutonReset_ = new JButton("Défaut");
	panneauBoutons.add(boutonReset_);
	boutonCancel_ = new JButton("Annuler");
	panneauBoutons.add(boutonCancel_);
	zoneDuBas.add(panneauBoutons, BorderLayout.SOUTH);

	// on ajoute la zone du bas dans la fenetre.
	add(zoneDuBas, BorderLayout.SOUTH);

	
	//Place les couleurs des panneaux
	
	
	boutonOk_.setActionCommand(ActionBouton.ACTION_OK.toString());
	boutonOk_.addActionListener(ecouteur_);
	
	boutonReset_.setActionCommand(ActionBouton.ACTION_RESET.toString());
	boutonReset_.addActionListener(ecouteur_);
	
	boutonCancel_.setActionCommand(ActionBouton.ACTION_CANCEL.toString());
	boutonCancel_.addActionListener(ecouteur_);

}



/**
 * Génère un joueur selon les valeurs contenues dans la fenêtre.
 * @return Le joueur créé.
 */
private JoueurAbstrait genererJoueur()
{
	JoueurAbstrait joueur;
	
		joueur=new JoueurVirtuel(componentNom_.getText(), componentVitesse_.getValue(), (componentPrecision_.getMaximum())-componentPrecision_.getValue());
	
	return joueur;
}


/**
 * Vérifie si le nom entré est valide. (En terme de caractères)
 * @param text Le nom à valider.
 * @return La validité du nom.
 */
private boolean nomValide(String text)
{
	if(text.length()==0)
	{
		JOptionPane.showMessageDialog(Fenetre.obtenirInstance(), "Vous devez inscrire un nom!");
		return false;
	}
	
	
	String reg = "[^(a-zA-Z0-9 )]";
	Pattern p = Pattern.compile(reg);
	Matcher m = p.matcher(text);
	if(m.find() || text.charAt(0) == ' ' || text.charAt(text.length() -1 ) == ' ')
	{
		JOptionPane.showMessageDialog(Fenetre.obtenirInstance(), "Les caractères spéciaux et les espaces superflues sont interdits!");
		return false;
	}
	return true;
}



/**
 * Vérifie si la soumission demandée est valide.
 * @return La validité de l'ajout.
 */
private boolean soumissionValide()
{
	//on valide le nom.
	if(!nomValide(componentNom_.getText()))
		return false;
	//on valide la précision
	if(componentPrecision_.getValue()<0||componentPrecision_.getValue()>100)
		return false;
	//on valide la vitesse.
	if(componentVitesse_.getValue()<0||componentVitesse_.getValue()>100)
		return false;
	//toutest ok, on retourne true.
	return true;
}






/**
 * Ecouteur qui s'occupe de gérer les événements sur les boutons de la
 * fenetre.
 * 
 * @author Gabriel Couvrette
 * 
 */
private class EcouteurBoutons implements ActionListener
{
	@Override
	public void actionPerformed(final ActionEvent e)
	{
		ActionBouton actionPerformee = ActionBouton.valueOf(e.getActionCommand());

		switch (actionPerformee)
		{
			case ACTION_OK:
				actionOk();
				break;
			case ACTION_RESET:
				actionReset();
				break;
			case ACTION_CANCEL:
				actionCancel();

		}
	}

	/**
	 * Que faire quand on appuie sur le bouton cancel?
	 */
	private void actionCancel()
	{
		GestionnaireEtats.obtenirInstance().allerEtatPrecedent();
	}

	/**
	 * Que faire quand on pese sur le bouton Défaut?
	 */
	private void actionReset()
	{
		// Reset le nom
		componentNom_.setText(joueur_.obtenirNom());
		// reset les sliders
		componentVitesse_.setValue(50);
		componentPrecision_.setValue(50);
	}

	/**
	 * Que faire quand on fait Ok?
	 */
	private void actionOk()
	{
		if(soumissionValide())
		{
			JoueurAbstrait joueurExistant = OperationsJoueursJNI.obtenirJoueur(componentNom_.getText());
			if(joueurExistant !=null && !joueurExistant.obtenirNom().equals(joueur_.obtenirNom()))
			{
				if(JOptionPane.showOptionDialog(Fenetre.obtenirInstance(), "Ce profil existe déjà! Voulez-vous l'écraser?", "Conflit!", JOptionPane.YES_NO_OPTION, JOptionPane.ERROR_MESSAGE, null, new String[]{"Oui","Non"}, "Non")!=JOptionPane.YES_OPTION)
				{
					return;
				}
			}
				OperationsJoueursJNI.ajouterJoueur(genererJoueur());
				GestionnaireEtats.obtenirInstance().allerEtatPrecedent();
		}
	}
}






@Override
protected void creer() {
	// TODO Auto-generated method stub
	
}


@Override
public void rafraichir() {
	// TODO Auto-generated method stub
	
}
}
