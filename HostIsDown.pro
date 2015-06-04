TARGET = harbour-hostisdown

CONFIG += sailfishapp

SOURCES += \
    src/main.cpp \
    src/pingaction.cpp \
    src/dbmanager.cpp \
    src/hostssqlmodel.cpp \
    src/hostsmanager.cpp

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    translations/*.ts \
    qml/pages/MainPage.qml \
    qml/HostIsDown.qml \
    harbour-hostisdown.desktop \
    rpm/harbour-hostisdown.changes \
    rpm/harbour-hostisdown.spec \
    rpm/harbour-hostisdown.yaml \
    qml/pages/HostDelegate.qml

CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/harbour-hostisdown-it.ts

HEADERS += \
    src/pingaction.h \
    src/dbmanager.h \
    src/hostssqlmodel.h \
    src/hostsmanager.h

QT += sql
