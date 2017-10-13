import QtQuick 2.6
import QtMultimedia 5.8
import QtQuick.Controls 2.0

import com.camera.vayra 1.0
import SceneGraphRendering 1.0

Item  {
    id: item
    objectName: item

    rotation: 90


    Rectangle {
        id: rectangle
        objectName: rectangle

        anchors.fill: parent
    }

    MouseArea  {
        id: mouseArea
        objectName: mouseArea

        anchors.fill: parent

        onClicked: {
            if ( camera.position == Camera.FrontFace )  {
//                camera.position = Camera.BackFace
                camera.stop();
                videoOutput.source = mediaplayer
                mediaplayer.play();
            }
            else  {
                camera.position = Camera.FrontFace
            }
        }
    }

    Camera  {
        id: camera
        objectName: camera

        captureMode: Camera.CaptureViewfinder

        viewfinder.resolution: Qt.size(640, 480)

        position: Camera.FrontFace
    }

    VideoOutput  {
        id: videoOutput
        objectName: videoOutput

        anchors.fill: parent

        fillMode: Image.PreserveAspectFit
//        fillMode: Image.Stretch  // Estira la imagen de la camara para ocupar todo videoOutput

        source: camera

        focus : visible
        filters: [videoFilter]
//        orientation: 270


    }

    Renderer {
        id: renderer
        objectName: renderer


    }

    VideoFilter  {
        id: videoFilter
        objectName: videoFilter
    }

    MediaPlayer  {
        id: mediaplayer
        source: ":/images/Mujer-maravilla.mp4"
    }

}
