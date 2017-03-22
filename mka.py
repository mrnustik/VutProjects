
import argparse


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


class NotExistingStateError(Exception):


class FiniteStateMachine:

    def __init__(self):
        self.states = []
        self.rules = []
        self.alphabet = []
        self.start = None
        self.final_states = []

    def add_state(self, state):
        self.states.append(state)

    def add_rule(self, rule):
        self.rules.append(rule)

    def add_alphabet_character(self, character):
        self.alphabet.append(character)

    def add_final_state(self, final_state):
        if final_state in self.states:
            self.final_states.append(final_state)
        else:
            raise

    def set_start(self, start_state):
        self.start = start_state

print(ahoj)
print(a)
print(b)
print(rule)

