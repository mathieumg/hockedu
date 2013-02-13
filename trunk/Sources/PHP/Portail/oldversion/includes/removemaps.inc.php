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
|   > includes/uploadmaps.inc.php : removemaps file
|   > Description: Allows the users to remove their maps
|   > Creation date: 12th August 2007
|   > Last modification date: 12th August 2007
|   > File author(s): mathieumg <mathieumg@qc-net.com>
+------------------------------------------------------------------------------
*/

// Security measure to protect includes
require_once("protect_includes.inc.php");

session_start();

if( isset($_POST["go"]) )
{

	$removalcode = addslashes($_POST["removalcode"]);

	if( empty($_POST["removalcode"]) || empty($_POST["captchacode"]) )
	{
		$this->oSmarty->assign('error', "One or more fields were not filled.");
	}
	elseif( strtoupper($_POST["captchacode"]) != $_SESSION["captchakeycode"] )
	{
		$this->oSmarty->assign('error', "The code entered doesn't match the one shown on the image, try again.");
	}
	else
	{

		$mysqli = new mysqli(DBHOST, DBUSR, DBPWD, DBNAME);
		$result = $mysqli->query("SELECT id, removalcode FROM custom_maps WHERE removalcode='" . $removalcode . "' AND accepted='1' LIMIT 1");
		
		if( $result->num_rows != 1 )
		{
			$this->oSmarty->assign('error', "A map with that removal code does not exist.");
		}
		else
		{
			$this->oSmarty->assign('success', "Your map was successfully removed! Sorry for any inconvenience.");
			$mysqli->query("UPDATE custom_maps SET accepted='0' WHERE removalcode='" . $removalcode . "' AND accepted='1'");
		}
		
		
		$mysqli->close();
	}
}

// Template used in the page
$this->sTemplateToInclude = "removemaps";
?>