TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    and.cpp \
    components.cpp \
    dflipflop.cpp \
    nand.cpp \
    node.cpp \
    nor.cpp \
    not.cpp \
    nxor.cpp \
    or.cpp \
    srlatch.cpp \
    xor.cpp \
    circuit.cpp

HEADERS += \
    and.h \
    components.h \
    dflipflop.h \
    nand.h \
    node.h \
    nor.h \
    not.h \
    nxor.h \
    or.h \
    srlatch.h \
    xor.h \
    circuit.h
