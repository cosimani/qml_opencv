import QtQuick 2.6
import QtMultimedia 5.8
import QtQuick.Controls 2.0
import QtWebView 1.0

import com.vayra.camera 1.0
import com.vayra.mp4 1.0
import com.vayra.rendering 1.0

Item  {
    id: item

    property bool reproduciendo: false

    onReproduciendoChanged: {
        if ( reproduciendo == false )  {
            mediaPlayer.pause();
        }
        else  {
            mediaPlayer.play();
        }
    }

    rotation: 90

    Rectangle {
        id: rectangle
//        objectName: rectangle

        anchors.fill: parent
    }

    MouseArea  {
        id: mouseArea
//        objectName: mouseArea

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

        viewfinder.resolution: Qt.size(640, 480)

//        position: Camera.FrontFace
        position: Camera.BackFace
    }

    VideoOutput  {
        id: videoOutput

        anchors.fill: parent

        fillMode: Image.PreserveAspectFit
//        fillMode: Image.Stretch  // Estira la imagen de la camara para ocupar todo videoOutput

        source: camera

        focus : visible
        filters: [videoFilter]
//        orientation: 270
    }

    VideoOutput {
        id: mp4VideoOutput
        source: mediaPlayer

        visible: false

        MediaPlayer {
            id: mediaPlayer
//            autoPlay: true
            volume: 1.0
            loops: Audio.Infinite

//            source: "file:///storage/emulated/0/DCIM/Camera/20170913_100145.mp4"
//            source: "file:///storage/emulated/0/Download/rentasonline.mp4"
            source: "assets:/videos/rentasonline.mp4"
//            source: "file:///storage/emulated/0/Download/rentasautomotor.mp4"
        }

        filters: [mp4Filter]
    }

    Renderer {
        id: renderer

        // Para que el render se haga justo donde esta la imagen de la camara
        x: videoOutput.contentRect.x
        y: videoOutput.contentRect.y
        width: videoOutput.contentRect.width
        height: videoOutput.contentRect.height


    }

    VideoFilter  {
        id: videoFilter
    }

    Mp4Filter  {
        id: mp4Filter
    }

//    WebView {
//        id: webView
//        anchors.fill: parent
//        url : "https://www.youtube.com/embed/iUdNKZUvpzk"
//    }


}
