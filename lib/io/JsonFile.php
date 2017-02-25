<?php

/**
 * Created by PhpStorm.
 * User: mrnda
 * Date: 19/02/2017
 * Time: 22:06
 */

class JsonFile extends File
{
    public function getJsonContent()
    {
        $json = json_decode($this->getContents());
        if($json == NULL)
        {
            throw new InvalidJsonFileException("Could not parse file: ".$this->getPath()." to a json.");
        }
        return $json;
    }
}