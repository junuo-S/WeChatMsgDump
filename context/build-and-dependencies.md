# 构建与依赖

## 构建系统

- CMake 最低版本：`3.16`
- 语言标准：`C++20`
- 自定义封装：`source/cmake/junuo.cmake`

## 目标产物

- `wechatmsgdump`：可执行程序（WIN32）。
- `msgui`：动态库。
- `msgcore`：动态库。
- `junuoui`：动态库。
- `junuobase`：动态库。

## 依赖

- Qt6 Core/Gui/Widgets/Network/Sql/LinguistTools
- OpenSSL
- Python3（用于 moc/rcc 辅助脚本）
- Protobuf（按模块启用）

## 构建命令（来自 README）

```bash
# Debug
cmake ../source -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=D:/GreenTools/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows

# Release（关闭优化）
cmake ../source -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=D:/GreenTools/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows -DCMAKE_C_FLAGS_RELEASE="/Od" -DCMAKE_CXX_FLAGS_RELEASE="/Od"
```

## 输出目录

- 运行时：`<build>/output/bin`
- 动态库/静态库：`<build>/output/lib`
- 翻译文件：`<build>/output/translation`
