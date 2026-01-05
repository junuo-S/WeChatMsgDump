# 构建

```
// Debug
cmake ../source -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=D:/GreenTools/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows

// Release禁用优化
cmake ../source -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=D:/GreenTools/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows -DCMAKE_C_FLAGS_RELEASE="/Od" -DCMAKE_CXX_FLAGS_RELEASE="/Od"
```
