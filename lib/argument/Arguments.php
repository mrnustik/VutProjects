<?php

class Arguments
{

    private static $instance = false;

    private function __construct(){}
    public static function getInstance(){
        if(self::$instance === false){
            self::$instance = new Arguments();
        }
        return self::$instance;
    }


    private $flag_help;

    private $flag_no_header;

    private $flag_elements_text;
    private $flag_elements_number;
    private $flag_elements_literal;

    private $flag_characeter_decode;

    private $flag_array_size;
    private $flag_array_index;
    private $array_start_index;

    private $flag_types;

    private $subst_character;

    private $root_element;

    private $array_name;
    private $item_name;

    private $input_file;
    private $output_file;


    public function getFlagHelp()
    {
        return $this->flag_help;
    }

    public function setFlagHelp($flag_help)
    {
        $this->flag_help = $flag_help;
    }

    public function getFlagNoHeader()
    {
        return $this->flag_no_header;
    }

    public function setFlagNoHeader($flag_no_header)
    {
        $this->flag_no_header = $flag_no_header;
    }

    public function getFlagElementsText()
    {
        return $this->flag_elements_text;
    }

    public function setFlagElementsText($flag_elements_text)
    {
        $this->flag_elements_text = $flag_elements_text;
    }

    public function getFlagElementsNumber()
    {
        return $this->flag_elements_number;
    }

    public function setFlagElementsNumber($flag_elements_number)
    {
        $this->flag_elements_number = $flag_elements_number;
    }

    public function getFlagElementsLiteral()
    {
        return $this->flag_elements_literal;
    }

    public function setFlagElementsLiteral($flag_elements_literal)
    {
        $this->flag_elements_literal = $flag_elements_literal;
    }

    public function getFlagCharaceterDecode()
    {
        return $this->flag_characeter_decode;
    }

    public function setFlagCharaceterDecode($flag_characeter_decode)
    {
        $this->flag_characeter_decode = $flag_characeter_decode;
    }

    public function getFlagArraySize()
    {
        return $this->flag_array_size;
    }

    public function setFlagArraySize($flag_array_size)
    {
        $this->flag_array_size = $flag_array_size;
    }

    public function getFlagArrayIndex()
    {
        return $this->flag_array_index;
    }

    public function setFlagArrayIndex($flag_array_index)
    {
        $this->flag_array_index = $flag_array_index;
    }

    public function getArrayStartIndex()
    {
        return $this->array_start_index;
    }

    public function setArrayStartIndex($array_start_index)
    {
        $this->array_start_index = $array_start_index;
    }

    public function getFlagTypes()
    {
        return $this->flag_types;
    }

    public function setFlagTypes($flag_types)
    {
        $this->flag_types = $flag_types;
    }

    public function getSubstCharacter()
    {
        return $this->subst_character;
    }

    public function setSubstCharacter($subst_character)
    {
        $this->subst_character = $subst_character;
    }

    public function getRootElement()
    {
        return $this->root_element;
    }

    public function setRootElement($root_element)
    {
        $this->root_element = $root_element;
    }

    public function getArrayName()
    {
        return $this->array_name;
    }

    public function setArrayName($array_name)
    {
        $this->array_name = $array_name;
    }

    public function getItemName()
    {
        return $this->item_name;
    }

    public function setItemName($item_name)
    {
        $this->item_name = $item_name;
    }

    public function getInputFile()
    {
        return $this->input_file;
    }

    public function setInputFile($input_file)
    {
        $this->input_file = $input_file;
    }

    public function getOutputFile()
    {
        return $this->output_file;
    }

    public function setOutputFile($output_file)
    {
        $this->output_file = $output_file;
    }
}