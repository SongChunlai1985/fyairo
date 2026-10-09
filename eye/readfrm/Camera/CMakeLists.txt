aux_source_directory(. DIR_SRCS)
add_library(Camera ${DIR_SRCS})

if (CMAKE_C_COMPILER MATCHES "arm-linux-gnueabihf-gcc")
    set(usr_DIR /home/root/eye/usr)
#    target_link_libraries(Camera
#            ${usr_DIR}/ffmpeg/lib/libavdevice.a
#            #${usr_DIR}/ffmpeg/lib/libavfilter.a
#            #${usr_DIR}/ffmpeg/lib/libavformat.a
#            #${usr_DIR}/ffmpeg/lib/libavcodec.a
#            #${usr_DIR}/ffmpeg/lib/libavutil.a
#            #${usr_DIR}/ffmpeg/lib/libpostproc.a
#            #${usr_DIR}/ffmpeg/lib/libswresample.a
#            #${usr_DIR}/ffmpeg/lib/libswscale.a
#            ${usr_DIR}/lib/libz.so.1
#            ${usr_DIR}/lib/librt.so.1
#            )
    target_link_libraries(Camera
            ${usr_DIR}/ffmpeg/lib/libavformat.so
            ${usr_DIR}/ffmpeg/lib/libavcodec.so
            ${usr_DIR}/ffmpeg/lib/libavutil.so
            ${usr_DIR}/ffmpeg/lib/libavdevice.so
            ${usr_DIR}/ffmpeg/lib/libswresample.so
            ${usr_DIR}/ffmpeg/lib/libavfilter.so
            ${usr_DIR}/ffmpeg/lib/libswscale.so
            ${usr_DIR}/ffmpeg/lib/libpostproc.so
            ${usr_DIR}/lib/libbz2.so
            ${usr_DIR}/opencv/lib/libopencv_core.so
            )
    include_directories(
            ${usr_DIR}/include
            ${usr_DIR}/opencv/include
            ${usr_DIR}/opencv/include/opencv4
            ${usr_DIR}/ffmpeg/include/
    )
else ()
    find_package(PkgConfig REQUIRED)
    find_package(OpenCV REQUIRED)
    pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
            libavdevice
            libavfilter
            libavformat
            libavcodec
            libswscale
            libavutil
            sdl                                                            #sudo aptitude install libsdl1.2-dev
            )

    target_link_libraries(Camera
            PkgConfig::LIBAV
            ${OpenCV_LIBS}
            )
    include_directories(
            ${OpenCV_INCLUDE_DIRS}
    )
    link_directories(${OpenCV_LIBRARY_DIRS})
endif ()

#add_executable(cam_test camera_test.cpp Camera.cpp)
#project(cam_test)
