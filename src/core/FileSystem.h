#pragma once

#include "Types.h"
#include <string>
#include <vector>

class FileSystem {
public:
    static void init();
    
    // 获取根目录（Windows是此电脑，Android是存储根）
    static std::vector<FileInfo> getRootDrives();
    
    // 获取目录下的文件列表
    static std::vector<FileInfo> listDirectory(const std::string& path);
    
    // 文件操作
    static bool createDirectory(const std::string& path);
    static bool deleteFile(const std::string& path);
    static bool renameFile(const std::string& oldPath, const std::string& newPath);
    static bool copyFile(const std::string& srcPath, const std::string& dstPath);
    static bool moveFile(const std::string& srcPath, const std::string& dstPath);
    
    // 文件信息
    static bool exists(const std::string& path);
    static bool isDirectory(const std::string& path);
    static u64 getFileSize(const std::string& path);
    static std::string getFileName(const std::string& path);
    static std::string getParentPath(const std::string& path);
    
    // 获取桌面路径
    static std::string getDesktopPath();
    
    // 剪贴板操作（文件复制剪切）
    static void setClipboardFiles(const std::vector<std::string>& files, bool cut);
    static std::vector<std::string> getClipboardFiles();
    static bool isClipboardCut();
    static void clearClipboard();
    
private:
    static std::vector<std::string> s_clipboardFiles;
    static bool s_clipboardCut;
};
