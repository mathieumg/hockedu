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
|   > includes/download.inc.php : play file
|   > Description: Contains code and display instructions for the play page
|   > Creation date: 12th July 2007
|   > Last modification date: 12th July 2007
|   > File author(s): mathieumg <mathieumg@qc-net.com>
+------------------------------------------------------------------------------
*/

// Security measure to protect includes
require_once("protect_includes.inc.php");

if(isset($_GET["mapid"]))
{
	$mysqli = new mysqli(DBHOST, DBUSR, DBPWD, DBNAME);
		
	$result = $mysqli->query("SELECT mapcode FROM custom_maps WHERE id='" . addslashes($_GET["mapid"]) . "' LIMIT 1");

	$code = $result->fetch_array(MYSQLI_ASSOC); 

	$mapcode = $code["mapcode"];
	
	$this->oSmarty->assign('loadmapcode', $mapcode);
}

// Template used in the page
$this->sTemplateToInclude = "play";
?>