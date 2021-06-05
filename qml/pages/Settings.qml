/*
  The MIT License (MIT)

  Copyright (c) 2016-2021 Andrea Scarpino <andrea@scarpino.dev>

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

    Column {
        width: parent.width

        PageHeader {
            title: qsTr("Settings")
        }

        TextSwitch {
            id: poller
            width: parent.width
            text: qsTr("Enable poller")
            description: qsTr("Pings every N minutes all hosts in the history")

            onCheckedChanged: pollerMinutes.enabled = checked;
        }

        TextField {
            id: pollerMinutes
            width: parent.width - Theme.horizontalPageMargin
            label: qsTr("Poller interval in minutes")
            inputMethodHints: Qt.ImhDigitsOnly
            validator: RegExpValidator { regExp: /^[0-9]+$/ }
            text: "60"
            enabled: poller.checked
        }
    }

    Component.onCompleted: {
        var m = manager.getPoller();
        if (m > 0) {
            poller.checked = true;
            pollerMinutes.text = m;
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Deactivating) {
            if (poller.checked && pollerMinutes.text > 0) {
                manager.setPoller(pollerMinutes.text);
            } else {
                manager.setPoller(0);
            }
        }
    }
}
