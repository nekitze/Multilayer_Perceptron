QT       += core gui charts printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QMAKE_CXXFLAGS += -O3

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Controller/controller.cc \
    Model/graphmultilayerperceptron.cc \
    Model/matrixmultilayerperceptron.cc \
    Model/multilayerperceptron.cc \
    Model/parser.cc \
    Model/Matrix/s21_matrix_oop.cc \
    View/paintscene.cc \
    View/qcustomplot.cpp \
    View/testwindow.cc \
    View/trainwindow.cc \
    main.cc \
    View/mainwindow.cc

HEADERS += \
    Controller/controller.h \
    Model/graphmultilayerperceptron.h \
    Model/matrixmultilayerperceptron.h \
    Model/multilayerperceptron.h \
    Model/parser.h \
    Model/Matrix/s21_matrix_oop.h \
    View/mainwindow.h \
    View/paintscene.h \
    View/qcustomplot.h \
    View/testwindow.h \
    View/trainwindow.h

FORMS += \
    View/mainwindow.ui \
    View/testwindow.ui \
    View/trainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
