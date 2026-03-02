#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#include <implot.h>
#include <implot_internal.h>
#include "implot.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stocksgui.h"
#include "Stock.h"
#include "implot_internal.h"
#include "websocketclient.h"
#include <algorithm>
#include <limits>

namespace MyImPlot {

template <typename T>
int BinarySearch(const T* arr, int l, int r, T x) {
    if (r >= l) {
        int mid = l + (r - l) / 2;
        if (arr[mid] == x)
            return mid;
        if (arr[mid] > x)
            return BinarySearch(arr, l, mid - 1, x);
        return BinarySearch(arr, mid + 1, r, x);
    }
    return -1;
}

void PlotCandlestick(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, bool tooltip, float width_percent, ImVec4 bullCol, ImVec4 bearCol) {

    // get ImGui window DrawList
    ImDrawList* draw_list = ImPlot::GetPlotDrawList();
    // calc real value width
    double half_width = count > 1 ? (xs[1] - xs[0]) * width_percent : width_percent;

    // custom tool
    if (ImPlot::IsPlotHovered() && tooltip) {
        ImPlotPoint mouse   = ImPlot::GetPlotMousePos();
        mouse.x             = ImPlot::RoundTime(ImPlotTime::FromDouble(mouse.x), ImPlotTimeUnit_Day).ToDouble();
        float  tool_l       = ImPlot::PlotToPixels(mouse.x - half_width * 1.5, mouse.y).x;
        float  tool_r       = ImPlot::PlotToPixels(mouse.x + half_width * 1.5, mouse.y).x;
        float  tool_t       = ImPlot::GetPlotPos().y;
        float  tool_b       = tool_t + ImPlot::GetPlotSize().y;
        ImPlot::PushPlotClipRect();
        draw_list->AddRectFilled(ImVec2(tool_l, tool_t), ImVec2(tool_r, tool_b), IM_COL32(128,128,128,64));
        ImPlot::PopPlotClipRect();
        // find mouse location index
        int idx = BinarySearch(xs, 0, count - 1, mouse.x);
        // render tool tip (won't be affected by plot clip rect)
        if (idx != -1) {
            ImGui::BeginTooltip();
            char buff[32];
            ImPlot::FormatDate(ImPlotTime::FromDouble(xs[idx]),buff,32,ImPlotDateFmt_DayMoYr,ImPlot::GetStyle().UseISO8601);
            ImGui::Text("Day:   %s",  buff);
            ImGui::Text("Open:  $%.2f", opens[idx]);
            ImGui::Text("Close: $%.2f", closes[idx]);
            ImGui::Text("Low:   $%.2f", lows[idx]);
            ImGui::Text("High:  $%.2f", highs[idx]);
            ImGui::EndTooltip();
        }
    }

    // begin plot item
     if (ImPlot::BeginItem(label_id)) {
        // override legend icon color
        ImPlot::GetCurrentItem()->Color = IM_COL32(64,64,64,255);
        // fit data if requested
        if (ImPlot::FitThisFrame()) {
            for (int i = 0; i < count; ++i) {
                ImPlot::FitPoint(ImPlotPoint(xs[i], lows[i]));
                ImPlot::FitPoint(ImPlotPoint(xs[i], highs[i]));
            }
        }
        // render data
        for (int i = 0; i < count; ++i) {
            ImVec2 open_pos  = ImPlot::PlotToPixels(xs[i] - half_width, opens[i]);
            ImVec2 close_pos = ImPlot::PlotToPixels(xs[i] + half_width, closes[i]);
            ImVec2 low_pos   = ImPlot::PlotToPixels(xs[i], lows[i]);
            ImVec2 high_pos  = ImPlot::PlotToPixels(xs[i], highs[i]);
            ImU32 color      = ImGui::GetColorU32(opens[i] > closes[i] ? bearCol : bullCol);
            draw_list->AddLine(low_pos, high_pos, color);
            draw_list->AddRectFilled(open_pos, close_pos, color);
        }

        // end plot item
        ImPlot::EndItem();
    }
}

}

void Demo_CustomPlottersAndTooltips(std::vector<StockEntry*>& entries)  {
    if(entries.empty()) {return;}
    double minT = INFINITY;
    for(auto& entry :entries) {
        if(entry->stock.dates.empty()) {continue;}
        if((*entry).stock.dates.front() < minT) {minT = (*entry).stock.dates.front();}
    }
    double maxT = -INFINITY;
    for(auto& entry :entries) {
        if(entry->stock.dates.empty()){continue;}
        if((*entry).stock.dates.back() > maxT) {maxT = (*entry).stock.dates.back();}
    }
    if(minT == INFINITY || maxT == -INFINITY) {
        return;
    }

    static bool tooltip = true;
    ImGui::Checkbox("Show Tooltip", &tooltip);
    ImGui::SameLine();
    static ImVec4 bullCol = ImVec4(0.000f, 1.000f, 0.441f, 1.000f);
    static ImVec4 bearCol = ImVec4(0.853f, 0.050f, 0.310f, 1.000f);
    ImGui::SameLine(); ImGui::ColorEdit4("##Bull", &bullCol.x, ImGuiColorEditFlags_NoInputs);
    ImGui::SameLine(); ImGui::ColorEdit4("##Bear", &bearCol.x, ImGuiColorEditFlags_NoInputs);
    ImPlot::GetStyle().UseLocalTime = false;



    if (ImPlot::BeginPlot("Candlestick Chart",ImVec2(-1,0))) {
        ImPlot::SetupAxes(nullptr,nullptr,0,ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_RangeFit);
        //ImPlot::SetupAxesLimits(1546300800, 1571961600, 1250, 1600);
        ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Time);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, minT, maxT);
        ImPlot::SetupAxisZoomConstraints(ImAxis_X1, 60*60*24*14, maxT-minT);
        ImPlot::SetupAxisFormat(ImAxis_Y1, "$%.0f");
        for(auto& entry : entries) {
            if(entry->stock.dates.empty()) {continue;}
            auto& stock = entry->stock;
            MyImPlot::PlotCandlestick(stock.symbol.c_str(),stock.dates.data(), stock.open.data(), stock.close.data(), stock.low.data(), stock.high.data(), stock.dates.size(), tooltip, 0.25f, bullCol, bearCol);

        }
        ImPlot::EndPlot();
    }
}

