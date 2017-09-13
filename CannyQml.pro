TEMPLATE = app

QT += qml quick multimedia widgets quickwidgets
CONFIG += c++11

SOURCES += main.cpp \
    videofilter.cpp \
    scene.cpp \
    registro.cpp \
    aruco/ar_omp.cpp \
    aruco/arucofidmarkers.cpp \
    aruco/board.cpp \
    aruco/boarddetector.cpp \
    aruco/cameraparameters.cpp \
    aruco/highlyreliablemarkers.cpp \
    aruco/marker.cpp \
    aruco/markerdetector.cpp \
    aruco/subpixelcorner.cpp

RESOURCES += qml.qrc \
    qmlcamera.qrc \
    files.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    videofilter.h \
    scene.h \
    registro.h \
    rgbframehelper.h \
    aruco/ar_omp.h \
    aruco/aruco.h \
    aruco/arucofidmarkers.h \
    aruco/board.h \
    aruco/boarddetector.h \
    aruco/cameraparameters.h \
    aruco/exports.h \
    aruco/highlyreliablemarkers.h \
    aruco/marker.h \
    aruco/markerdetector.h \
    aruco/subpixelcorner.h

! contains(ANDROID_TARGET_ARCH, armeabi-v7a)  {  # Si no es para Android entonces para Desktop

#    message( Compilacion para Desktop )

###############################################################################################
    unix:DIR_OPENCV_LIBS = /usr/local/lib  ####################################################
###############################################################################################

    unix:INCLUDEPATH += "/usr/include/GL/"                             # OpenGL
    unix:LIBS += "/usr/lib/x86_64-linux-gnu/libglut.so"                # OpenGL

    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_core.so         # OpenCV
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_highgui.so      # OpenCV
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_imgproc.so      # OpenCV
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_objdetect.so    # OpenCV
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_calib3d.so      # OpenCV
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_ml.so           # OpenCV
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_features2d.so

    DEFINES += DESKTOP
}

contains(ANDROID_TARGET_ARCH, armeabi-v7a)  {  # Para Android

#    message( Compilacion para Android )

###############################################################################################
    DIR_ANDROID_OPENCV = /home/ubp/OpenCV-3.2.0-android-sdk/sdk/native #####
###############################################################################################

    INCLUDEPATH += $$DIR_ANDROID_OPENCV/jni/include

    LIBS += $$DIR_ANDROID_OPENCV/libs/armeabi-v7a/libopencv_java3.so

    LIBS += -L"$$DIR_ANDROID_OPENCV/libs/armeabi-v7a" \
            -L"$$DIR_ANDROID_OPENCV/3rdparty/libs/armeabi-v7a"
    LIBS += \
        -lIlmImf\
        -llibjasper\
        -llibjpeg\
        -llibpng\
        -llibtiff\
        -llibwebp\
        -ltbb\
        -lopencv_core\
        -lopencv_imgproc\
        -lopencv_highgui\
        -lopencv_calib3d\
        -lopencv_features2d\
        -lopencv_ml

    DISTFILES += \
        android/AndroidManifest.xml \
        android/res/values/libs.xml \
        android/build.gradle

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

    ANDROID_EXTRA_LIBS = $$DIR_ANDROID_OPENCV/libs/armeabi-v7a/libopencv_java3.so
}

FORMS += \
    scene.ui \
    registro.ui

DISTFILES += \
    qmlcamera.qml \
    android/AndroidManifest.xml \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

