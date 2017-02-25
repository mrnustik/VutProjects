<?php
/**
 * Created by PhpStorm.
 * User: Michal Mrnuštík (xmrnus01)
 * Date: 15/02/2017
 * Time: 16:50
 */
include_once "lib/Logger.php";
include_once "lib/argument/exception/InvalidCommandLineArgumentException.php";
include_once "lib/argument/Arguments.php";
include_once "lib/argument/ArgumentsParser.php";
include_once "lib/io/exception/FileNotFoundException.php";
include_once "lib/io/exception/BadFilePermissionException.php";
include_once "lib/io/exception/JsonParseException.php";
include_once "lib/io/parser/exception/InvalidJsonFileException.php";
include_once "lib/io/xml/exception/InvalidJsonSubstitutionException.php";
include_once "lib/io/File.php";
include_once "lib/io/JsonFile.php";
include_once "lib/io/XMLFile.php";
include_once "lib/io/parser/JsonToXMLParser.php";
include_once "lib/io/xml/NameValidator.php";

try
{
    $arguments = ArgumentsParser::parseArguments($argc);

    if($arguments->getFlagHelp() == true)
    {
        Logger::printHelp();
        exit(0);
    }

    $json_file = new JsonFile($arguments->getInputFile(), 'r');

    $xml_file = new XMLFile($arguments->getOutputFile(), 'w');

    $parser = new JsonToXMLParser($xml_file);

    $parser->parse($json_file->getJsonContent());
}
catch(InvalidCommandLineArgumentException $exception)
{
    Logger::LogError($exception->getMessage());
    exit($exception->getCode());
}
catch (FileNotFoundException $exception)
{
    Logger::LogError($exception->getMessage());
    exit(2);
}
catch(BadFilePermissionException $exception)
{
    Logger::LogError($exception->getMessage());
    exit(3);
}
catch(InvalidJsonFileException $exception)
{
    Logger::LogError($exception->getMessage());
    exit(4);
}
catch(InvalidJsonSubstitutionException $exception)
{
    Logger::LogError($exception->getMessage());
    exit(51);
}

?>