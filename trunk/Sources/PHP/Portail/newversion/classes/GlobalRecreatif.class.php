<?php
/**
 * Class that manages actions specifically related to Global Récréatif.
 * 
 * Various actions that are specific to Global Récréatif.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 30/08/2012
 * @package Cloud
 */
 
 
/**
 * GlobalRecreatif
 *
 * Manages actions specifically related to Global Récréatif.
 * @package GlobalRecreatif
 */
class GlobalRecreatif
{          
    /**
     * Holds a single instance of the object.
     * @access private
     * @static
     * @var object|Common
     */
    private static $instance;
    
    private $db;
    
    /**
     * Constructor that initializes the database.
     */
    private function __construct( $dbDsn, $dbOptions )
    {
        $this->db = MDB2::singleton( $dbDsn, $dbOptions );
        $this->db->setFetchMode(MDB2_FETCHMODE_ASSOC);
        $this->db->query("SET NAMES 'utf8'");
    }
    
    /**
     * Returns the instance for the singleton pattern.
     * @access public
     * @return object|Common
     */
    public static function getInstance()
    {
        if( !isset( self::$instance ) )
        {
            $object = __CLASS__;
            $website = Website::getInstance();
            self::$instance = new $object( $website->getSetting( 'GlobalRecreatifDBDSN' ), $website->getSetting( 'GlobalRecreatifDBOptions' ) );
        }
        
        return self::$instance;
    }
    
    /**
     * Adds a new client to the client database.
     * @access public
     * @return integer The ID of the newly created client.
     */
    public function addNewClient()
    {        
        if( empty( $_POST['apptSuite'] ) )
		{
			$_POST['apptSuite'] = null;
		}
		
		$sql = 'INSERT INTO %s
                ( %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s )
                VALUES( %d, %d, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s )';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'gr_clients' ),
                        
                        $this->db->quoteIdentifier( 'date_ouverture' ),
                        $this->db->quoteIdentifier( 'type' ),
                        $this->db->quoteIdentifier( 'nom' ),
                        $this->db->quoteIdentifier( 'prenom' ),
                        $this->db->quoteIdentifier( 'no_permis_conduire' ),
                        $this->db->quoteIdentifier( 'nom_entreprise' ),
                        $this->db->quoteIdentifier( 'adresse' ),
                        $this->db->quoteIdentifier( 'appt_suite' ),
                        $this->db->quoteIdentifier( 'ville' ),
                        $this->db->quoteIdentifier( 'code_postal' ),
                        $this->db->quoteIdentifier( 'province_etat' ),
                        $this->db->quoteIdentifier( 'pays' ),
                        $this->db->quoteIdentifier( 'email' ),
                        $this->db->quoteIdentifier( 'tel_domicile' ),
                        $this->db->quoteIdentifier( 'tel_cell' ),
                        $this->db->quoteIdentifier( 'tel_autre' ),
                        $this->db->quoteIdentifier( 'notes' ),
                        
                        $this->db->quote( time(), 'integer' ),
                        $this->db->quote( $_POST['clientType'], 'integer' ),
                        $this->db->quote( $_POST['lastName'], 'text' ),
                        $this->db->quote( $_POST['firstName'], 'text' ),
                        $this->db->quote( $_POST['drivingLicence'], 'text' ),
                        $this->db->quote( $_POST['enterpriseName'], 'text' ),
                        $this->db->quote( $_POST['address'], 'text' ),
                        $this->db->quote( $_POST['apptSuite'], 'text' ),
                        $this->db->quote( $_POST['city'], 'text' ),
                        $this->db->quote( $_POST['postalCode'], 'text' ),
                        $this->db->quote( $_POST['province'], 'text' ),
                        $this->db->quote( $_POST['country'], 'text' ),
                        $this->db->quote( $_POST['email'], 'text' ),
                        $this->db->quote( $_POST['telHome'], 'text' ),
                        $this->db->quote( $_POST['telCell'], 'text' ),
                        $this->db->quote( $_POST['telOther'], 'text' ),
                        $this->db->quote( $_POST['notes'], 'text' )
                       );
        $this->db->query( $sql ); 

