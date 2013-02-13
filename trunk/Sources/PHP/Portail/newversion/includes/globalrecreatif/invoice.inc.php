<?php
/**
 * Invoice creation and consultation page.
 * 
 * This file handles the creation and display of invoices.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 13/12/2012
 * @package GlobalRecreatif
 */

$invoiceId = $Website->getModules( 1 );

if( $invoiceId === false )
{
    // No id was provided, display no ID error or do some default action?
    $Website->getSmarty()->assign( 'NoIdGiven', true );
}
elseif( $invoiceId === 'nouvelle' )
{
    // Prepare form for new entry.
    $Website->getSmarty()->assign( 'NewInvoice', true );
	
	// Was a pre-invoice id provided?
	$preInvoiceId = $Website->getModules( 2 );
	
	if( $preInvoiceId === false )
	{
		$Website->getSmarty()->assign( 'NoPreInvoiceIdGiven', true );
	}
	else
	{
		$GlobalRecreatif = GlobalRecreatif::getInstance();
		
		if( !$GlobalRecreatif->preInvoiceExists( $preInvoiceId ) )
		{
			$Website->getSmarty()->assign( 'NoPreInvoiceIdGiven', true );
		}
		else
		{
			$Website->getSmarty()->assign( 'IsPreInvoice', true );
			$Website->getSmarty()->assign( 'InvoiceData', $GlobalRecreatif->getInvoiceData( $preInvoiceId ) );
		}
	}
}
else
{
	// Is this an export?
	$export = $Website->getModules( 2 );
	
	// Display the invoice data.    
    $GlobalRecreatif = GlobalRecreatif::getInstance();
    
    if( !$GlobalRecreatif->invoiceExists( $invoiceId ) )
    {
        // Invoice doesn't seem to exist, display an error.
        $Website->getSmarty()->assign( 'InvoiceDoesNotExist', true );
    }
	elseif( $export === 'pdf' )
	{
		// Yes, PDF export.
		$Website->doNotDisplaySite();

		class PDF extends SFPDF
		{
			// Page header
			function Header()
			{
				// Logo			
				$this->Image( $this->logoURL, 10, 6, 0, 0, '', 'http://www.globalrecreatif.ca' );
				
				// Arial bold 15
				$this->SetFont('Arial','B',22);
				// Title
				$this->Cell(0,10,'FACTURE',0,0,'R');
				
				$this->SetFont('Arial','I',12);
				$this->SetY(15);
				$this->Cell(30,10,'Service, vente et entreposage',0,1);
				$this->SetFont('Arial','',12);
				$this->Cell(30,10,'6900 av. Marien',0,0);
				$this->Ln(5);
				$this->Cell(30,10,'Montréal-Est, H1B 4W3',0,0);
				$this->Ln(5);
				$this->Cell(30,10,'514-315-3707',0,0);

				// Line break
				$this->Ln(10);
			}

			// Page footer
			function Footer()
			{
				// Position at 1.5 cm from bottom
				$this->SetY(-15);
				// Arial italic 8
				$this->SetFont('Arial','I',8);
				// Page number
				$this->Cell(0,10,'Page '.$this->PageNo().'/{nb}',0,0,'C');
			}
			
			// Colored table
			function FancyTable($header, $data)
			{
				// Colors, line width and bold font
				$this->SetFillColor(255,255,0);
				$this->SetTextColor(0);
				$this->SetDrawColor(0,0,0);
				$this->SetLineWidth(.3);
				$this->SetFont('','B');
				// Header
				$w = array(110, 30, 30, 30);
				for($i=0;$i<count($header);$i++)
					$this->Cell($w[$i],7,$header[$i],1,0,'C',true);
				$this->Ln();
				// Color and font restoration
				$this->SetFillColor(255,255,200);
				$this->SetTextColor(0);
				$this->SetFont('');
				// Data
				$fill = false;
				$fontSize = 12;
				$decrementStep = 0.1;
				
				foreach($data as $row)
				{
					while( $this->GetStringWidth( $row['description'] ) > ( $w[0] - 1 ) ) 
					{
						$this->SetFontSize( $fontSize -= $decrementStep );
					}
					
					$this->Cell( $w[0], 6, $row['description'], 'LR', 0, 'L', $fill );
					$this->SetFontSize( 12 );
					$this->Cell( $w[1], 6, $row['quantity'], 'LR', 0, 'C', $fill );
					$this->Cell( $w[2], 6, money_format( '%n', $row['baseprice'] ), 'LR', 0, 'R', $fill );
					$this->Cell( $w[3], 6, money_format( '%n', $row['totalprice'] ), 'LR', 0, 'R', $fill );
					$this->Ln();
					$fill = !$fill;
				}
				// Closing line
				$this->Cell(array_sum($w),0,'','T');
			}
			
			function AutoPrint($dialog=false)
			{
				//Open the print dialog or start printing immediately on the standard printer
				$param=($dialog ? 'true' : 'false');
				$script="print($param);";
				$this->IncludeJS($script);
			}
			
			function setLogoURL( $url )
			{
				$this->logoURL = $url;
			}
			
			private $logoURL;
		}
		
		$invoiceData = $GlobalRecreatif->getInvoiceData( $invoiceId );
		
		// Instanciation of inherited class.
		$pdf = new PDF('Portrait', 'mm', 'Letter');
		$pdf->SetLogoURL( $Website->getContentURL() . 'img/' . $Website->getCurrentSubsite() . '/logo.png' );
		$pdf->AliasNbPages();
		$pdf->AddPage();
		$pdf->SetFont('Arial','B',12);
		
		$pdf->Cell(30,10,'Facturé à:',0,0);
		$pdf->Ln(10);
		
		// Customer contact information.
		if( $invoiceData['type'] == 0 )
		{
			$clientName = $invoiceData['prenom'] . ' ' . $invoiceData['nom'];
		} 
		else
		{
			$clientName = $invoiceData['nom_entreprise'];
		}
		
		$clientAddress = $invoiceData['adresse'];
		
		if( isset( $invoiceData['appt_suite'] ) )
		{
			$clientAddress .= ' appartement/suite ' . $invoiceData['appt_suite'];
		}
		
		$clientCity = $invoiceData['ville'] . ', ' . $invoiceData['province_etat'];
		
		if( $invoiceData['pays'] == 'unitedstates' )
		{
			$clientCountry = 'U.S.A.';
		}
		elseif( $invoiceData['pays'] == 'canada' )
		{
			$clientCountry = 'Canada';
		}

		if( isset( $invoiceData['tel_domicile'] ) )
		{
			$clientPhone = $invoiceData['tel_domicile'];
		}
		elseif( isset( $invoiceData['tel_cell'] ) )
		{
			$clientPhone = $invoiceData['tel_cell'];
		}
		else
		{
			$clientPhone = '';
		}
		
		$pdf->SetFont( 'Arial', 'I', 10 );		
		$pdf->Cell( 30, 10, iconv('UTF-8', 'windows-1252', $clientName), 0, 0 );
		$pdf->Ln(5);
		$pdf->Cell( 30, 10, iconv('UTF-8', 'windows-1252', $clientAddress), 0, 0 );
		$pdf->Ln(5);
		$pdf->Cell( 30, 10, iconv('UTF-8', 'windows-1252', $clientCity), 0, 0 );
		$pdf->Ln(5);
		$pdf->Cell( 30, 10, $clientCountry, 0, 0 );
		$pdf->Ln(5);
		$pdf->Cell( 30 , 10, $invoiceData['code_postal'], 0, 0 );
		$pdf->Ln(5);
		$pdf->Cell( 30, 10, $clientPhone, 0, 0 );

		// Invoice information.
		$invoiceIdPadded = str_pad( $invoiceData['id_facture'], 6, '0', STR_PAD_LEFT );
		
		$pdf->SetFont( 'Arial', '', 12 );
		$pdf->SetXY( 140, 25 );
		$pdf->Cell( 30, 10, 'Date:', 0, 0, 'R' );
		$pdf->Cell( 30, 10, date( 'd/m/Y', $invoiceData['date'] ), 0, 0 );
		$pdf->Ln(6);
		$pdf->SetX( 140 );
		$pdf->Cell( 30, 10, 'No client:', 0, 0, 'R' );
		$pdf->Cell( 30, 10, $invoiceData['id_client'], 0, 0 );
		$pdf->Ln(6);
		$pdf->SetX( 140 );
		$pdf->Cell( 30, 10, 'No facture:', 0, 0, 'R' );
		$pdf->Cell( 30, 10, $invoiceIdPadded, 0, 0 );
		$pdf->Ln(35);
		$pdf->SetX( 150 );
		$pdf->SetFont( 'Arial', 'B', 12 );
		$pdf->Cell( 50, 6, $invoiceIdPadded, 0, 0, 'C' );
		
		$pdf->Code128( 150, 60, $invoiceIdPadded, 50, 12 );
		
		$pdf->Rect( 145, 55, 60, 25 );

		$pdf->SetY( 100 );
		
		$header = array( 'Description', 'Quantité', 'Prix unitaire', 'Prix' );
		$data = array();
		$paddingCount = 18;
		
		// Extra pages necessary?
		while( count( $invoiceData['items'] ) > $paddingCount )
		{
			$paddingCount += 35;
		}
		
		$countCompare = count( $invoiceData['items'] );
		for( $i = 0; $i < ( $paddingCount - $countCompare ); $i++ )
		{
			$invoiceData['items'][] = array( 
											'description' => '', 
											'quantity' => '',
											'baseprice' => '',
											'totalprice' => ''
											);
		}
			
		$pdf->FancyTable( $header, $invoiceData['items'] );
		
		$pdf->Ln(5);
		$pdf->SetFont( 'Arial', '', 14 );
		$pdf->SetX( 130 );
		$pdf->Cell( 30, 10, 'SOUS-TOTAL :', 0, 0, 'L' );
		$pdf->Cell( 50, 10, money_format( '%n', $invoiceData['soustotal'] ), 0, 0, 'R' );
		
		$pdf->Ln(8);
		$pdf->SetX( 130 );
		$pdf->Cell( 30, 10, $invoiceData['tax1_name'] . ' (' . $invoiceData['tax1_percent'] . ' %) :', 0, 0, 'L' );
		$pdf->Cell( 50, 10, money_format( '%n', $invoiceData['tax1'] ), 0, 0, 'R' );
		
		$pdf->Ln(8);
		$pdf->SetX( 130 );
		$pdf->Cell( 30, 10, $invoiceData['tax2_name'] . ' (' . $invoiceData['tax2_percent'] . ' %) :', 0, 0, 'L' );
		$pdf->Cell( 50, 10, money_format( '%n', $invoiceData['tax2'] ), 0, 0, 'R' );
		
		$pdf->Ln(8);
		$pdf->SetFont( 'Arial', 'B', 14 );
		$pdf->SetX( 130 );
		$pdf->Cell( 30, 10, 'TOTAL :', 0, 0, 'L' );
		$pdf->Cell( 50, 10, money_format( '%n', $invoiceData['total'] ), 0, 0, 'R' );
		
		$pdf->Ln(2);
		$pdf->SetFont( 'Arial', 'I', 8 );
		$pdf->Cell( 30, 10, 'TPS R-144 836 103', 0, 0, 'L' );
		$pdf->Ln(3);
		$pdf->Cell( 30, 10, 'TVQ 12 0272 8312 TQ0001', 0, 0, 'L' );

		// Is invoice canceled?
		if( intval( $invoiceData['statut'] == GlobalRecreatifInvoiceStatus::Canceled ) )
		{
			$pdf->SetAlpha(0.5);
			$pdf->SetFont('Arial','B',100);
			$pdf->SetTextColor(255,0,0);
			$pdf->Rotate(45,55,210);
			$pdf->Text(55,210,'ANNULÉE');
			$pdf->Rotate(0);
			$pdf->SetTextColor(0,0,0);
			$pdf->SetAlpha(1.0);
		}
		
		// Need to print?
		$pdf->AutoPrint(false);
		
		$pdf->Output('Global Récréatif - Facture #' . $invoiceData['id_facture'], 'I');
	}
    else
    {
		// No export, just regular display.
		$Website->getSmarty()->assign( 'InvoiceData', $GlobalRecreatif->getInvoiceData( $invoiceId ) );		
    }
}
 

$Website->setTemplateToInclude( 'globalrecreatif/invoice' );

 ?>
 
