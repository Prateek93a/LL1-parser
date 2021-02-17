#include "utilities.hpp"

int main(){
    
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar;
    std::unordered_map<std::string, std::vector<std::string>> first;
    std::unordered_map<std::string, std::vector<std::string>> follow;
    std::unordered_map<std::string, int> non_terminal_to_index;
    std::unordered_map<std::string, int> terminal_to_index;
    std::vector<std::vector<std::vector<std::string> *>> parse_table;
    std::stack<std::string> parse_stack;
    std::string start;

    take_input(grammar, start);
    eliminate_left_recursion(grammar);


    std::cout<<"-----------Productions-----------"<<std::endl;
    for(auto i: grammar){
        std::cout<<i.first<<" => ";
        for(int j=0; j<i.second.size(); j++){
            auto &p = i.second[j];
            for(auto c: p){
                std::cout<<c;
            }
            if(j == (int)i.second.size()-1) continue;
            std::cout<<" | "; 
        }

        std::cout<<std::endl;
    }
    std::cout<<"-------------------------------"<<std::endl;

    bool has_updated = true;
    while(has_updated){
        has_updated = false;
        for(auto &i: grammar){
            has_updated |= find_first(i.first, grammar, first);
        }
    }
    //for(auto &i: grammar){
    //    if(!first.count(i.first)){
    //        find_first(i.first, grammar, first);
    //    }
    //}

    std::cout<<"-----------Firsts-----------"<<std::endl;
      for(auto i: grammar){
        std::cout<<i.first<<" => ";
        for(int j=0; j<first[i.first].size(); j++){
            std::cout<<first[i.first][j];
            if(j != (int)first[i.first].size()-1){
                std::cout<<", ";
            }
        }
        std::cout<<std::endl;
    }
    std::cout<<"-------------------------------"<<std::endl;

    follow[start].push_back("$");
    has_updated = true;
    //int temp = 0;

    while(has_updated){
        has_updated = false;
        for(auto &i: grammar){
            has_updated |= find_follow(i.first, grammar, follow, first);
        }
        //temp++;
    }


    std::cout<<"-----------Follows-----------"<<std::endl;
    for(auto i: grammar){
        std::cout<<i.first<<" => ";
        for(int j=0; j<follow[i.first].size(); j++){
            std::cout<<follow[i.first][j];
            if(j != (int)follow[i.first].size()-1){
                std::cout<<", ";
            }
        }
        std::cout<<std::endl;
    }
    std::cout<<"-------------------------------"<<std::endl;
    

    find_non_terminal_to_index_map(grammar, non_terminal_to_index);
    find_terminal_to_index_map(grammar, terminal_to_index);

    parse_table.assign(non_terminal_to_index.size(),
                    std::vector<std::vector<std::string>*>(terminal_to_index.size(),nullptr));

    compute_parse_table(parse_table, grammar, first, follow, non_terminal_to_index, terminal_to_index);

  
    while(true){
        std::cout<<"Enter the string, type 'end' to break\n";
        std::string input;
        std::getline(std::cin>>std::ws, input);
        if(input == "end") break;
        std::string scanned_input = scan(input);

        parse_stack.push("$");
        parse_stack.push(start);
        bool is_not_valid = false;
        int current_index = 0;

        while(!parse_stack.empty()){
            std::string current_symbol = "";
            current_symbol.push_back(scanned_input[current_index]);
            if(!terminal_to_index.count(current_symbol)){
                is_not_valid = true;
                while(!parse_stack.empty()){
                    parse_stack.pop();
                }
                break;
            }

            std::string current_stack_symbol = parse_stack.top();

            if(current_stack_symbol == current_symbol){
                parse_stack.pop();
                current_index++;
            }else if(grammar.count(current_stack_symbol)){
                int non_terminal_index = non_terminal_to_index[current_stack_symbol];
                int terminal_index = terminal_to_index[current_symbol];

                if(parse_table[non_terminal_index][terminal_index] != nullptr){
                    parse_stack.pop();
                    std::vector<std::string> *match = parse_table[non_terminal_index][terminal_index];
                    for(int i=(*match).size()-1; i>=0; i--){
                        if((*match)[i] != "epsilon"){
                            parse_stack.push((*match)[i]);
                        }
                    }
                }
                else{
                    current_index++;

                    if(current_index >= scanned_input.size()){
                        is_not_valid = true;
                        while(!parse_stack.empty()){
                            parse_stack.pop();
                        }
                    }

                }
            }else{
                is_not_valid = true;
                while(!parse_stack.empty()){
                    parse_stack.pop();
                }
            }
        }

        if(is_not_valid){
            std::cerr<<"Invalid string \n";
        }else{
            std::cout<<"String accepted \n";
        }
    }

}
