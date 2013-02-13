<?php
/**
 * Contract details page.
 * 
 * This file handles the display of a contract's details.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 11/10/2012
 * @package GlobalRecreatif
 */

$contractId = $Website->getModules( 1 );

if( $contractId === false )
{
    // No id was provided, display no ID error or do some default action?
    $Website->getSmarty()->assign( 'NoIdGiven', true );
}
elseif( $contractId === 'nouveau' )
{
    // Prepare form for new entry.
    $Website->getSmarty()->assign( 'NewContract', true );
	
	// Was a client id provided?
	$clientId = $Website->getModules( 2 );
	
	if( $clientId === false )
	{
		$Website->getSmarty()->assign( 'NoClientIdGiven', true );
	}
	else
	{
		$GlobalRecreatif = GlobalRecreatif::getInstance();
		
		if( !$GlobalRecreatif->clientExists( $clientId ) )
		{
			$Website->getSmarty()->assign( 'NoClientIdGiven', true );
		}
		else
		{
			$Website->getSmarty()->assign( 'ClientId', $clientId );
			
			// Populate the list of lots to select from.
			$Website->getSmarty()->assign( 'ClientLots', $GlobalRecreatif->getLotsList( $clientId ) );
		
			// Populate list of years for new lots.
			for( $i = date('Y'); $i >= 1900; $i-- ) 
			{
				$allowedYears[] = ( $i );
			}
			
			$Website->getSmarty()->assign( 'AllowedYears', $allowedYears );
			
			// Populate list of allowed vehicle lengths.
			for( $i = 1; $i <= 60; $i++ ) 
			{
				$allowedLengths[] = ( $i );
			}
			
			$Website->getSmarty()->assign( 'AllowedLengths', $allowedLengths );
			
			// Populate list of allowed vehicle widths.
			for( $i = 1; $i <= 10; $i++ ) 
			{
				$allowedWidths[] = ( $i );
			}
			
			$Website->getSmarty()->assign( 'AllowedWidths', $allowedWidths );
			
			// Populate list of available packages.
			$Website->getSmarty()->assign( 'PackageList', $GlobalRecreatif->getAvailablePackagesList() );
		}
	}
}
else
{
    // Verify if ID is valid, etc.    
    $GlobalRecreatif = GlobalRecreatif::getInstance();
    
    if( !$GlobalRecreatif->contractExists( $contractId ) )
    {
        // Contract doesn't seem to exist, display an error.
        $Website->getSmarty()->assign( 'ContractDoesNotExist', true );
    }
    else
    {
        // Provide contract ID to the templating system.
        $Website->getSmarty()->assign( 'ContractId', $contractId );
        
        // Provide contract information to the templating system.
        $contractData = $GlobalRecreatif->getContractData( $contractId );
        
        // Provide contract status information to the templating system.
        $Website->getSmarty()->assign( 'ContractStatus', $GlobalRecreatif->getContractStatus( $contractData['check_in_time'], $contractData['day_difference'], $contractData['check_out_time'], $contractData['nb_jours'] ) );
        
        if( $contractData['day_difference'] < 0 ) { $contractData['day_difference'] = abs( $contractData['day_difference'] ); }
        
        $Website->getSmarty()->assign( 'ContractData', $contractData );
        
        
    }
}
 

$Website->setTemplateToInclude( 'globalrecreatif/contract' );

 ?>
 
