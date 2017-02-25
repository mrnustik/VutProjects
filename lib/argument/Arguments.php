<?php

/**
 * Class Arguments
 * Used to represent arguments that have been passed to the script.
 */
class Arguments
{

    /**
     * @var Arguments instance of Arguments (singleton implementation)
     */
    private static $instance = false;

    /**
     * Arguments private constructor to prevent from creating other instances.
     */
    private function __construct(){}


    /**
     * Method to retrieve instance of the Arguments object.
     * @return Arguments returns Arguments instance if exists or creates new one.
     */
    public static function getInstance(){
        if(self::$instance === false){
            self::$instance = new Arguments();
        }
        return self::$instance;
    }

    /**
     * @var bool Determines whether --help flag was used.
     */
    private $flag_help;

    /**
     * Flag used to describe if the output XML should not have header.
     * @var bool Determines whether -n flag was used.
     */
    private $flag_no_header;

    /**
     * Flag used to describe if strings should be in elements instead of attributes.
     * @var bool Determines whether -s flag was used.
     */
    private $flag_elements_text;

    /**
     * Flag used to describe if numbers should be in elements instead of attributes.
     * @var bool Determines whether -i flag was used.
     */
    private $flag_elements_number;

    /**
     * Flag used to describe if literals (true, false, null) should be in elements instead of attributes.
     * @var bool Determines whether the -l flag was used.
     */
    private $flag_elements_literal;

    /**
     * Flag used to describe if 8-bit ASCII should be decode to 7-bit ASCCII (& to &amp;)
     * @var bool Determines whether the -c flag was used.
     */
    private $flag_character_decode;

    /**
     * Flag used to describe whether the arrays should have attribute size.
     * @var bool Determines whether the -a || --array-size flag was used.
     */
    private $flag_array_size;

    /**
     * Flag used to descirbe whether the array items should have index attribute.
     * @var bool Determines whether the -i || --index-items flag was used.
     */
    private $flag_array_index;

    /**
     * Must be used with the -i option.
     * @var int Starting index of array items.
     */
    private $array_start_index;

    /**
     * Flag used to describe whether the scalar elements should have attribute with their type.
     * @var bool Determines whether the --types flag was used.
     */
    private $flag_types;

    /**
     * @var string String that will be used to substitute invalid XML characters.
     */
    private $subst_character;

    /**
     * @var string Root element of the XML document.
     */
    private $root_element;

    /**
     * @var string Root element of the JSON array used in XML.
     */
    private $array_name;

    /**
     * @var string Name of element used to surround array items.
     */
    private $item_name;

    /**
     * @var string Path to input file (or php://stdin)
     */
    private $input_file;

    /**
     * @var string Path to output file (or php://stdout)
     */
    private $output_file;

    /**
     * @return bool flag_help
     * @see Arguments::$flag_help
     */
    public function getFlagHelp()
    {
        return $this->flag_help;
    }

    /**
     * @param $flag_help bool
     * @see Arguments::$flag_help
     */
    public function setFlagHelp($flag_help)
    {
        $this->flag_help = $flag_help;
    }

    /**
     * @return bool flag_no_header
     * @see Arguments::$flag_no_header
     */
    public function getFlagNoHeader()
    {
        return $this->flag_no_header;
    }

    /**
     * @param $flag_no_header bool
     * @see Arguments::$flag_no_header
     */
    public function setFlagNoHeader($flag_no_header)
    {
        $this->flag_no_header = $flag_no_header;
    }

    /**
     * @return bool flag_elements_text
     * @see Arguments::$flag_elements_text
     */
    public function getFlagElementsText()
    {
        return $this->flag_elements_text;
    }

    /**
     * @param $flag_elements_text bool
     * @see Arguments::$flag_elements_text
     */
    public function setFlagElementsText($flag_elements_text)
    {
        $this->flag_elements_text = $flag_elements_text;
    }

    /**
     * @return bool flag_elements_number;
     * @see Arguments::$flag_elements_number
     */
    public function getFlagElementsNumber()
    {
        return $this->flag_elements_number;
    }

    /**
     * @param $flag_elements_number bool
     * @see Arguments::$flag_elements_number
     */
    public function setFlagElementsNumber($flag_elements_number)
    {
        $this->flag_elements_number = $flag_elements_number;
    }

