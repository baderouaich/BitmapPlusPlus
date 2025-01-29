# Integration example using CPM

CPM stands for [CMake Package Manager](https://github.com/cpm-cmake) which is a convenience wrapper over CMake's
FetchContent module. You can download the CPM.cmake from the
project [releases page](https://github.com/cpm-cmake/CPM.cmake/releases/latest).

With CPM enabled, you can bring this library in using the following command:

```
CPMAddPackage ( "gh:baderouaich/BitmapPlusPlus#master" )
```

And link it to your target using:

```
target_link_libraries( ${TARGET} LINK_PRIVATE bpp::BitmapPlusPlus )
```

Note that your target needs to compile with C++17 or newer. After that, you can simply include the library in your code:

```cpp
#include <BitmapPlusPlus.hpp>
```
