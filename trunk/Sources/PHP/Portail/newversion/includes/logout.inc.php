<?php
/**
 * Logout page.
 * 
 * This file handles the logout process for the management system.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 26/07/2011
 * @package Cloud
 */

$nextPage = $Website->getSubsiteUrl( $_SESSION['userInformation']['lastSubsite'], false );
 
unset( $_SESSION['userInformation'] );

$Website->changePage( $nextPage ); 

 ?>
 