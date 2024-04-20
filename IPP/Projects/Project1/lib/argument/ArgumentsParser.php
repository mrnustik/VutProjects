<?php

/**
 * Created by PhpStorm.
 * User: mrnda
 * Date: 19/02/2017
 * Time: 19:13
 */

include_once "Arguments.php";

/**
 * Class ArgumentsParser
 * Static class used for arguments parsing from command line.
 */
class ArgumentsParser
{

    /**
     * Method used to retrieve Arguments object from command line.
     * @param $argc int arguments count
     * @return Arguments arguments parsed from command line
     * @throws InvalidCommandLineArgumentException when arguments from command line are invalid
     * @see Arguments
     */
    public static function parseArguments($argc)
    {
        $options = self::getOptions();

        if(count($options) != $argc - 1){
            throw new InvalidCommandLineArgumentException("Illegal arguments exception.");
        }

        $arguments = Arguments::getInstance();

        self::checkSubstCharacter($options, $arguments);
        self::checkRootElement($options, $arguments);
        self::checkArrayName($options, $arguments);
        self::checkItemName($options, $arguments);
        self::checkFlagHelp($options, $arguments);
        self::checkFlagNoHeader($options, $arguments);
        self::checkFlagElementText($options, $arguments);
        self::checkFlagElementNumber($options, $arguments);
        self::checkFlagElementLiteral($options, $arguments);
        self::checkFlagCharacterDecode($options, $arguments);
        self::checkFlagArraySize($options, $arguments);
        self::checkFlagArrayIndex($options, $arguments);
        self::checkArrayStartIndex($options, $arguments);
        self::checkFlagTypes($options, $arguments);
        self::checkInput($options, $arguments);
        self::checkOutput($options, $arguments);

        return $arguments;
    }

    /**
     * Checks for help argument flag.
     * @param $options array array from {@see getopt()}
     * @param $arguments Arguments arguments object instance
     * @throws InvalidCommandLineArgumentException when there is more arguments then help.

     */
    private static function checkFlagHelp($options, Arguments $arguments)
    {
        if (array_key_exists("help", $options)) {
            $arguments->setFlagHelp(true);
            if(count($options) != 1)
            {
                throw new InvalidCommandLineArgumentException("Help can be used only as solo parameter.");
            }
            Logger::LogDebug("Argument help found");
        } else {
            $arguments->setFlagHelp(false);
        }

    }

    /**
     * Checks for input file argument.
     * @param $options array array from {@see getopt()}
     * @param $arguments Arguments arguments object instance


     */
    private static function checkInput($options, Arguments $arguments)
    {
        if (array_key_exists("input", $options)) {
            $arguments->setInputFile(self::removeQuotes($options["input"]));
            Logger::LogDebug("Argument input found ".$options["input"]);
        } else {
            $arguments->setInputFile('php://stdin');
        }
    }

    /**
     * Check for output file argument.
     * @param $options array array from {@see getopt()}
     * @param $arguments Arguments argument object instance


     */
    private static function checkOutput($options, Arguments $arguments)
    {
        if (array_key_exists("output", $options)) {
            $arguments->setOutputFile(self::removeQuotes($options["output"]));
            Logger::LogDebug("Argument output found ".$options["output"]);
        } else {
            $arguments->setOutputFile('php://stdout');
        }
    }


    /**
     * Checks for no header argument flag.
     * @param $options array array from {@see getopt()}
     * @param Arguments $arguments argument object instance


     */
    private static function checkFlagNoHeader($options, Arguments $arguments)
    {
        if (array_key_exists("n", $options)) {
            $arguments->setFlagNoHeader(true);
            Logger::LogDebug("Argument flag no header found");
        } else {
            $arguments->setFlagNoHeader(false);
        }
    }

