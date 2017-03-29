#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import sys
import re
import os

EXIT_OK = 0
ERROR_INVALID_PARAMS = 1
ERROR_INVALID_INPUT = 2
ERROR_INVALID_OUTPUT = 3
ERROR_INVALID_FORMAT = 4
ERROR_SYNTACTIC = 60
ERROR_SEMANTIC = 61
ERROR_NOT_DSKA = 62

class ScannerStates(Enum):
    BEGIN = 0
    STATE = 1
    STATE_NEXT = 2
    BEGIN_ALPHA = 3
    ALPHABET = 4
    SYMBOL = 5
    SYMBOL_COMMA = 6
    SYMBOL_END = 7
    SYMBOL_NEXT = 8
    BEGIN_RULES = 9
    RULE_FROM = 10
    RULE_SYMBOL = 11
    RULE_SYMBOL_COMMA = 12
    RULE_ARROW_1 = 13
    RULE_ARROW_2 = 14
    RULE_TO = 15
    START_STATE = 16
    BEGIN_FINAL = 17
    FINAL_STATE = 18
    FINAL_STATE_NEXT = 19
    STATE_ALPHA_SEPARATOR = 20
    ALPHA_RULES_SEPARATOR = 21
    RULE_START_SEPARATOR = 22
    START_FINAL_SEPARATOR = 23
    END = 24

# (?!'),(?!')

def print_error(message, code):
    print(message, file=sys.stderr)
    exit(code)


class State:

    # inicializace stavu podle jmena
    def __init__(self, name):

        self.name = name


    # porovnani dvou stavu (jmen stavu)
    def __cmp__(self, other):
        if  self.name < other.name:
            return -1
        elif self.name == other.name:
            return 0
        else:
            return 1

    # equalita dvou stavu
    def __eq__(self, other):
        if type(other) is str:
            return self.name == other
        else:
            return other is self

    # navrat stavu jako stringu
    def __str__(self):
        #TODO Zmenit na odpovidajici syntaxi
        return "State: " + str(self.name)


class AlphabetCharacter:

    def __init__(self, character):
        self.character = character

    def __cmp__(self, other):
        if self.character < other.character:
            return -1
        elif self.character == other.character:
            return 0
        else:
            return 1

            # equalita dvou stavu

    def __eq__(self, other):
        if type(other) is str:
            return self.character == other
        else:
            return other is self

    def __str__(self):
        return "Character: " + self.character


class Rule:
    def __init__(self, fr, input, to):
        self.fr = fr
        self.input = input
        self.to = to

    def __eq__(self, other):
        if type(other) is Rule:
            return other.fr == self.fr and other.input == self.input and other.to == self.to
        else:
            return False

    def __str__(self):
        return "Rule: " + str(self.fr) + " '" + str(self.input) + "' -> " + str(self.to)


class ScriptException(Exception):
    def __init__(self, message, error):
        self.message = message
        self.error = error
    pass


class FiniteStateMachine:

    def __init__(self):
        self.states = []
        self.rules = []
        self.alphabet = []
        self.start = None
        self.final_states = []

    def add_state(self, state):
        self.states.append(state)

    def add_rule(self, state_from, character, state_to):
        if state_to in self.states and state_from in self.states and character in self.alphabet:
            rule = Rule(state_from, character, state_to)
            self.rules.append(rule)
        else:
            raise ScriptException("Trying to create rule with invalid parameter", ERROR_SEMANTIC)

    def add_alphabet_character(self, character):
        self.alphabet.append(character)

    def add_final_state(self, final_state):
        if final_state in self.states:
            self.final_states.append(final_state)
        else:
            raise ScriptException("Trying to add final state that is not defined", ERROR_SEMANTIC)

    def set_start(self, start_state):
        if start_state in self.states:
            self.start = start_state
        else:
            raise ScriptException("Trying to add start state that is not defined", ERROR_SEMANTIC)


class Arguments:
    def __init__(self):
        self.input = sys.stdin
        self.output = sys.stdout
        self.help = False
        self.nonFinishing = False
        self.minimize = False
        self.caseInsensitive = False


