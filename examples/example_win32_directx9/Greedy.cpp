#include "Greedy_h.h"
std::vector<bool> Greedy_h::Solution_Greedy(std::vector<values> data, int nominal) {
    std::vector<bool> solution;
    int total_cost = 0;
    int total_shares_profit = 0;
    int total_lot = 0;
    int sisa_uang = nominal;
    C_data.logs.AddLog("[INFO] Solution Greedy\n");
    for (int i = 0; i < data.size(); i++) {
        if (total_cost + data[i].total_cost <= nominal && data[i].total_cost >= 0) {

            total_cost += data[i].total_cost;
            total_shares_profit += data[i].total_shares_profit;
            total_lot += data[i].total_lot;
            solution.push_back(true);
            C_data.logs.AddLog("Saham(%s) : %d\n\n", data[i].KodeSaham.c_str(), data[i].total_cost);
        }
        else solution.push_back(false);
    }

    C_data.logs.AddLog("\n[INFO] Total Cost : %d", total_cost);
    C_data.logs.AddLog("\n[INFO] Total Shares Profit : %d", total_shares_profit);
    C_data.logs.AddLog("\n[INFO] Total Lot : %d", total_lot);

    this->cost = total_cost;
    this->profit = total_shares_profit;
    this->total_lot = total_lot;

    return solution;
}
