<?php
/**
 * Homepage.
 * 
 * This file is the Hockedu page that welcomes people.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 17/02/2013
 * @package Hockedu
 */

$shitsAngGiggles = $Website->getModules( 0 );

if( $shitsAngGiggles !== false && $shitsAngGiggles !== 'homepage' )
{
    $Website->getSmarty()->assign( 'Example', $shitsAngGiggles );
}

$Website->setTemplateToInclude( 'homepage' );

 ?>
 
