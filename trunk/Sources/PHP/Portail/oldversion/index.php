<?php
/*
+------------------------------------------------------------------------------
|   Smileys-War
|   ==================================================
|   by
|     Louis-Simon Ménard
|     Mathieu M-Gosselin
|   (C) 2007 - Interactive-Pixels
+------------------------------------------------------------------------------
|   > Version: 0.1.2 (03/10/2006)
|   > Changelog: Changelog.txt
+------------------------------------------------------------------------------
|   > index.php : Index file
|   > Description: Manage main actions, include configuration files
|   > Creation date: 15th August 2006
|   > Last modification date: 1st October 2006
|   > File author(s): mathieumg <mathieumg@qc-net.com>
+------------------------------------------------------------------------------
*/

//Include the "website" class, the one that will be managing all actions related to the website itself
require_once("classes/website.class.php");

$oSwar = new website();
		
// Load the website configuration.
if( $oSwar->bLoadConfig() == true )
{
	//Site configuration loaded, load the modules and let the thing roll!
	$oSwar->vLoadModules();
	
	// Manage the requests and load the pages
	$oSwar->vStartSite();
	$oSwar->vDisplaySite();
}
else
{
	// Impossible to load configuration file.
	die("Impossible to load the configuration file for the website. Please make sure that it was correctly installed, that the file is still there and that is has read permissions.");
}
?>