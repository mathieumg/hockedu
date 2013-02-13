<?php
/**
 * Wrapper of the Berpa Management System.
 * 
 * This file wraps the whole management system, it manages permissions as well
 * as which page to display. Technically, this page gets called for every
 * request in the system.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 04/07/2011
 * @package Cloud
 */
 
error_reporting( E_ERROR );

// Automatically load subsite classes upon first request.
function autoloadClasses( $className ) 
{
    $completeClassName = '/var/www/cloud/classes/' . $className . '.class.php';
    
    if( file_exists( $completeClassName ) ) 
    {
        require_once( $completeClassName );
        return;
    }
}

spl_autoload_register('autoloadClasses');


require_once( '/var/www/cloud/config/enums.php' );

require_once( 'MDB2.php' );

$Website = Website::getInstance();

// Check if the user is logged in, if not (and we're not on the login page or trying to load an 
// ajax request), redirect to the login page.
if( ( !$Website->isLoggedIn() ) && ( $Website->getModules( 0 ) != 'connexion' ) && ( $Website->getModules( 0 ) != 'ajax' ) )
{
    $Website->changePage( $_SERVER['SERVER_NAME'] . '/connexion' );
}
elseif( $Website->isLoggedIn() && ( $Website->getModules( 0 ) == 'connexion' ) )
{
    $Website->changePage( $_SERVER['SERVER_NAME'] . '/' );
}
else
{
    // Are we trying to access a page that is available site-wide?
    $commonModule = true;
    
    switch( $Website->getModules( 0 ) )
    {
        case 'connexion':
            $Website->setIncludeModule( 'login' );
            break;
        case 'ajax':
            $Website->setIncludeModule( 'ajax' );
            break;
        case 'deconnexion':
            $Website->setIncludeModule( 'logout' );
            break;
        default:
            $commonModule = false;
    }
    
    // Subsite-specific module.
    if( !$commonModule )
    {
        switch( $Website->getCurrentSubsite() )
        {
            case 'globalrecreatif':
                include( $Website->getModulePath( 'globalrecreatif/index' ) );
                break;
            case $Website->getSetting( 'MainSubDomain' ):
                // Redirect to the subsite chosen when logging in.
                $Website->changePage( $Website->getSubsiteUrl( $_SESSION['userInformation']['lastSubsite'], false ) );          
                break;
        }
    }
    
    include( $Website->getModulePath() );
}

$Website->display();

 ?>
 