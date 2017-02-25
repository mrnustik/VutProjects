<?php

/**
 * Created by PhpStorm.
 * User: mrnda
 * Date: 19/02/2017
 * Time: 19:19
 */
class Logger
{
    const IS_DEBUG = true;

    public static function LogDebug($message)
    {
        if(IS_DEBUG)
        {
            printf("D: %s\n", $message);
        }
    }

    public static function LogError($message)
    {
        fprintf(STDERR, "E: %s\n", $message);
    }

    public static function printHelp()
    {
    }


}