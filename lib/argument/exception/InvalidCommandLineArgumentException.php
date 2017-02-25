<?php

/**
 * Created by PhpStorm.
 * User: mrnda
 * Date: 20/02/2017
 * Time: 13:46
 */
class InvalidCommandLineArgumentException extends Exception
{
    public function __construct($message = "", $code = 1, Exception $previous = null)
    {
        parent::__construct($message, $code, $previous);
    }
}