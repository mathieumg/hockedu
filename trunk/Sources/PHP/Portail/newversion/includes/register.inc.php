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
 
// Was the form submitted?
if( isset( $_POST['submit'] ) )
{
	// Yes.
    $formError = false;
	
	// Initiate the DB.
	$Common = Common::getInstance();
	
	// Validate the username field.
    if( empty( $_POST['username'] ) )
    {
        // Username cannot be left empty.
        $formError = true;
        $Website->getSmarty()->assign( 'UsernameError', 'You have to specify a username.' );
    }
	elseif( strlen( $_POST['username'] ) < 5 )
    {
        // Username must be longer.
        $formError = true;
        $Website->getSmarty()->assign( 'UsernameError', 'Your username must be at least 5 characters long.' );
    }
    elseif( strlen( $_POST['username'] ) > 30 )
    {
        // Username must be shorter.
        $formError = true;
        $Website->getSmarty()->assign( 'UsernameError', 'Your username cannot exceed 30 characters in length.' );
    }
	elseif( $Common->userExists( $_POST['username'] ) )
    {
        // Username already exists.
        $formError = true;
        $Website->getSmarty()->assign( 'UsernameError', 'That username already exists.' );
    }
    else
    {
        // Username is good, keep it.
        $Website->getSmarty()->assign( 'GoodUsername', $_POST['username'] );
    }
	
	// Validate the password field.
    if( empty( $_POST['password'] ) )
    {
        // Password cannot be left empty.
        $formError = true;
        $Website->getSmarty()->assign( 'PasswordError', 'You have to specify a password.' );
    }
	elseif( strlen( $_POST['password'] ) < 6 )
    {
        // Password must be longer.
        $formError = true;
        $Website->getSmarty()->assign( 'PasswordError', 'Your password must be at least 6 characters long.' );
    }
    elseif( $_POST['password'] != $_POST['passwordconf'] )
    {
        $Website->getSmarty()->assign( 'GoodPassword', $_POST['password'] );
		
		// Passwords do not match.
        $formError = true;
        $Website->getSmarty()->assign( 'PasswordConfirmationError', 'Your passwords do not match.' );
    }
    else
    {
        // Password is good, keep it.
		$Website->getSmarty()->assign( 'GoodPassword', $_POST['password'] );
        $Website->getSmarty()->assign( 'GoodPasswordConfirmation', $_POST['password'] );
    }
	
	// Validate the email field.
    if( empty( $_POST['email'] ) )
    {
        // Email cannot be left empty.
        $formError = true;
        $Website->getSmarty()->assign( 'EmailError', 'You have to specify a email.' );
    }
    elseif( !preg_match('/^(?!(?:(?:\x22?\x5C[\x00-\x7E]\x22?)|(?:\x22?[^\x5C\x22]\x22?)){255,})(?!(?:(?:\x22?\x5C[\x00-\x7E]\x22?)|(?:\x22?[^\x5C\x22]\x22?)){65,}@)(?:(?:[\x21\x23-\x27\x2A\x2B\x2D\x2F-\x39\x3D\x3F\x5E-\x7E]+)|(?:\x22(?:[\x01-\x08\x0B\x0C\x0E-\x1F\x21\x23-\x5B\x5D-\x7F]|(?:\x5C[\x00-\x7F]))*\x22))(?:\.(?:(?:[\x21\x23-\x27\x2A\x2B\x2D\x2F-\x39\x3D\x3F\x5E-\x7E]+)|(?:\x22(?:[\x01-\x08\x0B\x0C\x0E-\x1F\x21\x23-\x5B\x5D-\x7F]|(?:\x5C[\x00-\x7F]))*\x22)))*@(?:(?:(?!.*[^.]{64,})(?:(?:(?:xn--)?[a-z0-9]+(?:-[a-z0-9]+)*\.){1,126}){1,}(?:(?:[a-z][a-z0-9]*)|(?:(?:xn--)[a-z0-9]+))(?:-[a-z0-9]+)*)|(?:\[(?:(?:IPv6:(?:(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){7})|(?:(?!(?:.*[a-f0-9][:\]]){7,})(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,5})?::(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,5})?)))|(?:(?:IPv6:(?:(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){5}:)|(?:(?!(?:.*[a-f0-9]:){5,})(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,3})?::(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,3}:)?)))?(?:(?:25[0-5])|(?:2[0-4][0-9])|(?:1[0-9]{2})|(?:[1-9]?[0-9]))(?:\.(?:(?:25[0-5])|(?:2[0-4][0-9])|(?:1[0-9]{2})|(?:[1-9]?[0-9]))){3}))\]))$/iD', $_POST['email']) )
    {
        // Email must be valid. See http://fightingforalostcause.net/misc/2006/compare-email-regex.php.
        $formError = true;
        $Website->getSmarty()->assign( 'EmailError', 'The email format is invalid.' ); 
    }
    elseif( strlen( $_POST['email'] ) > 255 )
    {
        // Email must be shorter.
        $formError = true;
        $Website->getSmarty()->assign( 'EmailError', 'Your email cannot exceed 255 characters in length.' );
    }
	elseif( $Common->emailExists( $_POST['email'] ) )
    {
        // Email already exists.
        $formError = true;
        $Website->getSmarty()->assign( 'EmailError', 'There is already another user registered with that email.' );
    }
    else
    {
        // Email is good, keep it.
        $Website->getSmarty()->assign( 'GoodEmail', $_POST['email'] );
    }
	
	if( $formError )
	{
		$Website->getSmarty()->assign( 'FormError', $formError );
	}
	else
	{
		$Common->registerUser( $_POST['username'], $_POST['password'], $_POST['email'] );
		
		$Website->getSmarty()->assign( 'RegistrationSuccess', true );
		mail('mathieumg@gmail.com', 'Hockedu registration', 'User: ' . $_POST['username'] . "\n" . 'Email: ' . $_POST['email'] . "\n" ); 
	
	    // Let the template know that everything is done.
        $Website->getSmarty()->assign( 'FormCompleted', true );
	}
}

$Website->setTemplateToInclude( 'register' );

 ?>
 