    /**
     * Checks for substitution argument.
     * @param $options array array from {@see getopt()}
     * @param Arguments $arguments argument object instance


     */
    private static function checkSubstCharacter($options, Arguments $arguments)
    {
        if (array_key_exists("h", $options)) {
            $arguments->setSubstCharacter($options["h"]);
            Logger::LogDebug("Argument substcharacter found ".$options["h"]);
        } else {
            $arguments->setSubstCharacter('-');
        }
    }

    /**
     * Check for root element argument.
     * @param $options array array from {@see getopt()}
     * @param Arguments $arguments argument object instance
     * @throws InvalidCommandLineArgumentException when root element has not valid xml syntax.


     */
    private static function checkRootElement($options, Arguments $arguments)
    {
        if (array_key_exists("r", $options)) {
            $arguments->setRootElement(self::removeQuotes($options['r']));
            if(NameValidator::invalidate($arguments->getRootElement()) == false)
            {
                throw new InvalidCommandLineArgumentException("Root element does not follow XML syntax", 50);
            }
            Logger::LogDebug("Argument root element found ".$arguments->getRootElement());
        }
    }

    /**
     * Check for array element argument
     * @param $options array array from {@see getopt()}
     * @param Arguments $arguments arguments object instance
     * @throws InvalidCommandLineArgumentException when array element has not valid xml syntax.


     */
    private static function checkArrayName($options, Arguments $arguments)
    {
        if (array_key_exists("array-name", $options)) {
            $arguments->setArrayName(self::removeQuotes($options["array-name"]));
            if(NameValidator::invalidate($arguments->getArrayName()) == false)
            {
                throw new InvalidCommandLineArgumentException("Array element does not follow XML syntax", 50);
            }
            Logger::LogDebug("Argument array name found ".$options["array-name"]);
        } else {
            $arguments->setArrayName("array");
        }
    }


    /**
     * Check for array item name argument
     * @param $options array array from {@see getopt()}
     * @param Arguments $arguments arguments object instance
     * @throws InvalidCommandLineArgumentException when array item element has invalid xml syntax


     */
    private static function checkItemName($options, Arguments $arguments)
    {
        if (array_key_exists("item-name", $options)) {
            $arguments->setItemName(self::removeQuotes($options["item-name"]));
            if(NameValidator::invalidate($arguments->getItemName()) == false)
            {
                throw new InvalidCommandLineArgumentException("Array item element does not follow XML syntax", 50);
            }
            Logger::LogDebug("Argument item name found ".$options["item-name"]);
        } else {
            $arguments->setItemName("item");
        }
    }

    /**
     * Check for text element argument flag.
     * @param $options array array from {@see getopt()}
     * @param $arguments Arguments instance of arguments object
     */
    private static function checkFlagElementText($options, Arguments $arguments)
    {
        if (array_key_exists("s", $options)) {
            $arguments->setFlagElementsText(true);
            Logger::LogDebug("Argument flag text element found");
        } else {
            $arguments->setFlagElementsText(false);
        }
    }

    /**
     * Check for number element argument flag.
     * @param $options array array from {@see getopt()}
     * @param $arguments Arguments instance of arguments object
     */
    private static function checkFlagElementNumber($options, Arguments $arguments)
    {
        if (array_key_exists("i", $options)) {
            $arguments->setFlagElementsNumber(true);
            Logger::LogDebug("Argument flag number element found");
        } else {
            $arguments->setFlagElementsNumber(false);
        }
    }

    /**
     * Check for literal element flag.
     * @param $options array array from {@see getopt()}
     * @param $arguments Arguments instance of arguments object
     */
    private static function checkFlagElementLiteral($options, Arguments $arguments)
    {
        if (array_key_exists("l", $options)) {
            $arguments->setFlagElementsLiteral(true);
            Logger::LogDebug("Argument flag element literal found");
        } else {
            $arguments->setFlagElementsLiteral(false);
        }
    }

