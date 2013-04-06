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
                // Get DB connection.
                $Common = Common::getInstance();
                
                if( empty( $_POST['user_id'] ) )
                {
                    $jsonResponse['maps'] = $Common->getMaps();
                }
                else
                {
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
                        $jsonResponse['maps'] = $Common->getUserMaps( $_POST['user_id'], $showPrivateMaps );
                    }
                }
                
                break;
                
            case 'getmap':
                if( empty( $_POST['map_id'] ) )
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
                        if( empty( $_POST['user_id'] ) )
                        {
                            $jsonResponse['error'] = 'UserIdMissing';
                        }
                        else if( empty( $_POST['auth_key'] ) )
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
                        $mapCacheFileName = $mapInfo['cache_name'];
                        $mapCacheFilePath = '/var/www/hockedu.com/maps/'; //TOREMOVE: Use code below to get maps path from settings cache.
                        
                        /*
                        $website = Website::getInstance();
                        $mapCacheFilePath = $website->getBasePath() . $website->getSetting( 'mapsDirectory' ) . '/' . $mapCacheFileName;
                        */
                        
                        //header('Content-type: text/xml');
                        //echo file_get_contents( $mapCacheFilePath . $mapCacheFileName );
                        
                        $jsonResponse['id'] = $mapInfo['id'];
                        $jsonResponse['author'] = $mapInfo['id_user'];
                        $jsonResponse['version'] = $mapInfo['version'];
                        $jsonResponse['name'] = $mapInfo['name'];
                        $jsonResponse['rating'] = $mapInfo['rating_average'];
                        $jsonResponse['public'] = $mapInfo['is_public'];
                        $jsonResponse['created'] = $mapInfo['creation_time'];
                        $jsonResponse['modified'] = $mapInfo['last_modified_time'];
                        $jsonResponse['content'] = file_get_contents( $mapCacheFilePath . $mapCacheFileName );
                    }
                }
                
                break;
                
            case 'sendmap':                                
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
                                
                                if( !empty( $_POST['map_id'] ) )
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
                
            case 'sendachievements': 
                if( empty( $_POST['user_id'] ) )
                {
                    $jsonResponse['error'] = 'UserIdMissing';
                }
                else if( empty( $_POST['auth_key'] ) )
                {
                    $jsonResponse['error'] = 'AuthKeyMissing';
                }
                else if( !isset( $_FILES['achievementsfile'] ) || $_FILES['achievementsfile']['size'] == 0 )
                {
                    $jsonResponse['error'] = 'AchievementsFileMissing';
                }
                else if( $_FILES['achievementsfile']['error'] !== 0 )
                {
                    $jsonResponse['error'] = 'AchievementsFileUploadError';
                }
                else if( $_FILES['achievementsfile']['type'] !== 'text/xml' )
                {
                    $jsonResponse['error'] = 'AchievementsFileInvalidType';
                }
                else
                {
                    // Get DB connection.
                    $Common = Common::getInstance();
                    
                    // Is the authentication key valid?
                    if( $Common->validateAuthenticationKey( $_POST['auth_key'], $_POST['user_id'] ) )
                    {
                        $userId = $_POST['user_id'];
                        $achievementsContent = file_get_contents( $_FILES['achievementsfile']['tmp_name'] );
                        
                        $xml = new SimpleXMLElement( $achievementsContent );
                        
                        foreach( $xml->Achievement as $achievement )
                        {
                            $achievementId = $achievement['Id'];
                            $isUnlocked = false;
                            $progressData = null;
                            
                            if( isset( $achievement['Unlocked'] ) )
                            {
                                $isUnlocked = true;
                            }
                            else if( isset( $achievement->Data ) )
                            {
                                $progressData = $achievement->Data->asXML();
                            }
                            
                            $Common->updateUserAchievement( $userId, $achievementId, $isUnlocked, $progressData );
                        }

                        //$jsonResponse['ok'] = 'AuthKeyInvalid';
                    }
                    else
                    {
                        $jsonResponse['error'] = 'AuthKeyInvalid';
                    }
                }
            
                break;
                
            case 'getachievements':
                if( empty( $_POST['user_id'] ) )
                {
                    $jsonResponse['error'] = 'UserIdMissing';
                }
                else if( empty( $_POST['auth_key'] ) )
                {
                    $jsonResponse['error'] = 'AuthKeyMissing';
                }
                else
                {
                     // Get DB connection.
                    $Common = Common::getInstance();
                    
                    // Is the authentication key valid?
                    if( $Common->validateAuthenticationKey( $_POST['auth_key'], $_POST['user_id'] ) )
                    {
                        $userId = $_POST['user_id'];
                        
                        $userAchievements = $Common->getUserAchievements( $userId );
                        
                        // Work-around for deep copy of SimpleXML elements.
                        function xml_adopt($root, $new) {
                            $node = $root->addChild($new->getName(), (string) $new);
                            foreach($new->attributes() as $attr => $value) {
                                $node->addAttribute($attr, $value);
                            }
                            foreach($new->children() as $ch) {
                                xml_adopt($node, $ch);
                            }
                        }
                        
                        $xml = new SimpleXMLElement('<Achievements></Achievements>'); 
                        
                        foreach( $userAchievements as $userAchievement )
                        {
                            $achievement = $xml->addChild('Achievement');
                            $achievement->addAttribute('Id', $userAchievement['id_achievement']);
                            
                            if( $userAchievement['unlocked'] == 1 )
                            {
                                $achievement->addAttribute('Unlocked', '1');
                            }
                            
                            if( !empty( $userAchievement['progress_data'] ) )
                            {
                                $dbNode = new SimpleXMLElement( $userAchievement['progress_data'] );
                                xml_adopt($achievement, $dbNode);
                            }
                        }                        

                        $jsonResponse['achievements'] = $xml->asXML();
                    }
                    else
                    {
                        $jsonResponse['error'] = 'AuthKeyInvalid';
                    }
                }
                
                break;

            default:
                $jsonResponse['error'] = 'UnknownRequest';
                break;                
        }
    }
}

// JSON-encode it.
//if( empty( $jsonResponse ) )
//{
//    $jsonResponse = '';
//}
    
header('Content-Type: application/json');
echo json_encode( (object)$jsonResponse );
//}

$Website->doNotDisplaySite();
