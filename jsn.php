<?php
/**
 * Created by PhpStorm.
 * User: Michal Mrnuštík (xmrnus01)
 * Date: 15/02/2017
 * Time: 16:50
 */
include_once "lib/Logger.php";
include_once "lib/argument/Arguments.php";
include_once "lib/argument/ArgumentsParser.php";
include_once "lib/io/File.php";
include_once "lib/io/JsonFile.php";
include_once "lib/io/XMLFile.php";
include_once "lib/io/parser/JsonToXMLParser.php";
include_once "lib/io/xml/NameValidator.php";

$arguments = ArgumentsParser::parseArguments($argc);

$json_file = new JsonFile($arguments->getInputFile(), 'r');

$xml_file = new XMLFile($arguments->getOutputFile(), 'w');

$parser = new JsonToXMLParser($xml_file);

$parser->parse($json_file->getJsonContent());

?>