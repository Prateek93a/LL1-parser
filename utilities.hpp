#if !defined(UTILITIES)
#define UTILITIES

#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <string>

std::string extract_non_terminal(std::string &);
std::string extract_production(std::string &);
std::string scan(std::string &);
void take_input(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &, 
                 std::string &);
void eliminate_left_recursion(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &);
void find_first(std::string, 
                std::unordered_map<std::string, std::vector<std::vector<std::string>>> &,
                std::unordered_map<std::string, std::vector<std::string>> &);
void find_follow(std::string,
                std::unordered_map<std::string, std::vector<std::vector<std::string>>> &, 
                 std::unordered_map<std::string, std::vector<std::string>> &, 
                std::unordered_map<std::string, std::vector<std::string>> &);
void find_non_terminal_to_index_map(std::unordered_map<std::string, 
                                    std::vector<std::vector<std::string>>> &,
                                    std::unordered_map<std::string, int> &);
void find_terminal_to_index_map(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &,
                                  std::unordered_map<std::string, int> &);
std::vector<std::string> find_follow_helper(std::string,
                                             std::unordered_map<std::string,
                                              std::vector<std::vector<std::string>>> &,
                                              std::unordered_map<std::string,
                                               std::vector<std::string>> &);

#endif // UTILITIES
