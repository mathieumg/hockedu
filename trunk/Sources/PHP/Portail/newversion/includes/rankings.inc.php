<?php
/**
 * Rankings page.
 * 
 * This file is the Hockedu page that lists all the users, starting with those with the most wins.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 07/04/2013
 * @package Hockedu
 */

// Title in the browser.
//$Website->setBrowserTitle( 'Rankings' );
 
$Common = Common::getInstance();

$userRankings = $Common->getUsersRankings();

$Website->getSmarty()->assign( 'UserRankings', $userRankings );

$Website->setTemplateToInclude( 'rankings' );

 ?>
 
