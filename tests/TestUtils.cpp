#include <tchar.h>
#include <minwindef.h>
#include <processenv.h>
#include "TestUtils.h"

std::string TestUtils::getMapFilePath(const std::string &name) {
    static TCHAR buffer[MAX_PATH];
    static bool assigned = false;
    if (!assigned) {
        SetCurrentDirectory(_T("../.."));
        GetCurrentDirectory(MAX_PATH, buffer);
        assigned = true;
    }
    std::string path = std::string(buffer) + "\\json-files\\" + name;
    return path;
}
