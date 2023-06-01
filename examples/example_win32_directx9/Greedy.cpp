#include "Greedy_h.h"


// Function Sorting By Profit
std::vector<std::vector<values>> Greedy_h::sort_market(std::vector<std::vector<values>> list_market) {
    std::sort(list_market.begin(), list_market.end(), [](const std::vector<values>& a, const std::vector<values>& b) {
        if (a[0].total_shares_profit != b[0].total_shares_profit)
        return a[0].total_shares_profit > b[0].total_shares_profit;
        else
            return a[0].total_cost < b[0].total_cost;
        });
       return list_market;
}

// Function Unsorted Market
std::vector<std::vector<values>> Greedy_h::unsorted_market(std::vector<std::vector<values>> list_market) {
    std::sort(list_market.begin(), list_market.end(), [](const std::vector<values>& a, const std::vector<values>& b) {
        if (a[0].total_shares_profit != b[0].total_shares_profit)
            return a[0].total_shares_profit < b[0].total_shares_profit;
        else
            return a[0].total_cost > b[0].total_cost;
        });
    return list_market;
}

std::vector<bool> Greedy_h::Solution_Greedy(std::vector<std::vector<values>> data, int nominal) {
    std::vector<bool> solution;
    int total_cost = 0;
    int total_shares_profit = 0;
    int total_lot = 0;
    int sisa_uang = nominal;
    C_data.logs.AddLog("[INFO] Solution Greedy\n");
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            if (total_cost + data[i][j].total_cost <= nominal && data[i][j].total_cost >= 0) {

                total_cost += data[i][j].total_cost;
                total_shares_profit += data[i][j].total_shares_profit;
                total_lot += data[i][j].total_lot;
                solution.push_back(true);
                C_data.logs.AddLog("Saham(%s) : %d\n\n", data[i][j].KodeSaham.c_str(), data[i][j].total_cost);
            }
            else solution.push_back(false);
        }
    }

    C_data.logs.AddLog("\n[INFO] Total Cost : %d", total_cost);
    C_data.logs.AddLog("\n[INFO] Total Shares Profit : %d", total_shares_profit);
    C_data.logs.AddLog("\n[INFO] Total Lot : %d", total_lot);

    this->cost = total_cost;
    this->profit = total_shares_profit;
    this->total_lot = total_lot;

    return solution;
}

std::vector<bool> Greedy_h::Solution_DynamicProgramming(std::vector<std::vector<values>> data, int nominal) {
    std::vector<bool> solution;
    int total_cost = 0;
    int total_shares_profit = 0;
    int total_lot = 0;
    int sisa_uang = nominal;
    C_data.logs.AddLog("[INFO] Solution Dynamic Programming\n");
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            if (total_cost + data[i][j].total_cost <= nominal && data[i][j].total_cost >= 0) {

                total_cost += data[i][j].total_cost;
                total_shares_profit += data[i][j].total_shares_profit;
                total_lot += data[i][j].total_lot;
                solution.push_back(true);
                C_data.logs.AddLog("Saham(%s) : %d\n\n", data[i][j].KodeSaham.c_str(), data[i][j].total_cost);
            }
            else solution.push_back(false);
        }
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
