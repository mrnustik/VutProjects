<?php

/**
 * Created by PhpStorm.
 * User: mrnda
 * Date: 19/02/2017
 * Time: 19:13
 */


require_once "Arguments.php";

class ArgumentsParser
{

    public static function parseArguments($argc)
    {
        $options = self::getOptions();

        if(count($options) != $argc - 1){
            Logger::LogError("Unknwon argument");
            exit(1);
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
     * @param $options
     * @param $arguments
     */
    private static function checkFlagHelp($options, $arguments)
    {
        if (array_key_exists("help", $options)) {
            $arguments->setFlagHelp(true);
            Logger::LogDebug("Argument help found");
        } else {
            $arguments->setFlagHelp(false);
        }

    }

    /**
     * @param $options
     * @param $arguments
     */
    private static function checkInput($options, $arguments)
    {
        if (array_key_exists("input", $options)) {
            $arguments->setInputFile(self::removeQuotes($options["input"]));
            Logger::LogDebug("Argument input found ".$options["input"]);
        } else {
            $arguments->setInputFile('php://stdin');
        }
    }

    /**
     * @param $options
     * @param $arguments
     */
    private static function checkOutput($options, $arguments)
    {
        if (array_key_exists("output", $options)) {
            $arguments->setOutputFile(self::removeQuotes($options["output"]));
            Logger::LogDebug("Argument output found ".$options["output"]);
        } else {
            $arguments->setOutputFile('php://stdout');
        }
    }

    /**
     * @param $options
     * @param $arguments
     */
    private static function checkFlagNoHeader($options, $arguments)
    {
        if (array_key_exists("n", $options)) {
            $arguments->setFlagNoHeader(true);
            Logger::LogDebug("Argument flag no header found");
        } else {
            $arguments->setFlagNoHeader(false);
        }
    }

    private static function checkSubstCharacter($options, $arguments)
    {
        if (array_key_exists("h", $options)) {
            $arguments->setSubstCharacter($options["h"]);
            Logger::LogDebug("Argument substcharacter found ".$options["h"]);
        } else {
            $arguments->setSubstCharacter('-');
        }
    }

    private static function checkRootElement($options, $arguments)
    {
        if (array_key_exists("r", $options)) {
            $arguments->setRootElement(self::removeQuotes($options['r']));
            Logger::LogDebug("Argument root element found ".$arguments->getRootElement());
        }
    }

    private static function checkArrayName($options, $arguments)
    {
        if (array_key_exists("array-name", $options)) {
            $arguments->setArrayName(self::removeQuotes($options["array-name"]));
            Logger::LogDebug("Argument array name found ".$options["array-name"]);
        } else {
            $arguments->setArrayName("array");
        }
    }


    private static function checkItemName($options, $arguments)
    {
        if (array_key_exists("item-name", $options)) {
            $arguments->setItemName(self::removeQuotes($options["item-name"]));
            Logger::LogDebug("Argument item name found ".$options["item-name"]);
        } else {
            $arguments->setItemName("item");
        }
    }

    /**
     * @param $options
     * @param $arguments
     */
    private static function checkFlagElementText($options, $arguments)
    {
        if (array_key_exists("s", $options)) {
            $arguments->setFlagElementsText(true);
            Logger::LogDebug("Argument flag text elelement found");
        } else {
            $arguments->setFlagElementsText(false);
        }
    }

    /**
     * @param $options
     * @param $arguments
     */
    private static function checkFlagElementNumber($options, $arguments)
    {
        if (array_key_exists("i", $options)) {
            $arguments->setFlagElementsNumber(true);
            Logger::LogDebug("Argument flag number elelement found");
        } else {
            $arguments->setFlagElementsNumber(false);
        }
    }

    /**
     * @param $options
     * @param $arguments
     */
    private static function checkFlagElementLiteral($options, $arguments)
    {
        if (array_key_exists("l", $options)) {
            $arguments->setFlagElementsLiteral(true);
            Logger::LogDebug("Argument flag element literal found");
        } else {
            $arguments->setFlagElementsLiteral(false);
        }
    }

    /**
     * @param $options
     * @param $arguments
     */
    private static function checkFlagCharacterDecode($options, $arguments)
    {
        if (array_key_exists("c", $options)) {
            $arguments->setFlagCharaceterDecode(true);
            Logger::LogDebug("Argument flag character decode found");
        } else {
            $arguments->setFlagCharaceterDecode(false);
        }
    }

    /**
     * @param $options
     * @param $arguments
     */
    private static function checkFlagArraySize($options, $arguments)
    {
        if (array_key_exists("a", $options) ||
            array_key_exists("array-size", $options)
        ) {
            $arguments->setFlagArraySize(true);
            Logger::LogDebug("Argument flag array size found");
        } else {
            $arguments->setFlagArraySize(false);
        }
    }

    /**
     * @param $options
     * @param $arguments
     */
    private static function checkFlagArrayIndex($options, $arguments)
    {
        if (array_key_exists("t", $options) ||
            array_key_exists("index-items", $options)
        ) {
            $arguments->setFlagArrayIndex(true);
            Logger::LogDebug("Argument flag index items found");
        } else {
            $arguments->setFlagArrayIndex(false);
        }
    }

    /**
     * @param $options
     * @param $arguments
     */
    private static function checkArrayStartIndex($options, $arguments)
    {
        if (array_key_exists("start", $options)) {
            $arguments->setArrayStartIndex(self::removeQuotes($options["start"]));
            Logger::LogDebug("Argument start index found ".$options["start"]);
        } else {
            $arguments->setArrayStartIndex(1);
        }
    }

    /**
     * @param $options
     * @param $arguments
     */
    private static function checkFlagTypes($options, $arguments)
    {
        if (array_key_exists("types", $options)) {
            $arguments->setFlagTypes(true);
            Logger::LogDebug("Argument flag types found");
        } else {
            $arguments->setFlagTypes(false);
        }
    }

    private static function removeQuotes($string)
    {
        return str_replace('"', "", $string);
    }

    /**
     * @return array
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