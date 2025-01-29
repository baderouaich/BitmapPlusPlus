# Integration example using FetchContent

[FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html) is a built-in CMake module that allows you to fetch and incorporate external dependencies directly into your project.
It eliminates the need for additional package managers and simplifies dependency management.

You can bring this library in using the following command:

```cmake
include(FetchContent)
FetchContent_Declare(BitmapPlusPlus
        GIT_REPOSITORY "https://github.com/baderouaich/BitmapPlusPlus"
        GIT_TAG "master"
)
FetchContent_MakeAvailable(BitmapPlusPlus)
```

And link it to your target using:

```cmake
target_link_libraries(${TARGET} LINK_PRIVATE bpp::BitmapPlusPlus)
```

Note that your target needs to compile with C++17 or newer. After that, you can simply include the library in your code:

```cpp
#include <BitmapPlusPlus.hpp>
```
