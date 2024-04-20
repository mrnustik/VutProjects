<?php

/**
 * Created by PhpStorm.
 * User: mrnda
 * Date: 20/02/2017
 * Time: 12:18
 */


/**
 * Class NameValidator
 * Static class for XML element names validation.
 */
class NameValidator
{
    const start_regex = '/^[^\p{L}|\_]/';
    const valid_regex = '/<|>|"|\'|\/|\\|&|&/';

    /**
     * Method used for XML element name validation checking.
     * @param $string string element name
     * @return bool true if is valid XML element name.
     */
    public static function invalidate($string)
    {
        if(preg_match(self::start_regex, $string)
            || preg_match(self::valid_regex, $string))
        {
            return false;
        }

        return true;
    }

    /**
     * Static method used for making valid XML element name when invalid.
     * @param $string string XML element
     * @return string validated string
     * @throws InvalidJsonSubstitutionException when was unable to perform validation.
     */
    public static function invalidateAndReplace($string)
    {
        $arguments = Arguments::getInstance();
        $substitution = $arguments->getSubstCharacter();
        $string = preg_replace(self::valid_regex, $substitution, $string);
        $string = preg_replace(self::start_regex, $substitution, $string);
        if(preg_match(self::valid_regex, $string)
            || preg_match(self::start_regex, $string))
        {
            throw new InvalidJsonSubstitutionException("Invalid JSON after substitution");
        }
        return $string;
    }
}