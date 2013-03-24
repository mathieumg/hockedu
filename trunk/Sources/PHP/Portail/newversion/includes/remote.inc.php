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
        $commonRemoteModule = false;
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
                        
                        $authenticationData = $Common->getCurrentAuthenticationData();
                        
                        $jsonResponse['auth_key'] = $authenticationData['key'];
                        $jsonResponse['auth_key_expiration'] = (int)$authenticationData['expiration'];
                    }
                }
                
                break;
                
            case 'listmaps':                
                if( empty( $_POST['user_id'] ) )
                {
                    $jsonResponse['error'] = 'UserIdMissing';
                }
                else
                {
                    // Get DB connection.
                    $Common = Common::getInstance();
                
                    $showPrivateMaps = false;
                    if( !empty( $_POST['auth_key'] ) )
                    {
                        // We have a auth key, is it valid?       
                        $validKey = $Common->validateAuthenticationKey( $_POST['auth_key'], $_POST['user_id'] );                  
                        
                        if( !$validKey )
                        {
                            $jsonResponse['error'] = 'InvalidAuthKey';
                        }
                        else
                        {
                            // Valid auth key. Allow to list the user's private maps.                        
                            $showPrivateMaps = true;
                        }
                    }

                    // List the user's maps.                        
                    $jsonResponse['user_maps'] = $Common->getUserMaps( $_POST['user_id'], $showPrivateMaps );
                }
                
                break;
                
            case 'getmap':
            
                break;
                
            case 'newmap':
            
                break;    
                
            case 'updatemap':
            
                break;  
        }
    }
}

// JSON-encode it.
echo json_encode( $jsonResponse );

$Website->doNotDisplaySite();

 ?>
 
