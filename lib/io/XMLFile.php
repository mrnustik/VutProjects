<?php

/**
 * Created by PhpStorm.
 * User: mrnda
 * Date: 19/02/2017
 * Time: 22:06
 */
class XMLFile extends File
{
    function __construct($path, $mode)
    {
        parent::__construct($path, $mode);

    }


    function getXmlWriter()
    {
        $xml_writer = new XMLWriter();
        $xml_writer->openURI($this->getPath());
        return $xml_writer;
    }
}