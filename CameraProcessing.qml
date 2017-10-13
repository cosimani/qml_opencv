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
                camera.position = Camera.BackFace
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

        // Para que el render se haga justo donde esta la imagen de la camara
        x: videoOutput.contentRect.x
        y: videoOutput.contentRect.y
        width: videoOutput.contentRect.width
        height: videoOutput.contentRect.height

    }

    VideoFilter  {
        id: videoFilter
        objectName: videoFilter
    }
}
