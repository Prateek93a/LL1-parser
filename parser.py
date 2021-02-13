#!/usr/bin/python3

import sys
valid_terminal_set = ['int', 'num', 'id', '*', '$', '+','-', '/', '(', ')', 'epsilon']

def convert_production_to_arr(grammar):
    non_terminals = [non_terminal for non_terminal in grammar]

    for non_terminal in non_terminals:
        productions = grammar[non_terminal]
        productions_tokens_arr = []

        for production in productions:
            production_tokens = []
            i = 0
            token = ''
            while i < len(production):
                token = token + production[i]
                ##print(token)
                ##if len(token)  == 1:
                ##    print(token)
                #if token == '*' or token == '-':
                #    print(token)
                if token in non_terminals or token in valid_terminal_set:
                    production_tokens.append(token)
                    token = ''
                i += 1
            productions_tokens_arr.append(production_tokens)
    
        grammar[non_terminal] = productions_tokens_arr


def extract_terminals(grammar, terminals):
    for productions in grammar.values():
        for production in productions:
            for symbol in production:
                if symbol in valid_terminal_set:
                    terminals.append(symbol)


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



def find_first_helper(non_terminal, grammar):
    non_terminals = [key for key in grammar]
    firsts = []

    for production in grammar[non_terminal]:
        if not len(production):
            continue
        if production[0] in valid_terminal_set:
            firsts.append(production[0])
        elif production[0] in non_terminals:
            firsts += find_first_helper(production[0], grammar)
    
    return list(set(firsts))

            
def find_first(grammar, first):
    non_terminals = [non_terminal for non_terminal in grammar]
    
    for non_terminal in non_terminals:
        first_res = find_first_helper(non_terminal, grammar)
        first[non_terminal] = first_res


def find_follow(grammar):
    non_terminals = [non_terminal for non_terminal in grammar]


grammar = {}
non_terminals = []
terminals = []
first = {}
follow = {}

accept_input_grammar(grammar)
print(grammar)

eliminate_left_recursion(grammar)
print(grammar)

convert_production_to_arr(grammar)
print(grammar)

find_first(grammar, first)
print(first)

non_terminals = [non_terminal for non_terminal in grammar]
extract_terminals(grammar, terminals)

parse_table = [['end'] * len(terminals)] * len(non_terminals)
#construct_parse_table(parse_table, first, follow, grammar)
