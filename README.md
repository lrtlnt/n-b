# Windows XP 模拟器 (C++ 跨平台版)

一个使用 C++17 + SDL2 开发的轻量级 Windows XP 模拟器，支持 Windows 和 Android 双平台运行，完全自绘 UI，无需系统原生控件。

## ✨ 功能特性

### 🎨 UI 系统
- 经典 Luna 主题蓝色渐变标题栏（活动窗口：`#0A246A` → `#3A6EA5`）
- XP 风格 3D 按钮，关闭按钮悬停变红效果
- 蓝色渐变任务栏 + 绿色开始按钮
- 系统托盘实时时钟
- XP 风格右键菜单，选中项蓝色高亮（`#316AC5`）
- 开始菜单向上弹出，底部对齐任务栏

### 🖥️ 桌面系统
- 代码生成经典 Bliss 蓝天绿草壁纸
- 可拖动桌面图标
- 桌面右键菜单、文件右键菜单
- 图标单击选中、双击打开、悬停效果
- 触屏适配：长按 500ms 模拟右键

### 🪟 窗口系统
- 可拖动窗口
- 最小化/最大化/关闭功能
- 多窗口 Z 序管理
- 任务栏窗口按钮同步
- 双击标题栏最大化/还原
- 窗口状态实时同步

### 📁 文件系统
- 真实文件系统访问
- Windows 下显示所有盘符，Android 下显示内部存储/SD卡
- 支持文件操作：复制/剪切/粘贴/删除/重命名/新建
- 跨平台路径处理

### 📱 内置应用
- 📝 记事本：支持文本编辑、打开保存文件
- 🔢 计算器：标准四则运算计算器
- 🎨 画图：简单画笔绘图，支持颜色选择
- 🖼️ 图片查看器：支持 BMP/PNG/JPG 图片查看
- 🌐 浏览器（开发中）
- ▶️ 运行对话框
- ℹ️ 关于窗口
- 📂 文件浏览器

## 🔨 编译说明

### 环境要求
- CMake 3.16+
- C++17 兼容编译器（MSVC 2019+, GCC 9+, Clang 10+）
- SDL2, SDL2_image, SDL2_ttf 开发库

---

### Windows 编译（本地）

1. **安装依赖**
   - 推荐使用 [vcpkg](https://github.com/microsoft/vcpkg) 安装依赖：
   ```bash
   vcpkg install sdl2:x64-windows sdl2-image:x64-windows sdl2-ttf:x64-windows
   vcpkg integrate install
   ```

2. **CMake 构建**
   ```bash
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=[你的vcpkg路径]/scripts/buildsystems/vcpkg.cmake
   cmake --build . --config Release
   ```

3. **运行**
   编译完成后，`build/Release` 目录下会生成 `XPSimulator.exe`，直接运行即可，SDL2 相关 DLL 会自动复制到该目录。

---

### Android 编译（本地）

1. **环境准备**
   - 安装 Android Studio 或 Android SDK + NDK r25+
   - 安装 JDK 11
   - 安装 CMake 3.22+

2. **准备依赖**
   ```bash
   cd android
   chmod +x prepare-deps.sh
   ./prepare-deps.sh
   ```

3. **编译 APK**
   ```bash
   cd android
   ./gradlew assembleRelease
   ```

4. **安装**
   编译完成后，APK 位于 `android/app/build/outputs/apk/release/app-release.apk`，安装到 Android 设备即可，支持 Android 5.0+ (API 21+)。

---

### GitHub Actions 自动编译

项目已经配置好 GitHub Actions CI，上传到 GitHub 后会自动编译：
1. 推送代码到 GitHub 仓库
2. 进入 Actions 页面，等待编译完成
3. 在 Artifacts 中下载编译好的 `XPSimulator-Windows.zip` 和 `XPSimulator-Android.zip`
4. Windows 版本解压直接运行 exe，Android 版本安装 APK 即可

## 🎮 使用说明

### 鼠标操作
- **左键单击**：选中项目、点击按钮
- **左键双击**：打开文件/文件夹、启动程序
- **右键单击**：弹出右键菜单
- **拖动**：移动窗口、拖动桌面图标
- **标题栏双击**：最大化/还原窗口

### 触屏操作（Android）
- **单击**：等同于鼠标左键单击
- **双击**：等同于鼠标左键双击
- **长按 500ms**：等同于鼠标右键
- **拖动**：移动窗口/图标

## 📁 项目结构
```
xp-simulator/
├── src/
│   ├── core/           # 核心模块：应用、窗口、图形、事件、文件系统
│   ├── ui/             # UI模块：控件、按钮、菜单、任务栏、桌面
│   ├── apps/           # 内置应用
│   └── main.cpp        # 程序入口
├── android/            # Android 编译配置
├── .github/workflows/  # GitHub Actions CI 配置
├── CMakeLists.txt      # CMake 构建配置
└── README.md           # 说明文档
```

## 📝 注意事项
- Android 版本首次运行需要授予存储权限才能访问文件
- 所有 UI 均使用 SDL2 自绘，无需任何外部图片资源
- 壁纸使用代码动态生成，无需外部资源文件
- 程序无额外运行时依赖，Windows 版 exe 可直接运行，Android 版 APK 安装即开即用

## 📄 开源协议
MIT License
