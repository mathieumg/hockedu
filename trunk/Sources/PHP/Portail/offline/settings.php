<?php

require_once( '/var/www/cloud/classes/CacheStore.class.php' ); 


$test = new CacheStore( 'settings' );


$settings = array(  'MainDomain' => 'hockedu.com', 
                    'MainSubDomain' => 'gestion', 
                    'ContentSubDomain' => 'content',
                    'CommonDBDSN' => array(
                        'phptype' => 'mysqli',
                        'option' => 'charset=utf8',
                        'hostspec' => 'localhost',
                        'port' => '3306',
                        'username' => 'root',
                        'password' => '',
                        'database' => 'gb_common'
                    ),
                    'CommonDBOptions' => array(),
                    'GlobalRecreatifDBDSN' => array(
                        'phptype' => 'mysqli',
                        'option' => 'charset=utf8',
                        'hostspec' => 'localhost',
                        'port' => '3306',
                        'username' => 'root',
                        'password' => '',
                        'database' => 'gb_globalrecreatif'
                    ),
                    'GlobalRecreatifDBOptions' => array()
                    );

$test->setCacheData( $settings );
$test->save();

 ?>