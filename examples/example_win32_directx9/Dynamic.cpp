#include "Dynamic_h.h"

std::vector<bool> Dynamic::dp(std::vector<values>data, int total_money) {
    // Menghitung jumlah saham yang tersedia
    int n = data.size();
    // Membuat vektor untuk menyimpan berat dan nilai setiap saham
    std::vector<int> w(n);
    std::vector<int> p(n);

    // Mengisi vektor berat dengan cost dan nilai dari data(profit)
    for (int i = 0; i < n; i++) {
        w[i] = data[i].total_cost;
        p[i] = data[i].total_shares_profit;
    }

    // Implementasi algoritma knapsack
    std::vector<std::vector<int>> table(n + 1, std::vector<int>(total_money + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= total_money; j++) {
            if (w[i - 1] <= j) {
                table[i][j] = max(p[i - 1] + table[i - 1][j - w[i - 1]], table[i - 1][j]);
            }
            else {
                table[i][j] = table[i - 1][j];
            }
        }
    }

    std::vector<bool> solution(data.size(), 0);
    int curr_w = total_money;
    int curr_i = data.size();
    int cost = 0;
    int profit = 0;
    while (curr_i != 0) {
        if (table[curr_i - 1][curr_w] < table[curr_i][curr_w]) {
            solution[curr_i - 1] = true;
            curr_w -= w[curr_i - 1];
            cost += w[curr_i - 1];
            profit += p[curr_i - 1];
            curr_i -= 1;
        }
        else {
            curr_i -= 1;
        }
    }
    for (int i = 0; i < solution.size(); i++) {
        if (solution[i] == 1) {
            C_data.logs.AddLog("%s\n", data[i].KodeSaham.c_str());
        }
    }
    this->cost_dp = cost;
    this->profit_dp = profit;
    return solution;
}
