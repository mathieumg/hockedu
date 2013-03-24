<?php
/**
 * Website management for the Hockedu System.
 * 
 * Tools for the container of the Hockedu system.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 17/02/2013
 * @package Hockedu
 */
 
 
/**
 * Website
 *
 * Manages everything related to the website itself.
 * @package Website
 */
class Website
{          
    /**
     * URL of where the publicly accessible content (images, stylesheets, javascript, etc.) is 
     * stored.
     * @access private
     * @var string
     */
    private $contentURL;
    
    private $displaySite;
    
    /**
     * Name of the file to include.
     * @access private
     * @var string
     */
    private $includeModule;
    
    /**
     * Path where files to include are stored.
     * @access private
     * @var string
     */
    private $includePath;
    
    /**
     * Holds a single instance of the object.
     * @access private
     * @static
     * @var object|Website
     */
    private static $instance;
        
    /**
     * Contains all the modules generated from the query string.
     * @access private
     * @var array
     */
    private $modules;
    
    /**
     * Contains all the settings for the management system.
     * @access private
     * @var array
     */
    private $settings;
    
    private $smarty;
    private static $smartyAssignmentsDone;
    
    /**
     * Contains the path of the template to include.
     * @access private
     * @var string
     */
    private $templateToInclude;    
    
    /**
     * Constructor that calls the {@link initialize} method.
     */
    private function __construct()
    {
        // Initialize the Website.
        $this->initialize();
    }
    
    /**
     * Initialize different modules for the website.
     */
    private function initialize()
    {
        // Load settings from cache.
        $settings = new CacheStore( 'settings' );
        if( $settings->exists() )
        {
            $this->settings = $settings->getCacheData();
        }
        else
        {
            $this->killSite( 'Incapable de charger les paramètres depuis la cache.' );
        }
        
        // Initialize some properties.
        $this->contentURL = 'http://' . $this->settings['ContentSubDomain'] . '.' . $this->settings['MainDomain'] . '/';
        $this->displaySite = true;
        $this->basePath = '/var/www/hockedu.com/';
        $this->includePath = $this->basePath . 'includes/';
        $this->templatesPath = $this->basePath . 'templates/';
        $this->templateToInclude = 'homepage';
        self::$smartyAssignmentsDone = false;
        
        // Generate module list from URL.
        $_GET['query'] = filter_var( $_GET['query'], FILTER_SANITIZE_STRING, FILTER_FLAG_STRIP_HIGH );
        $subModules = explode( '/', $_GET['query'] );
        array_shift( $subModules );
         
        foreach( $subModules as $key => $val )
        {
            if( !empty( $val ) )
            {
                $moduleList[] = $val;
            }
        }

        if( empty( $moduleList ) )
        {
            $moduleList[] = 'homepage';
        }

        $this->modules = $moduleList;
        
        // Initialize Smarty.
        $this->smarty = new Smarty();
        
        $this->smarty->setTemplateDir( $this->templatesPath . 'templates/');
        $this->smarty->setCompileDir( $this->templatesPath . 'templates_c/');
        $this->smarty->setConfigDir( $this->templatesPath . 'configs/');
        $this->smarty->setCacheDir( $this->templatesPath . 'cache/');
        
        // Set locale.
        //setlocale(LC_MONETARY, 'fr_CA.utf8');
        
        // Start sessions.
        session_set_cookie_params( 0, '/', '.hockedu.com' );
        session_start();
    }
    
    /**
     * Returns the instance for the singleton pattern.
     * @access public
     * @return object|Website
     */
    public static function getInstance()
    {
        if( !isset( self::$instance ) )
        {
            $object = __CLASS__;
            self::$instance = new $object;
        }
        
        return self::$instance;
    }
    
    public static function getEmailTemplate()
	{
		$mailMessage = new Email();
        
        $mailMessage->default_charset = 'UTF-8';
        
        $mailMessage->SetEncodedEmailHeader('From', 'no-reply@hockedu.com', 'Hockedu');
        $mailMessage->SetEncodedEmailHeader('Reply-To', 'contact@hockedu.com', 'Hockedu');
        $mailMessage->SetEncodedEmailHeader('Errors-To', 'webmaster@hockedu.com', 'Webmaster');
        
        return $mailMessage;
	}
    