    /**
     * @return bool flag_elements_literal
     * @see Arguments::$flag_elements_literal
     */
    public function getFlagElementsLiteral()
    {
        return $this->flag_elements_literal;
    }

    /**
     * @param $flag_elements_literal bool
     * @see Arguments::$flag_elements_literal
     */
    public function setFlagElementsLiteral($flag_elements_literal)
    {
        $this->flag_elements_literal = $flag_elements_literal;
    }

    /**
     * @return bool flag_character_decode
     * @see Arguments::$flag_character_decode
     */
    public function getFlagCharacterDecode()
    {
        return $this->flag_character_decode;
    }

    /**
     * @param $flag_character_decode bool
     * @see Arguments::$flag_character_decode
     */
    public function setFlagCharacterDecode($flag_character_decode)
    {
        $this->flag_character_decode = $flag_character_decode;
    }

    /**
     * @return bool flag_array_size
     * @see Arguments::$flag_array_size
     */
    public function getFlagArraySize()
    {
        return $this->flag_array_size;
    }

    /**
     * @param $flag_array_size bool
     * @see Arguments::$flag_array_size
     */
    public function setFlagArraySize($flag_array_size)
    {
        $this->flag_array_size = $flag_array_size;
    }

    /**
     * @return bool flag_array_index
     * @see Arguments::$flag_array_index
     */
    public function getFlagArrayIndex()
    {
        return $this->flag_array_index;
    }

    /**
     * @param $flag_array_index bool
     * @see Arguments::$flag_array_index
     */
    public function setFlagArrayIndex($flag_array_index)
    {
        $this->flag_array_index = $flag_array_index;
    }

    /**
     * @return int array_start_index
     * @see Arguments::$array_start_index
     */
    public function getArrayStartIndex()
    {
        return $this->array_start_index;
    }

    /**
     * @param $array_start_index int
     * @see Arguments::$array_start_index
     */
    public function setArrayStartIndex($array_start_index)
    {
        $this->array_start_index = $array_start_index;
    }

    /**
     * @return bool flag_types
     * @see Arguments::$flag_types
     */
    public function getFlagTypes()
    {
        return $this->flag_types;
    }

    /**
     * @param $flag_types bool
     * @see Arguments::$flag_types
     */
    public function setFlagTypes($flag_types)
    {
        $this->flag_types = $flag_types;
    }

    /**
     * @return string subst_character
     * @see Arguments::$subst_character
     */
    public function getSubstCharacter()
    {
        return $this->subst_character;
    }

    /**
     * @param $subst_character string
     * @see Arguments::$subst_character
     */
    public function setSubstCharacter($subst_character)
    {
        $this->subst_character = $subst_character;
    }

    /**
     * @return string root_element
     * @see Arguments::$root_element
     */
    public function getRootElement()
    {
        return $this->root_element;
    }

    /**
     * @param $root_element string
     * @see Arguments::$root_element
     */
    public function setRootElement($root_element)
    {
        $this->root_element = $root_element;
    }

    /**
     * @return string array_name
     * @see Arguments::$array_name
     */
    public function getArrayName()
    {
        return $this->array_name;
    }

    /**
     * @param $array_name string
     * @see Arguments::$array_name
     */
    public function setArrayName($array_name)
    {
        $this->array_name = $array_name;
    }

    /**
     * @return string item_name
     * @see Arguments::$item_name
     */
    public function getItemName()
    {
        return $this->item_name;
    }

    /**
     * @param $item_name string
     * @see Arguments::$item_name
     */
    public function setItemName($item_name)
    {
        $this->item_name = $item_name;
    }

    /**
     * @return string input_file
     * @see Arguments::$input_file
     */
    public function getInputFile()
    {
        return $this->input_file;
    }

    /**
     * @param $input_file string
     * @see Arguments::$input_file
     */
    public function setInputFile($input_file)
    {
        $this->input_file = $input_file;
    }

    /**
     * @return string output file
     * @see Arguments::$output_file
     */
    public function getOutputFile()
    {
        return $this->output_file;
    }

    /**
     * @param $output_file string
     * @see Arguments::$output_file
     */
    public function setOutputFile($output_file)
    {
        $this->output_file = $output_file;
    }
}