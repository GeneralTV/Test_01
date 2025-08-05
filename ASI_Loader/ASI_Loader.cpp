#include <Windows.h>
#include <fstream>
#include <filesystem>
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

namespace fs = std::filesystem;

// 1. Разбиваем JS-код на части (каждая < 64KB)
const char* custom_js_parts[] = {
    #include "custom_js_part1.h" // Часть 1
    #include "custom_js_part2.h" // Часть 2
    // ...
};

const char* index_html = R"(
<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, height=device-height, initial-scale=1.0, maximum-scale=1.0, user-scalable=no, viewport-fit=cover">
    <meta name="transparent" content="true">
    <title>Hassle interfaces</title>
    
    <!-- Основной CSS -->
    <link rel="stylesheet" href="./assets/index.css">
    
    <!-- Ваш кастомный JS со стилями -->
    <script src="./assets/custom.js"></script>
    
    <!-- Динамическое применение стилей из вашего JS -->
    <script>
        document.addEventListener('DOMContentLoaded', function() {
            if (window.yorenovNewHudAssets && window.yorenovNewHudAssets.style) {
                const styleTag = document.createElement('style');
                styleTag.textContent = window.yorenovNewHudAssets.style;
                document.head.appendChild(styleTag);
            }
        });
    </script>
</head>
<body>
    <noscript>
        <strong>Для работы интерфейса требуется JavaScript.</strong>
    </noscript>
    <div id="app"></div>
    
    <!-- Основной скрипт (если нужен) -->
    <script type="module" src="./assets/index.js"></script>
</body>
</html>
)";

// 2. Собираем файл из частей
std::string GetCustomJS() {
    std::string result;
    for (const char* part : custom_js_parts) {
        result += part;
    }
    return result;
}

void WriteHiddenFile(const std::string& path, const std::string& content) {
    std::ofstream f(path, std::ios::binary);
    f.write(content.data(), content.size());
    SetFileAttributesA(path.c_str(), FILE_ATTRIBUTE_HIDDEN);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        PathRemoveFileSpecA(path);

        std::string uiPath = std::string(path) + "\\uiresources";
        std::string assetsPath = uiPath + "\\assets";
        
        fs::create_directories(assetsPath);
        
        WriteHiddenFile(uiPath + "\\index.html", index_html);
        WriteHiddenFile(assetsPath + "\\custom.js", GetCustomJS());
        
        SetFileAttributesA(uiPath.c_str(), FILE_ATTRIBUTE_HIDDEN);
        SetFileAttributesA(assetsPath.c_str(), FILE_ATTRIBUTE_HIDDEN);
    }
    return TRUE;
}
