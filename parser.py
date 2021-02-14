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



# def find_first_helper(non_terminal, grammar):
#     non_terminals = [key for key in grammar]
#     firsts = []

#     for production in grammar[non_terminal]:
#         if not len(production):
#             continue
#         if production[0] in valid_terminal_set:
#             firsts.append(production[0])
#         elif production[0] in non_terminals:
#             firsts += find_first_helper(production[0], grammar)
    
#     return list(set(firsts))


def first(non_terminal, grammar):
    """This function returns the first of the non_terminal from grammer.
    Here '@' is the representation of epsilon.

    Args:
        non_terminal (String): It the non-terminal part from the grammer dictionary.
        grammar (Dictionary): Its keys are non_terminal and values are the list of the production from non_terminal

    Returns:
        ans [Set]: It contains the first of corresponding non_terminal
    """
    c = non_terminal[0]
    ans = set()
    if c.isupper():
	    for st in grammar[c]:
			if st == '@':				
				if len(non_terminal)!=1 :
					ans = ans.union(first(non_terminal[1:], grammar))
				else:
					ans = ans.union('@')
			else:
				f = first(st, grammar)
				ans = ans.union(x for x in f)
    else:
		ans = ans.union(c)
    return ans

def follow(non_terminal, grammar, ans, first_dict):
    """This function return the dictionary of follow for each non_terminal.

    Args:
        non_terminal (String): [description]
        grammar (Dictionary): Its keys are non_terminal and values are the list of the production from non_terminal
        ans (Dictionary): Empty dictionary where Keys are non_terminal and values are the set of follow for 
                            corresponding non_terminal
        first_dict (Dictionary): Dictionary with keys are non_terminal and values are the set which contains
                                the first of corresponding non_terminal

    Returns:
        ans (Dictionary): With Keys are non_terminal and values are the set of follow for 
                            corresponding non_terminal
    """

    for key in grammar:
		for value in grammar[key]:
			f = value.find(non_terminal)
			if f != -1:
				if f == (len(value)-1):
					if key != non_terminal:
						if key in ans:
							temp = ans[key]
						else:
							ans = follow(key, grammar, ans, first_dict)
							temp = ans[key]
						ans[non_terminal] = ans[non_terminal].union(temp)
				else:
					first_of_next = first_dict[value[f+1:]]
					if '@' in first_of_next:
						if key != non_terminal:
							if key in ans:
								temp = ans[key]
							else:
								ans = follow(key, grammar, ans, first_dict)
								temp = ans[key]
							ans[non_terminal] = ans[non_terminal].union(temp)
							ans[non_terminal] = ans[non_terminal].union(first_of_next) - {'@'}
					else:
						ans[non_terminal] = ans[non_terminal].union(first_of_next)
    return ans

def construct_parse_table():
    pass


# def find_first(grammar, first):
#     non_terminals = [non_terminal for non_terminal in grammar]
    
#     for non_terminal in non_terminals:
#         first_res = find_first_helper(non_terminal, grammar)
#         first[non_terminal] = first_res


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

# find_first(grammar, first)
# print(first)

non_terminals = [non_terminal for non_terminal in grammar]
extract_terminals(grammar, terminals)

parse_table = [['end'] * len(terminals)] * len(non_terminals)
#construct_parse_table(parse_table, first, follow, grammar)

#--------------------Calculation of First--------------------------
print('\nFirst\n')
for non_terminal in grammar:
    first_dict[non_terminal] = first(non_terminal, grammar)

for f in first_dict:
	print(str(f) + " : " + str(first_dict[f]))
print("")
#-----------------------------------------------------------------

#------------------Calculation of Follow--------------------------
print('\nFollow\n')
for non_terminal in grammar:
    follow_dict[non_terminal] = set()
    follow_dict[non_terminal].union('$')

for non_terminal in grammar:
	follow_dict = follow(non_terminal, grammar, follow_dict, first_dict)

for f in follow_dict:
	print(str(f) + " : " + str(follow_dict[f]))
#----------------------------------------------------------------

#-----------------Parser Table-----------------------------------




