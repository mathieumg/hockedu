<?php
/**
 * Global Récréatif modules.
 * 
 * Handles incoming requests for Global Récréatif pages and does the appropriate actions accordingly.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 26/07/2011
 * @package Cloud
 */

switch( $Website->getModules( 0 ) )
{
    case 'client';
        $Website->setIncludeSubModule( 'client' );
        break;
        
    case 'contrat';
        $Website->setIncludeSubModule( 'contract' );
        break;
		
	case 'facture';
        $Website->setIncludeSubModule( 'invoice' );
        break;
        
    default:
        $Website->setIncludeSubModule( 'homepage' );
}

 ?>
 
