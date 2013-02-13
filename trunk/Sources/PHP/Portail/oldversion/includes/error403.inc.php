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
|   > includes/error403.inc.php : 403 error file
|   > Description: Displays an error whenever access to a file/page is not allowed
|   > Creation date: 21st October 2006
|   > Last modification date: 21st October 2006
|   > File author(s): mathieumg <mathieumg@qc-net.com>
+------------------------------------------------------------------------------
*/

// Security measure to protect includes
require_once("protect_includes.inc.php");

// Template used in the page
$this->sTemplateToInclude = "error403";
?>