CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(slider2)

set (BOOST_INCLUDEDIR  "e:/program/boost/boost_1_58_0")
set (BOOST_LIBRARYDIR "e:/program/boost/boost_1_58_0/lib32-msvc-10.0")
set (Boost_USE_STATIC_LIBS   ON)
set (Qt5DIR "c:/Qt/5.5/msvc2010/")
#set (Qt5DIR "c:/Qt/5.5/Src/qtbase")

set(CMAKE_OSX_ARCHITECTURES "i386")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${BOOST_INCLUDEDIR}")

set(Qt5Widgets_DIR ${Qt5DIR}lib/cmake/Qt5Widgets/)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED )
find_package(Qt5PrintSupport REQUIRED )
find_package(Boost 1.58 COMPONENTS thread chrono system date_time)

file(GLOB ImageProcessor_SRC
    "*.h"
    "*.cpp"
)

ADD_EXECUTABLE(slider2
    main.cpp
    ${ImageProcessor_SRC}
    imageviewer.cpp
    imageviewer.h
    )

TARGET_LINK_LIBRARIES(slider2
    Qt5::Widgets 
    Qt5::PrintSupport
    ${Boost_LIBRARIES}
)
