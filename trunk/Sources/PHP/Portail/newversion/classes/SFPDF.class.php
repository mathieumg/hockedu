<?php
/**
 * Redirection to include the FPDF PDF system.
 * 
 * Includes the FPDF PDF system.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 04/01/2013
 * @package Cloud
 */
 
 
// Include the base FPDF class.
include( '../classes/FPDF/FPDF.class.php' );

// Include all script classes.
$scriptFiles = scandir('../classes/FPDF/scripts');

foreach( $scriptFiles as $key => $filename )
{
	if( substr($filename, -10) == '.class.php'  ) 
	{		
		include( '../classes/FPDF/scripts/' . $filename );
	} 
}

// Include the ultimate class.
include( '../classes/FPDF/SFPDF.class.php' );

?>
