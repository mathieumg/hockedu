<?php
/**
 * Login page.
 * 
 * This file is the Hockedu page that allows users to log in.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 07/04/2013
 * @package Hockedu
 */

// Title in the browser.
//$Website->setBrowserTitle( 'Login' );

// Was the form submitted?
if( isset( $_POST['submit'] ) )
{
    // Initiate the DB.
	$Common = Common::getInstance();
    
    $loginValid = $Common->login( $_POST['username'], $_POST['password'] );
    
    if( $loginValid )
    {
        $Website = Website::getInstance();
        
        $Website->changePage( $Website->getSetting('MainDomain') );
    }
}

$Website->setTemplateToInclude( 'login' );

 ?>
 
