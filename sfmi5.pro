QT       += core gui sql
QT += sql
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminwindow.cpp \
    company.cpp \
    databasehandler.cpp \
    facture.cpp \
    facturemanagement.cpp \
    factureobject.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    previousfacture.cpp \
    previousfactureobject.cpp \
    signupdialog.cpp \
    user.cpp \
    usermanagement.cpp \
    userwindow.cpp

HEADERS += \
    adminwindow.h \
    company.h \
    databasehandler.h \
    facture.h \
    facturemanagement.h \
    factureobject.h \
    logindialog.h \
    mainwindow.h \
    previousfacture.h \
    previousfactureobject.h \
    signupdialog.h \
    user.h \
    usermanagement.h \
    userwindow.h

FORMS += \
    adminwindow.ui \
    facture.ui \
    facturemanagement.ui \
    factureobject.ui \
    logindialog.ui \
    mainwindow.ui \
    previousfacture.ui \
    previousfactureobject.ui \
    signupdialog.ui \
    usermanagement.ui \
    userwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