def parse_arguments():
        parser = argparse.ArgumentParser(prog="mka.py", add_help=False)
        parser.add_argument('--help',
                            help='prints this help and exits',
                            required=False,
                            action='store_true')

        parser.add_argument('--input',
                            help="input file",
                            required=False,
                            default=sys.stdin)

        parser.add_argument('--output',
                            help="output file",
                            required=False,
                            default=sys.stdout)

        parser.add_argument('-f',
                            '--find-non-finishing',
                            help='find non finishing FSM state',
                            required=False,
                            action='store_true')

        parser.add_argument('-m',
                            '--minimize',
                            help='minimize given FSM',
                            required=False,
                            action='store_true')

        parser.add_argument('-i', '--case-insensitive',
                            help='if input should be treated case-insensitive',
                            required=False,
                            action='store_true')

        args = parser.parse_args()
        if args.help:
            if len(sys.argv) == 2:
                parser.print_help()
                exit(EXIT_OK)
            else:
                print_error('Invalid combination of parameters. '
                            'Run help to see the possible params',
                            ERROR_INVALID_PARAMS)

        if args.minimize and args.find_non_finishing:
            print_error("Invalid combination of parameters. "
                        "Run help to see the possible params.",
                        ERROR_INVALID_PARAMS)

        arguments = Arguments()
        arguments.nonFinishing = args.find_non_finishing
        arguments.caseInsensitive = args.case_insensitive
        arguments.input = args.input
        arguments.output = args.output
        arguments.minimize = args.minimize
        return arguments


def read(args):
    text = ""
    if args.input != sys.stdin:
        try:
            with open(args.input, 'r', encoding='utf8') as file:
                text = file.read()
        except:
            print_error('Cant open the file.', ERROR_INVALID_INPUT)
    else:
        text = args.input.read()
    if len(text) == 0:
        print_error("Invalid input file. It is empty.", ERROR_INVALID_INPUT)
    if args.caseInsensitive:
        return str(text).lower()
    else:
        return str(text)

