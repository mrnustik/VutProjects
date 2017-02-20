<?php
/**
 * Created by PhpStorm.
 * User: Michal Mrnuštík (xmrnus01)
 * Date: 15/02/2017
 * Time: 16:50
 */
require_once "lib/Logger.php";
require_once "lib/argument/Arguments.php";
require_once "lib/argument/ArgumentsParser.php";
require_once "lib/io/File.php";
require_once "lib/io/JsonFile.php";
require_once "lib/io/XMLFile.php";
require_once "lib/io/parser/JsonToXMLParser.php";

$arguments = ArgumentsParser::parseArguments($argc);

$json_file = new JsonFile($arguments->getInputFile(), 'r');

$xml_file = new XMLFile($arguments->getOutputFile(), 'w');

$parser = new JsonToXMLParser($xml_file);

$parser->parse($json_file->getJsonContent());

?>