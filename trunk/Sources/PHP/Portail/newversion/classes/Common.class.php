<?php
/**
 * Class that manages actions specifically related to the management system itself.
 * 
 * Various actions that have some effect over all of the management system and are not
 * subsite-specific.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 14/07/2011
 * @package Cloud
 */
 
 
/**
 * Common
 *
 * Manages actions specifically related to the management system itself.
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
    
    /**
     * Constructor that initializes the database.
     */
    private function __construct( $dbDsn, $dbOptions )
    {
        $this->db = MDB2::singleton( $dbDsn, $dbOptions );
        $this->db->setFetchMode(MDB2_FETCHMODE_ASSOC);
        $this->db->query("SET NAMES 'utf8'");
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
    
        
    public function getIpType( $ip )
    {
        // Check if an IP is in a blacklist, a whitelist or neither. (Unknown)
        $sql = 'SELECT %s
                FROM %s 
                WHERE %s=INET_ATON( %s ) AND %s=%d';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'type' ),
                        
                        $this->db->quoteIdentifier( 'comm_ipaddresses'),
                        
                        $this->db->quoteIdentifier( 'ipaddress' ),
                        $this->db->quote( $ip, 'text' ),
                        
                        $this->db->quoteIdentifier( 'statut' ),
                        $this->db->quote( IpStatus::Enabled, 'integer' )
                       );
        $this->db->setLimit( 1 );
        $ipInformation = $this->db->queryRow( $sql );
        
        if( empty( $ipInformation ) )
        {
            return IpType::Unknown;
        }
        else
        {
            // IP found, is it whitelisted or blacklisted?
            switch( $ipInformation['type'] )
            {
                case IpType::Whitelisted:
                    return IpType::Whitelisted;
                    
                case IpType::Blacklisted:
                default:
                    return IpType::Blacklisted;
            }
        }
    }
    
    public function activateIp( $activationCode )
    {
        // Validate parameters.
        if( empty( $activationCode ) )
        {
            return false;
        }
        
        // Check if that IP exists and needs to be activated.
        $sql = 'SELECT %s
                FROM %s 
                WHERE %s=%s AND %s=%d';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'id' ),
                        
                        $this->db->quoteIdentifier( 'comm_ipaddresses' ),
                        
                        $this->db->quoteIdentifier( 'activation' ),
                        $this->db->quote( $activationCode, 'text' ),
                        
                        $this->db->quoteIdentifier( 'statut' ),
                        $this->db->quote( IpStatus::Pending, 'integer' )
                       );
        $this->db->setLimit( 1 );
        $ipInformation = $this->db->queryRow( $sql );
        
        if( !empty( $ipInformation ) )
        {
            // Update the database to indicate that the IP is confirmed.
            $sql = 'UPDATE %s 
                    SET %s=%s
                    WHERE %s=%d';
            $sql = sprintf( $sql,
                            $this->db->quoteIdentifier( 'comm_ipaddresses' ),
                            
                            $this->db->quoteIdentifier( 'statut' ),
                            $this->db->quote( IpStatus::Enabled, 'integer' ),
                            
                            $this->db->quoteIdentifier( 'id' ),
                            $this->db->quote( $ipInformation['id'], 'integer' )
                           );
            $this->db->setLimit( 1 );
            $this->db->exec( $sql );
            
            return true;
        }
        
        return false;
    }
    
    public function addIp( $ip, $ipType )
    {
        // Validate parameters.
        if( empty( $ip ) || ( ( $ipType !== IpType::Whitelisted ) && ( $ipType !== IpType::Blacklisted ) ) )
        {
            return false;
        }
        
        // Check if that IP is in a blacklist or a whitelist.
        $sql = 'SELECT %s, %s
                FROM %s 
                WHERE %s=INET_ATON( %s ) AND %s=%d AND %s!=%d';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'type' ),
                        $this->db->quoteIdentifier( 'activation' ),
                        
                        $this->db->quoteIdentifier( 'comm_ipaddresses' ),
                        
                        $this->db->quoteIdentifier( 'ipaddress' ),
                        $this->db->quote( $ip, 'text' ),
                        
                        $this->db->quoteIdentifier( 'type' ),
                        $this->db->quote( $ipType, 'integer' ),
                        
                        $this->db->quoteIdentifier( 'statut' ),
                        $this->db->quote( IpStatus::Disabled, 'integer' )
                       );
        $this->db->setLimit( 1 );
        $ipInformation = $this->db->queryRow( $sql );
        
        if( empty( $ipInformation ) )
        {
            // The IP didn't exist among valid entries, generate a unique ID and insert it.
            $activationCode = sha1( microtime() . $_SESSION['requestIp']['ticket'] . uniqid( '', true ) );
            
            // Add the IP to the database.
            $sql = 'INSERT INTO %s
                    ( %s, %s, %s, %s, %s )
                    VALUES( INET_ATON( %s ), %d, %s, %d, %d )';
            $sql = sprintf( $sql,
                            $this->db->quoteIdentifier( 'comm_ipaddresses' ),
                            
                            $this->db->quoteIdentifier( 'ipaddress' ),
                            $this->db->quoteIdentifier( 'type' ),
                            $this->db->quoteIdentifier( 'activation' ),
                            $this->db->quoteIdentifier( 'expiration' ),                                        
                            $this->db->quoteIdentifier( 'statut' ),                                        
                            
                            $this->db->quote( $ip, 'text' ),
                            $this->db->quote( $ipType, 'integer' ),
                            $this->db->quote( $activationCode, 'text' ),
                            $this->db->quote( time() + ( 3600 * 24 ), 'integer' ),
                            $this->db->quote( IpStatus::Pending, 'integer' )
                           );
            
            $this->db->query( $sql );
            
            return $activationCode;
        }
        else
        {
            // The IP already exists.
            return $ipInformation['activation'];
        }
    }
    
    public function login( $username, $password, $subsite, &$Website )
    {
        // Check if a user with the provided credentials exist.
        $sql = 'SELECT %s, %s, %s, %s, %s, %s
                FROM %s 
                WHERE %s=%s AND %s=SHA1( CONCAT( SHA1( %s ), SHA1( %s ) ) ) AND %s=%s';
        $sql = sprintf( $sql,
                        $this->db->quoteIdentifier( 'id' ),
                        $this->db->quoteIdentifier( 'prenom' ),
                        $this->db->quoteIdentifier( 'nom' ),
                        $this->db->quoteIdentifier( 'username' ),
                        $this->db->quoteIdentifier( 'email' ),
                        $this->db->quoteIdentifier( 'permissions' ),
                        
                        $this->db->quoteIdentifier( 'comm_users'),
                        
                        $this->db->quoteIdentifier( 'username' ),
                        $this->db->quote( $username, 'text' ),
                        
                        $this->db->quoteIdentifier( 'password' ),
                        $this->db->quoteIdentifier( 'password_salt' ),
                        $this->db->quote( $password, 'text' ),
                        
                        $this->db->quoteIdentifier( 'statut' ),
                        $this->db->quote( UsernameStatus::Enabled, 'integer' )
                       );
        $this->db->setLimit( 1 );
        $userInformation = $this->db->queryRow( $sql );
                
        if( !empty( $userInformation ) )
        {
            $permissions = unserialize( $userInformation['permissions'] );
            
            $allowedSubsites = $Website->extractAllowedSubsites( $permissions );

            // If the user isn't allowed to connect to the website he requested.
            if( !in_array( $subsite, $allowedSubsites, true ) )
            {
                $this->recordLogin( $userInformation['id'], null, null, $subsite, false, $Website );
                
                return LoginResponse::NoSubsite;
            }
            
            $this->recordLogin( $userInformation['id'], null, null, $subsite, true, $Website );
            
            // Check whether the IP the user is logging in from is in the white/blacklist.
            $ipAddress = $Website->getIpAddress();
            
            switch( $this->getIpType( $ipAddress ) )
            {
                case IpType::Unknown:
                    // New IP, ask user to request IP ticket and activate it session-side.
                    $_SESSION['requestIp']['ticket'] = $ipAddress;
                    $_SESSION['requestIp']['email'] = $userInformation['email'];
                    $_SESSION['requestIp']['name'] = $userInformation['prenom'] . ' ' . $userInformation['nom'];
                    $_SESSION['requestIp']['subsite'] = $subsite;
                    return LoginResponse::NewIp;
                    
                case IpType::Whitelisted:
                    break;
                
                case IpType::Blacklisted:
                default:
                    return LoginResponse::Failure;
            }
            
            // If we got this far, it means the user has a valid IP.
            
            // Update the last_login_time and last_login_ip fields for that user.
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
            
            // Proceed with setting the right session variables.
            $userSessionInformation = array(
                                    'id' => $userInformation['id'],
                                    'prenom' => $userInformation['prenom'],
                                    'nom' => $userInformation['nom'],
                                    'username' => $userInformation['username'],
                                    'email' => $userInformation['email'],
                                    'permissions' => $permissions,
                                    'allowedSubsites' => $allowedSubsites,
                                    'lastSubsite' => $subsite
                                );
            $_SESSION['userInformation'] = $userSessionInformation;
        }
        else
        {
            // Could not login, record the login data.
            $this->recordLogin( 0, $username, $password, $subsite, false, $Website );
            
            // Login information error.
            return LoginResponse::Failure;
        }
        
        return LoginResponse::Success;
    }
    
    public function logout()
    {
        unset( $_SESSION['userInformation'] );
    }
}

?>
