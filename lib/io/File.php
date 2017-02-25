<?php

/**
 * Created by PhpStorm.
 * User: mrnda
 * Date: 19/02/2017
 * Time: 21:57
 */


class File
{

    function __construct($path, $mode)
    {
        $this->path = $path;
        $this->mode = $mode;
    }

    private $path;

    public function getPath()
    {
        return $this->path;
    }

    public function setPath($path)
    {
        $this->path = $path;
    }

    private $mode;

    public function getMode()
    {
        return $this->mode;
    }

    public function setMode($mode)
    {
        $this->mode = $mode;
    }

    function getContents()
    {
        $file = @ fopen($this->getPath(), $this->getMode());
        if($file != FALSE)
        {
            $content = "";
            while($line = fgets($file))
            {
                $content .= $line;
            }
            fclose($file);
            return $content;
        }
        else
        {
            throw new FileNotFoundException("File: ".$this->path." was not found.");
        }
    }

}