<?php
/**
 * Class that manages actions specifically related to the Hockedu system itself.
 * 
 * Actions that affect the Hockedu logic.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 19/02/2013
 * @package Hockedu
 */
 
 
/**
 * Common
 *
 * Manages actions specifically related to the Hockedu logic itself.
 * @package Common
 */
class Common
{          
    /**
     * Holds a single instance of the object.
     * @access private
     * @static
     * @var object|Common
     */
    private static $instance;
    
    private $db;
	
	private $currentAuthenticationData;
    
    /**
     * Constructor that initializes the database.
     */
    private function __construct( $dbDsn, $dbOptions )
    {
        $this->db = MDB2::singleton( $dbDsn, $dbOptions );
        $this->db->setFetchMode(MDB2_FETCHMODE_ASSOC);
        $this->db->query("SET NAMES 'utf8'");
		$this->currentAuthenticationData = false;
    }
    
    private function recordLogin( $userId, $username, $password, $subsite, $success, &$Website )
    {
        $loginData['subsite'] = $subsite;
        $loginData['success'] = $success;
        
        if( !empty( $username ) )
        {
            $loginData['username'] = $username;
        }
        
        if( !empty( $password ) )
        {
            $loginData['password'] = $password;
        }

        // Record the login in the database.
        $sql = 'INSERT INTO %s
                ( %s, %s, %s, %s, %s )
                VALUES( %d, %d, %s, INET_ATON( %s ), %d )';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'comm_log' ),
                        
                        $this->db->quoteIdentifier( 'user_id' ),
                        $this->db->quoteIdentifier( 'type' ),
                        $this->db->quoteIdentifier( 'data' ),
                        $this->db->quoteIdentifier( 'ipaddress' ),
                        $this->db->quoteIdentifier( 'time' ),
                        
                        $this->db->quote( $userId, 'integer' ),
                        $this->db->quote( CommonLogTypes::Login, 'integer' ),
                        $this->db->quote( serialize( $loginData ), 'text' ),
                        $this->db->quote( $Website->getIPAddress(), 'text' ),
                        $this->db->quote( time(), 'integer' )
                       );
        $this->db->query( $sql );    
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
            self::$instance = new $object( $website->getSetting( 'CommonDBDSN' ),  $website->getSetting( 'CommonDBOptions' ) );
        }
        
        return self::$instance;
    }
    
    public function &getDb()
    {
        return $this->db;
    }
    
    public function registerUser( $username, $password, $email )
    {
        // Generate the salt.
        $characterList = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%?&*()_+";
        $salt = '';

        for( $i = 0; $i < 10; $i++ )
        {
            $salt .= $characterList{ mt_rand( 0, strlen( $characterList ) ) };
        }
        
        $sql = 'INSERT INTO %s
                ( %s, %s, %s, %s, %s )
                VALUES( %s, SHA1( CONCAT( SHA1( %s ), SHA1( %s ) ) ), %s, %s, %d )';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'users' ),
                        
                        $this->db->quoteIdentifier( 'username' ),
                        $this->db->quoteIdentifier( 'password' ),
                        $this->db->quoteIdentifier( 'password_salt' ),
                        $this->db->quoteIdentifier( 'email' ),                                        
                        $this->db->quoteIdentifier( 'registration_time' ),                                      
                        
                        $this->db->quote( $username, 'text' ),
                        $this->db->quote( $password, 'text' ),
                        $this->db->quote( $salt, 'text' ),
                        $this->db->quote( $salt, 'text' ),
                        $this->db->quote( $email, 'text' ),
                        $this->db->quote( time() , 'integer' )
                       );
        
        $this->db->query( $sql );
    }
    
    /**
     * Indicates whether a user with the specified username exists.
     * @access public
     * @param string $username Username of the user to check for.
     * @return boolean Whether the specified user exists or not.
     */
    public function userExists( $username )
    { 
        $sql = 'SELECT %s
                FROM %s 
                WHERE %s=%s';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'id' ),
                        
                        $this->db->quoteIdentifier( 'users'),
                        
                        $this->db->quoteIdentifier( 'username' ),
                        $this->db->quote( $username, 'text' )
                       );
        $this->db->setLimit( 1 );
        $userInformation = $this->db->queryOne( $sql );
        
        if( empty( $userInformation ) )
        {
            // User doesn't seem to exist.
            return false;
        }
        
        return true;
    }
    
    public function getUserList()
    { 
        $sql = 'SELECT %s, %s
                FROM %s 
                ORDER BY %s DESC';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'username' ),
                        $this->db->quoteIdentifier( 'registration_time' ),
                        
                        $this->db->quoteIdentifier( 'users'),
                        
                        $this->db->quoteIdentifier( 'registration_time' )
                       );
        $userInformation = $this->db->queryAll( $sql );
        
        return $userInformation;
    }
    
    /**
     * Indicates whether a user with the specified email exists.
     * @access public
     * @param string $email Email of the user to check for.
     * @return boolean Whether the specified email exists or not.
     */
    public function emailExists( $email )
    { 
        $sql = 'SELECT %s
                FROM %s 
                WHERE %s=%s';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'id' ),
                        
                        $this->db->quoteIdentifier( 'users'),
                        
                        $this->db->quoteIdentifier( 'email' ),
                        $this->db->quote( $email, 'text' )
                       );
        $this->db->setLimit( 1 );
        $userInformation = $this->db->queryOne( $sql );
        
        if( empty( $userInformation ) )
        {
            // User doesn't seem to exist.
            return false;
        }
        
        return true;
    }
       
    public function login( $username, $password, $isRemote = false )
    {
        if( $isRemote )
        {
            //Password is already hashed.
            $passwordFormat = '%s';
        }
        else
        {
            $passwordFormat = 'SHA1( %s )';
        }
        
        // Check if a user with the provided credentials exist.
        $sql = 'SELECT %s, %s, %s
                FROM %s 
                WHERE %s=%s AND %s=SHA1( CONCAT( ' . $passwordFormat . ', SHA1( %s ) ) )';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'id' ),
                        $this->db->quoteIdentifier( 'username' ),
                        $this->db->quoteIdentifier( 'email' ),
                        
                        $this->db->quoteIdentifier( 'users'),
                        
                        $this->db->quoteIdentifier( 'username' ),
                        $this->db->quote( $username, 'text' ),
                        
                        $this->db->quoteIdentifier( 'password' ),
                        $this->db->quote( $password, 'text' ),
                        $this->db->quoteIdentifier( 'password_salt' )
                       );
        $this->db->setLimit( 1 );
        $userInformation = $this->db->queryRow( $sql );
                
        if( !empty( $userInformation ) )
        {
            //$this->recordLogin( $userInformation['id'], null, null, $subsite, true, $Website );
            
            // Update the last_login_time and last_login_ip fields for that user.
            /*
            $ipAddress = $Website->getIpAddress();
            
            $sql = 'UPDATE %s 
                    SET %s=%d AND %s=INET_ATON( %s )
                    WHERE %s=%s';
            $sql = sprintf( $sql,
                            $this->db->quoteIdentifier( 'comm_users' ),
                            
                            $this->db->quoteIdentifier( 'last_login_time' ),
                            $this->db->quote( time(), 'integer' ),
                            
                            $this->db->quoteIdentifier( 'last_login_ip' ),
                            $this->db->quote( $ipAddress, 'text' ),

                            $this->db->quoteIdentifier( 'id' ),
                            $this->db->quote( $userInformation['id'], 'integer' )
                           );
            $this->db->setLimit( 1 );
            $this->db->exec( $sql );
            */
            
            // Valid login.
            if( $isRemote )
            {
                // Manage remote client authentication data.
				$this->currentAuthenticationData = $this->getValidAuthenticationData( $userInformation['id'] );				
				
				if( $this->currentAuthenticationData === false )
				{
					// No valid authentication data was found, create it.
					$this->createAuthenticationData( $userInformation['id'] );
					
					$this->currentAuthenticationData = $this->getValidAuthenticationData( $userInformation['id'] );
				}
            }
            else
            {
                // Proceed with setting the right session variables.
                $userSessionInformation = array(
                                        'id' => $userInformation['id'],
                                        'username' => $userInformation['username'],
                                        'email' => $userInformation['email']
                                    );
                $_SESSION['userInformation'] = $userSessionInformation;
            }
            
            return true;
        }
        else
        {
            // Could not login, record the login data.
            //$this->recordLogin( 0, $username, $password, $subsite, false, $Website );
            
            // Login information error.
            return false;
        }
    }
	
	public function getCurrentAuthenticationData()
	{
		return $this->currentAuthenticationData;
	}
	
	public function getValidAuthenticationData( $userId )
	{
		$sql = 'SELECT %s, %s
				FROM %s 
				WHERE %s=%d AND %s>=%d
				ORDER BY %s DESC';
		$sql = sprintf( $sql,
						$this->db->quoteIdentifier( 'key' ),
						$this->db->quoteIdentifier( 'expiration' ),
						
						$this->db->quoteIdentifier( 'remote_authentication'),
						
						$this->db->quoteIdentifier( 'id_user' ),
						$this->db->quote( $userId, 'integer' ),
						
						$this->db->quoteIdentifier( 'expiration' ),
						$this->db->quote( time(), 'integer' ),
						
						$this->db->quoteIdentifier( 'creation' )
					   );
		$this->db->setLimit( 1 );
		$userInformation = $this->db->queryRow( $sql );
		
		if( !empty( $userInformation ) )
		{
			return $userInformation;
		}
		
		return false;
	}
	
	public function createAuthenticationData( $userId )
	{
		// Ticket creation time.
		$creation = time();
		$expiration = $creation + 3600;
		
		$sql = 'INSERT INTO %s
                ( %s, %s, %s, %s )
                VALUES( UUID(), %d, %d, %d )';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'remote_authentication' ),
                        
                        $this->db->quoteIdentifier( 'key' ),
                        $this->db->quoteIdentifier( 'id_user' ),
                        $this->db->quoteIdentifier( 'creation' ),
                        $this->db->quoteIdentifier( 'expiration' ),                                                                           
                        
                        $this->db->quote( $userId , 'integer' ),
                        $this->db->quote( $creation , 'integer' ),
                        $this->db->quote( $expiration, 'integer' )
                       );
        
        $this->db->query( $sql );
	}
    
    public function logout()
    {
        unset( $_SESSION['userInformation'] );
    }
}

?>
