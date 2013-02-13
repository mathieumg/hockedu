<?php

session_start();

//Setting variables
$height = 60;
$width = 140;

$verticallines = 3;
$horizontallines = 3;
$pixelspercentage = 0.15;

$characters = 7;
$fontsize = 20;
$linespacing = 1;

$dictionary_char;
$duplicated_character;

$debug = 0;	

//Build the $dictionary_char array
//Set the possible characters to be displayed in the picture
$dictionary_index = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

//Shuffle them
$dictionary = str_shuffle($dictionary_index);

//Keep the $characters first characters, making a double of one of them
$captchakey = substr($dictionary, 0, 7);

$_SESSION["captchakeycode"] = $captchakey;

//echo $captchakey;

//Define page header as a PNG Image
header("Content-type: image/png");

//Create image
$im = imagecreate($width,$height);

//Background color
$peach = imagecolorallocate($im, 200,236,126);
//Black text&lines&dots
$black = imagecolorallocate($im, 32, 96, 10);
//Red&Blue lines
$blue = imagecolorallocate($im, 0, 0, 255);
$red = imagecolorallocate($im, 255, 0, 0);

$left = 10;
//Put the characters one by one in the picture, and save the hitboxes for each

//Add the text to the image
imagefttext($im, $fontsize, rand(-10, 10), 10, 40, $black, "STAN.TTF", $captchakey, array("linespacing" => $linespacing));

//Add lines randomly ($horizontallines horizontals and $verticallines verticals)
		
//Vertical
$lines_top = 0;
$lines_bottom = $width;
for($i = 0;$i<$verticallines;$i++)
{
	$lines_top = rand($i*($width/$verticallines),($i+1)*($width/$verticallines));
	$lines_bottom = rand($width-$i*($width/$verticallines),($i-1)*($width/$verticallines));
	imageline($im, $lines_top, 0, $lines_bottom, $height, $black);
}

//Horizontal
$lines_left = 0;
$lines_right = $height;
for($i = 0;$i<$horizontallines;$i++)
{
	$lines_left = rand($i*($height/$horizontallines),($i+1)*($height/$horizontallines));
	$lines_right = rand($height-$i*($height/$horizontallines),($i-1)*($height/$horizontallines));
	imageline($im, 0, $lines_left, $width, $lines_right, $black);
}

//Add random pixels based on the $pixelspercentage
for($i = 0;$i<($width*$height)*$pixelspercentage;$i++)
{
	imagesetpixel($im, rand(0,$width), rand(0,$height), $black);
}

//Create the image and destroy the cache
ImagePng($im);
ImageDestroy($im);
?>
