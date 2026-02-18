//
// Created by rummy on 15/2/2026.
//

#ifndef STOCKSGUI_H
#define STOCKSGUI_H

namespace MyImPlot {

    template <typename T>
    int BinarySearch(const T* arr, int l, int r, T x);

    void PlotCandlestick(
        const char* label_id,
        const double* xs,
        const double* opens,
        const double* closes,
        const double* lows,
        const double* highs,
        int count,
        bool tooltip = true,
        float width_percent = 0.25f,
        ImVec4 bullCol = ImVec4(0.000f, 1.000f, 0.441f, 1.000f),
        ImVec4 bearCol = ImVec4(0.853f, 0.050f, 0.310f, 1.000f)
    );

} // namespace MyImPlot

void Demo_CustomPlottersAndTooltips();

#endif //STOCKSGUI_H
