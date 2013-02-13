<?php
/**
 * AJAX handler.
 * 
 * This file handles all the AJAX requests for Global Récréatif.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 30/08/2012
 * @package Cloud
 */

switch( $ajaxModule )
{
    case 'client':
        $ajaxSubModule = $Website->getModules( 2 );
        
        if( $ajaxSubModule === false )
        {
            // Exit if no ajax module was requested.
            exit;
        }
        
        switch( $ajaxSubModule )
        {
            case 'nouveau':               
                $GlobalRecreatif = GlobalRecreatif::getInstance();
                
                $newClientId = $GlobalRecreatif->addNewClient();
                
                $ajaxResponse['clientid'] = $newClientId;
                break;
                
            case 'informations':
                $clientId = $Website->getModules( 3 );
                
                if( $clientId === false )
                {
                    // No specific client information was requested, simply provide the form template.
                    $ajaxResponse['htmlcontent'] = $Website->getSmarty()->fetch( 'globalrecreatif/client_informations.tpl' );
                }
                else
                {
                    // Return data for the requested client.                    
                    $GlobalRecreatif = GlobalRecreatif::getInstance();
                    
                    $ajaxResponse['clientdata'] = $GlobalRecreatif->getClientData( $clientId );
                }
                
                break;
            case 'storage':
                $clientId = $Website->getModules( 3 );
                
                $GlobalRecreatif = GlobalRecreatif::getInstance();
				
				$Website->getSmarty()->assign('ClientId', $clientId );
                
                $Website->getSmarty()->assign('StorageList', $GlobalRecreatif->getStorageList( $clientId ) );
                
                $ajaxResponse['htmlcontent'] = $Website->getSmarty()->fetch( 'globalrecreatif/client_storage.tpl' );
                
                break;
            case 'invoicing':
            case 'notes':
                $ajaxResponse['htmlcontent'] = '<p>Test for ' . $ajaxSubModule . '</p>';
            break;
        }
    
        break;
		
	case 'contrat':
		$ajaxSubModule = $Website->getModules( 2 );
        
        if( $ajaxSubModule === false )
        {
            // Exit if no ajax module was requested.
            exit;
        }
        
        switch( $ajaxSubModule )
        {
            case 'nouveau':
				$GlobalRecreatif = GlobalRecreatif::getInstance();
				
				$preInvoiceId = $GlobalRecreatif->addNewContract();
				
				$ajaxResponse['invoiceid'] = $preInvoiceId;
		
				break;
		}
		
		break;
		
	case 'facture':

		$GlobalRecreatif = GlobalRecreatif::getInstance();
		
		$GlobalRecreatif->updateInvoice( $_POST['invoiceId'], $_POST['basketItems'], $_POST['subtotal'], $_POST['tax1'], $_POST['tax2'], $_POST['total'], intval( $_POST['validInvoice'] ) );
		
		$ajaxResponse['invoiceid'] = intval( $_POST['invoiceId'] );
		$ajaxResponse['validated'] = intval( $_POST['validInvoice'] );
		
		break;
}
