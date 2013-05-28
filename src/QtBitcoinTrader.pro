######################################################################
# Automatically generated by qmake (2.01a) Mon May 13 18:00:41 2013
######################################################################

TEMPLATE = app
TARGET = QtBitcoinTrader
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += addrulewindow.h \
           bitcointrader.h \
           bitcointraderupdater.h \
           feecalculator.h \
           julyaes256.h \
           julylightchanges.h \
           logthread.h \
           main.h \
           newpassworddialog.h \
           passworddialog.h \
           socketthread.h \
           tempwindow.h \
	   ruleholder.h
FORMS += addrulewindow.ui \
         feecalculator.ui \
         gsgtrader.ui \
         newpassworddialog.ui \
         passworddialog.ui \
         tempwindow.ui \
	 audioplayer.h \
         julyspinboxfix.h
SOURCES += addrulewindow.cpp \
           bitcointrader.cpp \
           bitcointraderupdater.cpp \
           feecalculator.cpp \
           julyaes256.cpp \
           julylightchanges.cpp \
           logthread.cpp \
           main.cpp \
           newpassworddialog.cpp \
           passworddialog.cpp \
           socketthread.cpp \
           tempwindow.cpp \
	   ruleholder.cpp \
	   audioplayer.cpp \
           julyspinboxfix.cpp
RESOURCES += QtResource.qrc
QT += network multimedia
LIBS += -lcrypto