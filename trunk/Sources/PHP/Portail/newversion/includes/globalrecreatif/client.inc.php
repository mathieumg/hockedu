<?php
/**
 * Client management page.
 * 
 * This file handles the creation and editing of a client, along with producing a new contract, a new invoice, etc.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 26/07/2011
 * @package GlobalRecreatif
 */

$clientId = $Website->getModules( 1 );

if( $clientId === false )
{
    // No id was provided, display no ID error or do some default action?
    $Website->getSmarty()->assign( 'NoIdGiven', true );
}
elseif( $clientId === 'nouveau' )
{
    // Prepare form for new entry.
    $Website->getSmarty()->assign( 'NewClient', true );
}
else
{
    // Verify if ID is valid, etc.    
    $GlobalRecreatif = GlobalRecreatif::getInstance();
    
    if( !$GlobalRecreatif->clientExists( $clientId ) )
    {
        // Client doesn't seem to exist, display an error.
        $Website->getSmarty()->assign( 'ClientDoesNotExist', true );
    }
    else
    {
        // Provide client ID to the templating system.
        $Website->getSmarty()->assign( 'ClientId', $clientId );
    }
}
 

$Website->setTemplateToInclude( 'globalrecreatif/client' );

 ?>
 
