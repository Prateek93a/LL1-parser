#!/usr/bin/python3

import sys

def accept_input_grammar(grammar):
    line = input()
    while line != 'end':
        line = ''.join(line.split())
        non_terminal, production = line.split(':')
        
        if grammar.get(non_terminal) is None:
            grammar[non_terminal] = []
        
        grammar[non_terminal].append(production)
        line = input()


def check_for_left_recursion(non_terminal, productions):
    # making assumption that non terminal is single letter only
    for production in productions:
        if production[0] == non_terminal:
            return True

    return False

def find_non_recursive_symbol(non_terminal, productions):
    for production in productions:
        if production[0] != non_terminal:
            return production[0]
    return ''

def find_recursive_sub_expressions(non_terminal, productions):
    sub_expressions = []
    for production in productions:
        if production[0] == non_terminal:
            sub_expressions.append(production[1:])
    return sub_expressions

def eliminate_left_recursion(grammar):
    non_terminals = [non_terminal for non_terminal in grammar]

    for non_terminal in non_terminals:
        if check_for_left_recursion(non_terminal, grammar[non_terminal]):
            new_non_terminal = non_terminal.lower()
            new_productions = []

            recursive_sub_expressions = find_recursive_sub_expressions(non_terminal,grammar[non_terminal])
            for expression in recursive_sub_expressions:
                new_productions.append(expression + new_non_terminal)

            grammar[new_non_terminal] = new_productions

            non_recursive_symbol = find_non_recursive_symbol(non_terminal, grammar[non_terminal])
            new_rule = non_recursive_symbol + new_non_terminal
            grammar[non_terminal] = [new_rule]

            
def first(grammar):
    non_terminals = [non_terminal for non_terminal in grammar]



grammar = {}

accept_input_grammar(grammar)
print(grammar)

eliminate_left_recursion(grammar)
print(grammar)

