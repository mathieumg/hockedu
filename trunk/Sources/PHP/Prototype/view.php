<?php

if( !file_exists( 'uploads/' . $_GET['id'] . '.jpg' ) )
{
	die('Cette image n\'existe pas!');
}

echo '<img src="/uploads/' . $_GET['id'] . '.jpg" />';

?>
