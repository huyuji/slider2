CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(slider2)

set(BOOST_INCLUDEDIR  "e:/program/boost/boost_1_58_0")
set(BOOST_LIBRARYDIR "e:/program/boost/boost_1_58_0/lib32-msvc-10.0")
set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost 1.58 COMPONENTS thread chrono system date_time filesystem)

set(Qt5DIR "c:/Qt/5.5/msvc2010/")
set(Qt5Widgets_DIR ${Qt5DIR}lib/cmake/Qt5Widgets/)
find_package(Qt5Widgets REQUIRED )
find_package(Qt5PrintSupport REQUIRED )
set(CMAKE_AUTOMOC ON)

set(OpenCV_DIR e:/lib/opencv/3.0.0/install)
#set(OpenCV_STATIC ON)
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
    src
)

ADD_EXECUTABLE(${PROJECT_NAME}
    ${ImageProcessor_SRC}
    ${SRC}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${Boost_LIBRARIES}
    Qt5::Widgets 
    Qt5::PrintSupport
    ${OpenCV_LIBS}
)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${BOOST_INCLUDEDIR}")