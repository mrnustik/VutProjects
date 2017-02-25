<?php

/**
 * Class XMLFile
 * XML file wrappes.
 * Enables to get XMLWriter of given file.
 */
class XMLFile extends File
{

    /**
     * Method to retrieve XMLWriter API object.
     * @return XMLWriter object to communicate with libxml API
     * @{see XMLWriter}
     */
    function getXmlWriter()
    {
        $xml_writer = new XMLWriter();
        $xml_writer->openURI($this->getPath());
        return $xml_writer;
    }
}