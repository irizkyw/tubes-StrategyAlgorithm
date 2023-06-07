#include "data.h"

using namespace std;

class Greedy_h
{
public:
        Data C_data;
        int cost = 0;
        int profit = 0;
        int total_lot = 0;
        std::vector<values> sort_market(std::vector<values> list_market);
        std::vector<values> unsorted_market(std::vector<values> list_market);
        std::vector<bool> Solution_Greedy(std::vector<values> data, int nominal);
        bool* vector_to_array(vector<bool> v);
};
