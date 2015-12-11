CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(slider2)

set(BOOST_INCLUDEDIR  "e:/lib/boost/boost_1_58_0")
set(BOOST_LIBRARYDIR "e:/lib/boost/boost_1_58_0/lib32-msvc-10.0")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.58 COMPONENTS thread chrono system date_time filesystem)

set(Qt5DIR "e:/lib/Qt/5.5/msvc2010")
set(Qt5Widgets_DIR ${Qt5DIR}lib/cmake/Qt5Widgets)
find_package(Qt5Widgets REQUIRED )
set(CMAKE_AUTOMOC ON)

set(OpenCV_DIR e:/lib/opencv/3.0.0/install)
find_package(OpenCV REQUIRED )

file(GLOB ImageProcessor_SRC
    "ImageProcessor/*.h"
    "ImageProcessor/*.cpp"
)

file(GLOB SRC
    "src/*.h"
    "src/*.cpp"
)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${BOOST_INCLUDEDIR}
    ImageProcessor
    src
)

ADD_EXECUTABLE(${PROJECT_NAME}
    ${ImageProcessor_SRC}
    ${SRC}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${Boost_LIBRARIES}
    Qt5::Widgets
    ${OpenCV_LIBS}
)