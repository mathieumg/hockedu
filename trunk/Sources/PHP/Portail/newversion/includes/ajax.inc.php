<?php
/**
 * AJAX handler.
 * 
 * This file handles all the AJAX requests, and other requests that require JSON output.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 20/02/2013
 * @package Hockedu
 */

header('Content-Type: application/json');

$ajaxModule = $Website->getModules( 1 );

if( $ajaxModule === false )
{
    // Exit if no ajax module was requested.
    exit;
}

$ajaxResponse = array();

// Are we trying to access a page that is available site-wide?
$commonAjaxModule = true;

switch( $ajaxModule )
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

if( !$commonAjaxModule )
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
        switch( $ajaxModule )
        {
            /*
			case 'login':
                //$Common->getDb()->query( 'SELECT * FROM comm_settings' );
                
                // Some sanitizing.
                $_POST['username'] = filter_var( $_POST['username'], FILTER_SANITIZE_STRING, FILTER_FLAG_STRIP_HIGH );
                $_POST['password'] = filter_var( $_POST['password'], FILTER_SANITIZE_STRING, FILTER_FLAG_STRIP_HIGH );
                $_POST['system'] = filter_var( $_POST['system'], FILTER_SANITIZE_STRING, FILTER_FLAG_STRIP_HIGH );
                
                // Do we have valid data to work with?
                if( empty( $_POST['username'] ) || empty( $_POST['password'] ) || empty( $_POST['system'] ) || !$Website->isValidSubsite( $_POST['system'] ) )
                {
                    $ajaxResponse['response'] = 'denied';
                }
                else
                {
                    $Common = Common::getInstance();
                    
                    // Which subsite is the login request for?
                    if( $_POST['system'] === $Website->getSetting( 'MainSubDomain' ) )
                    {
                        $subsite = $Website->getCurrentSubsite();
                    }
                    else
                    {
                        $subsite = $_POST['system'];
                    }
                    
                    // The login logic is done at the class-level in Common.
                    $loginResult = $Common->login( $_POST['username'], $_POST['password'], $subsite, $Website );
                    
                    // The AJAX output depends on what Common::login returned.
                    switch( $loginResult )
                    {
                        case LoginResponse::Success:
                            $ajaxResponse['response'] = 'granted';
                            break;
                            
                        case LoginResponse::NewIp:
                            $ajaxResponse['ipAddress'] = $Website->getIpAddress();
                            $ajaxResponse['response'] = 'newip';
                            break; 
                            
                        case LoginResponse::NoSubsite:
                            $ajaxResponse['response'] = 'nosubsite';
                            break;  
                            
                        case LoginResponse::Failure:
                        default:
                            $ajaxResponse['response'] = 'denied';
                    }
                }

                break;
                
            case 'sendipticket':
                if( !empty( $_SESSION['requestIp']['ticket'] ) && !empty( $_SESSION['requestIp']['email'] ) )
                {
                    $ajaxResponse['response'] = 'success';
                    
                    $Common = Common::getInstance();
                    
                    $whitelist = $Common->addIp( $_SESSION['requestIp']['ticket'], IpType::Whitelisted );
                    
                    if( $whitelist === false )
                    {
                        $ajaxResponse['response'] = 'failure';
                    }
                    else
                    {
                        // Send email.
                        $messageBody = "Veuillez cliquer sur le lien suivant pour ajouter l'adresse " . $_SESSION['requestIp']['ticket'] . " à la liste des adresses IP ayant la permission d'accéder au système de gestion pour les prochaines 24 heures:\n\n" . $Website->getSubsiteUrl( $_SESSION['requestIp']['subsite'] ) . 'connexion/' . $whitelist . "\n\nMerci\n\nGroupe Berpa";
                        
                        $mailMessage = $Website->getEmailTemplate();
                        $mailMessage->SetEncodedEmailHeader( 'To', $_SESSION['requestIp']['email'], $_SESSION['requestIp']['name'] );
                        $mailMessage->SetEncodedHeader( 'Subject', 'Activation de ' . $_SESSION['requestIp']['ticket'] . ' pour le système de gestion' );
                        $mailMessage->AddQuotedPrintableTextPart( $messageBody );
                        @$mailMessage->Send();
                    }
                    
                    unset( $_SESSION['requestIp'] );
                }
                else
                {
                    $ajaxResponse['response'] = 'failure';
                }
                
                break;
			*/
				
			case 'listusers':
				$finalUsersArray = array();
				$Common = Common::getInstance();
				$usersArray = $Common->getUserList();
				
				foreach( $usersArray as $user )
				{
					$finalUsersArray[] = array( 'text' => 'Registered on the ' . date('jS \of F Y \a\t g:i A', $user['registration_time']) , 'user' => array( 'username' => $user['username'] ) );
				}
				
				$ajaxResponse['data'] = $finalUsersArray;
        }
    }
}

// If plainText is set to true, just send that information, otherwise JSON-encode it.
echo json_encode( $ajaxResponse );

$Website->doNotDisplaySite();

 ?>
 
