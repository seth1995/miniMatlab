import QtQuick 1.1

import QtWebKit 1.0


Rectangle {
    id: rectangle1
    width: 360
    height: 360
    Text {
        anchors.centerIn: parent
        text: "Hello World"
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }

    WebView {
        x: -62
        y: -56
        width: 499
        height: 513
        url: "http://www.nokia.com"
        preferredWidth: 490
        preferredHeight: 400
        scale: 0.5
        smooth: false
    }
}

