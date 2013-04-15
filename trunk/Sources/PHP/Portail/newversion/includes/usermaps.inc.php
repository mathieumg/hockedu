<?php
/**
 * Maps page.
 * 
 * This file is the Hockedu page that list public games uploaded by the users.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 07/04/2013
 * @package Hockedu
 */

// Title in the browser.
//$Website->setBrowserTitle( 'Maps' );

$Common = Common::getInstance();

$mapsList = $Common->getMaps();

$Website->getSmarty()->assign( 'MapsList', $mapsList );

$Website->setTemplateToInclude( 'usermaps' );

 ?>
 
