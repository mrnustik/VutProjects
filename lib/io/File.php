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
        if(file_exists($this->path))
        {
            return file_get_contents($this->path);
        }
        else
        {
            throw new FileNotFoundException("File: ".$this->path." was not found.");
        }
    }

}