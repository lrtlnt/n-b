#include "FileSystem.h"
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;

std::vector<std::string> FileSystem::s_clipboardFiles;
bool FileSystem::s_clipboardCut = false;

void FileSystem::init() {
    s_clipboardFiles.clear();
    s_clipboardCut = false;
}

std::vector<FileInfo> FileSystem::getRootDrives() {
    std::vector<FileInfo> drives;
    
#ifdef _WIN32
    // Windows下获取所有盘符
    char driveLetters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (char c : driveLetters) {
        std::string drivePath = std::string(1, c) + ":\\";
        if (GetDriveTypeA(drivePath.c_str()) != DRIVE_NO_ROOT_DIR) {
            FileInfo info;
            info.name = std::string(1, c) + ": 本地磁盘";
            info.path = drivePath;
            info.type = FileType::Drive;
            info.size = 0;
            info.modifiedTime = 0;
            drives.push_back(info);
        }
    }
#else
    // Android/Linux下显示存储目录
    FileInfo internal;
    internal.name = "内部存储";
    internal.path = "/sdcard";
    internal.type = FileType::Drive;
    internal.size = 0;
    internal.modifiedTime = 0;
    drives.push_back(internal);
    
    // 检查外部存储
    if (fs::exists("/storage/extSdCard")) {
        FileInfo external;
        external.name = "SD卡";
        external.path = "/storage/extSdCard";
        external.type = FileType::Drive;
        external.size = 0;
        external.modifiedTime = 0;
        drives.push_back(external);
    }
#endif
    
    return drives;
}

std::vector<FileInfo> FileSystem::listDirectory(const std::string& path) {
    std::vector<FileInfo> files;
    
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            FileInfo info;
            info.name = entry.path().filename().string();
            info.path = entry.path().string();
            info.modifiedTime = fs::last_write_time(entry).time_since_epoch().count();
            
            if (entry.is_directory()) {
                info.type = FileType::Directory;
                info.size = 0;
            } else {
                info.type = FileType::File;
                info.size = fs::file_size(entry);
            }
            
            files.push_back(info);
        }
        
        // 排序：目录在前，文件在后，按名称排序
        std::sort(files.begin(), files.end(), [](const FileInfo& a, const FileInfo& b) {
            if (a.type != b.type) {
                return a.type == FileType::Directory;
            }
            return a.name < b.name;
        });
    } catch (...) {
        // 目录无法访问，返回空
    }
    
    return files;
}

bool FileSystem::createDirectory(const std::string& path) {
    try {
        return fs::create_directories(path);
    } catch (...) {
        return false;
    }
}

bool FileSystem::deleteFile(const std::string& path) {
    try {
        if (fs::is_directory(path)) {
            return fs::remove_all(path) > 0;
        } else {
            return fs::remove(path);
        }
    } catch (...) {
        return false;
    }
}

bool FileSystem::renameFile(const std::string& oldPath, const std::string& newPath) {
    try {
        fs::rename(oldPath, newPath);
        return true;
    } catch (...) {
        return false;
    }
}

bool FileSystem::copyFile(const std::string& srcPath, const std::string& dstPath) {
    try {
        if (fs::is_directory(srcPath)) {
            fs::copy(srcPath, dstPath, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        } else {
            fs::copy_file(srcPath, dstPath, fs::copy_options::overwrite_existing);
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool FileSystem::moveFile(const std::string& srcPath, const std::string& dstPath) {
    try {
        fs::rename(srcPath, dstPath);
        return true;
    } catch (...) {
        // 跨分区移动失败，用复制+删除
        if (copyFile(srcPath, dstPath)) {
            return deleteFile(srcPath);
        }
        return false;
    }
}

bool FileSystem::exists(const std::string& path) {
    return fs::exists(path);
}

bool FileSystem::isDirectory(const std::string& path) {
    return fs::is_directory(path);
}

u64 FileSystem::getFileSize(const std::string& path) {
    try {
        return fs::file_size(path);
    } catch (...) {
        return 0;
    }
}

std::string FileSystem::getFileName(const std::string& path) {
    return fs::path(path).filename().string();
}

std::string FileSystem::getParentPath(const std::string& path) {
    return fs::path(path).parent_path().string();
}

std::string FileSystem::getDesktopPath() {
#ifdef _WIN32
    // Windows桌面路径
    char* userProfile = getenv("USERPROFILE");
    if (userProfile) {
        return std::string(userProfile) + "\\Desktop";
    }
    return "C:\\Users\\Public\\Desktop";
#else
    // Android下用应用私有目录或者下载目录作为桌面
    return "/sdcard/Download";
#endif
}

void FileSystem::setClipboardFiles(const std::vector<std::string>& files, bool cut) {
    s_clipboardFiles = files;
    s_clipboardCut = cut;
}

std::vector<std::string> FileSystem::getClipboardFiles() {
    return s_clipboardFiles;
}

bool FileSystem::isClipboardCut() {
    return s_clipboardCut;
}

void FileSystem::clearClipboard() {
    s_clipboardFiles.clear();
    s_clipboardCut = false;
}
