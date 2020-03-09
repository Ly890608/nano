# nano library

## Build IOS

```bash
mkdir build-ios
cd build-ios
cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../cmake_module/ios.toolchain.cmake -DPLATFORM=OS64
```