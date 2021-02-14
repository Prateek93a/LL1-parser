#include "utilities.hpp"

std::string extract_non_terminal(std::string &rule){
    std::string non_terminal = "";
    for(char c: rule){
        if(c == ':') break;
        if(c == ' ' || c == '\t') continue;
        non_terminal.push_back(c);
    }

    return non_terminal;
}

std::string extract_production(std::string &rule){
    std::string production = "";
    bool is_non_terminal = true;

    for(char c: rule){
        if(c == ':'){
            is_non_terminal = false;
            continue;
        }
        if(c == ' ' || c == '\t') continue;
        if(!is_non_terminal) production.push_back(c);
    }

    return production;
}

std::vector<std::string> convert_to_arr(std::string &production_string){
    std::vector<std::string> production_arr;
    for(char c: production_string){
        std::string symbol = "";
        if(c == '#'){
            if(production_string.length() == 1) symbol.append("epsilon");
            else continue;
        }else{
            symbol.push_back(c);
        }
        
        production_arr.push_back(symbol);
    }

    return production_arr;
}

std::string scan(std::string &input){
    std::string scanned_input = "";
    int i = 0;
    while (i < input.size()){
        if(input[i] == ' ' || input[i] == '\t'){
            i++;
            continue;
        }  
        if(input[i] >= '0' && input[i] <= '9'){
            while(i < input.size() && input[i] >= '0' && input[i] <= '9'){
                i++;
            }
            scanned_input.push_back('n');
        }else if((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z')){
            while(i < input.size() && (input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z')){
                i++;
            }
            scanned_input.push_back('i');
        }else{
            scanned_input.push_back(input[i]);
            i++;
        }
    }
    
    scanned_input.push_back('$');
    return scanned_input;
}

void take_input(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar, std::string &start){
    int num_rules;
    
    std::cout<<"How many production rules?"<<std::endl;
    std::cin>>num_rules;

    std::cout<<"Enter the rules:"<<std::endl;
    while(num_rules--){
        std::string rule;
        std::getline(std::cin>>std::ws, rule);
        std::string non_terminal = extract_non_terminal(rule);
        std::string production_string = extract_production(rule);
        std::vector<std::string> production_arr = convert_to_arr(production_string);
        grammar[non_terminal].push_back(production_arr);
    }

    std::cout<<"What is the start symbol\n";
    std::cin>>start; // handle invalid start symbol error
    if(!grammar.count(start)){
        std::cerr<<"Invalid start symbol\n";
        exit(EXIT_FAILURE);
    }
}


bool check_left_recursion(std::string &non_terminal, std::vector<std::vector<std::string>> &productions){
    for(std::vector<std::string> &production: productions){
        if(production[0] == non_terminal) return true;
    }
    return false;
}

void eliminate_left_recursion(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar){
    std::vector<std::string> non_terminals;
    for(auto &i: grammar){
        non_terminals.push_back(i.first); // change to pointers
    }

    for(std::string &non_terminal: non_terminals){
        if(check_left_recursion(non_terminal, grammar[non_terminal])){
            std::string new_non_terminal = non_terminal + "'";

            for(std::vector<std::string> &production: grammar[non_terminal]){
                if(production[0] == non_terminal){
                    std::vector<std::string> new_production;
                    for(int i=1; i<production.size(); i++){
                        new_production.push_back(production[i]);
                    }
                    new_production.push_back(new_non_terminal);
                    grammar[new_non_terminal].push_back(new_production);

                }else{
                    std::vector<std::string> changed_production;
                    for(int i=0; i<production.size(); i++){
                        changed_production.push_back(production[i]);
                    }
                    changed_production.push_back(new_non_terminal);
                    grammar[non_terminal] = {changed_production};
                }
            }

            grammar[new_non_terminal].push_back({"epsilon"});
        }
    }

}


void find_first(std::string non_terminal, 
                std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar,
                std::unordered_map<std::string, std::vector<std::string>> &first){

    std::unordered_set<std::string> first_terminals_set;
    for(auto &production: grammar[non_terminal]){
        if(!grammar.count(production[0])){
            first_terminals_set.insert(production[0]);
        }else{
            if(!first.count(production[0])){
                find_first(production[0], grammar, first);
            }
            for(auto &symbol: first[production[0]]){
                first_terminals_set.insert(symbol);
            }
        }
    }

    for(auto &symbol: first_terminals_set) {
        first[non_terminal].push_back(symbol);
    } 
}


bool find_follow_helper(std::unordered_set<std::string> &follow_terminals_set, 
                        std::vector<std::string> &production, int i,
                        std::string &left_non_terminal, std::string &non_terminal,
                        std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar,
                        std::unordered_map<std::string, std::vector<std::string>> &first,
                        std::unordered_map<std::string, std::vector<std::string>> &follow){

    bool has_updated = false;
                      
    if(i == production.size()){
        if(non_terminal == left_non_terminal || !follow.count(left_non_terminal)) return has_updated;
        for(auto &symbol: follow[left_non_terminal]){
            if(!follow_terminals_set.count(symbol)){
                follow_terminals_set.insert(symbol);
                has_updated = true;
            }
        }
    }
    else if(grammar.count(production[i])){
        bool has_epsilon = true;
        while(i < production.size() && has_epsilon && grammar.count(production[i])){
            has_epsilon = false;
            for(auto &terminal: first[production[i]]){
                if(terminal != "epsilon"){
                    if(!follow_terminals_set.count(terminal)){
                        follow_terminals_set.insert(terminal);
                        has_updated = true;
                    }
                }else{
                    has_epsilon = true;
                }
            }
            i++;
        }

        if(has_epsilon){
            if(i == production.size()){
                if(!follow.count(left_non_terminal)) return has_updated;
                  for(auto &symbol: follow[left_non_terminal]){
                    if(!follow_terminals_set.count(symbol)){
                        follow_terminals_set.insert(symbol);
                        has_updated = true;
                    }
                }
            }else{
                if(!follow_terminals_set.count(production[i])){
                    follow_terminals_set.insert(production[i]);
                    has_updated = true;
                }
            }
        }
    }
    else{
        if(!follow_terminals_set.count(production[i])){
            follow_terminals_set.insert(production[i]);
            has_updated = true;
        }
    }

    return has_updated;
}


bool find_follow(std::string non_terminal,
                std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar,
                 std::unordered_map<std::string, std::vector<std::string>> &follow,
                 std::unordered_map<std::string, std::vector<std::string>> &first){

    std::unordered_set<std::string> follow_terminals_set;
    bool has_updated = false;
    
    if(follow.count(non_terminal)){
        for(auto &i: follow[non_terminal]) {
            follow_terminals_set.insert(i);
        }
    }

    for(auto &rule: grammar){
        for(auto &production: rule.second){
            int i=0;
            while(i < production.size()){
                if(production[i] == non_terminal){
                    break;
                }
                i++;
            }
            if(i == production.size()) continue;

            std::string left_non_terminal = rule.first;

            has_updated |= find_follow_helper(follow_terminals_set, production, i+1, left_non_terminal, non_terminal, grammar, first, follow);        
        }
    }

    if(has_updated){
        for(auto &symbol: follow_terminals_set) {
            bool contains = false;
            if(follow.count(non_terminal)){
                for(auto &i: follow[non_terminal]){
                    if(i == symbol){
                        contains = true;
                        break;
                    }
                }
            }

            if(!contains){
                follow[non_terminal].push_back(symbol);
            }
        }
    }

    return has_updated;
    
}


void find_non_terminal_to_index_map(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar, 
                                     std::unordered_map<std::string, int> &non_terminal_to_index){
    int index = 0;
    for(auto &i: grammar){
        non_terminal_to_index[i.first] = index;
        index++;
    }
}

void find_terminal_to_index_map(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar, 
                                std::unordered_map<std::string, int> &terminal_to_index){

    std::unordered_set<std::string> terminals;
    for(auto &i: grammar){
        for(auto &production: i.second){
            for(auto &symbol: production){
                if(!grammar.count(symbol)){
                    terminals.insert(symbol);
                }
            }
        }
    }

    int index = 0;
    for(auto &i: terminals){
        terminal_to_index[i] = index;
        index++;
    }
    terminal_to_index["$"] = index;
}


void compute_parse_table(std::vector<std::vector<std::vector<std::string> *>> &parse_table,
                    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar, 
                    std::unordered_map<std::string, std::vector<std::string>> &first, 
                    std::unordered_map<std::string, std::vector<std::string>> &follow,
                    std::unordered_map<std::string, int> &non_terminal_to_index,
                    std::unordered_map<std::string, int> &terminal_to_index){

    // check for errors
    for(auto &non_terminal_pair: non_terminal_to_index){
        for(auto &terminal_pair: terminal_to_index){
            int i = non_terminal_pair.second;
            int j = terminal_pair.second;
            std::string non_terminal = non_terminal_pair.first;
            std::string terminal = terminal_pair.first;

            for(auto &production: grammar[non_terminal]){
                if(!production.size()) continue;
                if(grammar.count(production[0])){
                    int count = 0;
                    for(auto &symbol: first[production[0]]){
                        if(symbol == terminal){
                            count++;
                        }
                    }

                    if(count == 1){
                        parse_table[i][j] = &production;
                        break;
                    }else if(count > 1){
                        std::cerr<<"Invalid grammar rules\n";
                        exit(EXIT_FAILURE);
                    }
                }else{
                    if(production[0] == terminal){
                       parse_table[i][j] = &production;
                       break; 
                    }else if(production[0] == "epsilon" && production.size() == 1){
                        bool has_found = false;
                        for(auto &symbol: follow[non_terminal]){
                            if(symbol == terminal){
                                has_found = true;
                                break;
                            }
                        }
                        if(has_found){
                            parse_table[i][j] = &production;
                            break;
                        }
                    }
                }
            }
        }
    }               
}