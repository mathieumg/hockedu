<?php
/**
 * Enumerations.
 * 
 * Constants used throughout the management system.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 14/07/2011
 * @package Cloud
 */
 
 
abstract class CommonLogTypes
{
    const Login = 0;
}
 
abstract class IpStatus
{
    const Enabled = 0;
    const Disabled = 1;
    const Pending = 2;
}
 
abstract class IpType
{
    const Unknown = 0;
    const Whitelisted = 1;
    const Blacklisted = 2;
}
 
abstract class LoginResponse
{
    const Success = 0;
    const NewIp = 1;
    const NoSubsite = 2;
    const Failure = 3;
}

abstract class UsernameStatus
{
    const Enabled = 0;
    const Disabled = 1;
}

// Include enums from subsites.
foreach ( glob('../config/*_enums.php') as $filename)
{
    include_once( $filename );
}
 
 ?>
 