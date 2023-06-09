#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <tuple>
#include <d3d9.h>
#include <tchar.h>
#include <unordered_map>
#include <thread>
#include <windows.h>
#include <algorithm>

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "imgui_stdlib.h"


// Structs
struct AppLogs
{
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool                AutoScroll;  // Keep scrolling if already at the bottom.

    AppLogs()
    {
        AutoScroll = true;
        Clear();
    }

    void    Clear()
    {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
    }

    void    Draw(const char* title, bool* p_open = NULL)
    {
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        // Main window
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        bool clear = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        Filter.Draw("Filter", -100.0f);

        ImGui::Separator();

        if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (clear)
                Clear();
            if (copy)
                ImGui::LogToClipboard();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            const char* buf = Buf.begin();
            const char* buf_end = Buf.end();
            if (Filter.IsActive())
            {
                // In this example we don't use the clipper when Filter is enabled.
                // This is because we don't have random access to the result of our filter.
                // A real application processing logs with ten of thousands of entries may want to store the result of
                // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
                for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    if (Filter.PassFilter(line_start, line_end))
                        ImGui::TextUnformatted(line_start, line_end);
                }
            }
            else
            {
                // The simplest and easy way to display the entire buffer:
                //   ImGui::TextUnformatted(buf_begin, buf_end);
                // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
                // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
                // within the visible area.
                // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
                // on your side is recommended. Using ImGuiListClipper requires
                // - A) random access into your data
                // - B) items all being the  same height,
                // both of which we can handle since we have an array pointing to the beginning of each line of text.
                // When using the filter (in the block of code above) we don't have random access into the data to display
                // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
                // it possible (and would be recommended if you want to search through tens of thousands of entries).
                ImGuiListClipper clipper;
                clipper.Begin(LineOffsets.Size);
                while (clipper.Step())
                {
                    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                    {
                        const char* line_start = buf + LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                        ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                clipper.End();
            }
            ImGui::PopStyleVar();

            // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
            // Using a scrollbar or mouse-wheel will take away from the bottom edge.
            if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();
        ImGui::End();
    }
};

struct values {
    int no = 0;
    std::string KodeSaham;
    std::string NamaPerusahaan;
    std::string Remarks;
    std::string Sebelumnya;
    std::string OpenPrice;
    std::string TanggalPerdaganganTerakhir;
    std::string FirstTrade;
    std::string Tertinggi;
    std::string Terendah;
    std::string Penutupan;
    std::string Selisih;
    std::string Volume;
    std::string Nilai;
    std::string Frekuensi;
    std::string IndexIndividual;
    std::string Offer;
    std::string OfferVolume;
    std::string Bid;
    std::string BidVolume;
    std::string ListedShares;
    std::string TradebleShares;
    std::string WeightForIndex;
    std::string ForeignSell;
    std::string ForeignBuy;
    std::string NonRegularVolume;
    std::string NonRegularValue;
    std::string NonRegularFrequency;

    int total_lot = 0; // (uang / open price)
    int total_cost = 0; // (open price x lot)
    int total_shares_profit = 0; // (harga penutupan - harga pembukaan) x jumlah saham
    int take = 0;
};

class Data
{
public:
    bool file_button = false;
    bool allOK = false;
    bool solution_button = false;

    int nominal = 100000;
    int qty_lot = 1;
    
    
    std::string file_path,file_name;
    std::vector<values> data;
    std::vector<values> sort_market;
    std::vector<values> column;
    AppLogs logs;

    void openFile(HWND hWnd, std::string &file_path, std::string &file_name) {
        std::ofstream saveFile("logs.ini");
        OPENFILENAMEA ofn = { 0 };
        char szFile[200] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAMEA));