    /**
     * Check for character decode flag.
     * @param $options array array from {@see getopt()}
     * @param $arguments Arguments instance of arguments object
     */
    private static function checkFlagCharacterDecode($options, Arguments $arguments)
    {
        if (array_key_exists("c", $options)) {
            $arguments->setFlagCharacterDecode(true);
            Logger::LogDebug("Argument flag character decode found");
        } else {
            $arguments->setFlagCharacterDecode(false);
        }
    }

    /**
     * Checks for array size argument flag.
     * @param $options array array from {@see getopt()}
     * @param $arguments Arguments instance of arguments object
     * @throws InvalidCommandLineArgumentException when argument has been there more times.
     */
    private static function checkFlagArraySize($options, Arguments $arguments)
    {
        if (array_key_exists("a", $options) ||
            array_key_exists("array-size", $options))
        {
            if(array_key_exists("a", $options) && array_key_exists("array-size", $options))
            {
                throw new InvalidCommandLineArgumentException("Redefining already defined argument --array-size");
            }
            $arguments->setFlagArraySize(true);
            Logger::LogDebug("Argument flag array size found");
        } else {
            $arguments->setFlagArraySize(false);
        }
    }

    /**
     * Checks for array index argument flag.
     * @param $options array array from {@see getopt()}
     * @param $arguments Arguments instance of arguments object
     * @throws InvalidCommandLineArgumentException when argument has been there more times.
     */
    private static function checkFlagArrayIndex($options, Arguments $arguments)
    {
        if (array_key_exists("t", $options) ||
            array_key_exists("index-items", $options))
        {
            if(array_key_exists("t", $options) && array_key_exists("index-items", $options))
            {
                throw new InvalidCommandLineArgumentException("Redefining already defined argument --index-items");
            }
            $arguments->setFlagArrayIndex(true);
            Logger::LogDebug("Argument flag index items found");
        } else {
            $arguments->setFlagArrayIndex(false);
        }
    }

    /**
     * Checks for index start argument.
     * @param $options array array from {@see getopt()}
     * @param $arguments Arguments instance of arguments object
     * @throws InvalidCommandLineArgumentException -t or --index-items flag was not defined.
     */
    private static function checkArrayStartIndex($options, Arguments $arguments)
    {
        if (array_key_exists("start", $options)) {
            if (array_key_exists("t", $options) ||
                array_key_exists("index-items", $options)) {
                $arguments->setArrayStartIndex(self::removeQuotes($options["start"]));
                Logger::LogDebug("Argument start index found " . $options["start"]);
            }
            else
            {
                throw new InvalidCommandLineArgumentException("Index items flag has to be specified with the start argument.");
            }
        } else {
            $arguments->setArrayStartIndex(1);
        }
    }

    /**
     * Check for type argument flag.
     * @param $options array array from {@see getopt()}
     * @param Arguments $arguments instance of arguments object
     */
    private static function checkFlagTypes($options, Arguments $arguments)
    {
        if (array_key_exists("types", $options)) {
            $arguments->setFlagTypes(true);
            Logger::LogDebug("Argument flag types found");
        } else {
            $arguments->setFlagTypes(false);
        }
    }

    /**
     * Removes quotes from argument string.
     * @param $string string value with quotes
     * @return string value with removed quotes
     */
    private static function removeQuotes($string)
    {
        return str_replace('"', "", $string);
    }

    /**
     * Parses arguments from command line using {@see getopt()}
     * @return array from {@see getopt()}
     */
    private static function getOptions()
    {
        $short_options = "";
        $short_options .= "h:";
        $short_options .= "n";
        $short_options .= "r:";
        $short_options .= "s";
        $short_options .= "i";
        $short_options .= "l";
        $short_options .= "c";
        $short_options .= "a";
        $short_options .= "t";

        $long_options = array(
            "help",
            "input:",
            "output:",
            "array-name:",
            "item-name:",
            "array-size",
            "index-items",
            "start:",
            "types"
        );

        $options = getopt($short_options, $long_options);
        return $options;
    }

}