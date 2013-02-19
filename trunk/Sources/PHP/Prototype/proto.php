<?php

/*

typedef enum {
   UIImageOrientationUp,
   UIImageOrientationDown,   // 180 deg rotation
   UIImageOrientationLeft,   // 90 deg CW
   UIImageOrientationRight,   // 90 deg CCW
   UIImageOrientationUpMirrored,    // as above but image mirrored along
   // other axis. horizontal flip
   UIImageOrientationDownMirrored,  // horizontal flip
   UIImageOrientationLeftMirrored,  // vertical flip
   UIImageOrientationRightMirrored, // vertical flip
} UIImageOrientation;

*/


set_time_limit ( 3600 );

ini_set ( 'memory_limit' , '64M' );
ini_set ( 'upload_max_filesize' , '40M' );
ini_set ( 'post_max_size' , '50M' );

function imageRot( $fileName, $degrees )
{
	// Content type
	//header('Content-type: image/jpeg');

	// Load
	$source = imagecreatefromjpeg($fileName);

	// Rotate
	$rotate = imagerotate($source, $degrees, 0);

	// Output
	imagejpeg($rotate, $fileName);
}

define("MIRROR_HORIZONTAL", 1);
define("MIRROR_VERTICAL", 2);
define("MIRROR_BOTH", 3);

function imageFlip($fileName, $type)
{
  $imgsrc = imagecreatefromjpeg($fileName);
  $width = imagesx($imgsrc);
  $height = imagesy($imgsrc);
  $imgdest = imagecreatetruecolor($width, $height);
  
  for ($x=0 ; $x<$width ; $x++)
    {
      for ($y=0 ; $y<$height ; $y++)
    {
      if ($type == MIRROR_HORIZONTAL) imagecopy($imgdest, $imgsrc, $width-$x-1, $y, $x, $y, 1, 1);
      if ($type == MIRROR_VERTICAL) imagecopy($imgdest, $imgsrc, $x, $height-$y-1, $x, $y, 1, 1);
      if ($type == MIRROR_BOTH) imagecopy($imgdest, $imgsrc, $width-$x-1, $height-$y-1, $x, $y, 1, 1);
    }
    }
  
  imagejpeg($imgdest, $fileName);
  
  imagedestroy($imgsrc);
  imagedestroy($imgdest);
}

$uploaddir = 'uploads/';
//$ext = explode( $_FILES['myfile']['name'], '.' );
$uniqueId = substr(uniqid(), -6);
//$uploadfile = $uploaddir . $uniqueId . '_' . $_POST['orientation'] . '.jpg';
$uploadfile = $uploaddir . $uniqueId . '.jpg';

$response = array();

if (move_uploaded_file($_FILES['myfile']['tmp_name'], $uploadfile)) 
{
	$exif = exif_read_data($uploadfile);
	$ort = $exif['IFD0']['Orientation'];

	switch($ort)
	{
		case 1: // nothing
			break;

		case 2: // horizontal flip
			imageFlip($uploadfile,1);
			break;
								
		case 3: // 180 rotate left
			imageRot($uploadfile,180);
			break;
					
		case 4: // vertical flip
			imageFlip($uploadfile,2);
			break;
				
		case 5: // vertical flip + 90 rotate right
			imageFlip($uploadfile, 2);
			imageRot($uploadfile, -90);
			break;
				
		case 6: // 90 rotate right
			imageRot($uploadfile, -90);
			break;
				
		case 7: // horizontal flip + 90 rotate right
			imageFlip($uploadfile,1);    
			imageRot($uploadfile, -90);
			break;
				
		case 8: // 90 rotate left
			imageRot($uploadfile, 90);
			break;
	}
	
    //$response['location'] = 'http://hockedu.com/view.php?id=' . $uniqueId . '&o=' . $_POST['orientation'];
    $response['location'] = 'http://old.hockedu.com/i/' . $uniqueId;
}
else {
    $response['error'] = 'Error dude';
}

echo json_encode( $response );

?>
