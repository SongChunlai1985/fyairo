aux_source_directory(. DIR_SRCS)
add_library(local ${DIR_SRCS})

if (CMAKE_C_COMPILER MATCHES "arm-linux-gnueabihf-gcc")
    set(usr_DIR /home/root/eye/usr)
    target_link_libraries(local
        ${usr_DIR}/lib/libdlib.a
        ${usr_DIR}/opencv/lib/libopencv_core.so
    )
include_directories(
    ${usr_DIR}/include
    ${usr_DIR}/opencv/include
    ${usr_DIR}/opencv/include/opencv4
    )

else()
 find_package(OpenCV REQUIRED)
 find_package(dlib REQUIRED)
 target_link_libraries(local
     dlib::dlib
     ${OpenCV_LIBS}
     )
 include_directories(
     ${OpenCV_INCLUDE_DIRS}
     )
 link_directories(${OpenCV_LIBRARY_DIRS})
endif()
