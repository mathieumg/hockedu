<?php
/**
 * Remote handler.
 * 
 * This file handles all the remote requests, which require JSON output.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 12/03/2013
 * @package Hockedu
 */

header('Content-Type: application/json');

$remoteModule = $Website->getModules( 1 );

if( $remoteModule === false )
{
    // Exit if no remote module was requested.
    exit;
}

$jsonResponse = array();

// Are we trying to access a page that is available site-wide?
$commonRemoteModule = true;

switch( $remoteModule )
{
    case 'resources':
        $ajaxResponse['permissions']['isLoggedIn'] = $Website->isLoggedIn();
        $ajaxResponse['subsite'] = $Website->getCurrentSubsite();
        $ajaxResponse['domainName'] = $Website->getSetting( 'MainDomain' );
        $ajaxResponse['contentLocation'] = $Website->getContentURL();
        break;
        
    default:
        $commonAjaxModule = false;
}

if( !$commonRemoteModule )
{
    // Requests that require the user to be logged in.
    if( $Website->isLoggedIn() )
    {    
        /*
		$ajaxPath = $Website->getCurrentSubsiteAjaxPath();
        
        if( file_exists( $ajaxPath ) )
        {
            require_once( $ajaxPath );
        }
		*/
    }
    // Requests that do not require the user to be logged in.
    else
    {
        switch( $remoteModule )
        {
            case 'authenticate':                
                if( empty( $_POST['username'] ) )
                {
                    $jsonResponse['error'] = 'NoUsernameSpecified';
                }
                else if( empty( $_POST['password'] ) )
                {
                    $jsonResponse['error'] = 'NoPasswordSpecified';
                }
                else
                {
                    // We have a username and a SHA-1'd password, attempt to log in.
                    $Common = Common::getInstance();
                    
                    $loginSuccess = $Common->login( $_POST['username'], $_POST['password'], true );
                    
                    if( !$loginSuccess )
                    {
                        $jsonResponse['error'] = 'InvalidCredentials';
                    }
                    else
                    {
                        $authenticationData = $Common->getAuthenticationData( $_POST['username'], $_POST['password'] );
                        
                        $jsonResponse['auth_key'] = $authenticationData['auth_key'];
                        $jsonResponse['auth_key_expiration'] = $authenticationData['auth_key_expiration'];
                    }
                }
        }
    }
}

// JSON-encode it.
echo json_encode( $jsonResponse );

$Website->doNotDisplaySite();

 ?>
 
