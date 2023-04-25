QT     += core gui widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

OUTPUT_FILE_NAME_UNIX = "bignumcalc"
OUTPUT_FILE_NAME_WIN = "BigNumCalc"
OUTPUT_FILE_NAME_MAC = "BigNumCalc"
GMP_LIB_NAME_UNIX = "gmp"
GMP_LIB_NAME_WIN = "gmp-10"
GMP_LIB_NAME_MAC = "gmp"
MPFR_LIB_NAME_UNIX = "mpfr"
MPFR_LIB_NAME_WIN = "mpfr-6"
MPFR_LIB_NAME_MAC = "mpfr"

unix:!mac {
    TARGET = $$OUTPUT_FILE_NAME_UNIX
    LIBS += -L$$PWD/libraries/unix/gmp -l$$GMP_LIB_NAME_UNIX
    LIBS += -L$$PWD/libraries/unix/mpfr -l$$MPFR_LIB_NAME_UNIX
}

win32 {
    TARGET = $$OUTPUT_FILE_NAME_WIN
    LIBS += -L$$PWD/libraries/win/gmp -l$$GMP_LIB_NAME_WIN
    LIBS += -L$$PWD/libraries/win/mpfr -l$$MPFR_LIB_NAME_WIN
    RC_FILE = $$PWD/resources/resource_win.rc
}

mac {
    TARGET = $$OUTPUT_FILE_NAME_MAC
    LIBS += -L$$PWD/libraries/mac/gmp -l$$GMP_LIB_NAME_MAC
    LIBS += -L$$PWD/libraries/mac/mpfr -l$$MPFR_LIB_NAME_MAC
}

INCLUDEPATH += $$PWD/libraries/include

HEADERS += \
    src/common_functions.h \
    src/mainwindow.h \
    src/widgets_reimplement.h \
    src/worker.h

SOURCES += \
    src/common_functions.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mainwindow_widgets.cpp \
    src/widgets_reimplement.cpp \
    src/worker.cpp

FORMS += \
    src/mainwindow.ui

RESOURCES += \
    resources/resource.qrc
