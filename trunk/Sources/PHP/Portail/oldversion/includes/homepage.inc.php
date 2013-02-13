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
|   > includes/homepage.inc.php : homepage file
|   > Description: Contains code and display instructions for the homepage
|   > Creation date: 8th October 2006
|   > Last modification date: 21st October 2006
|   > File author(s): mathieumg <mathieumg@qc-net.com>
+------------------------------------------------------------------------------
*/

// Security measure to protect includes
require_once("protect_includes.inc.php");

$mysqli = new mysqli(DBHOST, DBUSR, DBPWD, DBNAME);

$result = $mysqli->query("SELECT title, content, author, postdate FROM news ORDER BY id DESC LIMIT 10");

while ($news= $result->fetch_assoc() )
{
	$results[]= $news;
}

$this->oSmarty->assign('results', $results );

$result->close();

$mysqli->close();

// Template used in the page
$this->sTemplateToInclude = "homepage";
?>