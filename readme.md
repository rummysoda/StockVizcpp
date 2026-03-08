# StockVizcpp
A desktop stock visualizer built in C++ that displays interactive candlestick charts and a merged price-weighted index

## Demo

https://github.com/rummysoda/StockVizcpp/tree/main/docs/demonstration.mp4

## Features
- OHLC candlestick charts rendered with ImPlot, with hover tooltips showing each day's open, close, low, and high
- Add and remove multiple stocks from the sidebar
- Verifies stock symbols exist using Finnhub's symbol lookup before adding, and prevents duplicate entries
- Streams real-time trades from Finnhub via a WebSocket connection using Boost.Beast over TLS/SSL
- Combines all selected stocks into a merged chart that uses price-weighted index  formula as Dow Jones for example.

    <p align="center"><img src=https://latex.codecogs.com/png.image?\large&space;\dpi{110}\bg{white}$$\text{Index&space;Value}=\frac{\sum\text{Stock&space;Prices}}{\text{Divisor}}$$></p>

- All text input is automatically converted to uppercase to keep symbol handling consistent
- Historical OHLC data is pulled from Yahoo Finance via a Python fetcher script and saved as CSVs for the C++ app to read
## How It Works
The application has two data pipelines:

1. **Historical data** is fetched by fetcher.py, which calls yfinance to download OHLC data and saves it as a CSV file. The C++ application reads the CSV, stores the data in Stock class vectors, then deletes the CSV file.

2. **Live data** streams in through a WebSocket connection to Finnhub. The `websocketclient` module uses Boost.Beast with OpenSSL for a secure TLS connection, receiving trade data taht gets aggregated into daily candlesticks on the chart.

The GUI is built with SFML (window management and input) and Dear ImGui/ImPlot (UI widgets and chart rendering). The sidebar lets you type in ticker symbols, add them with **+**, and remove them with **x**. The merged ticks chart recalculates the price-weighted index whenever stocks are added or removed.

## Project Structure

```
├── Interface/
│   ├── AB/                       # Add button assets
│   │   ├── plusClicked.png
│   │   ├── plusDisabled.png
│   │   ├── plusHover.png
│   │   └── plusMain.png
│   ├── BG/                       # Background textures
│   │   ├── bg.png
│   │   └── bg2.png
│   ├── CB/                       # Close button assets
│   │   ├── closeDisabled.png
│   │   ├── closeHovered.png
│   │   ├── closeMain.png
│   │   └── closePressed.png
│   ├── TB/                       # Text button assets
│   │   ├── buttonDefault.png
│   │   ├── buttonDisabled.png
│   │   ├── buttonHovered.png
│   │   └── buttonpressed.png
│   ├── WarningS/                 # Validation feedback
│   │   ├── NotFound.png
│   │   └── exists.png
│   ├── font/
│   │   ├── AcuminR.ttf
│   │   └── mfont.ttf
│   └── icon/
│       └── logo.jpg
├── docs/
│   └── readme.md
│   └── demonstration.mp4
└── src/
    ├── main.cpp                  # Entry point
    ├── stocksgui.cpp / .h        # ImGui/ImPlot chart rendering
    ├── apiclient.cpp / .h
    ├── websocketclient.cpp / .h  # Boost.Beast WebSocket for live trades
    ├── window.cpp / .h           # SFML window setup
    ├── fetcher.py                # yfinance historical data fetcher
    ├── rapidcsv.h                # CSV parsing
    ├── AddButton.h
    ├── CloseButton.h
    ├── TextButton.h
    ├── ImGuiColor.h              # Custom color definitions
    ├── Stock.h                   # Stock data model
    └── stockname.txt             # Stock symbol reference
```

## Dependencies

|Library|Purpose|
|---|---|
|[SFML](https://github.com/SFML/SFML)|Window and input|
|[Dear ImGui](https://github.com/ocornut/imgui) + [ImPlot](https://github.com/epezent/implot)|GUI and candlestick charts|
|[nlohmann/json](https://github.com/nlohmann/json)|JSON parsing|
|[Boost.Beast](https://github.com/boostorg/beast)|WebSocket client|
|[OpenSSL](https://github.com/openssl/openssl)|TLS/SSL for secure connections|
|[yfinance](https://github.com/ranaroussi/yfinance)|Historical stock data (Python)|
## Prerequisites
- A [Finnhub](https://finnhub.io/) API key (free)
## Building

**Platform:** Windows

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

Make sure all C++ dependencies are installed and accessible to CMake. You'll also need Python 3 with yfinance:

```bash
pip install yfinance
```