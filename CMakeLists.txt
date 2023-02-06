# set minimum required version of CMake 
cmake_minimum_required(VERSION 3.1)

# set the project name and details
project(Katip VERSION 1.2
              DESCRIPTION "Resim Yazı İşleme Programı"
              LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set visual studio project as unicode character set
ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

# set release runtime to MT
set(CMAKE_CXX_FLAGS_RELEASE "/MT")

#set debug runtime to MTd
set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

# include OpenCV include directory
include_directories(libs/OpenCV/include)

# include Tesseract include directoy
include_directories(libs/Tesseract/include)

# include FreeType include directory
include_directories(libs/FreeType/include)

# link OpenCV input directory
link_directories(libs/OpenCV/lib/x64)

# link Tesseract input directory
link_directories(libs/Tesseract/lib/x64)

# link FreeType input directory
link_directories(libs/FreeType/lib/x64)

# add executable
add_executable(${PROJECT_NAME} WIN32 main.cpp application.cpp error.cpp graphics.cpp gui.cpp system.cpp main.hpp application.hpp error.hpp graphics.hpp gui.hpp system.hpp)

# set OpenCV library
set(OpenCV 
    debug     opencv_core454d.lib
    optimized opencv_core454.lib
    debug     opencv_dnn454d.lib 
    optimized opencv_dnn454.lib 
    debug     opencv_imgcodecs454d.lib 
    optimized opencv_imgcodecs454.lib 
    debug     opencv_imgproc454d.lib 
    optimized opencv_imgproc454.lib
    debug     opencv_highgui454d.lib 
    optimized opencv_highgui454.lib)
	
# target OpenCV library
target_link_libraries(${PROJECT_NAME} ${OpenCV})      

# set Tesseract library
set(Tesseract 
    debug     lzmad.lib
    optimized lzma.lib
    debug     leptonica-1.81.1d.lib
    optimized leptonica-1.81.1.lib
    debug     tesseract41d.lib
    optimized tesseract41.lib
    debug     jpegd.lib
    optimized jpeg.lib
    debug     webpd.lib
    optimized webp.lib
    debug     tiffd.lib
    optimized tiff.lib
    debug     libpng16d.lib
    optimized libpng16.lib
    debug     gifd.lib
    optimized gif.lib
    debug     zlibd.lib
    optimized zlib.lib
)
	
# target Tesseract library
target_link_libraries(${PROJECT_NAME} ${Tesseract}) 

# set FreeType library
set(FreeType
    debug     freetyped.lib
    optimized freetype.lib)

# target FreeType library
target_link_libraries(${PROJECT_NAME} ${FreeType})   
