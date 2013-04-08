<?php
/**
 * Wrapper of the Hockedu Portal System.
 * 
 * This file wraps the whole management system, it manages permissions as well
 * as which page to display. Technically, this page gets called for every
 * request in the system.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 17/02/2013
 * @package Hockedu
 */
 
/* Manually activate error display, no need because server has it enabled in settings.
ini_set('display_errors',1);
ini_set('display_startup_errors',1);
error_reporting(-1);
*/

// Automatically load subsite classes upon first request.
function autoloadClasses( $className ) 
{
    $completeClassName = '/var/www/hockedu.com/classes/' . $className . '.class.php';
    
    if( file_exists( $completeClassName ) ) 
    {
        require_once( $completeClassName );
        return;
    }
}

spl_autoload_register('autoloadClasses');


require_once( '/var/www/hockedu.com/config/enums.php' );

require_once( 'MDB2.php' );

$Website = Website::getInstance();

// Pages that do not require authentication.
switch( $Website->getModules( 0 ) )
{
    case 'features':
        $Website->setIncludeModule( 'features' );
        break;        
    case 'register':
        $Website->setIncludeModule( 'register' );
        break;
    case 'download':
        $Website->setIncludeModule( 'download' );
        break;
    case 'rankings':
        $Website->setIncludeModule( 'rankings' );
        break;
    case 'maps':
        $Website->setIncludeModule( 'usermaps' );
        break;
    case 'map':
        $Website->setIncludeModule( 'usermap' );
        break;
    case 'achievements':
        $Website->setIncludeModule( 'achievements' );
        break;
    case 'user':
        $Website->setIncludeModule( 'userprofile' );
        break;
    case 'support':
        $Website->setIncludeModule( 'support' );
        break;
    case 'about':
        $Website->setIncludeModule( 'about' );
        break;
    case 'login':
        $Website->setIncludeModule( 'login' );
        break;
    case 'ajax':
        $Website->setIncludeModule( 'ajax' );
        break;
    case 'remote':
        $Website->setIncludeModule( 'remote' );
        break;
    default:
        $Website->setIncludeModule( 'homepage' );
}

if( $Website->isLoggedIn() )
{
	// Pages that require authentication.
	switch( $Website->getModules( 0 ) )
    {
        case 'myaccount':
            $Website->setIncludeModule( 'useraccount' );
            break;
        case 'logout':
            $Website->setIncludeModule( 'logout' );
            break;
    }
}  

include( $Website->getModulePath() );

$Website->display();

