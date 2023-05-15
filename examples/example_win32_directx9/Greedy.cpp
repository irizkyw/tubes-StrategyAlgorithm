#include "Greedy_h.h"


// Function Sorting By Profit
std::vector<std::tuple<std::string, int, int, int>> Greedy_h::sort_market(std::vector<std::tuple<std::string, int, int, int >> list_market) {
        std::sort(list_market.begin(), list_market.end(),
        [](const auto& a, const auto& b) {
            return std::get<2>(a) > std::get<2>(b);
        });

        return list_market;
}

// get solusi, profit, cost. 
std::tuple<int, int, std::vector<bool>> Greedy_h::get_profit_cost(std::vector<std::tuple<std::string, int, int, int >> list_market, int budget) {
    // Sorting list market by profit
    std::vector<std::tuple<std::string, int, int, int >> sorted = sort_market(list_market);
    int total_profit = 0;
    int total_cost = 0;

    std::vector<bool> is_selected; // jika true maka dipilih.
    for (int i = 0; i < sorted.size(); i++) {
        if (total_cost + std::get<1>(sorted[i]) <= budget) { // jika cost + cost dari market <= budget
            total_profit += std::get<2>(sorted[i]); // profit ditambahkan
            total_cost += std::get<1>(sorted[i]); // cost ditambahkan

            is_selected.push_back(true); // jika true maka dipilih.
        }
        else {
            is_selected.push_back(false);
        }
    }
    return std::make_tuple(total_profit, total_cost, is_selected); // return profit, cost, dan is_selected
}

bool* Greedy_h::vector_to_array(vector<bool> v) {
    bool* arr = new bool[v.size()];
    for (int i = 0; i < v.size(); i++) {
        arr[i] = v[i];
    }
    return arr;
}
