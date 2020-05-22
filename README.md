# nano library

![Windows Build Status](https://github.com/lyLoveSharon/nano/workflows/Windows/badge.svg)
![iOS Build Status](https://github.com/lyLoveSharon/nano/workflows/iOS/badge.svg)

## Build on Windows

```bash
mkdir build-windows
cd build-windows
cmake ..
```

## Build on IOS

```bash
mkdir build-ios
cd build-ios
cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../cmake_module/ios.toolchain.cmake -DPLATFORM=OS64
```
