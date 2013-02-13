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
|   > includes/error404.inc.php : 404 error file
|   > Description: Displays an error whenever a page is not found
|   > Creation date: 8th October 2006
|   > Last modification date: 21st October 2006
|   > File author(s): mathieumg <mathieumg@qc-net.com>
+------------------------------------------------------------------------------
*/

// Security measure to protect includes
require_once("protect_includes.inc.php");

// Template used in the page
$this->sTemplateToInclude = "error404";
?>