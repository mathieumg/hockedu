<?php
/**
 * Enumerations.
 * 
 * Constants used throughout Global Récréatif.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 30/08/2012
 * @package Cloud
 */
 
 
abstract class GlobalRecreatifClientType
{
    const Personal = 0;
    const Business = 1;
}

abstract class GlobalRecreatifContractStatus
{
    const NotYetStarted = 1;
    const OnGoing = 2;
    const Expired = 3;
    const Over = 4;
}

abstract class GlobalRecreatifInvoiceStatus
{
    const Prepared = 1;
    const Paid = 2;
    const Canceled = 3;
}
 
?>
 
