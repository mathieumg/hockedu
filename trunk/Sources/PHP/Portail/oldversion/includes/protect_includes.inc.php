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
|   > includes/protect_includes.inc.php : page to be included in pages to protect
|   > Description: Makes sure that the page this file is included in, is included in the main website
|   > Creation date: 8th October 2006
|   > Last modification date: 8th October 2006
|   > File author(s): mathieumg <mathieumg@qc-net.com>
+------------------------------------------------------------------------------
*/

// If the IN_SWAR constant is not defined, this means that the current page has not been included in the main site. Therefore, this is NOT good, kill the site.
if(!defined('IN_SWAR'))
{
	//Kill the site with a message
	die("Why the hell would you, " . $_SERVER["REMOTE_ADDR"] . ", directly open this page? This is none of your business!");
}
?>