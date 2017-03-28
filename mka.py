
import argparse
import sys
from pprint import pprint

EXIT_OK = 0
ERROR_INVALID_PARAMS = 1
ERROR_INVALID_INPUT = 2
ERROR_INVALID_OUTPUT = 3
ERROR_INVALID_FORMAT = 4
ERROR_SYNTACTIC = 60
ERROR_SEMANTIC = 61
ERROR_NOT_DSKA = 62


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


class InvalidStateException(Exception):
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
            raise InvalidStateException("Trying to create rule with invalid parameter", ERROR_SEMANTIC)

    def add_alphabet_character(self, character):
        self.alphabet.append(character)

    def add_final_state(self, final_state):
        if final_state in self.states:
            self.final_states.append(final_state)
        else:
            raise InvalidStateException("Trying to add final state that is not defined", ERROR_SEMANTIC)

    def set_start(self, start_state):
        if start_state in self.states:
            self.start = start_state
        else:
            raise InvalidStateException("Trying to add start state that is not defined", ERROR_SEMANTIC)


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
                            default='sys.stdin')

        parser.add_argument('--output',
                            help="output file",
                            required=False,
                            default='sys.stdout')

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
                print_error('Invalid combination of parameters. Run help to see the possible params', ERROR_INVALID_PARAMS)

        if args.minimize and args.find_non_finishing:
            print_error("Invalid combination of parameters. Run help to see the possible params.", ERROR_INVALID_PARAMS)

        arguments = Arguments()
        arguments.nonFinishing = args.find_non_finishing
        arguments.caseInsensitive = args.case_insensitive
        arguments.input = args.input
        arguments.output = args.output
        arguments.minimize = args.minimize

        return arguments



args = parse_arguments()




