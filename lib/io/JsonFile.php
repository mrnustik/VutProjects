<?php



/**
 * Class JsonFile
 * Json file wrapper. Used for retrieving Json contents of file.
 */
class JsonFile extends File
{
    /**
     * Metho used for getting json file representation.
     * @return mixed json representation of file
     * @throws InvalidJsonFileException when was invalid json.
     */
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