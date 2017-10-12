import QtQuick 2.6
import QtMultimedia 5.7
import com.camera.vayra 1.0
//import io.qt.examples.backend 1.0

import QtQuick.Controls 2.0

import SceneGraphRendering 1.0

Item  {

    Rectangle {
        anchors.fill: parent
        color: blue
        border.width: 50
    }

    MouseArea  {
        anchors.fill: parent

        onClicked: {
            if ( camera.position == Camera.FrontFace )  {
                camera.position = Camera.BackFace
            }
            else  {
                camera.position = Camera.FrontFace
            }
        }
    }

    Camera  {
        id: camera

        captureMode: Camera.CaptureViewfinder

//        viewfinder.resolution: "480x320"
        viewfinder.resolution: Qt.size(640, 480)

        position: Camera.FrontFace
//        position: Camera.BackFace

    }

    VideoOutput  {
        id: videoOutput

        width: 640*2
        height: 480*2
//        anchors.fill: parent

//        fillMode: PreserveAspectCrop

        source: camera
        focus : visible
        filters: [videoFilter]
//        orientation: 270


        Renderer {
            id: renderer

            anchors.fill: parent

        }



    }

    VideoFilter  {
        id: videoFilter
    }

}
