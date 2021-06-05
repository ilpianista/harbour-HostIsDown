TARGET = harbour-hostisdown

CONFIG += sailfishapp

QT += sql

SOURCES += \
    src/main.cpp \
    src/pingaction.cpp \
    src/dbmanager.cpp \
    src/hostssqlmodel.cpp \
    src/hostsmanager.cpp

HEADERS += \
    src/pingaction.h \
    src/dbmanager.h \
    src/hostssqlmodel.h \
    src/hostsmanager.h

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    qml/pages/HostDelegate.qml \
    qml/pages/MainPage.qml \
    qml/pages/Settings.qml \
    qml/HostIsDown.qml \
    harbour-hostisdown.desktop \
    translations/*.ts \
    rpm/harbour-hostisdown.changes \
    rpm/harbour-hostisdown.spec \
    rpm/harbour-hostisdown.yaml

CONFIG += sailfishapp_i18n

TRANSLATIONS += \
    translations/harbour-hostisdown-et.ts \
    translations/harbour-hostisdown-fi.ts \
    translations/harbour-hostisdown-fr.ts \
    translations/harbour-hostisdown-hr.ts \
    translations/harbour-hostisdown-it.ts \
    translations/harbour-hostisdown-nb_NO.ts \
    translations/harbour-hostisdown-nl_BE.ts \
    translations/harbour-hostisdown-nl.ts \
    translations/harbour-hostisdown-sr.ts \
    translations/harbour-hostisdown-sv.ts \
    translations/harbour-hostisdown-tr.ts \
    translations/harbour-hostisdown-zh_Hant.ts