    public function getIpAddress()
	{       
        $realIP = $_SERVER['REMOTE_ADDR'];
        
        if( !empty( $_SERVER['HTTP_CLIENT_IP'] ) )
        {      
            // Shared internet.
            $realIP = $_SERVER['HTTP_CLIENT_IP'];    
        }
        elseif( !empty( $_SERVER['HTTP_X_FORWARDED_FOR'] ) )
        {      
            // Proxy.
            $realIP = $_SERVER['HTTP_X_FORWARDED_FOR'];    
        }

        return $realIP;        
	}
    
        
    public function getModulePath( $moduleName = null )
    {
        $this->assignSmarty();
        
        if( $moduleName !== null )
        {
            return $this->includePath . $moduleName . '.inc.php';
        }
        
        return $this->includePath . $this->includeModule . '.inc.php';
    }

    /**
     * Returns the list of modules generated from the query string.
     * @access public
     * @return array
     */
    public function getModules( $level = -1 )
    {
        if( $level >= 0 )
        {
            if( isset( $this->modules[ $level ] ) )
            {
                return $this->modules[ $level ];
            }
            
            return false;
        }
        else
        {
            return $this->modules;
        }
    }
    
    public function getSetting( $name )
    {
        if( isset( $this->settings[ $name ] ) )
        {       
            return $this->settings[ $name ];
        }
        
        // Not returning false because some settings are booleans.
        return 'Setting "' . $name . '" not found.';
    }
    
    public function &getSmarty()
    {
        return $this->smarty;
    }    
    
    public function setIncludeModule( $name )
    {
        $this->includeModule = $name;
    }     
    
    public function setTemplateToInclude( $name )
    {
        $this->templateToInclude = $name;
    }
    
    /**
     * Redirects to a new page.
     * @access public
     * @param string $address URL of the page to redirect to.
     */
    public function changePage( $address )
	{	
		// Redirect to the new page.
		header('HTTP/1.1 301 Moved Permanently');
		header('Location: http://' . $address);
        
        // Prevent further loading.
        exit;
	}
    
    private function assignSmarty()
    {
        if( !self::$smartyAssignmentsDone )
        {
            $this->smarty->assign( 'CompleteUrl', $this->settings['MainDomain'] );
            
            if( !empty( $_SESSION['userInformation'] ) )
            {
                $this->smarty->assign( 'IsLoggedIn', true );
                $this->smarty->assign( 'UserInformation', $_SESSION['userInformation'] );
            }
            
            $this->smarty->assign( 'ModuleName', $this->modules[0] );
            
            $this->smarty->assign( 'StylesURL', $this->contentURL . 'styles/' );
            
            $this->smarty->assign( 'JavascriptURL', $this->contentURL . 'js/' );
            
            $this->smarty->assign( 'ImagesURL', $this->contentURL . 'img/' );
            
            self::$smartyAssignmentsDone = true;
        }
    }
    
    public function display()
    {
        // If required, display the website.
        if( $this->displaySite === true )
        {
            $this->assignSmarty();
                        
            $this->smarty->assign( 'TemplateToInclude', $this->templateToInclude . '.tpl' );
            
            $this->smarty->display( 'index.tpl' );
        }
    }
     
    public function doNotDisplaySite()
    {
        $this->displaySite = false;
    }
	
	public function formatPostalCode( $rawPostalCode, $rawCountry )
	{
		switch( $rawCountry )
		{
			case 'canada': return strtoupper( substr($rawPostalCode, 0, 3) . ' ' . substr($rawPostalCode, 3, 3) );
			default:
			case 'unitedstates' : return $rawPostalCode;
		}
	}
	
