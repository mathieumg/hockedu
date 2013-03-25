<?php
/**
 * Remote handler.
 * 
 * This file handles all the remote requests, which require JSON output.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 12/03/2013
 * @package Hockedu
 */

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
                        $jsonResponse['auth_key_expiration'] = $authenticationData['expiration'];
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
                            $jsonResponse['error'] = 'AuthKeyInvalid';
                        }
                        else
                        {
                            // Valid auth key. Allow to list the user's private maps.                        
                            $showPrivateMaps = true;
                        }
                    }

                    // List the user's maps.
                    if( empty( $jsonResponse['error'] ) )
                    {
                        $jsonResponse['user_maps'] = $Common->getUserMaps( $_POST['user_id'], $showPrivateMaps );
                    }
                }
                
                break;
                
            case 'getmap':
                if( empty( $_POST['user_id'] ) )
                {
                    $jsonResponse['error'] = 'UserIdMissing';
                }
                else if( empty( $_POST['map_id'] ) )
                {
                    $jsonResponse['error'] = 'MapIdMissing';
                }
                else
                {
                    // Get DB connection.
                    $Common = Common::getInstance();
                    
                    $mapInfo = $Common->getMapInfo( $_POST['map_id'] );
                    
                    $showMap = false;
                    if( $mapInfo['is_public'] == 1 )
                    {
                        $showMap = true;
                    }
                    else
                    {
                        if( empty( $_POST['auth_key'] ) )
                        {
                            $jsonResponse['error'] = 'AuthKeyMissing';
                        }
                        else
                        {
                            // We have a auth key, is it valid?       
                            $validKey = $Common->validateAuthenticationKey( $_POST['auth_key'], $_POST['user_id'] );                  
                            
                            if( !$validKey )
                            {
                                $jsonResponse['error'] = 'AuthKeyInvalid';
                            }
                            else
                            {
                                // Valid auth key. Allow the download of this map.                        
                                $showMap = true;
                            }
                        }
                    }
                    
                    if( $showMap )
                    {
                        //$mapCacheFileName = $mapInfo['cache_name'];
                        $mapCacheFileName = 'db3d4a31785b535f5c02d9aa6410a2788d310841';
                        $mapCacheFilePath = '/var/www/hockedu.com/maps/'; //TOREMOVE: Use code below to get maps path from settings cache.
                        
                        /*
                        $website = Website::getInstance();
                        $mapCacheFilePath = $website->getBasePath() . $website->getSetting( 'mapsDirectory' ) . '/' . $mapCacheFileName;
                        */
                        
                        header('Content-type: text/xml');
                        echo file_get_contents( $mapCacheFilePath . $mapCacheFileName );
                    }
                }
                
                break;
                   
            case 'updatemap':
            
                if( empty( $_POST['map_id'] ) )
                {
                    $jsonResponse['error'] = 'MapIdMissing';
                    break;
                }
                
                $updateMap = true;
                
            case 'newmap':                                
                if( empty( $_POST['user_id'] ) )
                {
                    $jsonResponse['error'] = 'UserIdMissing';
                }
                else if( empty( $_POST['auth_key'] ) )
                {
                    $jsonResponse['error'] = 'AuthKeyMissing';
                }
                else if( empty( $_POST['name'] ) )
                {
                    $jsonResponse['error'] = 'MapNameMissing';
                }
                else if( !isset( $_FILES['mapfile'] ) || $_FILES['mapfile']['size'] == 0 )
                {
                    $jsonResponse['error'] = 'MapFileMissing';
                }
                else if( $_FILES['mapfile']['error'] !== 0 )
                {
                    $jsonResponse['error'] = 'MapFileUploadError';
                }
                else if( $_FILES['mapfile']['type'] !== 'text/xml' )
                {
                    $jsonResponse['error'] = 'MapFileInvalidType';
                }
                else
                {
                    // Get DB connection.
                    $Common = Common::getInstance();
                    
                    // Is the authentication key valid?
                    if( $Common->validateAuthenticationKey( $_POST['auth_key'], $_POST['user_id'] ) )
                    {
                        $userId = $_POST['user_id'];
                        $mapName = $_POST['name'];
                        $mapDescription = empty( $_POST['description'] ) ? '' : $_POST['description'];
                        $mapIsPublic = empty( $_POST['public'] ) ? 0 : 1;
                        $mapCacheFileName = sha1( $userId . $mapName . microtime() );
                        $mapCacheFilePath = '/var/www/hockedu.com/maps/'; //TOREMOVE: Use code below to get maps path from settings cache.
                        
                        /*
                        $website = Website::getInstance();
                        $mapCacheFilePath = $website->getBasePath() . $website->getSetting( 'mapsDirectory' ) . '/' . $mapCacheFileName;
                        */
                        
                        if( file_exists( $mapCacheFilePath . $mapCacheFileName ) )
                        {
                            $jsonResponse['error'] = 'MapFileCacheError';
                        }
                        else
                        {
                            if( move_uploaded_file( $_FILES['mapfile']['tmp_name'], $mapCacheFilePath . $mapCacheFileName ) )
                            {
                                $mapContent = file_get_contents( $mapCacheFilePath . $mapCacheFileName );
                                
                                if( isset( $updateMap ) )
                                {
                                    $mapId = $_POST['map_id'];
                                    $Common->updateUserMap( $mapId, $mapName, $mapDescription, $mapIsPublic, $mapCacheFileName );
                                }
                                else
                                {
                                    $mapId = $Common->addUserMap( $userId, $mapName, $mapDescription, $mapIsPublic, $mapCacheFileName );
                                }
                                
                                if( $mapId )
                                {
                                    $jsonResponse['map_id'] = $mapId;
                                }
                                else
                                {
                                    $jsonResponse['error'] = 'MapUnableToAdd';
                                }
                            }
                            else
                            {
                                $jsonResponse['error'] = 'MapFileMoveError';
                            }
                        }
                    }
                    else
                    {
                        $jsonResponse['error'] = 'AuthKeyInvalid';
                    }
                }
                
                break;    
        }
    }
}

// JSON-encode it.
if( !empty( $jsonResponse ) )
{
    header('Content-Type: application/json');
    echo json_encode( $jsonResponse );
}

$Website->doNotDisplaySite();
