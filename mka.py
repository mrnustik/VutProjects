#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import sys
import re
from enum import Enum

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
    def __init__(self, name):

        self.name = name

    def __cmp__(self, other):
        if self.name < other.name:
            return -1
        elif self.name == other.name:
            return 0
        else:
            return 1

    def __eq__(self, other):
        if type(other) is str:
            return self.name == other
        if type(other) is State:
            return self.name == other.name
        else:
            return other is self

    def __str__(self):
        return str(self.name)


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

    def __eq__(self, other):
        if type(other) is str:
            return self.character == other
        elif type(other) is AlphabetCharacter:
            return self.character == other.character
        else:
            return other is self

    def __str__(self):
        return self.character


class Rule:
    def __init__(self, fr, symbol, to):
        self.fr = fr
        self.input = symbol
        self.to = to

    def __eq__(self, other):
        if type(other) is Rule:
            return other.fr == self.fr and other.input == self.input and other.to == self.to
        else:
            return False

    def __hash__(self):
        return hash(repr(self))

    def __str__(self):
        return str(self.fr) + " '" + str(self.input) + "' -> " + str(self.to)


class ScriptException(Exception):
    def __init__(self, message, error):
        self.message = message
        self.error = error

    pass


class GroupState:
    def __init__(self, states):
        self.states = []
        for state in states:
            self.add_state(state)

    def __str__(self):
        string = ""
        for state in self.states:
            if state == self.states[len(self.states) - 1]:
                string += str(state)
            else:
                string += str(state)+"_"
        return string

    def __eq__(self, other):
        return self is other

    def __len__(self):
        return len(self.states)

    def __iter__(self):
        for state in self.states:
            yield state

    def __hash__(self):
        return hash(repr(self))

    def contains(self, state):
        for st in self.states:
            if st == state:
                return True
        return False

    def add_state(self, state):
        self.states.append(state)


    @staticmethod
    def findGroup(groups, state):
        for group in groups:
            if state in group.states:
                return group
        return None



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
            if not rule in self.rules:
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

    def __find_to_rules(self, state, symbol=None):
        rules = []
        for rule in self.rules:
            if rule.to == state and (symbol is None or rule.input == symbol):
                rules.append(rule)
        return rules

    def __find_from_rules(self, state, symbol):
        rules = []
        for rule in self.rules:
            if rule.fr == state and (rule.input == symbol or symbol is None):
                rules.append(rule)
        return rules

    def find_non_finishing(self):
        non_finishing = []
        stack = self.final_states[:]
        visited = []
        while stack:
            current_state = stack.pop()
            if current_state not in visited:
                visited.append(current_state)
            rules = self.__find_to_rules(current_state, None)

            for rule in rules:
                if rule.fr not in visited:
                    stack.append(rule.fr)

        for state in self.states:
            if state not in visited:
                non_finishing.append(state)

        return non_finishing

    def find_unreachable(self):
        for state in self.states:
            if state != self.start:
                rules = self.__find_to_rules(state)
                rules_valid = False
                for rule in rules:
                    if rule.fr != rule.to:
                       rules_valid = True
                if len(rules) == 0 or not rules_valid:
                    return state
        return None

    def minimize(self):
        groups = []
        groups.append(GroupState(self.final_states))
        non_final = GroupState([])
        for state in self.states:
            if state not in self.final_states:
                non_final.add_state(state)
        groups.append(non_final)

        split = True
        while split:
            split = False
            for group in groups:
                group_dic = dict()
                for symbol in self.alphabet:
                    for state in group:
                        rule = self.__find_from_rules(state, symbol)[0]
                        to_state = rule.to
                        to_group = GroupState.findGroup(groups, to_state)
                        if to_group in group_dic:
                            group_dic[to_group].append(state)
                        else:
                            group_dic[to_group] = [state]
                    if len(group_dic) == 1:
                        group_dic.clear()
                    else:
                        for group1, states in group_dic.items():
                            groups.append(GroupState(states))
                        groups.remove(group)
                        split = True
                        break
                if split:
                    break

        newFsm = FiniteStateMachine()

        for group in groups:
            group.states.sort(key=lambda x: x.name)
            newFsm.add_state(State(str(group)))
            newFsm.states.sort(key=lambda x: x.name)

        for symbol in self.alphabet:
            newFsm.add_alphabet_character(symbol)

        for rule in self.rules:
            fr = State(str(GroupState.findGroup(groups, rule.fr)))
            symbol = rule.input
            to = State(str(GroupState.findGroup(groups, rule.to)))
            newFsm.add_rule(fr, symbol, to)

        newFsm.set_start(State(str(GroupState.findGroup(groups, self.start))))

        for finals in self.final_states:
            newFsm.add_final_state(str(GroupState.findGroup(groups, finals)))
        return newFsm

    def is_well_specified(self):
        unreachable = self.find_unreachable()
        if unreachable is not None:
            return False

        for state in self.states:
            for symbol in self.alphabet:
                rules = self.__find_from_rules(state, symbol)
                if len(rules) != 1:
                    print_error("Deterministic rule violated. State: " + str(state) + " symbol: " + str(symbol),
                                ERROR_NOT_DSKA)
                    return False

        non_finishing = self.find_non_finishing()
        if len(non_finishing) > 1:
            return False

        return True

    def str_states(self):
        states = "{"
        for state in self.states:
            if self.states[len(self.states) - 1] == state:
                states += str(state)
            else:
                states += str(state) + ", "
        states += "}"
        return states

    def str_alphabet(self):
        alphabet = "{"
        for symbol in self.alphabet:
            if self.alphabet[len(self.alphabet) - 1] == symbol:
                alphabet += "\'" + str(symbol) + "\'"
            else:
                alphabet += "\'" + str(symbol) + "\', "
        alphabet += "}"
        return alphabet

    def str_rules(self):
        rules = "{\n"
        for rule in self.rules:
            if self.rules[len(self.rules) - 1] == rule:
                rules += str(rule) + "\n"
            else:
                rules += str(rule) + ",\n"
        rules += "}"
        return rules

    def str_final_states(self):
        states = "{"
        for state in self.final_states:
            if self.final_states[len(self.final_states) - 1] == state:
                states += str(state)
            else:
                states += str(state) + ", "
        states += "}"
        return states

    def __str__(self):
        string = "(\n"
        string += self.str_states() + ",\n"
        string += self.str_alphabet() + ",\n"
        string += self.str_rules() + ",\n"
        string += str(self.start) + ",\n"
        string += self.str_final_states() + "\n"
        string += ")\n"
        return string


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

    read_args = parser.parse_args()
    if read_args.help:
        if len(sys.argv) == 2:
            parser.print_help()
            exit(EXIT_OK)
        else:
            print_error('Invalid combination of parameters. '
                        'Run help to see the possible params',
                        ERROR_INVALID_PARAMS)

    if read_args.minimize and read_args.find_non_finishing:
        print_error("Invalid combination of parameters. "
                    "Run help to see the possible params.",
                    ERROR_INVALID_PARAMS)

    arguments = Arguments()
    arguments.nonFinishing = read_args.find_non_finishing
    arguments.caseInsensitive = read_args.case_insensitive
    arguments.input = read_args.input
    arguments.output = read_args.output
    arguments.minimize = read_args.minimize
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
        if input_text[i] == '(':
            i += 1  # prvni znak nas nezajima
        if re.match("\s", input_text[i]) and state != ScannerStates.SYMBOL and state != ScannerStates.RULE_SYMBOL:
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
        elif state == ScannerStates.STATE_NEXT:
            state = ScannerStates.STATE
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
                raise ScriptException("Unexpected character in alphabet section", ERROR_SYNTACTIC)
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
                raise ScriptException("Alphabet symbol has to be 1 character placed between apostrophes",
                                      ERROR_SYNTACTIC)
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
                fsm.add_rule(State(current_from_state_name), AlphabetCharacter(current_symbol),
                             State(current_to_state_name))
                current_from_state_name = ""
                current_symbol = ""
                current_to_state_name = ""
                state = ScannerStates.RULE_FROM
            elif input_text[i] == '}':
                fsm.add_rule(State(current_from_state_name), AlphabetCharacter(current_symbol),
                             State(current_to_state_name))
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
                state = ScannerStates.FINAL_STATE
            else:
                current_state_name += input_text[i]
        elif state == ScannerStates.END:
            break
        i += 1
    return fsm


def write_result(machine, args):
    if args.output != sys.stdout:
        try:
            with open(args.output, 'w') as file:
                file.write(str(machine))
        except:
            print_error("Cant open the output file.", ERROR_INVALID_OUTPUT)
    else:
        args.output.write(str(machine))
    pass


try:
    args = parse_arguments()
    input_text = read(args)
    input_text = re.sub(r'#.*', repl='', string=input_text)
    machine = scan(input_text)
    if not machine.is_well_specified():
        print_error("Finite state machine not well specified", ERROR_NOT_DSKA)
    if args.nonFinishing:
        non_finishing = machine.find_non_finishing()
        write_result(str(non_finishing[0]) + "\n", args)
    else:
        if args.minimize:
            machine = machine.minimize()
        write_result(machine, args)
except ScriptException as ex:
    print_error(ex.message, ex.error)
