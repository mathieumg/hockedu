<?php
/**
 * Achievements page.
 * 
 * This file is the Hockedu page that lists all the possible achievements in the game.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 07/04/2013
 * @package Hockedu
 */

// Title in the browser.
//$Website->setBrowserTitle( 'Achievements' );

$Common = Common::getInstance();

$achievementsList = $Common->getAchievementList();

$Website->getSmarty()->assign( 'AchievementsList', $achievementsList );

$Website->setTemplateToInclude( 'achievements' );

 ?>
 
