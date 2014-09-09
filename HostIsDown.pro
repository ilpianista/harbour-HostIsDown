TARGET = HostIsDown

CONFIG += sailfishapp

SOURCES += \
    src/main.cpp \
    src/pingaction.cpp

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    rpm/HostIsDown.spec \
    rpm/HostIsDown.yaml \
    translations/*.ts \
    HostIsDown.desktop \
    qml/pages/MainPage.qml \
    qml/HostIsDown.qml \
    rpm/HostIsDown.changes

CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/HostIsDown-it.ts

HEADERS += \
    src/pingaction.h

