import QtQuick 2.0
import qt.labs.twittertimeline 1.0

Rectangle {
    id: root;
    color: "black"
    width: 640; height: 480

    FontLoader { id: titilium; source: "qrc:/TitilliumWeb-Regular.ttf" }

    ShaderEffect {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: root.height
        property variant src: root
        property vector2d iResolution: Qt.vector2d(1.0, 1.0);
        property double iTime: 1.0
        Timer {
            id: timer
            running: true
            repeat: true
            interval: 42
            onTriggered: parent.iTime += 0.042
        }

        vertexShader: "qrc:/vertex_shader.glsl"
        fragmentShader: "qrc:/fragment_shader.glsl"
    }

    TwitterTimeLineModel { id: twitterModel }

    Rectangle {
        color: "transparent"
        anchors.fill: parent

        ListView {
            id: listView
            anchors.fill: parent
            orientation: Qt.Horizontal
            model: twitterModel
            snapMode: ListView.SnapOneItem
            delegate: Item {
                height: root.height
                width: root.width
                Image {
                    id: image
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.margins: 100
                    source: profileImage
                }
                Text {
                    anchors.left: image.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 100
                    color: "white"
                    font.family: titilium.name
                    font.pointSize: 32
                    width: root.width - 200
                    text: "@" + user + ":<br />" + tweet
                    wrapMode: Text.Wrap
                }
            }

            NumberAnimation { id: animation; target: listView; property: "contentX"; duration: 2000
                easing.type: Easing.InOutExpo
            }

            Timer {
                running: true
                interval: 10000
                repeat: true
                onTriggered: {
                    if (parent.count > 0) {
                        animation.running = false
                        var pos = listView.contentX;
                        var destPos;
                        parent.positionViewAtIndex(Math.random() * parent.count,
                                                   ListView.Beginning)
                        destPos = listView.contentX;
                        animation.from = pos;
                        animation.to = destPos;
                        animation.running = true;
                    }
                }
            }
        }
    }

    Text {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 32
        color: "white"
        font.family: titilium.name
        font.pointSize: 32
        linkColor: "#41cd52"
        text: "Tag your tweets with <a href=\"https://twitter.com/search?q=%23UbuconEU18\">" +
              "#UbuconEU18</a>"
    }

    Image {
        source: "qrc:twitter.png"
        height: sourceSize.height / 2
        width: sourceSize.width / 2
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 32
    }

    Image {
        height: sourceSize.height / 4
        width: sourceSize.width / 4
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 32
        source: "qrc:/Built_with_Qt_RGB_logo_white.png"
    }
}
