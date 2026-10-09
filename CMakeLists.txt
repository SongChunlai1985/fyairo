cmake_minimum_required(VERSION 3.10.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 改ip地址
if (CC)
    add_definitions(-DCC)
endif ()

if (CMAKE_C_COMPILER MATCHES "arm-linux-gnueabihf-gcc")
    add_definitions(-DFYAIRO_1_0_0_ARM)
    set(usr_DIR /home/root/eye/usr)              #!
    PROJECT(FYAIRO_arm)
    #   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -std=c++11 -fPIC -Wno-unused -Wno-psabi")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -fPIC -Wno-unused -Wno-psabi -fpermissive")
    ADD_EXECUTABLE(${PROJECT_NAME} main.cpp)
    include_directories(
            ${usr_DIR}/include
            ${usr_DIR}/ffmpeg/include/
            ${usr_DIR}/GTEngine/include
            ${usr_DIR}/opencv/include
            ${usr_DIR}/opencv/include/opencv4
            ${usr_DIR}/XSGX/include
            ${usr_DIR}/XSGX/include/freetype2
    )
    add_subdirectory(eye/readfrm/socketclient)
    add_subdirectory(eye/readfrm/socketserver)
    add_subdirectory(eye/readfrm/Camera)
    add_subdirectory(eye/dfrc)
    add_subdirectory(eye/facedection)
    add_subdirectory(eye/readfrm)
    add_subdirectory(eye/mov)
    add_subdirectory(eye/cvface2)
    add_subdirectory(mind/local)
    add_subdirectory(mind/3d)
    add_subdirectory(mind)
    add_subdirectory(d2)
    add_subdirectory(fycrc)
    add_subdirectory(ear)
    target_link_libraries(${PROJECT_NAME}
            pthread
            socketserver
            socketclient
            dfrc
            facedection
            readfrm
            mov
            cvface2
            mind
            local
            3d
            d2
            fycrc
            asound
            ear

            ${usr_DIR}/lib/libdlib.a
            ${usr_DIR}/lib/libtiff.so.5
            ${usr_DIR}/lib/libzbar.so
            ${usr_DIR}/opencv/lib/libopencv_core.so
            ${usr_DIR}/opencv/lib/libopencv_video.so
            ${usr_DIR}/opencv/lib/libopencv_imgproc.so
            ${usr_DIR}/opencv/lib/libopencv_videoio.so
            ${usr_DIR}/opencv/lib/libopencv_imgcodecs.so
            ${usr_DIR}/opencv/lib/libopencv_highgui.so
            ${usr_DIR}/opencv/lib/libopencv_dnn.so
            #   ${usr_DIR}/opencv/lib/libopencv_ximgproc.so
            ${usr_DIR}/opencv/lib/libopencv_face.so
            ${usr_DIR}/opencv/lib/libopencv_objdetect.so
            ${usr_DIR}/opencv/lib/libopencv_calib3d.so

            ${usr_DIR}/XSGX/lib/libGL.so
            #   ${usr_DIR}/XSGX/lib/libglut.so                     #文件有问题

            #   ${usr_DIR}/lib/libjpeg.so.8
            #   ${usr_DIR}/lib/libopenblas.so.0
            #    ${usr_DIR}/lib/libgfortran.so.4
            #    ${usr_DIR}/lib/libblas.so
            #    ${usr_DIR}/lib/libgfortran.so.4
            #    ${usr_DIR}/lib/liblapack.so
            #    ${usr_DIR}/lib/libcblas.so
            #    ${usr_DIR}/lib/libopenblas.so
            #    ${usr_DIR}/opencv//lib/libopencv_face.so
            #    ${usr_DIR}/opencv//lib/libopencv_dnn_objdetect.so
            #    ${usr_DIR}/opencv//lib/libopencv_datasets.so
            #    ${usr_DIR}/opencv//lib/libopencv_bgsegm.so
            #    ${usr_DIR}/opencv//lib/libopencv_flann.so
            #    ${usr_DIR}/opencv//lib/libopencv_fuzzy.so
            #    ${usr_DIR}/opencv//lib/libopencv_gapi.so
            #    ${usr_DIR}/opencv//lib/libopencv_hfs.so
            #    ${usr_DIR}/opencv//lib/libopencv_img_hash.so
            #    ${usr_DIR}/opencv//lib/libopencv_optflow.so
            #    ${usr_DIR}/opencv//lib/libopencv_phase_unwrapping.so
            #    ${usr_DIR}/opencv//lib/libopencv_plot.so
            #    ${usr_DIR}/opencv//lib/libopencv_stitching.so
            #    ${usr_DIR}/opencv//lib/libopencv_superres.so
            #    ${usr_DIR}/opencv//lib/libopencv_videostab.so
            #    ${usr_DIR}/opencv//lib/libopencv_xobjdetect.so
            #    ${usr_DIR}/opencv//lib/
            )


elseif (CMAKE_C_COMPILER MATCHES "/gcc-7")
    set(usr_DIR /home/s/FYSERVER/usr/local)              #!
    PROJECT(FYAIRO_server)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -fPIC -Wno-unused -Wno-psabi")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -std=c++11 -fPIC -Wno-unused -Wno-psabi")
    ADD_EXECUTABLE(${PROJECT_NAME} main.cpp)
    find_package(OpenCV REQUIRED)
    find_package(dlib REQUIRED)
    add_subdirectory(eye/readfrm/socketclient)
    add_subdirectory(eye/readfrm/socketserver)
    add_subdirectory(eye/readfrm/Camera)
    add_subdirectory(eye/dfrc)
    add_subdirectory(eye/facedection)
    add_subdirectory(eye/readfrm)
    add_subdirectory(eye/mov)
    add_subdirectory(eye/cvface2)
    add_subdirectory(mind/local)
    add_subdirectory(mind/3d)
    add_subdirectory(mind)
    add_subdirectory(d2)
    add_subdirectory(fycrc)
    add_subdirectory(ear)
    target_link_libraries(${PROJECT_NAME}
            pthread
            socketserver
            socketclient
            dfrc
            facedection
            readfrm
            mov
            cvface2
            mind
            local
            3d
            d2
            fycrc
            GL
            glut
            GLU
            zbar
            dlib::dlib
            ${OpenCV_LIBS}
            asound
            ear
            )

    include_directories(
            /usr/include/freetype2/
            ${OpenCV_INCLUDE_DIRS}
    )
    link_directories(${OpenCV_LIBRARY_DIRS})
    message("CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}")
    message("OpenCV_LIBRARY_DIRS = ${OpenCV_LIBRARY_DIRS}")
    message("OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
    message("OpenCV_LIBS = ${OpenCV_LIBS}")

else ()
    PROJECT(FYAIRO_desktop)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread -std=c++11 -fPIC -Wall -Wno-unused")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -pthread -std=c++11 -fPIC -Wall -Wno-unused")
    ADD_EXECUTABLE(${PROJECT_NAME} main.cpp)
    find_package(OpenCV REQUIRED)
    find_package(dlib REQUIRED)
    add_subdirectory(eye/readfrm/socketclient)
    add_subdirectory(eye/readfrm/socketserver)
    add_subdirectory(eye/readfrm/Camera)
    add_subdirectory(eye/dfrc)
    add_subdirectory(eye/facedection)
    add_subdirectory(eye/readfrm)
    add_subdirectory(eye/mov)
    add_subdirectory(eye/cvface2)
    add_subdirectory(mind/local/)
    add_subdirectory(mind/3d)
    add_subdirectory(mind)
    add_subdirectory(d2)
    add_subdirectory(fycrc)
    add_subdirectory(ear)
    target_link_libraries(${PROJECT_NAME}
            pthread
            ${OpenCV_LIBS}
            dlib
            X11
            socketserver
            socketclient
            dfrc
            facedection
            readfrm
            mov
            cvface2
            mind
            local
            3d
            d2
            fycrc
            GL
            glut
            GLU
            zbar
            asound
            ear
            )
    include_directories(
            /usr/include/freetype2/
            ${OpenCV_INCLUDE_DIRS}
    )
    link_directories(
            ${OpenCV_LIBRARY_DIRS}
    )
    message("OpenCV_LIBRARY_DIRS=${OpenCV_LIBRARY_DIRS}")
    message("OpenCV_INCLUDE_DIRS=${OpenCV_INCLUDE_DIRS}")
    message("OpenCV_LIBS=${OpenCV_LIBS}")
    message("CMAKE_C_COMPILER=${CMAKE_C_COMPILER}")

endif ()

#add_library(resnet SHARED ./eye/resnet.cpp )
#add_library(fisheye SHARED ./eye/fisheye.cpp )
#add_library(dlibface SHARED ./eye/dlibface.cpp )
#add_library(readframe SHARED ./eye/readframe.cpp )
#add_library(
#     socketclient SHARED
#     ./eye/socketclient/socketclient.cpp )

#add_library(
#     socketserver SHARED
#    ./eye/socketserver/socketserver.cpp )

SET(PROJECT_SOURCE_DIR ./)


#set(net_srcs
#    ./eye/dlibface.cpp
#    ./eye/fisheye.cpp
#    ./eye/resnet.cpp
#    ./readframe.cpp
#    )
