<?php
/**
 * Login page.
 * 
 * This file is the Hockedu page that allows users to log out.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 07/04/2013
 * @package Hockedu
 */
 
unset( $_SESSION['userInformation'] );

$Website->changePage( $Website->getSetting('MainDomain') ); 

 ?>
 