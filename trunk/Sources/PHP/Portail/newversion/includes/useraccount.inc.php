<?php
/**
 * My account page.
 * 
 * This file is the Hockedu page that allows users to manage their account.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 07/04/2013
 * @package Hockedu
 */

// Title in the browser.
//$Website->setBrowserTitle( 'My account' );

$Common = Common::getInstance();

$mapsList = $Common->getUserMaps( $_SESSION['userInformation']['id'], true );

$Website->getSmarty()->assign( 'MapsList', $mapsList );

$achievementsList = $Common->getUserAchievementProgress( $_SESSION['userInformation']['id'] );

$Website->getSmarty()->assign( 'AchievementsList', $achievementsList );

$Website->setTemplateToInclude( 'useraccount' );

 ?>
 
