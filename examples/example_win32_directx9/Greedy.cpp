#include "Greedy_h.h"

// Function Sorting By Profit
std::vector<values> Greedy_h::sort_market(std::vector<values> list_market) {
    std::sort(list_market.begin(), list_market.end(), [](const values& a, const values& b) {
        if (a.total_shares_profit != b.total_shares_profit)
        return a.total_shares_profit > b.total_shares_profit;
        else
            return a.total_cost < b.total_cost;
        });
    return list_market;
}

// Function Unsorted Market
std::vector<values> Greedy_h::unsorted_market(std::vector<values> list_market) {
    std::sort(list_market.begin(), list_market.end(), [](const values& a, const values& b) {
        if (a.total_shares_profit != b.total_shares_profit)
            return a.total_shares_profit < b.total_shares_profit;
        else
            return a.total_cost > b.total_cost;
        });
    return list_market;
}

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

bool* Greedy_h::vector_to_array(vector<bool> v) {
    bool* arr = new bool[v.size()];
    for (int i = 0; i < v.size(); i++) {
        arr[i] = v[i];
    }
    return arr;
}
