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
|   > includes/browsemaps.inc.php : custommaps file
|   > Description: Lists the custom maps available for download
|   > Creation date: 29th July 2006
|   > Last modification date: 29th July 2006
|   > File author(s): mathieumg <mathieumg@qc-net.com>
+------------------------------------------------------------------------------
*/

// Security measure to protect includes
require_once("protect_includes.inc.php");


$mapsperpage = 100;

if( empty($_GET["page"]) )
{
	$page = 1;
}
else
{
	$page = $_GET["page"];
}

if(isset($_POST["sortparam"]))
{
  $param = $_POST["sortparam"];
  
  $urlchanger = "";
  if($param == "rating")
  {
    $urlchanger = "-r";
  }
  elseif($param == "namea")
  {
    $urlchanger = "-a";
  }
  elseif($param == "namez")
  {
    $urlchanger = "-a-d";
  }
  
  $this->vChangePage("browsemaps-" . $page . $urlchanger . ".html");
}

$sortmode = "";
$sortorder = "";
if($_GET["sort"] == "a")
{
  $sortmode = "a";
  if($_GET["order"] == "d")
  {
    $sortorder = "d";
    $orderby = "author DESC";
  }
  else
  {
    $orderby = "author ASC";
  }
}
elseif($_GET["sort"] == "r")
{
  $sortmode = "r";
  $orderby = "avgrating DESC, ratings_count DESC, name";
}
else
{
  $orderby = "id DESC";
}

$this->oSmarty->assign('sortmode', $sortmode );
$this->oSmarty->assign('sortorder', $sortorder );

$startmap = abs(intval($page) - 1) * $mapsperpage;
$endmap = $startmap + $mapsperpage;

$mysqli = new mysqli(DBHOST, DBUSR, DBPWD, DBNAME);

$totalentries = $mysqli->query("SELECT id FROM custom_maps WHERE accepted='1' LIMIT 10000");
$totalentries_count = $totalentries->num_rows;

$totalpages = ceil($totalentries_count/$mapsperpage);

$result = $mysqli->query("SELECT id, name, author, description, ratings_count, avgrating FROM custom_maps WHERE accepted='1' ORDER BY " . $orderby . " LIMIT " . $startmap . "," . $endmap);

for($i=1; $i < $totalpages; $i++)
{
	$finalpages[] = $i;
}

$this->oSmarty->assign('finalpages', $finalpages );
$this->oSmarty->assign('currentpage', $page );

while ($maps = $result->fetch_assoc() )
{
	$results[] = $maps;
}

$this->oSmarty->assign('results', $results );

$result->close();

$mysqli->close();

// Template used in the page
$this->sTemplateToInclude = "browsemaps";
?>