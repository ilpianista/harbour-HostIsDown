/*
  Copyright (C) 2014-2015 Andrea Scarpino <me@andreascarpino.it>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {

    Connections {
        target: pingAction

        onResult: {
            console.log("Ping reply is " + exitCode);
            button.enabled = true;

            switch (exitCode) {
            case 0:
                errorMsg.color = Theme.secondaryColor;
                errorMsg.text = qsTr("The host is UP!");
                break;
            case 2:
                errorMsg.color = Theme.secondaryHighlightColor;
                errorMsg.text = qsTr("The host is UNKNOWN");
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
                text: qsTr("Clear history")
                onClicked: db.clearHistory()
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

                onTextChanged: {
                    if (target.text.match('([a-zA-Z1-9]{1,4}:){5}[a-zA-Z1-9]{1,4}')) {
                        ipv6.checked = true;
                    } else if (target.text.match('([1-9]{1,3}.){3}[1-9]{1,3}')) {
                        ipv6.checked = false;
                    }
                }
            }

            TextSwitch {
                id: ipv6
                text: qsTr("IPv6")
            }

            Label {
                id: errorMsg
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                id: button
                text: qsTr("Ping it")
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    errorMsg.text = "";
                    if (target.text.length !== 0) {
                        button.enabled = false;
                        pingAction.ping(target.text, ipv6.checked);
                    }
                }
            }

            SectionHeader {
               text: qsTr("History")
            }

            Repeater {
                model: recentHosts
                delegate: HostDelegate {}
            }
        }
    }
}
