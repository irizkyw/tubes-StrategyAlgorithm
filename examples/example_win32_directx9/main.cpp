#include "Dynamic_h.h"

// Data
static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void embraceTheDarkness()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
    style.ScrollbarRounding = 0;

    style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
}

// Main code
int main(int, char**)
{
    // declare
    Data data;
    Greedy_h greedy;
    Dynamic dynamic;
    bool* solution = new bool[data.sort_market.size()];

    //
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Strategy Algorithm", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    embraceTheDarkness();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    bool simple_layout = false;
    bool tree_contributor = true;
    bool console = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            g_d3dpp.BackBufferWidth = g_ResizeWidth;
            g_d3dpp.BackBufferHeight = g_ResizeHeight;
            ResetDevice();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        std::vector<std::pair<std::string, int>> menu_sidebar = {
                   {"Introduction", 0},
                   {"Data",1},
                   {"Solution",2}
        };
        std::vector< std::tuple<std::string, int, int, int> > list_market{
            std::make_tuple("A", 10, 0,0),
            std::make_tuple("B", 10, 0,0),
            std::make_tuple("C", 10, 0,0),
            std::make_tuple("D", 10, 0,0),
            std::make_tuple("E", 10, 0,0),
            std::make_tuple("F", 10, 0,0),
            std::make_tuple("G", 10, 0,0),
            
        };

        // Show simple layout
        static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        static bool use_work_area = true;
        ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
        ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
        //ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("Strategy Algorithm", &simple_layout, window_flags))
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Settings"))
                {
                    if (ImGui::MenuItem("Console", NULL, console)) {
                        console = !console;
                    }

                    if (ImGui::MenuItem("Demo", NULL, show_demo_window)) {
                        show_demo_window = !show_demo_window;
                    }

                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            // Left - Sidebar
            static int selected = 0;
            {
                ImGui::BeginChild("left pane", ImVec2(150, 0), true);


                for (const auto& item : menu_sidebar) {
                    char label[128];
                    sprintf(label, std::strcpy(label, item.first.c_str()));
                    if (ImGui::Selectable(label, selected == item.second))
                        selected = item.second;
                }

                ImGui::EndChild();
            }
            ImGui::SameLine();

            // Right - Content
            {
                ImGui::BeginGroup();
                ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
                ImGui::Text(" # %s", menu_sidebar[selected].first.c_str());
                ImGui::Separator();
                if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
                {
                    // Introduction
                    if (menu_sidebar[selected].second == 0) {
                        if (ImGui::BeginTabItem("Description"))
                        {
                            ImGui::SetNextItemOpen(true, ImGuiTreeNodeFlags_DefaultOpen);
                            if (ImGui::TreeNode("Kontributor"))
                            {
                                ImGui::BulletText("Caecarryo Bagus Dewanata - 1301213514");
                                ImGui::BulletText("Andhika loiz - 1301210336");
                                ImGui::BulletText("Ichwan Rizky Wahyudin - 1301213434");
                                ImGui::TreePop();
                            }
                            ImGui::Text("\nID: 0x%d", menu_sidebar[selected].second);
                            ImGui::EndTabItem();
                        }
                    }

                    // Data
                    if (menu_sidebar[selected].second == 1) {
                        enum ContentsType { CT_Text, CT_FillButton };
                        static int contents_type = CT_Text;
                        static ImGuiTableFlags flags_table = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

                        if (ImGui::BeginTabItem("Description")) {
                            ImGui::Text("Data yang digunakan adalah data saham yang diperoleh dari BEI (Bursa Efek Indonesia) yang berisi data saham yang diperdagangkan di BEI.");
                            ImGui::Text("Data saham ini sudah diringkas dalam diperdagangkan pada tahun 2022 - 2023.\n");
                            ImGui::Text("File:");
                            ImGui::SameLine();;
                            // call data
                            ImGui::PushItemWidth(400);
                            data.file_button = ImGui::Button((!data.file_button) ? "Select File" : data.file_name.c_str());
                            ImGui::BeginChild("Data",ImVec2(0,0));
                            if(data.allOK)
                            if (ImGui::BeginTable("table1", 6, flags_table))
                            {

                                ImGui::TableSetupColumn(data.column[0].KodeSaham.c_str() ,100.0f);
                                ImGui::TableSetupColumn(data.column[0].OpenPrice.c_str());
                                ImGui::TableSetupColumn(data.column[0].Penutupan.c_str());
                                ImGui::TableSetupColumn(data.column[0].Selisih.c_str());
                                ImGui::TableSetupColumn(data.column[0].Volume.c_str());
                                ImGui::TableSetupColumn(data.column[0].Frekuensi.c_str());


                                ImGui::TableHeadersRow();


                                for (int row = 0; row < data.data.size(); row++)
                                {
                                    if (data.data[row].OpenPrice == "0") continue;
                                    ImGui::TableNextRow();
                                    ImGui::TableSetColumnIndex(0);
                                    ImGui::Text("%s", data.data[row].KodeSaham.c_str());
                                    ImGui::TableSetColumnIndex(1);
                                    ImGui::Text("%s", data.data[row].OpenPrice.c_str());
                                    ImGui::TableSetColumnIndex(2);
                                    ImGui::Text("%s", data.data[row].Penutupan.c_str());
                                    ImGui::TableSetColumnIndex(3);
                                    ImGui::Text("%s", data.data[row].Selisih.c_str());
                                    ImGui::TableSetColumnIndex(4);
                                    ImGui::Text("%s", data.data[row].Volume.c_str());
                                    ImGui::TableSetColumnIndex(5);
                                    ImGui::Text("%s", data.data[row].Frekuensi.c_str());
                                }
                                ImGui::EndTable();
                            }
                            data.checkBtn(hwnd);
                            ImGui::EndChild();
                            ImGui::EndTabItem();
                        }
                        ImGui::Text("\nID: 0x%d", menu_sidebar[selected].second);
                    }

                    // Calculate
                    if (menu_sidebar[selected].second == 2) {
                        enum ContentsType { CT_Text, CT_FillButton };
                        static int contents_type = CT_Text;
                        static int nominal = 0;
                        static int value = 0;
                        static bool btn_state = false;

                        if (ImGui::BeginTabItem("Calculate")) {
                            size_t tuple_size = std::tuple_size<decltype(list_market)::value_type>::value;
                            // Input value
                            {
                                ImGui::BeginChild("data", ImVec2(0, 150), true);
                                ImGui::Text("Masukan Uang yang dimiliki : ");
                                ImGui::InputInt("##input", &data.nominal);
                                //ImGui::Text("Jumlah saham yang akan dibeli: ");
                                //ImGui::InputInt("##saham", &nominal);
                                if (data.nominal < 0) data.nominal = 0;

                                ImGui::Text("Masukan jumlah Lot yang dibeli : ");
                                ImGui::InputInt("##input_lot", &data.qty_lot);
                                if (data.qty_lot < 0) data.qty_lot = 0;

                                if (ImGui::Button("Calculate")) {
                                    data.calculate(data.data, data.nominal, data.qty_lot);
                                    data.sort_market = data.Sort_market(data.data);
                                    greedy.solution = greedy.Solution_Greedy(data.sort_market, data.nominal);
                                    dynamic.solution_dp = dynamic.dp(data.data, data.nominal);
                                }
                                ImGui::EndChild();
                            }

                            // List Market
                            {
                                ImGui::BeginGroup();
                                ImGui::BeginChild("list_market", ImVec2(1400, list_market.size() * 30), true);
                                ImGui::Text("List Market saat ini:");
                                static ImGuiTableFlags flags_table = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
                                if (data.allOK) {
                                    if (ImGui::BeginTable("table1", 6, flags_table))
                                    {

                                        ImGui::TableSetupColumn(data.column[0].KodeSaham.c_str());
                                        ImGui::TableSetupColumn(data.column[0].OpenPrice.c_str());
                                        ImGui::TableSetupColumn(data.column[0].Penutupan.c_str());
                                        ImGui::TableSetupColumn(data.column[0].Selisih.c_str());
                                        ImGui::TableSetupColumn(data.column[0].Volume.c_str());
                                        ImGui::TableSetupColumn(data.column[0].Frekuensi.c_str());


                                        ImGui::TableHeadersRow();


                                        for (int row = 0; row < data.data.size(); row++)
                                        {
                                            if (data.data[row].OpenPrice == "0") continue;
                                            ImGui::TableNextRow();
                                            ImGui::TableSetColumnIndex(0);
                                            ImGui::Text("%s", data.data[row].KodeSaham.c_str());
                                            ImGui::TableSetColumnIndex(1);
                                            ImGui::Text("%s", data.formatCurrencyString(data.data[row].OpenPrice).c_str());
                                            ImGui::TableSetColumnIndex(2);
                                            ImGui::Text("%s", data.formatCurrencyString(data.data[row].Penutupan).c_str());
                                            ImGui::TableSetColumnIndex(3);
                                            ImGui::Text("%s", data.formatCurrencyString(data.data[row].Selisih).c_str());
                                            ImGui::TableSetColumnIndex(4);
                                            ImGui::Text("%s", data.formatCurrencyString(data.data[row].Volume).c_str());
                                            ImGui::TableSetColumnIndex(5);
                                            ImGui::Text("%s", data.formatCurrencyString(data.data[row].Frekuensi).c_str());
                                        }
                                        ImGui::EndTable();
                                    }
                                }
                                else ImGui::Text("Data not found");


                                ImGui::EndChild();
                            }

                            // Greedy Solution
                            {
                                ImGui::BeginChild("list_greedy", ImVec2(1400, list_market.size() * 30), true);
                                ImGui::Text("Greedy:");
                                static ImGuiTableFlags flags_table = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
                                if (data.allOK) {

                                    if (ImGui::BeginTable("tb_greedy", 6, flags_table, ImVec2(1400, 0))) {
                                        ImGui::TableSetupColumn(data.column[0].KodeSaham.c_str());
                                        ImGui::TableSetupColumn(data.column[0].OpenPrice.c_str());
                                        ImGui::TableSetupColumn(data.column[0].Penutupan.c_str());
                                        ImGui::TableSetupColumn("Total lot");
                                        ImGui::TableSetupColumn("Total Cost");
                                        ImGui::TableSetupColumn("Est. Profit");

                                        ImGui::TableHeadersRow();

                                        for (int row = 0; row < data.sort_market.size(); ++row) {
                                            ImGui::TableNextRow();
                                            ImGui::TableSetColumnIndex(0);
                                            ImGui::Selectable(data.sort_market[row].KodeSaham.c_str(), greedy.solution[row], ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap);
                                            ImGui::TableSetColumnIndex(1);
                                            ImGui::Text("%s", data.formatCurrencyString(data.sort_market[row].OpenPrice).c_str());
                                            ImGui::TableSetColumnIndex(2);
                                            ImGui::Text("%s", data.formatCurrencyString(data.sort_market[row].Penutupan).c_str());
                                            ImGui::TableSetColumnIndex(3);
                                            ImGui::Text("%s", data.formatCurrencyInt(data.sort_market[row].total_lot).c_str());
                                            ImGui::TableSetColumnIndex(4);
                                            ImGui::Text("%s", data.formatCurrencyInt(data.sort_market[row].total_cost).c_str());
                                            ImGui::TableSetColumnIndex(5);
                                            ImGui::Text("%s", data.formatCurrencyInt(data.sort_market[row].total_shares_profit).c_str());
                                        }

                                        ImGui::EndTable();
                                    }
                                }
                                else ImGui::Text("Data not found!!");
                                ImGui::EndChild();
                                ImGui::EndGroup();

                                ImGui::SameLine();

                                // Summary Greedy
                                ImGui::BeginGroup();

                                ImGui::BeginChild("greedy", ImVec2(335, (list_market.size() * 30) * 2), true);
                                ImGui::Text("Greedy : ");
                                if (data.allOK) {
                                    if (ImGui::BeginTable("split_greedy", 5, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
                                    {
                                        for (int i = 0; i < data.sort_market.size(); ++i) {
                                            if (data.sort_market[i].OpenPrice == "0") continue;
                                            ImGui::TableNextColumn();
                                            ImGui::Selectable(data.sort_market[i].KodeSaham.c_str(), greedy.solution[i]);
                                        }
                                        ImGui::EndTable();
                                    }

                                    ImGui::Spacing();

                                    ImGui::Text("\nEstimasi total Profit : %s", data.formatCurrencyInt(greedy.profit).c_str());
                                    ImGui::Text("\nTotal Cost : %s", data.formatCurrencyInt(greedy.cost).c_str());
                                    ImGui::Text("\nSisa Uang : %s", data.formatCurrencyInt(data.nominal - greedy.cost).c_str());
                                }else ImGui::Text("Data not found!!");
                                ImGui::EndChild();
                                ImGui::EndGroup();
                            }

                            // Dynamic Solution
                            {
                                ImGui::BeginGroup();
                                static ImGuiTableFlags flags_table = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
                                ImGui::BeginChild("d_programing", ImVec2(1400, list_market.size() * 60), true);
                                ImGui::Text("Dynamic Programming:");
                                if (data.allOK) {
                                    if (ImGui::BeginTable("tb_dynamic", 6, flags_table)) {
                                        ImGui::TableSetupColumn(data.column[0].KodeSaham.c_str());
                                        ImGui::TableSetupColumn(data.column[0].OpenPrice.c_str());
                                        ImGui::TableSetupColumn(data.column[0].Penutupan.c_str());
                                        ImGui::TableSetupColumn("Total lot");
                                        ImGui::TableSetupColumn("Total Cost");
                                        ImGui::TableSetupColumn("Est. Profit");

                                        ImGui::TableHeadersRow();
                                        if (dynamic.solution_dp.size() > 0)
                                        for (int i = 0; i < data.data.size(); ++i) {
                                            ImGui::TableNextRow();
                                            ImGui::TableSetColumnIndex(0);
                                            ImGui::Selectable(data.data[i].KodeSaham.c_str(), dynamic.solution_dp[i], ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap);
                                            ImGui::TableSetColumnIndex(1);
                                            ImGui::Text("%s", data.data[i].OpenPrice.c_str());
                                            ImGui::TableSetColumnIndex(2);
                                            ImGui::Text("%s", data.data[i].Penutupan.c_str());
                                            ImGui::TableSetColumnIndex(3);
                                            ImGui::Text("%s", data.formatCurrencyInt(data.data[i].total_lot).c_str());
                                            ImGui::TableSetColumnIndex(4);
                                            ImGui::Text("%s", data.formatCurrencyInt(data.data[i].total_cost).c_str());
                                            ImGui::TableSetColumnIndex(5);
                                            ImGui::Text("%s", data.formatCurrencyInt(data.data[i].total_shares_profit).c_str());

                                        }
                                    }

                                    ImGui::EndTable();
                                }
                                else ImGui::Text("Data not found!!");

                                ImGui::EndChild();
                                ImGui::EndGroup(); ImGui::SameLine();

                                // summary dp
                                ImGui::BeginGroup();

                                ImGui::BeginChild("summary_dp", ImVec2(335, (list_market.size() * 30) * 2), true);
                                ImGui::Text("Dynamic Programing : ");
                                if (data.allOK && dynamic.solution_dp.size() > 0) {
                                    if (ImGui::BeginTable("split_dp", 5, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
                                    {
                                        for (int i = 0; i < data.data.size(); ++i) {
                                            ImGui::TableNextColumn();
                                            ImGui::Selectable(data.data[i].KodeSaham.c_str(), dynamic.solution_dp[i]);
                                        }
                                        ImGui::EndTable();
                                    }
                                    ImGui::Text("\nEstimasi total Profit : %s", data.formatCurrencyInt(dynamic.profit_dp).c_str());
                                    ImGui::Text("\nTotal Cost : %s", data.formatCurrencyInt(dynamic.cost_dp).c_str());
                                    ImGui::Text("\nSisa Uang : %s", data.formatCurrencyInt(data.nominal - dynamic.cost_dp).c_str());
                                }
                                else ImGui::Text("Data not found!!");
                                ImGui::EndChild();
                                ImGui::EndGroup();

                            }
                            ImGui::EndTabItem();
                        }
                        
                        ImGui::Text("\nID: 0x%d", menu_sidebar[selected].second);
                    }

                    ImGui::EndTabBar();
                }
                if(console)
                    data.logs.Draw("Console", &console);
                ImGui::EndChild();
                ImGui::EndGroup();
            }
        }

        ImGui::End();
        

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
