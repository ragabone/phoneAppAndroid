TEMPLATE = app

QT += core qml quick gui multimedia network
QT += xml
#include(/home/james/vlc/libQML/QmlVlc-master/QmlVlc.pri)
#INCLUDEPATH += /home/james/vlc/vlc_native/vlc/include

SOURCES += main.cpp \
    key_emitter.cpp \
    registrarui.cpp \
    sipstackif.cpp \
    audiohandle.cpp \
    tcpclientRead.cpp \
    dm355mpeg4.cpp \
    myhttp.cpp \
    entryctrl.cpp \
    xmlwriter.cpp \
    combobox.cpp \
    numplan.cpp \
    squircle.cpp \
    videothread.cpp \
    mpeg4Decoder.c

#    videoGL.cpp


SOURCES += ../pjproject/registrar/sip_android.c

ANDROID {
    QT += androidextras
    DEFINES += ANDROID

    SOURCES += lockhelper.cpp
    PJPROJECTPATH=/home/james/Programs/pjproject/pjproject_2.4.5/pjproject-2.4.5
    BUILDTYPE=arm-unknown-linux-androideabi

    DEFINES +=  PJ_AUTOCONF=1

   #INCLUDEPATH += /home/james/vlc/libQML/QmlVlcDemo-master
  #LIBS += -L/home/james/vlc/libQML/QmlVlcDemo-master/android/libs/armeabi-v7a -lvlcjni

    LIBS += -L/home/james/develop/libs/android-ndk-r10e/platforms/android-21/arch-arm/usr/lib -lOpenSLES

    INCLUDEPATH += /home/james/develop/libs/android-ndk-r10e/platforms/android-21/arch-arm/usr/include/
    #ANDROID_PACKAGE_SOURCE_DIR = /home/james/vlc/libQML/QmlVlcDemo-master/android
    ANDROID_PACKAGE_SOURCE_DIR = /home/james/Programs/SIPphone/android
    #ANDROID_PACKAGE_SOURCE_DIR = /home/james/Programs/SIPphone/android/
}
else {

    PJPROJECTPATH=/home/james/Programs/pjproject/pjproject_2.4.5/native/trunk
    BUILDTYPE=x86_64-unknown-linux-gnu


    #INCLUDEPATH += /home/james/vlc/install/native/include/
    #INCLUDEPATH += /home/james/vlc/install/native/include/vlc/plugins/
}
INCLUDEPATH += /home/james/ffmpeg_build/include
INCLUDEPATH += /home/james/Programs/pjproject
INCLUDEPATH += $$PJPROJECTPATH
INCLUDEPATH += $$PJPROJECTPATH/pjsip/include
INCLUDEPATH += $$PJPROJECTPATH/pjmedia/include
INCLUDEPATH += $$PJPROJECTPATH/pjlib-util/include
INCLUDEPATH += $$PJPROJECTPATH/pjlib/include
INCLUDEPATH += $$PJPROJECTPATH/pjnath/include

LIBS += -L$$PJPROJECTPATH/pjlib/lib
LIBS += -L$$PJPROJECTPATH/pjlib-util/lib
LIBS += -L$$PJPROJECTPATH/pjnath/lib
LIBS += -L$$PJPROJECTPATH/pjmedia/lib
LIBS += -L$$PJPROJECTPATH/pjsip/lib
LIBS += -L$$PJPROJECTPATH/third_party/lib



LIBS += -lpjmedia-videodev-$$BUILDTYPE
LIBS += -lpjsua-$$BUILDTYPE \
  -lpjsip-ua-$$BUILDTYPE \
  -lpjsip-simple-$$BUILDTYPE \
 -lpjsip-$$BUILDTYPE \
  -lpjmedia-codec-$$BUILDTYPE \
  -lpjmedia-$$BUILDTYPE \
  -lpjnath-$$BUILDTYPE \
  -lpjlib-util-$$BUILDTYPE \
  -lresample-$$BUILDTYPE \
  -lsrtp-$$BUILDTYPE \
  -lpj-$$BUILDTYPE \
  -lspeex-$$BUILDTYPE

LIBS += -lpjmedia-audiodev-$$BUILDTYPE
ANDROID {


LIBS += -lavformat -lavcodec -lswscale -lavutil -lavfilter -lswresample -lavdevice


LIBS     +=-lEGL -lGLESv2 -landroid


}
else {
    LIBS += -L/home/james/ffmpeg_build/lib -lavformat -lavcodec -lswscale -lavutil -lavfilter -lswresample -lavdevice
    LIBS += -lz -lbz2
    #LIBS += -lmilenage-$$BUILDTYPE
    LIBS += -luuid  -lv4l2 \
            -lm -lasound -lcrypto -lSDL2 -lrt

#LIBS     += -L/home/james/vlc/build-qtvlclib-Desktop_Qt_5_5_1_GCC_64bit-Debug
#LIBS     += -L/home/james/vlc/install/native/lib
#LIBS     += -L/home/james/vlc/install/native/lib/vlc


}

RESOURCES += \
    qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    key_emitter.h \
    sipEnventFilter.h \
    registrarui.h \
    sipstackif.h \
    audiohandle.h \
    tcpclientRead.h \
    myhttp.h \
    entryctrl.h \
    xmlwriter.h \
    ../pjproject/ESip.h \
    combobox.h \
    numplan.h \
    mpeg4decoder.h \
    squircle.h \
    videothread.h \
    lockhelper.h
#    videoGL.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat



