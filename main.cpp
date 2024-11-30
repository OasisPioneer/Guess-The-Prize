#include <iostream>
#include <random>
#include <string>
#include <sstream>
#include <climits>
#include <cstdlib>
#include <fstream>

#include "qrcodegen.hpp"

#define Length 12

std::string WebSite = "https://jml.adongyu.com/l/";
std::string GenerateRandomString();

void printQr(const qrcodegen::QrCode& qr);
static std::string toSvgString(const qrcodegen::QrCode &qr, int border);

void openFileInBrowser(const std::string& filename);

int main()
{
    const qrcodegen::QrCode::Ecc errCorLvl = qrcodegen::QrCode::Ecc::LOW;

    std::cout << "欢迎使用猜奖娱乐工具" << std::endl;
    std::cout << "请输入你需要产码数量" << std::endl;
    std::cout << "请输入纯数字:";

    int index;
    std::cin >> index;

    for (int var = 0; var < index; ++var) {
        std::cout << "随机字符Log: " << GenerateRandomString() << std::endl;
        std::cout << "随机接口Log: " << WebSite + GenerateRandomString() << std::endl;
        std::string QrcCode = WebSite + GenerateRandomString();
        const char *C_QrcCode = QrcCode.c_str();
        const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(C_QrcCode, errCorLvl);
        // printQr(qr); // 生成控制台二维码
        std::string SvgData = toSvgString(qr, 4);
        std::string FileName = "QrCode"+std::to_string(var)+".xml";
        std::ofstream outFile(FileName);

        if(!outFile.is_open()){
            std::cerr << "* 文件创建失败 *" << std::endl;
            return -1;
        }

        outFile << SvgData;
        outFile.close();
        std::cout << "* 文件创建成功 *" << std::endl;
        std::cout << "* 即将为您打开 *" << std::endl;
        openFileInBrowser(FileName);
    }

    return 0;
}
/* 生成随机字符 */
std::string GenerateRandomString(){
    std::string AllChar = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    std::string value;// 返回值变量
    std::random_device Seed;// 随机数种子
    std::mt19937 generator(Seed());
    std::uniform_int_distribution<> distribution(0, AllChar.size() - 1);

    for (size_t var = 0; var < Length; ++var) {
        value += AllChar[distribution(generator)];
    }

    return value;
}
/* 控制台输出二维码 */
void printQr(const qrcodegen::QrCode& qr) {
    const char* light = "  ";
    const char* dark = "◼◼";
    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            std::cout << (qr.getModule(x, y) ? dark : light);
        }
        std::cout << std::endl;
    }
}
/* 二维码生成 */
static std::string toSvgString(const qrcodegen::QrCode &qr, int border) {
    if (border < 0)
        throw std::domain_error("Border must be non-negative");
    if (border > INT_MAX / 2 || border * 2 > INT_MAX - qr.getSize())
        throw std::overflow_error("Border too large");

    std::ostringstream sb;
    sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
    sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
    sb << (qr.getSize() + border * 2) << " " << (qr.getSize() + border * 2) << "\" stroke=\"none\">\n";
    sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
    sb << "\t<path d=\"";
    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            if (qr.getModule(x, y)) {
                if (x != 0 || y != 0)
                    sb << " ";
                sb << "M" << (x + border) << "," << (y + border) << "h1v1h-1z";
            }
        }
    }
    sb << "\" fill=\"#000000\"/>\n";
    sb << "</svg>\n";
    return sb.str();
}

void openFileInBrowser(const std::string& filename) {
// 根据不同的操作系统执行不同的命令
#if defined(_WIN32) || defined(_WIN64)
    std::string command = "start " + filename;
#elif defined(__APPLE__)
    std::string command = "open " + filename;
#elif defined(__linux__)
    std::string command = "xdg-open " + filename;
#else
#error "Unsupported OS"
#endif

    // 执行系统命令
    system(command.c_str());
}
