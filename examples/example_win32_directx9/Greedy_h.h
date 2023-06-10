#include "data.h"

using namespace std;

class Greedy_h
{
public:
        int cost = 0;
        int profit = 0;
        int total_lot = 0;
        std::vector<bool> solution;
        std::vector<bool> Solution_Greedy(std::vector<values> data, int nominal);
};
