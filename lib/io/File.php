<?php

/**
 * Created by PhpStorm.
 * User: mrnda
 * Date: 19/02/2017
 * Time: 21:57
 */


/**
 * Class File
 * File wrapper. Wrapps functionalitz of the file.
 */
class File
{

    /**
     * File constructor.
     * @param $path string file path
     * @param $mode string file opening mode
     * @throws BadFilePermissionException when was unable to open given file.
     */
    function __construct($path, $mode)
    {
        $this->path = $path;
        $this->mode = $mode;
        $output_file =  @ fopen($path, $mode);
        if($output_file == false)
        {
            throw new BadFilePermissionException("Can't open file $path for mode ".$mode.".");
        }
        fclose($output_file);
    }

    /**
     * @var string file path
     */
    private $path;

    /**
     * @return string path {@see $path}
     */
    public function getPath()
    {
        return $this->path;
    }


    /**
     * @var string file mode
     */
    private $mode;

    /**
     * @return string file mode
     */
    public function getMode()
    {
        return $this->mode;
    }

    /**
     * Method for retrieving file contents.
     * @return string file contents
     */
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
        return "";
    }

}