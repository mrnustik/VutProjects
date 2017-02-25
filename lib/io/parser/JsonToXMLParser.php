<?php

/**
 * Created by PhpStorm.
 * User: mrnda
 * Date: 19/02/2017
 * Time: 22:29
 */



class JsonToXMLParser
{

    private $xml_writer;

    function __construct(XMLFile $output_file)
    {
        $this->xml_writer = $output_file->getXmlWriter();
        $this->xml_writer->setIndent(true);
    }

    public function parse($json)
    {
        if(is_object($json) || is_array($json))
        {
            $arguments = Arguments::getInstance();
            if($arguments->getFlagNoHeader() == false)
            {
                $this->xml_writer->startDocument("1.0", "UTF-8");
            }
            if($arguments->getRootElement() != null)
            {
                $this->xml_writer->startElement($arguments->getRootElement());
                $this->parseValue($json);
                $this->xml_writer->endElement();
            }
            else
            {
                $this->parseValue($json);
            }

        }
        else
        {
            throw new InvalidJsonFileException("The file does not start as object or array.");
        }
    }

    private function parseArray($json)
    {
        $arguments = Arguments::getInstance();
        $this->xml_writer->startElement($arguments->getArrayName());
        if($arguments->getFlagArraySize())
        {
            $this->xml_writer->writeAttribute("size", count($json));
        }
        $index = $arguments->getArrayStartIndex();
        foreach ($json as $value)
        {
            $this->xml_writer->startElement($arguments->getItemName());
            if($arguments->getFlagArrayIndex())
            {
                $this->xml_writer->writeAttribute("index", $index++);
            }
            $this->parseValue($value);
            $this->xml_writer->endElement();
        }
        $this->xml_writer->endElement();
    }

    private function parseObject($json)
    {
        foreach ($json as $key => $value)
        {
            $this->xml_writer->startElement(NameValidator::invalidateAndReplace($key));
            $this->parseValue($value);
            $this->xml_writer->endElement();
        }
    }

    private function parseValue($json)
    {
        if(is_object($json))
        {
            $this->parseObject($json);
        }
        elseif(is_array($json))
        {
            $this->parseArray($json);
        }
        elseif(is_int($json) || is_float($json))
        {
            $this->parseNumber($json);
        }
        elseif (is_string($json))
        {
            $this->parseString($json);
        }
        elseif($json === false
                    || $json === true
                    || $json == null)
        {
            $this->parseLiteral($json);
        }
        else
        {
            $this->xml_writer->writeRaw($json);
        }
    }

    /**
     * @param $json
     */
    private function parseLiteral($json)
    {
        $arguments = Arguments::getInstance();
        if ($arguments->getFlagTypes()) {
            $this->xml_writer->writeAttribute("type", "literal");
        }
        if ($arguments->getFlagElementsLiteral()) {
            if ($json === false) {
                $this->xml_writer->writeElement("false");
            } elseif ($json === true) {
                $this->xml_writer->writeElement("true");
            } else {
                $this->xml_writer->writeElement("null");
            }
        } else {
            if ($json === false) {
                $this->xml_writer->writeAttribute("value", "false");
            } elseif ($json === true) {
                $this->xml_writer->writeAttribute("value", "true");
            } else {
                $this->xml_writer->writeAttribute("value", "null");
            }
        }

    }

    /**
     * @param $json
     */
    private function parseString($json)
    {
        $arguments = Arguments::getInstance();
        if ($arguments->getFlagTypes()) {
            $this->xml_writer->writeAttribute("type", "string");
        }
        if ($arguments->getFlagElementsText()) {
            if ($arguments->getFlagCharacterDecode()) {
                $this->xml_writer->text($json);
            } else {
                $this->xml_writer->writeRaw($json);
            }
        } else {
            $this->xml_writer->writeAttribute("value", $json);
        }

    }

    /**
     * @param $json
     */
    private function parseNumber($json)
    {
        $arguments = Arguments::getInstance();
        if ($arguments->getFlagTypes()) {
            $this->xml_writer->writeAttribute("type", is_int($json) ? "integer" : "real");
        }
        if ($arguments->getFlagElementsNumber()) {
            $this->xml_writer->text(floor($json));
        } else {
            $this->xml_writer->writeAttribute("value", floor($json));
        }

    }


}