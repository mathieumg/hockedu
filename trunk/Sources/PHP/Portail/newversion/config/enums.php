<?php
/**
 * Enumerations.
 * 
 * Constants used throughout the Hockedu system.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 17/02/2013
 * @package Hockedu
 */
 
 
abstract class CommonLogTypes
{
    const Login = 0;
}
 
abstract class IpStatus
{
    const Enabled = 0;
    const Disabled = 1;
}
 
abstract class LoginResponse
{
    const Success = 0;
    const Failure = 1;
}

abstract class UsernameStatus
{
    const Enabled = 0;
    const Disabled = 1;
}
 
