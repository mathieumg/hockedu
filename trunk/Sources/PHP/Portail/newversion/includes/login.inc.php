<?php
/**
 * Login management page.
 * 
 * This file handles the whole login process for the management system.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 11/07/2011
 * @package Cloud
 */

$activationCode = $Website->getModules( 1 );

if( $activationCode !== false )
{
    // Is this code valid?
    $Common = Common::getInstance();
    
    if( $Common->activateIp( $activationCode ) !== false )
    {
        $Website->getSmarty()->assign( 'IpActivated', true );
    }
    
    // Otherwise, just disregard it.
}

$Website->setTemplateToInclude( 'login' );

 ?>
 
