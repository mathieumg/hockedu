<?php

require_once( '/var/www/hockedu.com/classes/CacheStore.class.php' ); 


$test = new CacheStore( 'settings' );


$settings = array(  'MainDomain' => 'hockedu.com', 
                    'ContentSubDomain' => 'content',
                    'CommonDBDSN' => array(
                        'phptype' => 'mysqli',
                        'option' => 'charset=utf8',
                        'hostspec' => 'localhost',
                        'port' => '3306',
                        'username' => 'hockedu',
                        'password' => 'yUhdmyXtQxJpBBxN',
                        'database' => 'poly_hockedu'
                    ),
                    'CommonDBOptions' => array()
                    );

$test->setCacheData( $settings );
$test->save();

 ?>