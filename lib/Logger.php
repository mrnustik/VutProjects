<?php

/**
 * Created by PhpStorm.
 * User: mrnda
 * Date: 19/02/2017
 * Time: 19:19
 */
const IS_DEBUG = false;

class Logger
{

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
        echo "Json2Xml parser 0.9 (Frebruary 25th 2017)\n";
        echo "Usage:\n";
        echo "php jsn.php [OPTION]\n";
        echo "\n";
        echo "OPTIONS:\n";
        echo "\t--help\t\tprints this help\n";
        echo "\t--input=path\t\twill specify path to the original JSON file (if not specified command line input will be used)\n";
        echo "\t--output=path\t\twill specify path to the generated XML document (if not specified command line output will be used)\n";
        echo "\t-h=\"string\"\t\treplaces invalid XML characters with string\n";
        echo "\t-n\t\tscript wont generate XML properties header\n";
        echo "\t-r=\"root_element\"\t\twill add root with value \"root_element\"\n";
        echo "\t--array-name=\"array\"\t\twill rename the XML element surrounding JSON array ([]) (default is array)\n";
        echo "\t--item-name=\"item\"\t\twill rename the XML element sorrounding JSON array item (default is item)\n";
        echo "\t-s\t\tstring will be transformed into text elements instead of value attribute\n";
        echo "\t-i\t\tnumbers will be transformed into text elements instead of value atributes\n";
        echo "\t-l\t\tliterals (true, false, null) will be transformed into XML elements <true/> <false/> <null/>\n";
        echo "\t-c\t\tproblematic characters (&, <, >,...) will be translated into XML meta characters (&amp;, &lt;, &gt;,...)\n";
        echo "\t-a | --aray-size\t\twill add size attributes into the XML array elements\n";
        echo "\t-t | --index-items\t\twill add index attributes into the XML array item elements\n";
        echo "\t--start=number\t\twill change the starting number of indexing (need to be used with -t)\n";
        echo "\t--types\t\telements surrounding scalar values will have attribute type according to the value (integer, real, string , literal)\n";
    }


}