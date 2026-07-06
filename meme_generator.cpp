// meme_generator.cpp
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

void putTextWithOutline(Mat& img, const string& text, Point pos, int fontFace,
                        double fontScale, Scalar color, Scalar outlineColor,
                        int thickness, int outlineThickness) {
    // Рисуем обводку
    for (int dx = -outlineThickness; dx <= outlineThickness; ++dx) {
        for (int dy = -outlineThickness; dy <= outlineThickness; ++dy) {
            if (dx != 0 || dy != 0) {
                putText(img, text, Point(pos.x + dx, pos.y + dy), fontFace,
                        fontScale, outlineColor, thickness, LINE_AA);
            }
        }
    }
    putText(img, text, pos, fontFace, fontScale, color, thickness, LINE_AA);
}

void memeGenerator(const string& inputPath, const string& outputPath,
                   const string& topText, const string& bottomText,
                   int fontSize=48, const string& fontPath="",
                   const string& color="#FFFFFF",
                   const string& strokeColor="#000000",
                   int strokeWidth=2, bool shadow=false,
                   int padding=20) {
    Mat img = imread(inputPath, IMREAD_COLOR);
    if (img.empty()) {
        cerr << "Ошибка загрузки " << inputPath << endl;
        return;
    }
    int w = img.cols, h = img.rows;

    // Цвета
    Scalar textColor(255,255,255), outlineColor(0,0,0);
    if (color[0] == '#') {
        int r = stoi(color.substr(1,2), nullptr, 16);
        int g = stoi(color.substr(3,2), nullptr, 16);
        int b = stoi(color.substr(5,2), nullptr, 16);
        textColor = Scalar(b,g,r);
    }
    if (strokeColor[0] == '#') {
        int r = stoi(strokeColor.substr(1,2), nullptr, 16);
        int g = stoi(strokeColor.substr(3,2), nullptr, 16);
        int b = stoi(strokeColor.substr(5,2), nullptr, 16);
        outlineColor = Scalar(b,g,r);
    }

    int fontFace = FONT_HERSHEY_DUPLEX;
    double fontScale = fontSize / 30.0;
    int thickness = 2;

    // Верхний текст
    if (!topText.empty()) {
        int baseline;
        Size textSize = getTextSize(topText, fontFace, fontScale, thickness, &baseline);
        int x = (w - textSize.width) / 2;
        int y = padding + textSize.height;
        putTextWithOutline(img, topText, Point(x, y), fontFace, fontScale,
                           textColor, outlineColor, thickness, strokeWidth);
    }

    // Нижний текст
    if (!bottomText.empty()) {
        int baseline;
        Size textSize = getTextSize(bottomText, fontFace, fontScale, thickness, &baseline);
        int x = (w - textSize.width) / 2;
        int y = h - padding - baseline;
        putTextWithOutline(img, bottomText, Point(x, y), fontFace, fontScale,
                           textColor, outlineColor, thickness, strokeWidth);
    }

    imwrite(outputPath, img);
}

void batchProcess(const string& inputDir, const string& outputDir, const string& topText,
                  const string& bottomText, int fontSize, const string& fontPath,
                  const string& color, const string& strokeColor, int strokeWidth,
                  bool shadow, int padding) {
    fs::create_directories(outputDir);
    vector<string> exts = {".png", ".jpg", ".jpeg", ".bmp", ".tiff", ".webp"};
    vector<string> files;
    for (const auto& entry : fs::directory_iterator(inputDir)) {
        if (entry.is_regular_file()) {
            string ext = entry.path().extension().string();
            transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            if (find(exts.begin(), exts.end(), ext) != exts.end()) {
                files.push_back(entry.path().string());
            }
        }
    }
    for (size_t i = 0; i < files.size(); ++i) {
        string outPath = outputDir + "/" + fs::path(files[i]).filename().string();
        cout << "[" << i+1 << "/" << files.size() << "] " << fs::path(files[i]).filename().string() << endl;
        memeGenerator(files[i], outPath, topText, bottomText, fontSize, fontPath,
                      color, strokeColor, strokeWidth, shadow, padding);
    }
}

int main(int argc, char* argv[]) {
    string input, output = "meme.jpg";
    string topText, bottomText, fontPath, color = "#FFFFFF", strokeColor = "#000000";
    int fontSize = 48, strokeWidth = 2, padding = 20;
    bool shadow = false;
    string batchDir;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-t" && i+1 < argc) topText = argv[++i];
        else if (arg == "-b" && i+1 < argc) bottomText = argv[++i];
        else if (arg == "-o" && i+1 < argc) output = argv[++i];
        else if (arg == "-s" && i+1 < argc) fontSize = stoi(argv[++i]);
        else if (arg == "-c" && i+1 < argc) color = argv[++i];
        else if (arg == "-f" && i+1 < argc) fontPath = argv[++i];
        else if (arg == "--stroke" && i+1 < argc) strokeColor = argv[++i];
        else if (arg == "--stroke-width" && i+1 < argc) strokeWidth = stoi(argv[++i]);
        else if (arg == "--shadow") shadow = true;
        else if (arg == "--padding" && i+1 < argc) padding = stoi(argv[++i]);
        else if (arg == "--batch" && i+1 < argc) batchDir = argv[++i];
        else if (arg == "-h" || arg == "--help") {
            cout << "Usage: meme_generator <input> [options]\n"
                 << "  -t <text>    Top text\n"
                 << "  -b <text>    Bottom text\n"
                 << "  -o <file>    Output\n"
                 << "  -s <N>       Font size\n"
                 << "  -c <hex>     Color\n"
                 << "  -f <file>    Font path\n"
                 << "  --stroke <hex>  Stroke color\n"
                 << "  --stroke-width <N>  Stroke width\n"
                 << "  --shadow     Add shadow\n"
                 << "  --padding <N>  Padding\n"
                 << "  --batch <dir>  Batch\n";
            return 0;
        } else if (input.empty()) input = arg;
    }
    if (input.empty()) { cerr << "Укажите входной файл." << endl; return 1; }

    if (!batchDir.empty()) {
        batchProcess(input, batchDir, topText, bottomText, fontSize, fontPath,
                     color, strokeColor, strokeWidth, shadow, padding);
        cout << "✅ Batch processing completed. Results in " << batchDir << endl;
    } else {
        memeGenerator(input, output, topText, bottomText, fontSize, fontPath,
                      color, strokeColor, strokeWidth, shadow, padding);
        cout << "✅ Meme saved to " << output << endl;
    }
    return 0;
}
