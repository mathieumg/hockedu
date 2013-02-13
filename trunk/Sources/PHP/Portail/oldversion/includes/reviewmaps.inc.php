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
|   > includes/browsemaps.inc.php : reviewmaps file
|   > Description: List a map's details and allows users to review and rate it
|   > Creation date: 12th August 2007
|   > Last modification date: 12th August 2007
|   > File author(s): mathieumg <mathieumg@qc-net.com>
+------------------------------------------------------------------------------
*/

// Security measure to protect includes
require_once("protect_includes.inc.php");

session_start();

$mysqli = new mysqli(DBHOST, DBUSR, DBPWD, DBNAME);

$result = $mysqli->query("SELECT id, name, author, description, ratings_count, avgrating, mapcode FROM custom_maps WHERE id='" . addslashes($_GET["mapid"]) . "'AND accepted='1' LIMIT 1");

while ($maps = $result->fetch_assoc() )
{
	$results[] = $maps;
}

$totalratings = $results[0]["ratings_count"];
$previousrating = $results[0]["avgrating"];

$this->oSmarty->assign('results', $results );

if( isset($_POST["go"]) && $result->num_rows != 0 )
{
	$authorname = htmlentities($_POST["authorname"]);
	$mapreview = htmlentities($_POST["mapreview"]);

	$this->oSmarty->assign('authorname', $authorname);
	$this->oSmarty->assign('mapreview', $mapreview);
	
	if( !empty($_POST["mapscore"]) )
	{
		$mapscore = intval($_POST["mapscore"]);
		$this->oSmarty->assign('mapscore', $mapscore);
	}
	
	if( empty($_POST["authorname"]) || empty($_POST["mapreview"]) || empty($_POST["mapscore"]) || empty($_POST["captchacode"]) )
	{
		$this->oSmarty->assign('error', "One or more fields were not filled.");
	}
	elseif( $mapscore < 1 || $mapscore > 10 )
	{
		$this->oSmarty->assign('error', "Invalid map rating.");
	}
	elseif( strtoupper($_POST["captchacode"]) != $_SESSION["captchakeycode"] )
	{
		$this->oSmarty->assign('error', "The code entered doesn't match the one shown on the image, try again.");
	}
	elseif( strlen($_POST["authorname"]) > 60 )
	{
		$this->oSmarty->assign('error', "The name entered is too long.");
	}
	elseif( strlen($_POST["mapreview"]) > 2000 )
	{
		$this->oSmarty->assign('error', "The map review entered is too long.");
	}
	else
	{
		$alreadyreview = $mysqli->query("SELECT mapid, authorip FROM maps_reviews WHERE mapid='" . addslashes($_GET["mapid"]) . "'AND authorip='" . $_SERVER["REMOTE_ADDR"] . "' AND accepted='1' LIMIT 1");
		
		if( $alreadyreview->num_rows != 0)
		{
			$this->oSmarty->assign('error', "You have already rated and reviewed this map.");
		}
		else
		{					

			$finalmapscore = round(((($previousrating * $totalratings) + $mapscore)/($totalratings + 1)), 2);

			$results[0]["ratings_count"] = $totalratings + 1;
			$results[0]["avgrating"] = $finalmapscore;

			$this->oSmarty->assign('results', $results );
			
			$mysqli->query("UPDATE custom_maps SET avgrating='" . $finalmapscore . "', ratings_count='". ($totalratings+1) . "' WHERE id='" . addslashes($_GET["mapid"]) . "' AND accepted='1' LIMIT 1");
			$mysqli->query("INSERT INTO maps_reviews (id, mapid, authorname, authorip, review, score, reviewdate, accepted) VALUES('', '" . addslashes($_GET["mapid"]) . "', '" . addslashes($authorname) . "', '" . $_SERVER["REMOTE_ADDR"] . "', '" . addslashes($mapreview) . "', '" . $mapscore . "', '" . date('Y-m-d') . "','1')");
			
			$this->oSmarty->assign('success', "Thank you for taking the time to rate and review this map!");
		}
	}
	
}

$result = $mysqli->query("SELECT id, authorname, review, score, reviewdate FROM maps_reviews WHERE mapid='" . addslashes($_GET["mapid"]) . "' AND accepted='1' ORDER BY id DESC LIMIT 5000");

while ($reviews = $result->fetch_assoc() )
{
	$reviewsresults[] = $reviews;
}

$this->oSmarty->assign('reviewsresults', $reviewsresults );

$mysqli->close();

// Template used in the page
$this->sTemplateToInclude = "reviewmaps";
?>