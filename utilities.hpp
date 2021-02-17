#if !defined(UTILITIES)
#define UTILITIES

#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <string>


void take_input(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &, 
                 std::string &);
void eliminate_left_recursion(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &);
bool find_first(std::string, 
                std::unordered_map<std::string, std::vector<std::vector<std::string>>> &,
                std::unordered_map<std::string, std::vector<std::string>> &);
bool find_follow(std::string non_terminal,
                std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar,
                 std::unordered_map<std::string, std::vector<std::string>> &follow,
                 std::unordered_map<std::string, std::vector<std::string>> &first);
void find_non_terminal_to_index_map(std::unordered_map<std::string, 
                                    std::vector<std::vector<std::string>>> &,
                                    std::unordered_map<std::string, int> &);
void find_terminal_to_index_map(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &,
                                  std::unordered_map<std::string, int> &);
void compute_parse_table(std::vector<std::vector<std::vector<std::string> *>> &,
                    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &, 
                 std::unordered_map<std::string, std::vector<std::string>> &, 
                std::unordered_map<std::string, std::vector<std::string>> &,
                std::unordered_map<std::string, int> &,
                std::unordered_map<std::string, int> &);

std::string scan(std::string &);

#endif // UTILITIES
