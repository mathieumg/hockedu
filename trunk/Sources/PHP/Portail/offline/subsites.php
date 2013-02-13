<?php

require_once( '/var/www/cloud/classes/CacheStore.class.php' ); 


$test = new CacheStore( 'subsites' );


$subsites = array( 'globalrecreatif' => 'Global Récréatif',
                    'rcaaq' => 'RCAAQ', 
                    'gps' => 'GPS Logistique', 
                    'encan' => 'Encans Berpa' );

$test->setCacheData( $subsites );
$test->save();

 ?>