        return $this->db->lastInsertID( 'gr_clients' );
    }
	
	/**
     * Adds a new contract to the contract database. Creates a pre invoice.
     * @access public
     * @return integer The ID of the newly created pre invoice.
     */
    public function addNewContract()
    {        
        // Basic item description.
		$contractItemDescription = 'Entreposage ' . $_POST['numberOfDays'] . ' jours - ';
		
		// Do we need to create a new lot?
		$lotId = intval( $_POST['lotId'] );
		if( $lotId === 0 )
		{
			// Yes.
			$sql = 'INSERT INTO %s
                ( %s, %s, %s, %s, %s, %s, %s )
                VALUES( %d, %s, %d, %s, %s, %d, %d )';
			$sql = sprintf( $sql,
							$this->db->quoteIdentifier( 'gr_lots' ),
							
							$this->db->quoteIdentifier( 'id_client' ),
							$this->db->quoteIdentifier( 'description' ),
							$this->db->quoteIdentifier( 'annee' ),
							$this->db->quoteIdentifier( 'no_serie' ),
							$this->db->quoteIdentifier( 'no_plaque' ),
							$this->db->quoteIdentifier( 'longueur' ),
							$this->db->quoteIdentifier( 'largeur' ),
							
							$this->db->quote( $_POST['clientId'], 'integer' ),
							$this->db->quote( $_POST['lotDescription'], 'text' ),
							$this->db->quote( $_POST['lotYear'], 'integer' ),
							$this->db->quote( $_POST['lotSerialNumber'], 'text' ),
							$this->db->quote( $_POST['lotLicencePlate'], 'text' ),
							$this->db->quote( $_POST['lotLength'], 'integer' ),
							$this->db->quote( $_POST['lotWidth'], 'integer' )
						   );
			$this->db->query( $sql ); 
			
			$lotId = $this->db->lastInsertID( 'gr_lots' );
			
			$contractItemDescription .= $_POST['lotDescription'] . ' ' . $_POST['lotYear'] . ' ' . $_POST['lotLength'] . '\' X ' . $_POST['lotWidth'] . '\'';
		}
		else
		{
			// No, but we need to get the details from the existing lot.
			$lotInformation = $this->getLotData( $lotId );
			
			$contractItemDescription .= $lotInformation['description'] . ' ' . $lotInformation['annee'] . ' ' . $lotInformation['longueur'] . '\' X ' . $lotInformation['largeur'] . '\'';
		}
		
		// Create the contract.
		$sql = 'INSERT INTO %s
                ( %s, %s, %s, %s, %s )
                VALUES( %d, %d, %d, %d, %d )';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'gr_contrats' ),
                        
                        $this->db->quoteIdentifier( 'id_client' ),
                        $this->db->quoteIdentifier( 'id_lot' ),
                        $this->db->quoteIdentifier( 'check_in_time' ),
                        $this->db->quoteIdentifier( 'nb_jours' ),
                        $this->db->quoteIdentifier( 'date_creation' ),
                        
                        $this->db->quote( $_POST['clientId'], 'integer' ),
                        $this->db->quote( $lotId, 'integer' ),
                        $this->db->quote( $_POST['startDate'], 'integer' ),
                        $this->db->quote( $_POST['numberOfDays'], 'integer' ),
						$this->db->quote( time(), 'integer' )
                       );
        $this->db->query( $sql ); 

        $contractId = $this->db->lastInsertID( 'gr_contrats' );
		
		// Create the pre invoice.
		$basketItems = array();

		$basketItems[] = array();
		$basketItems[0]['description'] = $contractItemDescription;
		$basketItems[0]['contractid'] = $contractId;
		$basketItems[0]['modifiable'] = false;
		$basketItems[0]['baseprice'] = floatval( $_POST['totalPrice'] );
		$basketItems[0]['totalprice'] = floatval( $_POST['totalPrice'] );
		$basketItems[0]['quantity'] = 1;
		
		$sql = 'INSERT INTO %s
                ( %s, %s, %s, %s )
                VALUES( %d, %s, %s, %d )';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'gr_factures' ),
                        
                        $this->db->quoteIdentifier( 'id_client' ),
                        $this->db->quoteIdentifier( 'items' ),
                        $this->db->quoteIdentifier( 'soustotal' ),
                        $this->db->quoteIdentifier( 'date' ),
                        
                        $this->db->quote( $_POST['clientId'], 'integer' ),
						$this->db->quote( serialize( $basketItems ), 'text' ),
						$this->db->quote( $_POST['totalPrice'], 'float' ),
						$this->db->quote( time(), 'integer' )
                       );
        $this->db->query( $sql ); 

        return $this->db->lastInsertID( 'gr_factures' );
    }
    
    /**
     * Indicates whether a client with the specified ID exists.
     * @access public
     * @param integer $clientId ID of the possible client to check for.
     * @return boolean Whether the specified client exists or not.
     */
    public function clientExists( $clientId )
    { 
        $sql = 'SELECT %s
                FROM %s 
                WHERE %s=%d';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'id_client' ),
                        
                        $this->db->quoteIdentifier( 'gr_clients'),
                        
                        $this->db->quoteIdentifier( 'id_client' ),
                        $this->db->quote( $clientId, 'integer' )
                       );
        $this->db->setLimit( 1 );
        $clientInformation = $this->db->queryOne( $sql );
        
        if( empty( $clientInformation ) )
        {
            // Client doesn't seem to exist.
            return false;
        }
        
        return true;
    }
    
    /**
     * Indicates whether a contract with the specified ID exists.
     * @access public
     * @param integer $contractId ID of the possible contract to check for.
     * @return boolean Whether the specified contract exists or not.
     */
    public function contractExists( $contractId )
    {         
        $sql = 'SELECT %s
                FROM %s 
                WHERE %s=%d';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'id_contrat' ),
                        
                        $this->db->quoteIdentifier( 'gr_contrats'),
                        
                        $this->db->quoteIdentifier( 'id_contrat' ),
                        $this->db->quote( $contractId, 'integer' )
                       );
        $this->db->setLimit( 1 );
        $contractInformation = $this->db->queryOne( $sql );
        
        if( empty( $contractInformation ) )
        {
            // Contract doesn't seem to exist.
            return false;
        }
        
        return true;
    }
	
	/**
     * Indicates whether a pre invoice with the specified ID exists.
     * @access public
     * @param integer $preInvoiceId ID of the possible pre invoice to check for.
     * @return boolean Whether the specified pre invoice exists or not.
     */
    public function preInvoiceExists( $preInvoiceId )
    {         
        $sql = 'SELECT %s
                FROM %s 
                WHERE %s=%d
				AND %s=%d';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'id_facture' ),
                        
                        $this->db->quoteIdentifier( 'gr_factures'),
                        
                        $this->db->quoteIdentifier( 'statut' ),
                        $this->db->quote( GlobalRecreatifInvoiceStatus::Prepared, 'integer' ),
						
                        $this->db->quoteIdentifier( 'id_facture' ),
                        $this->db->quote( $preInvoiceId, 'integer' )
                       );
        $this->db->setLimit( 1 );
        $preInvoiceInformation = $this->db->queryOne( $sql );
        
        if( empty( $preInvoiceInformation ) )
        {
            // Pre invoice doesn't seem to exist.
            return false;
        }
        
        return true;
    }
	
	/**
     * Returns the data associated with an invoice (or a pre invoice).
     * @access public
     * @param integer $invoiceId ID of invoice to get data from.
     * @return array Data pertaining to the invoice.
     */
    public function getInvoiceData( $invoiceId )
    { 
        $sql = 'SELECT %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s
                FROM %s 
				LEFT JOIN %s ON %s=%s
                WHERE %s=%d';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'id_facture' ),
                        $this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'id_client' ),
                        $this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'items' ),
                        $this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'soustotal' ),
                        $this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'tax1' ),
                        $this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'tax1_name' ),
                        $this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'tax1_percent' ),
                        $this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'tax1_overtaxes' ),                        $this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'tax2' ),
                        $this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'tax2_name' ),
                        $this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'tax2_percent' ),
                        $this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'tax2_overtaxes' ),
                        $this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'total' ),
                        $this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'date' ),
                        $this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'statut' ),
						
                        $this->db->quoteIdentifier('gr_clients') . '.' . $this->db->quoteIdentifier( 'type' ),
                        $this->db->quoteIdentifier('gr_clients') . '.' . $this->db->quoteIdentifier( 'nom' ),
                        $this->db->quoteIdentifier('gr_clients') . '.' . $this->db->quoteIdentifier( 'prenom' ),
                        $this->db->quoteIdentifier('gr_clients') . '.' . $this->db->quoteIdentifier( 'nom_entreprise' ),
                        $this->db->quoteIdentifier('gr_clients') . '.' . $this->db->quoteIdentifier( 'adresse' ),
                        $this->db->quoteIdentifier('gr_clients') . '.' . $this->db->quoteIdentifier( 'appt_suite' ),
                        $this->db->quoteIdentifier('gr_clients') . '.' . $this->db->quoteIdentifier( 'ville' ),
                        $this->db->quoteIdentifier('gr_clients') . '.' . $this->db->quoteIdentifier( 'code_postal' ),
                        $this->db->quoteIdentifier('gr_clients') . '.' . $this->db->quoteIdentifier( 'province_etat' ),
                        $this->db->quoteIdentifier('gr_clients') . '.' . $this->db->quoteIdentifier( 'pays' ),
                        $this->db->quoteIdentifier('gr_clients') . '.' . $this->db->quoteIdentifier( 'tel_domicile' ),
                        $this->db->quoteIdentifier('gr_clients') . '.' . $this->db->quoteIdentifier( 'tel_cell' ),
						
						$this->db->quoteIdentifier('gr_factures'),
						
						$this->db->quoteIdentifier('gr_clients'),
						$this->db->quoteIdentifier('gr_clients') . '.' . $this->db->quoteIdentifier( 'id_client' ),
						$this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'id_client' ),
                        
                        $this->db->quoteIdentifier('gr_factures') . '.' . $this->db->quoteIdentifier( 'id_facture' ),
                        $this->db->quote( $invoiceId, 'integer' )
                       );
        $this->db->setLimit( 1 );
        $invoiceInformation = $this->db->queryRow( $sql );
		
		$invoiceInformation['tax1_percentcalc'] = $invoiceInformation['tax1_percent'] / 100;
		$invoiceInformation['tax2_percentcalc'] = $invoiceInformation['tax2_percent'] / 100;

		$invoiceInformation['items'] = unserialize( $invoiceInformation['items'] );
		
		$website = Website::getInstance();
		
		$invoiceInformation['province_etat'] = $website->formatProvinceState( $invoiceInformation['province_etat'] );
		
		if( isset( $invoiceInformation['tel_domicile'] ) )
		{
			$invoiceInformation['tel_domicile'] = $website->formatTelephoneNumber( $invoiceInformation['tel_domicile'] );
		}
		elseif( isset( $invoiceInformation['tel_cell'] ) )
		{
			$invoiceInformation['tel_cell'] = $website->formatTelephoneNumber( $invoiceInformation['tel_cell'] );
		}
		
		$invoiceInformation['code_postal'] = $website->formatPostalCode( $invoiceInformation['code_postal'], $invoiceInformation['pays'] );
        
        return $invoiceInformation;
    }
    
    /**
     * Returns the data associated with a client.
     * @access public
     * @param integer $clientId ID of client to get data from.
     * @return array Data pertaining to the client.
     */
    public function getClientData( $clientId )
    { 
        $sql = 'SELECT %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s
                FROM %s 
                WHERE %s=%d';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'type' ),
                        $this->db->quoteIdentifier( 'nom' ),
                        $this->db->quoteIdentifier( 'prenom' ),
                        $this->db->quoteIdentifier( 'no_permis_conduire' ),
                        $this->db->quoteIdentifier( 'nom_entreprise' ),
                        $this->db->quoteIdentifier( 'adresse' ),
                        $this->db->quoteIdentifier( 'appt_suite' ),
                        $this->db->quoteIdentifier( 'ville' ),
                        $this->db->quoteIdentifier( 'code_postal' ),
                        $this->db->quoteIdentifier( 'province_etat' ),
                        $this->db->quoteIdentifier( 'pays' ),
                        $this->db->quoteIdentifier( 'email' ),
                        $this->db->quoteIdentifier( 'tel_domicile' ),
                        $this->db->quoteIdentifier( 'tel_cell' ),
                        $this->db->quoteIdentifier( 'tel_autre' ),
                        $this->db->quoteIdentifier( 'notes' ),
                        
                        $this->db->quoteIdentifier( 'gr_clients'),
                        
                        $this->db->quoteIdentifier( 'id_client' ),
                        $this->db->quote( $clientId, 'integer' )
                       );
        $this->db->setLimit( 1 );
        $clientInformation = $this->db->queryRow( $sql );
        
        return $clientInformation;
    }
	
    
    /**
     * Returns the data associated with a contract.
     * @access public
     * @param integer $contractId ID of contract to get data from.
     * @return array Data pertaining to the contract.
     */
    public function getContractData( $contractId )
    { 
        $sql = 'SELECT %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, UNIX_TIMESTAMP( DATE_ADD( FROM_UNIXTIME(%s), INTERVAL %s DAY ) ) AS anticipated_checkout, DATEDIFF( DATE_ADD( FROM_UNIXTIME(%s), INTERVAL %s DAY ), CURDATE() ) AS day_difference
                FROM %s 
                LEFT JOIN %s ON %s=%s
                LEFT JOIN %s ON %s=%s
                WHERE %s=%d';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'id_contrat' ),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'id_client' ),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'id_lot' ),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'id_facture' ),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'check_in_time' ),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'check_out_time' ),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'nb_jours' ),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'date_creation' ),
                        $this->db->quoteIdentifier('gr_lots') . '.' . $this->db->quoteIdentifier( 'description' ),
                        $this->db->quoteIdentifier('gr_lots') . '.' . $this->db->quoteIdentifier( 'annee' ),
                        $this->db->quoteIdentifier('gr_lots') . '.' . $this->db->quoteIdentifier( 'no_serie' ),
                        $this->db->quoteIdentifier('gr_lots') . '.' . $this->db->quoteIdentifier( 'no_plaque' ),
                        $this->db->quoteIdentifier('gr_lots') . '.' . $this->db->quoteIdentifier( 'longueur' ),
                        $this->db->quoteIdentifier('gr_lots') . '.' . $this->db->quoteIdentifier( 'largeur' ),
                        $this->db->quoteIdentifier('gr_clients') . '.' . $this->db->quoteIdentifier( 'nom' ),
                        $this->db->quoteIdentifier('gr_clients') . '.' . $this->db->quoteIdentifier( 'prenom' ),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'check_in_time' ),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'nb_jours' ),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'check_in_time' ),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'nb_jours' ),
                        
                        $this->db->quoteIdentifier('gr_contrats'),
                        
                        $this->db->quoteIdentifier('gr_lots'),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier('id_lot'),
                        $this->db->quoteIdentifier('gr_lots') . '.' . $this->db->quoteIdentifier('id_lot'),
                        
                        $this->db->quoteIdentifier('gr_clients'),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier('id_client'),
                        $this->db->quoteIdentifier('gr_clients') . '.' . $this->db->quoteIdentifier('id_client'),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'id_contrat' ),
                        $this->db->quote( $contractId, 'integer' )
                        
                       );
        $this->db->setLimit( 1 );
        $contractInformation = $this->db->queryRow( $sql );
        
        return $contractInformation;
    }
    
    /**
     * Returns the status of a contract.
     * @access public
     * @param integer $startTime Timestamp of the start of the contract.
     * @param integer $duration Duration of the contract, in days.
     * @param integer $endTime Timestamp of when the contract was closed.
     * @return array Calculated checkout time, status of the contract, status in text form and difference in days since the end/from the end/from the beginning.
     */
    public function getContractStatus( $startTime, $dayDifference, $endTime = 0, $nbDays = 0 )
    { 
        $returnArray['daydifference'] = $dayDifference;
        $returnArray['daydifferenceAbs'] = abs( $dayDifference );
        
        if( intval( $endTime ) > 0 )
        {
            $returnArray['statuscode'] = GlobalRecreatifContractStatus::Over;
            $returnArray['statustext'] = htmlentities( 'Terminé' );
        }
        else if( $dayDifference < 0 )
        {
            $returnArray['statuscode'] = GlobalRecreatifContractStatus::Expired;
            $returnArray['statustext'] = 'En retard';
        }
        else if( unixtojd( $startTime ) > unixtojd() )
        {
            $returnArray['statuscode'] = GlobalRecreatifContractStatus::NotYetStarted;
            $returnArray['statustext'] = 'En attente';
            $returnArray['daydifference'] = $dayDifference - $nbDays;
        }
        else
        {
            $returnArray['statuscode'] = GlobalRecreatifContractStatus::OnGoing;
            $returnArray['statustext'] = 'Actif';
        }
        
        return $returnArray;
    }
	
	/**
     * Returns the data associated with a lot.
     * @access public
     * @param integer $lotId ID of lot to get data from.
     * @return array Data pertaining to the lot.
     */
    public function getLotData( $lotId )
    { 
        $sql = 'SELECT %s, %s, %s, %s
                FROM %s 
                WHERE %s=%d';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'description' ),
                        $this->db->quoteIdentifier( 'annee' ),
                        $this->db->quoteIdentifier( 'longueur' ),
                        $this->db->quoteIdentifier( 'largeur' ),
                        
                        $this->db->quoteIdentifier( 'gr_lots'),
                        
                        $this->db->quoteIdentifier( 'id_lot' ),
                        $this->db->quote( $lotId, 'integer' )
                       );
        $this->db->setLimit( 1 );
        $lotInformation = $this->db->queryRow( $sql );
        
        return $lotInformation;
    }
    
    /**
     * Returns the data associated with a client.
     * @access public
     * @param integer $clientId ID of client to get data from.
     * @return array List of storage items for the specified client.
     */
    public function getStorageList( $clientId )
    { 
        $sql = 'SELECT %s, %s, %s, %s, %s, %s, %s, UNIX_TIMESTAMP( DATE_ADD( FROM_UNIXTIME(%s), INTERVAL %s DAY ) ) AS anticipated_checkout, DATEDIFF( DATE_ADD( FROM_UNIXTIME(%s), INTERVAL %s DAY ), CURDATE() ) AS day_difference
                FROM 
                (
                    (
                        SELECT *
                        FROM %s 
                        WHERE %s=%d
                        AND %s IS NULL
                        AND DATE_ADD( FROM_UNIXTIME(%s), INTERVAL %s DAY )<CURDATE()
                        ORDER BY (%s+%s*86400) ASC
                        LIMIT 18446744073709551615
                    )
                    UNION
                    (
                        SELECT *
                        FROM %s 
                        WHERE %s=%d
                        AND %s IS NULL
                        AND DATE( FROM_UNIXTIME(%s) )<=CURDATE()
                        AND DATE_ADD( DATE( FROM_UNIXTIME(%s) ), INTERVAL %s DAY )>=CURDATE()
                        ORDER BY (%s+%s*86400) ASC
                        LIMIT 18446744073709551615
                    )
                    UNION
                    (
                        SELECT *
                        FROM %s 
                        WHERE %s=%d
                        AND %s IS NULL
                        AND DATE( FROM_UNIXTIME(%s) )>CURDATE()
                        ORDER BY %s ASC
                        LIMIT 18446744073709551615
                    )
                    UNION
                    (    
                        SELECT *
                        FROM %s 
                        WHERE %s=%d
                        AND %s IS NOT NULL
                        ORDER BY %s DESC
                        LIMIT 18446744073709551615
                    )
                ) %s
                LEFT JOIN %s ON %s=%s';
        $sql = sprintf( $sql,                        
                        $this->db->quoteIdentifier('contrats') . '.' . $this->db->quoteIdentifier( 'id_contrat' ),
                        $this->db->quoteIdentifier('contrats') . '.' . $this->db->quoteIdentifier( 'id_lot' ),
                        $this->db->quoteIdentifier('contrats') . '.' . $this->db->quoteIdentifier( 'check_in_time' ),
                        $this->db->quoteIdentifier('contrats') . '.' . $this->db->quoteIdentifier( 'check_out_time' ),
                        $this->db->quoteIdentifier('contrats') . '.' . $this->db->quoteIdentifier( 'nb_jours' ),
                        $this->db->quoteIdentifier('gr_lots') . '.' . $this->db->quoteIdentifier( 'description' ),
                        $this->db->quoteIdentifier('gr_lots') . '.' . $this->db->quoteIdentifier( 'annee' ),
                        $this->db->quoteIdentifier('contrats') . '.' . $this->db->quoteIdentifier( 'check_in_time' ),
                        $this->db->quoteIdentifier('contrats') . '.' . $this->db->quoteIdentifier( 'nb_jours' ),
                        $this->db->quoteIdentifier('contrats') . '.' . $this->db->quoteIdentifier( 'check_in_time' ),
                        $this->db->quoteIdentifier('contrats') . '.' . $this->db->quoteIdentifier( 'nb_jours' ),
                        
                        // Late contracts.
                        
                        $this->db->quoteIdentifier('gr_contrats'),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'id_client' ),
                        $this->db->quote( $clientId, 'integer' ),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'check_out_time' ),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'check_in_time' ),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'nb_jours' ),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'check_in_time' ),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'nb_jours' ),
                        
                        // Active contracts.
                        
                        $this->db->quoteIdentifier('gr_contrats'),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'id_client' ),
                        $this->db->quote( $clientId, 'integer' ),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'check_out_time' ),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'check_in_time' ),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'check_in_time' ),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'nb_jours' ),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'check_in_time' ),
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'nb_jours' ),
                        
                        // Contracts not yet started.
                        
                        $this->db->quoteIdentifier('gr_contrats'),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'id_client' ),
                        $this->db->quote( $clientId, 'integer' ),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'check_out_time' ),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'check_in_time' ),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'check_in_time' ),
                        
                        // Terminated contracts.
                        
                         $this->db->quoteIdentifier('gr_contrats'),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'id_client' ),
                        $this->db->quote( $clientId, 'integer' ),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'check_out_time' ),
                        
                        $this->db->quoteIdentifier('gr_contrats') . '.' . $this->db->quoteIdentifier( 'check_out_time' ),
                        
                        // Join vehicle information.
                        
                        $this->db->quoteIdentifier('contrats'),
                        
                        $this->db->quoteIdentifier('gr_lots'),
                        $this->db->quoteIdentifier('contrats') . '.' . $this->db->quoteIdentifier('id_lot'),
                        $this->db->quoteIdentifier('gr_lots') . '.' . $this->db->quoteIdentifier('id_lot')
                        
                       );

        $storageList = $this->db->queryAll( $sql );
        
        foreach( $storageList as $key => $value )
        {           
            $contractStatus = $this->getContractStatus( $value['check_in_time'], $value['day_difference'], $value['check_out_time'] );
            
            $storageList[$key]['status']['code'] = $contractStatus['statuscode'];
            $storageList[$key]['status']['text'] = $contractStatus['statustext'];
        }
        
        return $storageList;
    }
	
    /**
     * Returns the data associated with a client.
     * @access public
     * @param integer $clientId ID of client to get data from.
     * @return array List of lots associated with the specified client.
     */
    public function getLotsList( $clientId )
    { 
        $sql = 'SELECT %s, %s, %s, %s, %s
                FROM %s 
                WHERE %s=%d
				ORDER BY %s DESC';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'id_lot' ),
                        $this->db->quoteIdentifier( 'description' ),
                        $this->db->quoteIdentifier( 'annee' ),
                        $this->db->quoteIdentifier( 'longueur' ),
                        $this->db->quoteIdentifier( 'largeur' ),
                        
                        $this->db->quoteIdentifier( 'gr_lots'),
                        
                        $this->db->quoteIdentifier( 'id_client' ),
                        $this->db->quote( $clientId, 'integer' ),
						
						$this->db->quoteIdentifier( 'id_lot' )
                       );
        $lotsList = $this->db->queryAll( $sql );
        
        return $lotsList;
    }
	
	/**
     * Returns the list of available packages.
     * @access public
     * @return array List of packages that are available.
     */
    public function getAvailablePackagesList( )
    { 
        $sql = 'SELECT %s, %s, %s, %s
                FROM %s 
                WHERE %s=%d
				ORDER BY %s, %s';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'id_forfait' ),
                        $this->db->quoteIdentifier( 'description' ),
                        $this->db->quoteIdentifier( 'prix' ),
                        $this->db->quoteIdentifier( 'nb_jours' ),
                        
                        $this->db->quoteIdentifier( 'gr_forfaits'),
                        
                        $this->db->quoteIdentifier( 'actif' ),
                        $this->db->quote( 1, 'integer' ),
						
						$this->db->quoteIdentifier( 'ordre' ),
						$this->db->quoteIdentifier( 'id_forfait' )
                       );
        $packagesList = $this->db->queryAll( $sql );
        
        return $packagesList;
    }
	
	/**
     * Indicates whether an invoice with the specified ID exists.
     * @access public
     * @param integer $invoiceId ID of the possible invoice to check for.
     * @return boolean Whether the specified invoice exists or not.
     */
    public function invoiceExists( $invoiceId )
    {         
        $sql = 'SELECT %s
                FROM %s 
                WHERE %s!=%d
				AND %s=%d';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'id_facture' ),
                        
                        $this->db->quoteIdentifier( 'gr_factures'),
                        
                        $this->db->quoteIdentifier( 'statut' ),
                        $this->db->quote( GlobalRecreatifInvoiceStatus::Prepared, 'integer' ),
						
                        $this->db->quoteIdentifier( 'id_facture' ),
                        $this->db->quote( $invoiceId, 'integer' )
                       );
        $this->db->setLimit( 1 );
        $invoiceInformation = $this->db->queryOne( $sql );
        
        if( empty( $invoiceInformation ) )
        {
            // Invoice doesn't seem to exist.
            return false;
        }
        
        return true;
    }
	
	/**
     * Returns the data associated with a client.
     * @access public
     * @param integer $invoiceId ID of the invoice to update.
     * @param array $basketItems All the items of the updated invoice, will overwrite current items.
	 * @param float $subTotal Subtotal of the updated invoice.
	 * @param float $tax1 Total amount of the first tax for the updated invoice.
	 * @param float $tax2 Total amount of the second tax for the updated invoice.
	 * @param float $total Total of the updated invoice.
     * @param boolean $validInvoice Completes the invoice if true, cancels it otherwise.
     */
    public function updateInvoice( $invoiceId, $basketItems, $subTotal, $tax1, $tax2, $total, $validInvoice )
    { 		
		if( $validInvoice )
		{
			$invoiceStatus = GlobalRecreatifInvoiceStatus::Paid;
		}
		else
		{
			$invoiceStatus = GlobalRecreatifInvoiceStatus::Canceled;
		}
		
		$sql = 'UPDATE %s
                SET %s=%s, %s=%s, %s=%s, %s=%s, %s=%s, %s=%d
                WHERE %s=%d';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'gr_factures' ),
						
                        $this->db->quoteIdentifier( 'items' ),
						$this->db->quote( serialize( $basketItems ), 'text' ),
						
						$this->db->quoteIdentifier( 'soustotal' ),
						$this->db->quote( floatval( $subTotal ), 'text' ),	
						
						$this->db->quoteIdentifier( 'tax1' ),
						$this->db->quote( floatval( $tax1 ), 'text' ),	
						
						$this->db->quoteIdentifier( 'tax2' ),
						$this->db->quote( floatval( $tax2 ), 'text' ),	
						
						$this->db->quoteIdentifier( 'total' ),
						$this->db->quote( floatval( $total ), 'text' ),
						
                        $this->db->quoteIdentifier( 'statut' ),
                        $this->db->quote( $invoiceStatus, 'integer' ),
                        
                        $this->db->quoteIdentifier( 'id_facture' ),
                        $this->db->quote( intval( $invoiceId ), 'integer' )
                       );
        $this->db->exec($sql);
	}
    
    public function &getDb()
    {
        return $this->db;
    }
}

?>
