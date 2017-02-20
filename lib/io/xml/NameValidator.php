<?php

/**
 * Created by PhpStorm.
 * User: mrnda
 * Date: 20/02/2017
 * Time: 12:18
 */


class NameValidator
{
    const start_regex = '/^[^\p{L}|\_]/';
    const valid_regex = '/<|>|"|\'|\/|\\|&|&/';

    public static function invalidate($string)
    {
        if(preg_match(self::start_regex, $string)
            || preg_match(self::valid_regex, $string))
        {
            return false;
        }

        return true;
    }

    public static function invalidateAndReplace($string)
    {
        $arguments = Arguments::getInstance();
        $substitution = $arguments->getSubstCharacter();
        $string = preg_replace(self::valid_regex, $substitution, $string);
        $string = preg_replace(self::start_regex, $substitution, $string);
        if(preg_match(self::valid_regex, $string)
            || preg_match(self::start_regex, $string))
        {
            //TODO throw exception
        }
        return $string;
    }
}