package ca.polymtl.inf2990.Etats;

import ca.polymtl.inf2990.Jeu.CanalRadio;
import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.PanneauGestionCanal;

/**
 * État d'interface lorsqu'on affiche le panneau de création de canal radio.
 * @author Gabriel Couvrette
 *
 */
public class EtatGererCanal extends EtatAbstrait {

	CanalRadio canal_; //le canal à gérer.
	/**
	 * Constructeur qui prend une valeur de canal pour modifier. NULL si on veut créer un nouveau canal.
	 * @param canal Un canal.
	 */
	public EtatGererCanal(CanalRadio canal) {
		super();
		canal_=canal;
		
		// TODO Auto-generated constructor stub
	}
	@Override
	public void initialiser() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void terminer() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mettreAuRepos() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void reveiller() {
		// TODO Auto-generated method stub
		((PanneauGestionCanal)obtenirPanneau()).setCanal(canal_);
		((PanneauGestionCanal)obtenirPanneau()).chargerCanal();
	}

	@Override
	public Etats obtenirNom() {
		// TODO Auto-generated method stub
		return Etats.MODE_CANAL_RADIO;
	}

	@Override
	protected PanneauAbstrait creerPanneau() {
		return new PanneauGestionCanal(canal_);
	}

}
