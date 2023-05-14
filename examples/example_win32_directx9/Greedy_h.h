#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

class Greedy_h
{
    public:
        std::vector<std::tuple<std::string, int, int, int>> sort_market(std::vector<std::tuple<std::string, int, int, int >> list_market);
        std::tuple<int, int, std::vector<bool>> get_profit_cost(std::vector<std::tuple<std::string, int, int, int >> list_market, int budget);
        // vector to array boolean
        bool* vector_to_array(vector<bool> v);
};