def scan(input_text):
    current_state_name = ""
    current_from_state_name = ""
    current_symbol = ""
    current_to_state_name = ""
    fsm = FiniteStateMachine()
    i = 0
    part = 1
    state = ScannerStates.BEGIN;
    while i < len(input_text) and part < 6:
        if i == 0 and input_text[i] == '(':
            i += 1 #prvni znak nas nezajima
        if re.match("\s", input_text[i]) and state != ScannerStates.SYMBOL and state != ScannerStates.RULE_SYMBOL
            i += 1
            continue
        if state == ScannerStates.BEGIN:
            if input_text[i] == '{':
                state = ScannerStates.STATE
            else:
                raise ScriptException("Expecting { as first character", ERROR_SYNTACTIC)
        elif state == ScannerStates.STATE:
            if input_text[i] == ',':
                fsm.add_state(State(current_state_name))
                current_state_name = ""
                state = ScannerStates.STATE_NEXT
            elif input_text[i] == '}':
                fsm.add_state(State(current_state_name))
                current_state_name = ""
                state = ScannerStates.STATE_ALPHA_SEPARATOR
            else:
                current_state_name += input_text[i]
        elif state == ScannerStates.STATE_ALPHA_SEPARATOR:
            if input_text[i] == ',':
                state = ScannerStates.BEGIN_ALPHA
            else:
                raise ScriptException("Expecting separator(,) between states and alphabet", ERROR_SYNTACTIC)
        elif state == ScannerStates.BEGIN_ALPHA:
            if input_text[i] == '{':
                state = ScannerStates.ALPHABET
            else:
                raise ScriptException("Expecting { before alphabet list starts", ERROR_SYNTACTIC)
        elif state == ScannerStates.ALPHABET:
            if input_text[i] == '\'':
                state = ScannerStates.SYMBOL
            else:
                raise ScriptException("Alphabet has to contain at least one symbol of the alphabet", ERROR_SEMANTIC)
        elif state == ScannerStates.SYMBOL:
            fsm.add_alphabet_character(AlphabetCharacter(input_text[i]))
            state = ScannerStates.SYMBOL_COMMA
        elif state == ScannerStates.SYMBOL_COMMA:
            if input_text[i] == '\'':
                state = ScannerStates.SYMBOL_END
            else:
                raise ScriptException("Symbol has to be 1 character surrounded with apostrophes.", ERROR_SYNTACTIC)
        elif state == ScannerStates.SYMBOL_END:
            if input_text[i] == ',':
                state = ScannerStates.SYMBOL_NEXT
            elif input_text[i] == '}':
                state = ScannerStates.ALPHA_RULES_SEPARATOR
            else:
                raise ScriptException("Unexpected character in alphabet section",ERROR_SYNTACTIC)
        elif state == ScannerStates.SYMBOL_NEXT:
            if input_text[i] == '\'':
                state = ScannerStates.SYMBOL
            else:
                raise ScriptException("Symbol has to be 1 character surrounded with apostrophes", ERROR_SYNTACTIC)
        elif state == ScannerStates.ALPHA_RULES_SEPARATOR:
            if input_text[i] == ',':
                state = ScannerStates.BEGIN_RULES
            else:
                raise ScriptException("Expecting separator(,) between alphabet and rulse", ERROR_SYNTACTIC)
        elif state == ScannerStates.BEGIN_RULES:
            if input_text[i] == '{':
                state = ScannerStates.RULE_FROM
            else:
                raise ScriptException("Rules has to start with left bracket {")
        elif state == ScannerStates.RULE_FROM:
            if input_text[i] == '\'':
                state = ScannerStates.RULE_SYMBOL
            else:
                current_from_state_name += input_text[i]
        elif state == ScannerStates.RULE_SYMBOL:
            current_symbol = input_text[i]
            state = ScannerStates.RULE_SYMBOL_COMMA
        elif state == ScannerStates.RULE_SYMBOL_COMMA:
            if input_text[i] == '\'':
                state = ScannerStates.RULE_ARROW_1
            else:
                raise ScriptException("Alphabet symbol has to be 1 character placed between apostrophes", ERROR_SYNTACTIC)
        elif state == ScannerStates.RULE_ARROW_1:
            if input_text[i] == '-':
                state = ScannerStates.RULE_ARROW_2
            else:
                raise ScriptException("Arrow has to be placed in rule between symbol and next state", ERROR_SYNTACTIC)
        elif state == ScannerStates.RULE_ARROW_2:
            if input_text[i] == '>':
                state = ScannerStates.RULE_TO
            else:
                raise ScriptException("Arrow has to be placed in rule between symbol and next state", ERROR_SYNTACTIC)
        elif state == ScannerStates.RULE_TO:
            if input_text[i] == ',':
                fsm.add_rule(current_from_state_name, current_symbol, current_to_state_name)
                current_from_state_name = ""
                current_symbol = ""
                current_to_state_name = ""
                state = ScannerStates.RULE_FROM
            elif input_text[i] == '}':
                fsm.add_rule(current_from_state_name, current_symbol, current_to_state_name)
                current_from_state_name = ""
                current_symbol = ""
                current_to_state_name = ""
                state = ScannerStates.RULE_START_SEPARATOR
            else:
                current_to_state_name += input_text[i]
        elif state == ScannerStates.RULE_START_SEPARATOR:
            if input_text[i] == ',':
                state = ScannerStates.START_STATE
            else:
                raise ScriptException("Expecting separator(,) between rules and start state", ERROR_SYNTACTIC)
        elif state == ScannerStates.START_STATE:
            if input_text[i] == ',':
                fsm.set_start(State(current_state_name))
                current_state_name = ""
                state = ScannerStates.BEGIN_FINAL
            else:
                current_state_name += input_text[i]
        elif state == ScannerStates.BEGIN_FINAL:
            if input_text[i] == '{':
                state = ScannerStates.FINAL_STATE
            else:
                raise ScriptException("{ has to be before final states", ERROR_SYNTACTIC)
        elif state == ScannerStates.FINAL_STATE:
            if input_text[i] == '}':
                fsm.add_final_state(State(current_state_name))
                current_state_name = ""
                state = ScannerStates.END
            elif input_text[i] == ',':
                fsm.add_final_state(State(current_state_name))
                current_state_name = ""
                state = ScannerStates.FINAL_STATE_NEXT
            else:
                current_state_name += input_text[i]
        elif state == ScannerStates.FINAL_STATE_NEXT:
            if input_text[i] == ',':
                state = ScannerStates.FINAL_STATE
            else:
                raise ScriptException("After comma has to be next state in finals", ERROR_SYNTACTIC)
        elif state == ScannerStates.END:
            break
        i += 1

                



args = parse_arguments()
input_text = read(args)
input_text = re.sub(r'#.*', repl='', string=input_text)


