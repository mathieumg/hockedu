<?php
/**
 * Download page.
 * 
 * This file is the Hockedu page that allows to download the game.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 07/04/2013
 * @package Hockedu
 */

// Title in the browser.
//$Website->setBrowserTitle( 'Download' );

$downloadId = $Website->getModules( 1 );

if( $downloadId == '1' )
{
    header('Location: http://content.hockedu.com/HockeduInstallerv1.exe');
}


$Website->setTemplateToInclude( 'download' );

 ?>
 
