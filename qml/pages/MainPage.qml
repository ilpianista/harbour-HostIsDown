/*
  The MIT License (MIT)

  Copyright (c) 2014-2016 Andrea Scarpino <me@andreascarpino.it>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {

    allowedOrientations: Orientation.All

    Connections {
        target: manager

        onPingResult: {
            console.log("Ping reply for " + host + " is " + exitCode);
            target.enabled = true;
            useIpv6.enabled = true;
            busy.running = false;
            busy.visible = false;
            button.enabled = true;

            switch (exitCode) {
            case 0:
                errorMsg.color = Theme.secondaryColor;
                errorMsg.text = qsTr("The host is UP!");
                break;
            case 2:
                errorMsg.color = Theme.secondaryHighlightColor;
                errorMsg.text = qsTr("The host is UNKNOWN!");
                break;
            case 1:
            default:
                errorMsg.color = Theme.highlightColor;
                errorMsg.text = qsTr("The host is DOWN!");
            }
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                text: qsTr("Settings")

                onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
            }

            MenuItem {
                id: pingAll
                text: qsTr("Ping all")
                enabled: hosts.count > 0

                onClicked: {
                    target.text = "";
                    errorMsg.text = "";

                    manager.pingAll();
                }
            }

            MenuItem {
                id: clearHistory
                text: qsTr("Clear history")
                enabled: hosts.count > 0

                onClicked: {
                    errorMsg.text = "";
                    enabled = false;
                    pingAll.enabled = false;

                    manager.clearHistory();
                }
            }
        }

        Column {
            id: column
            width: parent.width

            PageHeader {
                title: qsTr("Ping any host!")
            }

            TextField {
                id: target
                width: column.width
                focus: true
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhUrlCharactersOnly
                placeholderText: qsTr("An IP address or hostname")
                validator: RegExpValidator { regExp: /^[\w\.-]*$/ }

                onTextChanged: {
                    button.enabled = text.length > 0;
                    errorMsg.text = "";

                    if (text.match('([a-zA-Z1-9]{1,4}:){5}[a-zA-Z1-9]{1,4}')) {
                        useIpv6.checked = true;
                    } else if (text.match('([1-9]{1,3}.){3}[1-9]{1,3}')) {
                        useIpv6.checked = false;
                    }
                }

                EnterKey.iconSource: "image://theme/icon-m-mobile-network"
                EnterKey.enabled: text.length > 0
                EnterKey.onClicked: pingHost();
            }

            TextSwitch {
                id: useIpv6
                text: qsTr("Use IPv6")
            }

            Label {
                id: errorMsg
                anchors.horizontalCenter: parent.horizontalCenter
            }

            BusyIndicator {
                id: busy
                visible: false
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                id: button
                text: qsTr("Ping it")
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: false

                onClicked: pingHost();
            }

            SectionHeader {
               text: qsTr("History")
            }

            Repeater {
                id: hosts
                model: recentHosts
                delegate: HostDelegate {}
            }
        }
    }

    function pingHost() {
        errorMsg.text = "";
        target.enabled = false;
        useIpv6.enabled = false;
        busy.visible = true;
        busy.running = true;
        button.enabled = false;
        clearHistory.enabled = true;
        pingAll.enabled = true;

        manager.ping(target.text, useIpv6.checked);
    }
}
