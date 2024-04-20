<?php

/**
 * Class InvalidCommandLineArgumentException
 * Exception that is used to represent invalid arguments of the script.
 */
class InvalidCommandLineArgumentException extends Exception
{
    /**
     * InvalidCommandLineArgumentException constructor. Creates the exception.
     * @param string $message Message that will be displayed in the Logger.
     * @param int $code Code of the issue that occured (default 1)
     */
    public function __construct($message = "", $code = 1)
    {
        parent::__construct($message, $code, null);
    }
}