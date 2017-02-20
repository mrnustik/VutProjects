#!/usr/bin/env bash
echo "===============================================================================";
echo "=  __      ___               _____                                            =";
echo "=  \ \    / / |        /\   |  __ \                                           =";
echo "=   \ \  / /| |       /  \  | |  | |                                          =";
echo "=    \ \/ / | |      / /\ \ | |  | |                                          =";
echo "=     \  /  | |____ / ____ \| |__| |                                          =";
echo "=      \/   |______/_/    \_\_____/                                           =";
echo "=   _______ _    _ ______   _______ ______  _____ _______ ______ _____        =";
echo "=  |__   __| |  | |  ____| |__   __|  ____|/ ____|__   __|  ____|  __ \       =";
echo "=     | |  | |__| | |__       | |  | |__  | (___    | |  | |__  | |__) |      =";
echo "=     | |  |  __  |  __|      | |  |  __|  \___ \   | |  |  __| |  _  /       =";
echo "=     | |  | |  | | |____     | |  | |____ ____) |  | |  | |____| | \ \       =";
echo "=     |_|  |_|  |_|______|    |_|  |______|_____/   |_|  |______|_|  \_|      =";
echo "===============================================================================";

echo -e "\n\n\n"

INTERPRETER="php" # PATH TO YOUR PHP INTERPRETER
SOURCE="./jsn.php"


echo "===============================================================================";
echo "=-PHP VERSION =============================================================-=-=";
echo "===============================================================================";
printf "\n"
$INTERPRETER -v
printf "\n"

echo "===============================================================================";
echo "=-SOURCE PATH =============================================================-=-=";
echo "===============================================================================";
printf "\n"
echo $SOURCE
printf "\n"

INPUT_PATH="./tests/input/"
OUTPUT_PATH="./tests/real-out/"
REFERENCE_OUTPUT_PATH="./tests/ref-out/"

INPUT_JSONS=(./tests/input/*.json)
INPUT_PARAMS=(./tests/input/*.params)

REF_OUT_XML=(./tests/ref-out/*.xml)
REF_OUT_CODE=(./tests/ref-out/*.!!!)
REF_OUT_ERR=(./tests/ref-out/*.err)

RED='\033[0;31m'
GREEN='\033[0;32m'
NO_COLOR='\033[0m'

echo "===============================================================================";
echo "Number of tests:" ${#INPUT_JSONS[@]}
echo "===============================================================================";

for i in ${!INPUT_JSONS[*]}; do 
    echo "===============================================================================";
    echo "Running test number: " $i;
    echo "Input test file:" ${INPUT_JSONS[$i]}
    echo "Test params: " $(cat ${INPUT_PARAMS[$i]})
    $INTERPRETER $SOURCE --input=${INPUT_JSONS[$i]} --output=$OUTPUT_PATH$i.xml $(cat ${INPUT_PARAMS[$i]}) 2> $OUTPUT_PATH$i.err
    echo -n $? > $OUTPUT_PATH$i.code
    echo -n "Test exit code: " 
    diff -y ${REF_OUT_CODE[$i]} $OUTPUT_PATH$i.code > /dev/null
    code=$?
    if [ ! $code -eq 0 ]; then
        echo ""
        echo -e "\t${RED}Error: Bad error code."
        echo -e "\tExpected:  $(cat ${REF_OUT_CODE[$i]})"
        echo -e "\tBut received:  $(cat $OUTPUT_PATH$i.code)"
    else 
        echo -e "${GREEN}OK"
    fi;
    echo -e "${NO_COLOR}"
    echo -n "Test output:"
    OUTPUT=$(diff -b -y --ignore-all-space ${REF_OUT_XML[$i]} $OUTPUT_PATH$i.xml 2>/dev/null)
    if [ ! $? -eq 0 ]; then
        echo ""
        echo -e "\t${RED}Error: Bad output xml."
        echo -e "\tDiff output:\n$OUTPUT"
    else 
        echo -e "${GREEN}OK"
    fi;
    echo -e "${NO_COLOR}"
done;


echo "===============================================================================";
