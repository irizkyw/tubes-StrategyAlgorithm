#include "Greedy_h.h"

class Dynamic {
public:
    Data C_data;

    // dp
    int cost_dp = 0;
    int profit_dp = 0;
    std::vector<values> data_dp;
    std::vector<bool> solution_dp;

    std::vector<bool> dp(std::vector<values>data, int total_money);
};

