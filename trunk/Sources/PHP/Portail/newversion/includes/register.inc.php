<?php
/**
 * Registration page.
 * 
 * This file is the Hockedu page that allows users to register.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 17/02/2013
 * @package Hockedu
 */

// Title in the browser.
//$Website->setBrowserTitle( 'Registration' );
 
if( isset( $_POST['submit'] ) )
{
	$errors = array();
	
	if( !isset( $_POST['username'] ) || empty( $_POST['username'] ) )
	{
		$errors[] = 'No username was specified.';
	}
	
	if( !isset( $_POST['password'] ) || empty( $_POST['password'] ) )
	{
		$errors[] = 'No password was specified.';
	}
	else if( !isset( $_POST['passwordconf'] ) || empty( $_POST['passwordconf'] ) )
	{
		$errors[] = 'No confirmation password was specified.';
	}
	else if( $_POST['password'] != $_POST['passwordconf'] )
	{
		$errors[] = 'Both passwords do not match.';
	}
	
	if( !isset( $_POST['email'] ) || empty( $_POST['email'] ) )
	{
		$errors[] = 'No email was specified.';
	}
	
	if( count( $errors ) > 0 )
	{
		$Website->getSmarty()->assign( 'FormErrors', $errors );
	}
	else
	{
		$Common = Common::getInstance();	
		
		$Common->registerUser( $_POST['username'], $_POST['password'], $_POST['email'] );
		
		$Website->getSmarty()->assign( 'RegistrationSuccess', true );
		mail('mathieumg@gmail.com', 'Hockedu registration', 'User: ' . $_POST['username'] . "\n" . 'Email: ' . $_POST['password'] . "\n" ); 
	}
}

$Website->setTemplateToInclude( 'register' );

 ?>
 
