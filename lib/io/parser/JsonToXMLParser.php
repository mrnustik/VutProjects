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
                $this->parse_value($json);
                $this->xml_writer->endElement();
            }
            else
            {
                $this->parse_value($json);
            }

        }
        else
        {
            throw new InvalidJsonFileException("The file does not start as object or array.");
        }
    }

    private function parse_array($json)
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
            $this->parse_value($value);
            $this->xml_writer->endElement();
        }
        $this->xml_writer->endElement();
    }

    private function parse_object($json)
    {
        foreach ($json as $key => $value)
        {
            $this->xml_writer->startElement(NameValidator::invalidateAndReplace($key));
            $this->parse_value($value);
            $this->xml_writer->endElement();
        }
    }

    private function parse_value($json)
    {
        if(is_object($json))
        {
            $this->parse_object($json);
        }
        elseif(is_array($json))
        {
            $this->parse_array($json);
        }
        elseif(is_int($json) || is_float($json))
        {
            $this->parse_number($json);
        }
        elseif (is_string($json))
        {
            $this->parse_string($json);
        }
        elseif($json === false
                    || $json === true
                    || $json == null)
        {
            $this->parse_literal($json);
        }
        else
        {
            $this->xml_writer->writeRaw($json);
        }
    }

    /**
     * @param $json
     */
    private function parse_literal($json)
    {
        $arguments = Arguments::getInstance();
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
        if ($arguments->getFlagTypes()) {
            $this->xml_writer->writeAttribute("type", "literal");
        }
    }

    /**
     * @param $json
     */
    private function parse_string($json)
    {
        $arguments = Arguments::getInstance();
        if ($arguments->getFlagElementsText()) {
            if ($arguments->getFlagCharaceterDecode()) {
                $this->xml_writer->text($json);
            } else {
                $this->xml_writer->writeRaw($json);
            }
        } else {
            $this->xml_writer->writeAttribute("value", $json);
        }
        if ($arguments->getFlagTypes()) {
            $this->xml_writer->writeAttribute("type", "string");
        }
    }

    /**
     * @param $json
     */
    private function parse_number($json)
    {
        $arguments = Arguments::getInstance();
        if ($arguments->getFlagElementsNumber()) {
            $this->xml_writer->text(floor($json));
        } else {
            $this->xml_writer->writeAttribute("value", floor($json));
        }
        if ($arguments->getFlagTypes()) {
            $this->xml_writer->writeAttribute("type", is_int($json) ? "integer" : "real");
        }
    }


}