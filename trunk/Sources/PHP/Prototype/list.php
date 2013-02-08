<?php

// create an array to hold directory list
$results = array();

// create a handler for the directory
$handler = opendir('uploads/');

// open directory and walk through the filenames
while ($file = readdir($handler)) {

  // if file isn't this directory or its parent, add it to the results
  if ($file != "." && $file != "..") {
	$baseName = explode( '.', $file );
	$results[] = array( 
		'name' => $baseName[0], 
		'when' => date("F d Y H:i:s", filemtime('uploads/' . $file) + 10800 )
	);
  }

}

// tidy up: close the handler
closedir($handler);

echo '<ul>';

$results = array_reverse( $results );

foreach( $results as $fileEntry )
{
	echo '<li><a href="/i/' . $fileEntry['name'] . '">' . $fileEntry['name'] . ' (' . $fileEntry['when'] . ')</li>';
}

echo '</ul>';

?>