	public function formatProvinceState( $rawProvinceState )
	{
		switch( $rawProvinceState )
		{
            case 'ontario': return 'Ontario';
            case 'alberta': return 'Alberta';
            case 'britishcolumbia': return 'Colombie-Britannique';
            case 'pei': return 'Île-du-prince-Édouard';
            case 'manitoba': return 'Manitoba';
            case 'newbrunswick': return 'Nouveau Brunswick';
            case 'novascotia': return 'Nouvelle-Écosse';
            case 'nunavut': return 'Nunavut';
            case 'saskatchewan': return 'Saskatchewan';
            case 'newfoundland': return 'Terre-Neuve';
            case 'northwestterritories': return 'Territoires du Nord-Ouest';
            case 'yukon': return 'Yukon';
            case 'al': return 'Alabama'; 
            case 'ak': return 'Alaska'; 
            case 'az': return 'Arizona'; 
            case 'ar': return 'Arkansas'; 
            case 'ca': return 'California'; 
            case 'co': return 'Colorado'; 
            case 'ct': return 'Connecticut'; 
            case 'de': return 'Delaware'; 
            case 'dc': return 'District Of Columbia'; 
            case 'fl': return 'Florida'; 
            case 'ga': return 'Georgia'; 
            case 'hi': return 'Hawaii'; 
            case 'id': return 'Idaho'; 
            case 'il': return 'Illinois'; 
            case 'in': return 'Indiana'; 
            case 'ia': return 'Iowa'; 
            case 'ks': return 'Kansas'; 
            case 'ky': return 'Kentucky'; 
            case 'la': return 'Louisiana'; 
            case 'me': return 'Maine'; 
            case 'md': return 'Maryland'; 
            case 'ma': return 'Massachusetts'; 
            case 'mi': return 'Michigan'; 
            case 'mn': return 'Minnesota'; 
            case 'ms': return 'Mississippi'; 
            case 'mo': return 'Missouri'; 
            case 'mt': return 'Montana'; 
            case 'ne': return 'Nebraska'; 
            case 'nv': return 'Nevada'; 
            case 'nh': return 'New Hampshire'; 
            case 'nj': return 'New Jersey'; 
            case 'nm': return 'New Mexico'; 
            case 'ny': return 'New York'; 
            case 'nc': return 'North Carolina'; 
            case 'nd': return 'North Dakota'; 
            case 'oh': return 'Ohio'; 
            case 'ok': return 'Oklahoma'; 
            case 'or': return 'Oregon'; 
            case 'pa': return 'Pennsylvania'; 
            case 'ri': return 'Rhode Island'; 
            case 'sc': return 'South Carolina'; 
            case 'sd': return 'South Dakota'; 
            case 'tn': return 'Tennessee'; 
            case 'tx': return 'Texas'; 
            case 'ut': return 'Utah'; 
            case 'vt': return 'Vermont'; 
            case 'va': return 'Virginia'; 
            case 'wa': return 'Washington'; 
            case 'wv': return 'West Virginia'; 
            case 'wi': return 'Wisconsin'; 
            case 'wy': return 'Wyoming';
			default: 
			case 'quebec': return 'Québec';
		}
	}
	
	public function formatTelephoneNumber( $rawTelephoneNumber )
	{
		$phoneNumber = substr($rawTelephoneNumber, 0, 3) . '-' . substr($rawTelephoneNumber, 3, 3) . '-' . substr($rawTelephoneNumber, 6, 4);
		$extension = substr($rawTelephoneNumber, 10);
		
		if( !empty( $extension ) )
		{
			$phoneNumber .= ' x' . $extension;
		}
		
		return $phoneNumber;
	}
    
    /**
     * Returns true if the user is currently logged in.
     * @access public
     * @return boolean
     */
    public function isLoggedIn()
    {
        return !empty( $_SESSION['userInformation'] );
    }
	
	/**
     * Returns the URL of where to access the static content (images, etc) for the website.
     * @access public
     * @return string
     */
    public function getContentURL()
    {
        return $this->contentURL;
    }
    
    /**
     * Kills the website with a fatal message.
     * @access public
     * @param string $message Message to display, most oftenly a critical error that led to killing
     * the site.
     */
    public function killSite( $message )
    {
        die( '<p>' . $message . '</p><p>Veuillez contacter un administrateur.</p>' );
    }
}

?>
