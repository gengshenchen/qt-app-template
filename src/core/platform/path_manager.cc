#include "core/platform/path_manager.h"

#include <cstdlib>  // for getenv
#include <iostream>
#include <stdexcept>

#include "shared/Constants.h"
// --- 平台相關的頭文件 ---
#ifdef _WIN32
#include <Windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <unistd.h>  // for realpath
#elif defined(__linux__)
#include <limits.h>
#include <unistd.h>
#endif

namespace qt_app_template::core {

namespace fs = std::filesystem;

PathManager& PathManager::instance() {
    static PathManager instance;
    return instance;
}

PathManager::PathManager() {
    // 構造時立即初始化所有路徑，確保後續調用高效
    initialize_paths();
}

// --- Public Getters ---
const fs::path& PathManager::executable_path() const {
    return executable_path_;
}
const fs::path& PathManager::executable_dir() const {
    return executable_dir_;
}
const fs::path& PathManager::config_dir() const {
    return config_dir_;
}
const fs::path& PathManager::data_dir() const {
        return data_dir_;
}
const fs::path& PathManager::cache_dir() const {
    return cache_dir_;
}
const fs::path& PathManager::resources_dir() const {
    return resources_dir_;
}
const fs::path& PathManager::log_dir() const {
    return log_dir_;
}
const fs::path& PathManager::machine_config_dir() const {
    return machine_config_dir_;
}

// --- Private Implementation ---

std::filesystem::path PathManager::find_executable_path() {
    // 平台特定的路徑查找，並進行了錯誤處理
#ifdef _WIN32
    wchar_t path_buffer[MAX_PATH] = {0};
    if (GetModuleFileNameW(NULL, path_buffer, MAX_PATH) == 0) {
        throw std::runtime_error("Failed to get executable path on Windows.");
    }
    return fs::path(path_buffer);
#elif defined(__linux__)
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    if (count > 0) {
        return fs::path(std::string(result, count));
    }
#elif defined(__APPLE__)
    char path_buffer[1024];
    uint32_t size = sizeof(path_buffer);
    if (_NSGetExecutablePath(path_buffer, &size) == 0) {
        // _NSGetExecutablePath might return a path that needs resolving (e.g., with "..")
        char real_path[PATH_MAX];
        if (realpath(path_buffer, real_path) != nullptr) {
            return fs::path(real_path);
        }
        return fs::path(path_buffer);  // Fallback to unresolved path
    }
#endif
    throw std::runtime_error("Failed to determine executable path on this platform.");
}

void PathManager::initialize_paths() {
    try {
        executable_path_ = find_executable_path();
        executable_dir_ = executable_path_.parent_path();

        const std::string app_name = Constants::APP_NAME.toStdString();
        const std::string company_name = Constants::ORG_NAME.toStdString();

        // 跨平台用戶目錄邏輯
#ifdef _WIN32
        const char* appdata = getenv("APPDATA");
        const char* local_appdata = getenv("LOCALAPPDATA");
        const char* programdata = getenv("PROGRAMDATA");

        if (!appdata || !local_appdata || !programdata) {
            throw std::runtime_error("Could not find standard environment variables.");
        }
        config_dir_ = fs::path(appdata) / company_name / app_name / "config";
        data_dir_ = fs::path(local_appdata) / company_name / app_name / "data";
        log_dir_ = fs::path(local_appdata) / company_name / app_name / "logs";
        cache_dir_ = fs::path(local_appdata) / company_name / app_name / "cache";
        machine_config_dir_ = fs::path(programdata) / company_name / app_name;
#elif defined(__APPLE__)
        const char* home = getenv("HOME");
        if (!home) { throw std::runtime_error("HOME environment variable not found."); }
        data_dir_ = fs::path(home) / "Library" / "Application Support" / app_name;
        config_dir_ = data_dir_ / "config"; // 簡化處理，配置也放在App Support中
        log_dir_ = data_dir_ / "logs"; // 簡化處理，配置也放在App Support中
        cache_dir_ = fs::path(home) / "Library" / "Caches" / app_name;
        machine_config_dir_ = "/Library/Application Support" / app_name;
#elif defined(__linux__)
        const char* home = getenv("HOME");
        if (!home) {
            throw std::runtime_error("HOME environment variable not found.");
        }
        const char* xdg_config_home = getenv("XDG_CONFIG_HOME");
        config_dir_ = xdg_config_home ? fs::path(xdg_config_home) : fs::path(home) / ".config";
        config_dir_ /= app_name;

        const char* xdg_data_home = getenv("XDG_DATA_HOME");
        data_dir_ = xdg_data_home ? fs::path(xdg_data_home) : fs::path(home) / ".local" / "share";
        data_dir_ /= app_name;

        log_dir_ = data_dir_ / "logs";
        const char* xdg_cache_home = getenv("XDG_CACHE_HOME");
        cache_dir_ = xdg_cache_home ? fs::path(xdg_cache_home) : fs::path(home) / ".cache";
        cache_dir_ /= app_name;
        machine_config_dir_ = fs::path("/etc") / app_name;

#endif

        // 區分開發與發布環境的資源路徑
#ifdef PROJECT_SOURCE_DIR
        // 開發模式：CMake傳入了源碼根目錄宏，直接指向源碼中的resources
        resources_dir_ = fs::path(PROJECT_SOURCE_DIR) / "resources";
#else
        // 發布模式：使用相對於可執行文件的約定路徑
#ifdef __APPLE__
        resources_dir_ = executable_dir_ / ".." / "Resources";
#else
        resources_dir_ = executable_dir_ / ".." / "share" / app_name;
#endif
#endif
        // 確保所有用戶可寫目錄都存在
        fs::create_directories(config_dir_);
        fs::create_directories(data_dir_);
        fs::create_directories(cache_dir_);
        fs::create_directories(log_dir_);

    } catch (const std::exception& e) {
        // 如果在初始化時發生嚴重錯誤，我們需要一種方式來處理它
        // 在這裡可以選擇拋出異常，或者降級到一個安全的路徑（例如可執行文件目錄）
        // 為了健壯性，我們暫時不拋出，而是記錄錯誤並提供一個備用路徑
        std::cerr << "CRITICAL: PathManager initialization failed: " << e.what() << std::endl;
        if (config_dir_.empty())
            config_dir_ = executable_dir_;
        if (data_dir_.empty())
            data_dir_ = executable_dir_;
        if (cache_dir_.empty())
            cache_dir_ = executable_dir_;
        if (resources_dir_.empty())
            resources_dir_ = executable_dir_;
        if (log_dir_.empty())
            log_dir_ = executable_dir_;
        if (machine_config_dir_.empty())
            machine_config_dir_ = executable_dir_;
    }
}

}  // namespace qt_app_template::core
