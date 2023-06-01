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




    /// <summary>
    // Securities Code|Securities Name       |Scurities Type|Issuer                    |Registrar              |Isin Code   |ISIN Status|Listing Date|Number of Sec.|Stock Exchange|Status   |Nominal|Currency|Form      |Eff. Isin Date|Sec. sector
    // AALI           |ASTRA AGRO LESTARI Tbk|EQUITY        |ASTRA AGRO LESTARI Tbk, PT|RAYA SAHAM REGISTRA, PT|ID1000066004|Active     |09-DEC-1997 |1924688333    |IDX           |AVAILABLE|500    |IDR     |Electronic|04-APR-2001   |PLANTATION
    /// </summary>
/*
struct values {
    std::string SecuritiesCode;
    std::string SecuritiesName;
    std::string SecuritiesType;
    std::string Issuer;
    std::string Registrar;
    std::string IsinCode;
    std::string IsinStatus;
    std::string ListingDate;
    std::string NumberOfSecurities;
    std::string StockExchange;
    std::string Status;
    std::string Nominal;
    std::string Currency;
    std::string Form;
    std::string EffectiveIsinDate;
    std::string SecuritiesSector;
    int key;
};
*/

/// <summary>
/// No|Kode Saham|Nama Perusahaan|Remarks|Sebelumnya|Open Price|Tanggal Perdagangan Terakhir|First Trade|Tertinggi|Terendah|Penutupan|Selisih|Volume|Nilai|Frekuensi|Index Individual|Offer|Offer Volume|Bid|Bid Volume|Listed Shares|Tradeble Shares|Weight For Index|Foreign Sell|Foreign Buy|Non Regular Volume|Non Regular Value|Non Regular Frequency
/// 1 | AALI | Astra Agro Lestari Tbk. | --S - 18AEM16000D232------------ | 7400 | 0 | 23 Mei 2023 | 0 | 7550 | 7400 | 7500 | 100 | 666700 | 4993422500 | 621 | 609.3 | 7525 | 9100 | 7500 | 262800 | 1924688333 | 1924688333 | 390711732 | 168700 | 113600 | 88 | 653400 | 1
/// </summary>
struct values {
    int no;
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

    int total_lot; // (uang / open price)
    int total_cost; // (open price x lot)
    int total_shares_profit; // (harga penutupan - harga pembukaan) x jumlah saham
};

class Data
{
public:
    bool file_button = false;
    bool allOK = false;
    bool solution_button = false;
    int nominal = 0;
    int qty_lot = 0;
    std::vector<bool> solution;
    std::string file_path,file_name;
    std::vector<std::vector<values>> data;
    std::vector<std::vector<values>> sort_market;
    std::vector<std::vector<values>> column;
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

                    row.push_back(value);
                }
                key++;
                data.push_back(row);
            }
            file.close();
        }
        // print to logs
        for (int i = 0; i < data.size(); i++) {
            logs.AddLog("Row : %d\n", i);
            for (int j = 0; j < data[i].size(); j++) {
                logs.AddLog("Key : %d\n", data[i][j].no);
                logs.AddLog("KodeSaham : %s\n", data[i][j].KodeSaham.c_str());
                logs.AddLog("NamaPerusahaan : %s\n", data[i][j].NamaPerusahaan.c_str());
                logs.AddLog("Remarks : %s\n", data[i][j].Remarks.c_str());
                logs.AddLog("Sebelumnya : %s\n", data[i][j].Sebelumnya.c_str());
                logs.AddLog("OpenPrice : %s\n", data[i][j].OpenPrice.c_str());
                logs.AddLog("TanggalPerdaganganTerakhir : %s\n", data[i][j].TanggalPerdaganganTerakhir.c_str());
                logs.AddLog("FirstTrade : %s\n", data[i][j].FirstTrade.c_str());
                logs.AddLog("Tertinggi : %s\n", data[i][j].Tertinggi.c_str());
                logs.AddLog("Terendah : %s\n", data[i][j].Terendah.c_str());
                logs.AddLog("Penutupan : %s\n", data[i][j].Penutupan.c_str());
                logs.AddLog("Selisih : %s\n", data[i][j].Selisih.c_str());
                logs.AddLog("Volume : %s\n", data[i][j].Volume.c_str());
                logs.AddLog("Nilai : %s\n", data[i][j].Nilai.c_str());
                logs.AddLog("Frekuensi : %s\n", data[i][j].Frekuensi.c_str());
                logs.AddLog("IndexIndividual : %s\n", data[i][j].IndexIndividual.c_str());
                logs.AddLog("Offer : %s\n", data[i][j].Offer.c_str());
                logs.AddLog("OfferVolume : %s\n", data[i][j].OfferVolume.c_str());
                logs.AddLog("Bid : %s\n", data[i][j].Bid.c_str());
                logs.AddLog("BidVolume : %s\n", data[i][j].BidVolume.c_str());
                logs.AddLog("ListedShares : %s\n", data[i][j].ListedShares.c_str());
                logs.AddLog("TradebleShares : %s\n", data[i][j].TradebleShares.c_str());
                logs.AddLog("WeightForIndex : %s\n", data[i][j].WeightForIndex.c_str());
                logs.AddLog("ForeignSell : %s\n", data[i][j].ForeignSell.c_str());
                logs.AddLog("ForeignBuy : %s\n", data[i][j].ForeignBuy.c_str());
                logs.AddLog("NonRegularVolume : %s\n", data[i][j].NonRegularVolume.c_str());
                logs.AddLog("NonRegularValue : %s\n", data[i][j].NonRegularValue.c_str());
                logs.AddLog("NonRegularFrequency : %s\n\n", data[i][j].NonRegularFrequency.c_str());
            }
        }

        for (int i = 0; i < data[0].size(); i++) {
            std::vector<values> row;
            for (int j = 0; j < data.size(); j++) {
                row.push_back(data[j][i]);
            }
            column.push_back(row);
        }
        data.erase(data.begin());

        logs.AddLog("[INFO] Load Data Success\n");


       
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
            logs.AddLog("[INFO] Load Data Success!!\n");
        }

        if (this->data.size() > 0) {
            this->allOK = true;
            calculate(this->data, this->nominal,this->qty_lot);
            return true;
        }
        else
            return false;
    }

    

    void calculate(std::vector<std::vector<values>>& data, int nominal,int lot) {
        for (int i = 0; i < data.size(); i++) {
            for (int j = 0; j < data[i].size(); j++) {
                if (data[i][j].OpenPrice == "0") {
                    data[i][j].total_lot = 0;
                    continue;
                }

                if(lot != 0)
                    data[i][j].total_lot = lot;
                else
                    data[i][j].total_lot = (int)(nominal / std::stoi(data[i][j].OpenPrice));

                data[i][j].total_cost = (int)(std::stoi(data[i][j].OpenPrice) * data[i][j].total_lot);
                data[i][j].total_shares_profit = (int)(data[i][j].total_lot * (std::stoi(data[i][j].Penutupan) - std::stoi(data[i][j].OpenPrice)));
            }
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
};
