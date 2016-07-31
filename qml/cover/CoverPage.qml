/*
  The MIT License (MIT)

  Copyright (c) 2014-2015 Andrea Scarpino <me@andreascarpino.it>

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

CoverBackground {

    Connections {
        target: manager

        onPingResult: {
            console.log("Ping reply for " + host + " is " + exitCode);
            hostLabel.text = host;

            switch (exitCode) {
            case 0:
                statusLabel.text = qsTr("is UP!");
                statusLabel.color = Theme.secondaryColor;
                break;
            case 2:
                statusLabel.text = qsTr("is UNKNOWN!");
                statusLabel.color = Theme.secondaryHighlightColor;
                break;
            case 1:
            default:
                statusLabel.text = qsTr("is DOWN!");
                statusLabel.color = Theme.highlightColor;
            }
        }
    }

    Column {
        anchors.centerIn: parent
        width: parent.width

        Label {
            id: hostLabel
            width: parent.width
            wrapMode: Text.Wrap
            font.pixelSize: Theme.fontSizeSmall
            horizontalAlignment: Text.AlignHCenter
        }

        Label {
            id: statusLabel
            width: parent.width
            text: qsTr("Ping It!")
            horizontalAlignment: Text.AlignHCenter
        }
    }


    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "image://theme/icon-cover-refresh"

            onTriggered: {
                manager.pingLast();
            }
        }
    }
}
