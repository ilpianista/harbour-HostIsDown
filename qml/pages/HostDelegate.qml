/*
  The MIT License (MIT)

  Copyright (c) 2015 Andrea Scarpino <me@andreascarpino.it>

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

BackgroundItem {
    id: item
    width: parent.width
    height: contentItem.childrenRect.height

    Column {
        Label {
            font.pixelSize: Theme.fontSizeMedium
            text: host
            wrapMode: Text.Wrap
            color: {
                switch (status) {
                case 0: return Theme.secondaryColor;
                case 2: return Theme.secondaryHighlightColor;
                case 1:
                default: return Theme.highlightColor;
                }
            }
        }

        Label {
            width: item.width
            font.pixelSize: Theme.fontSizeSmall
            text: formatDate(timestamp)
            horizontalAlignment: Text.AlignRight
        }
    }

    onClicked: {
        console.log("Selected host " + host);
        target.text = host;
        useIpv6.checked = ipv6;
    }

    /**
      * Formats a datetime to a string with a custom pattern which updates
      * hours and minutes to user timezone.
      */
    function formatDate(timestamp) {
        var date = new Date(timestamp);
        var toLocalTime = new Date(date.getTime() + (date.getTimezoneOffset() * 60000));
        toLocalTime.setHours(date.getHours() - (date.getTimezoneOffset() / 60));

        return Qt.formatDateTime(toLocalTime, "hh:mm, dd/MM/yyyy");
    }
}
