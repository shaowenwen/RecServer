TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    cJSON.cpp \
    dbaccess.cpp \
    topkg.cpp \
    configpara.cpp \
    tinyxml2.cpp \
    appmanager.cpp

LIBS +=-L../lib
LIBS += -D_REENTRANT  -fpermissive -lstdc++ -lzlog -lcrypto -lrdkafka -lzmq -lczmq -locilib
LIBS +=-L/usr/local/lib/ -levpp -levpp_boost

HEADERS += \
    cJSON.h \
    dbaccess.h \
    topkg.h \
    configpara.h \
    tinyxml2.h \
    appmanager.h

