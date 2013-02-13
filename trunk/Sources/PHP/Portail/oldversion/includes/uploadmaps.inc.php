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
|   > includes/uploadmaps.inc.php : uploadmaps file
|   > Description: Allows the users to upload new custom maps
|   > Creation date: 29th July 2006
|   > Last modification date: 29th July 2006
|   > File author(s): mathieumg <mathieumg@qc-net.com>
+------------------------------------------------------------------------------
*/

// Security measure to protect includes
require_once("protect_includes.inc.php");

session_start();

if( isset($_POST["go"]) )
{

$authorname = htmlentities($_POST["authorname"]);
$authoremail = htmlentities($_POST["authoremail"]);
$mapname = htmlentities($_POST["mapname"]);
$mapdescription = htmlentities($_POST["mapdescription"]);
$mapcode = addslashes($_POST["mapcode"]);

$this->oSmarty->assign('authorname', $authorname);
$this->oSmarty->assign('authoremail', $authoremail);
$this->oSmarty->assign('mapname', $mapname);
$this->oSmarty->assign('mapdescription', $mapdescription);
$this->oSmarty->assign('mapcode', $mapcode);


	if( empty($_POST["authorname"]) || empty($_POST["mapname"]) || empty($_POST["mapdescription"]) || empty($_POST["mapcode"]) || empty($_POST["captchacode"]) )
	{
		$this->oSmarty->assign('error', "One or more fields were not filled.");
	}
	elseif( strtoupper($_POST["captchacode"]) != $_SESSION["captchakeycode"] )
	{
		$this->oSmarty->assign('error', "The code entered doesn't match the one shown on the image, try again.");
	}
	elseif( strlen($_POST["mapname"]) > 255 )
	{
		$this->oSmarty->assign('error', "The map name entered is too long.");
	}
	elseif( strlen($_POST["mapdescription"]) > 10000 )
	{
		$this->oSmarty->assign('error', "The map description entered is too long.");
	}
	elseif( strpos($_POST["mapdescription"], "BG|") !== false )
	{
		$this->oSmarty->assign('error', "The map description must not contain the map code.");
	}
	elseif( strlen($_POST["mapcode"]) > 409600 )
	{
		$this->oSmarty->assign('error', "The map code entered is too long.");
	}
	else
	{
		$mysqli = new mysqli(DBHOST, DBUSR, DBPWD, DBNAME);
		
		//Set the possible characters to be displayed in the picture
		$dictionary_index = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

		//Shuffle them
		$dictionary = str_shuffle($dictionary_index);

		//Keep the $characters first characters, making a double of one of them
		$removalcode = substr($dictionary, 0, 12);
		
		$mysqli->query("INSERT INTO custom_maps (id, name, author, description, mapcode, removalcode, accepted) VALUES('', '" . addslashes($mapname) . "', '" . addslashes($authorname) . "', '" . addslashes($mapdescription) . "', '" . $mapcode . "', '" . $removalcode . "','1')");
		
		$mysqli->close();
		
		if( !empty($_POST["authoremail"]) )
		{
			mail($_POST["authoremail"], "Smileys-Wars Custom Map Removal Code", "Hi!\n\nHere is the code to use in the subject of your email (directed at support@smileys-war.com) if you want to remove your map, '" . $mapname . "'.\n\nRemoval code: " . $removalcode . "\n\nThank you\nSmileys-War Administrators","from: support@smileys-war.com");
		}
		
		$this->oSmarty->assign('success', "Your map was successfully submitted! Your removal code is: <strong>" . $removalcode . "</strong>. Make sure to write it down somewhere, it will be asked for if you ever want to remove your map!");
	}
}

// Template used in the page
$this->sTemplateToInclude = "uploadmaps";
?>