        ofn.lStructSize = sizeof(OPENFILENAMEA);
        ofn.hwndOwner = hWnd;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "Txt Files (*.txt)\0*.txt\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileNameA(&ofn) == TRUE)
        {
            file_path = ofn.lpstrFile;
            std::string::size_type filePos = file_path.rfind('\\');
            if (filePos != std::string::npos)
                ++filePos;
            else
                filePos = 0;
            saveFile << "\nPath Txt : " << file_path << std::endl;
            file_name = file_path.substr(filePos);
            saveFile << "\nName Txt : " << file_name << std::endl;

            logs.AddLog("Path Txt : %s\n", file_path.c_str());
            logs.AddLog("Name Txt : %s\n", file_name.c_str());
            saveFile.close();
        }
    }

    void readFile(std::string file_path) {
        std::ifstream file(file_path);
        std::string line;
        std::vector<values> temp;
        int key = 0;
        if (file.is_open()) {
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string item;
                std::vector<values> row;
                while (std::getline(ss, item, '|')) {
                    values value;
                    value.no = key;
                    std::getline(ss, item, '|');
                    value.KodeSaham = item;
                    std::getline(ss, item, '|');
                    value.NamaPerusahaan = item;
                    std::getline(ss, item, '|');
                    value.Remarks = item;
                    std::getline(ss, item, '|');
                    value.Sebelumnya = item;
                    std::getline(ss, item, '|');
                    value.OpenPrice = item;
                    std::getline(ss, item, '|');
                    value.TanggalPerdaganganTerakhir = item;
                    std::getline(ss, item, '|');
                    value.FirstTrade = item;
                    std::getline(ss, item, '|');
                    value.Tertinggi = item;
                    std::getline(ss, item, '|');
                    value.Terendah = item;
                    std::getline(ss, item, '|');
                    value.Penutupan = item;
                    std::getline(ss, item, '|');
                    value.Selisih = item;
                    std::getline(ss, item, '|');
                    value.Volume = item;
                    std::getline(ss, item, '|');
                    value.Nilai = item;
                    std::getline(ss, item, '|');
                    value.Frekuensi = item;
                    std::getline(ss, item, '|');
                    value.IndexIndividual = item;
                    std::getline(ss, item, '|');
                    value.Offer = item;
                    std::getline(ss, item, '|');
                    value.OfferVolume = item;
                    std::getline(ss, item, '|');
                    value.Bid = item;
                    std::getline(ss, item, '|');
                    value.BidVolume = item;
                    std::getline(ss, item, '|');
                    value.ListedShares = item;
                    std::getline(ss, item, '|');
                    value.TradebleShares = item;
                    std::getline(ss, item, '|');
                    value.WeightForIndex = item;
                    std::getline(ss, item, '|');
                    value.ForeignSell = item;
                    std::getline(ss, item, '|');
                    value.ForeignBuy = item;
                    std::getline(ss, item, '|');
                    value.NonRegularVolume = item;
                    std::getline(ss, item, '|');
                    value.NonRegularValue = item;
                    std::getline(ss, item, '|');
                    value.NonRegularFrequency = item;

                    data.push_back(value);
                }
                key++;
            }
            file.close();
        }
        
        column.push_back(data[0]);
        data.erase(data.begin());

        logs.AddLog("[INFO] Load Data Success\n");
    }

    void dropRowsWithZeroOpenPrice(std::vector<values>& list_market) {
        auto it = list_market.begin();
        while (it != list_market.end()) {
            if (it->OpenPrice == "0") {
                it = list_market.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    bool checkBtn(HWND hWnd) {
        if (file_button) {
            logs.AddLog("[INFO] Load Data!!\n");
            logs.AddLog("[WARNING] Clear Data!!\n");
            this->data.clear();
            this->column.clear();
            this->allOK = false;
            logs.AddLog("[INFO] Clear Data Success!!\n");
            openFile(hWnd,file_path, file_name);
            readFile(file_path);
            dropRowsWithZeroOpenPrice(this->data);
            logs.AddLog("[INFO] Load Data Success!!\n");
        }

        if (this->data.size() > 0) {
            this->allOK = true;
            ///calculate(this->data, this->nominal,this->qty_lot);
            return true;
        }
        else
            return false;
    }

    void calculate(std::vector<values>& data, int nominal,int lot) {
        for (int i = 0; i < data.size(); i++) {
            if (data[i].OpenPrice == "0") {
                data[i].total_lot = 0;
                continue;
            }

            if (lot != 0)
                data[i].total_lot = lot;
            else
                data[i].total_lot = (int)(nominal / std::stoi(data[i].OpenPrice));

            data[i].total_cost = (int)(std::stoi(data[i].OpenPrice) * data[i].total_lot);
            data[i].total_shares_profit = (int)(data[i].total_lot * (std::stoi(data[i].Penutupan) - std::stoi(data[i].OpenPrice)));
        }
    }

    std::string formatCurrencyInt(int number) {
        std::stringstream ss;
        ss.imbue(std::locale(""));
        ss << number;
        return ss.str();
    }

    std::string formatCurrencyString(std::string numberString) {
        int length = numberString.length();
        int commaCount = (length - 1) / 3;

        for (int i = 1; i <= commaCount; i++) {
            int insertPosition = length - (i * 3);
            numberString.insert(insertPosition, ".");
        }

        return numberString;
    }

    std::vector<values> sortByCost(std::vector<values> list_market) {
        std::sort(list_market.begin(), list_market.end(), [](const values& a, const values& b) {
            return a.total_cost > b.total_cost;
            });
        return list_market;
    }

    // Function Sorting By Profit
    std::vector<values> Sort_market(std::vector<values> list_market) {
        std::sort(list_market.begin(), list_market.end(), [](const values& a, const values& b) {
            if (a.total_shares_profit != b.total_shares_profit)
            return a.total_shares_profit > b.total_shares_profit;
            else
                return a.total_cost < b.total_cost;
            });
        return list_market;
    }

    // Function Unsorted Market
    std::vector<values> unsorted_market(std::vector<values> list_market) {
        std::sort(list_market.begin(), list_market.end(), [](const values& a, const values& b) {
            if (a.total_shares_profit != b.total_shares_profit)
            return a.total_shares_profit < b.total_shares_profit;
            else
                return a.total_cost > b.total_cost;
            });
        return list_market;
    }
};
