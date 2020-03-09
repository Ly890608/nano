# nano library

![Windows Build Status](https://github.com/lyLoveSharon/nano/workflows/Windows/badge.svg)

## Build IOS

```bash
mkdir build-ios
cd build-ios
cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../cmake_module/ios.toolchain.cmake -DPLATFORM=OS64
```
