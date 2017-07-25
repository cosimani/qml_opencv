import QtQuick 2.0
import QtMultimedia 5.7
import QtQuick.Controls 1.2

Rectangle  {
    id : cameraUI

    color: "#A90230"

    signal signal_imageProcessing(string msg)
    signal signal_imagePath(string msg)


    function cerrarCamara(msg)  {
        camera.stop()
        return "La camara se apago"
    }

    function abrirCamara(msg)  {
        camera.start()
        return "La camara se encendio"
    }

    Camera {
        id: camera

//        exposure.exposureMode: Camera.ExposureAuto

//        flash.mode: Camera.FlashAuto

        focus {
            focusMode: Camera.FocusContinuous
//            focusMode: Camera.FocusMacro
//            focusMode: Camera.FocusAuto
//            focusPointMode: Camera.FocusPointAuto

//            customFocusPoint: Qt.point(0.2, 0.2) // Focus relative to top-left corner
        }

        position: Camera.FrontFace
//        position: Camera.BackFace

                captureMode: Camera.CaptureStillImage
//        captureMode: Camera.CaptureViewfinder
        //        captureMode: Camera.CaptureVideo

        imageCapture {
            //            resolution: "640x480"
            onImageCaptured: {

                photoPreview.source = preview  // Show the preview in an Image
                photoPreview.visible = true;
                previewTimer.running = true;

                cameraUI.signal_imageProcessing(preview)

            }

            onImageSaved: {
                cameraUI.signal_imagePath(path)
            }

        }
    }

    Image  {
        id: photoPreview
        anchors.fill: viewfinder
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            camera.imageCapture.capture()
        }
    }

    VideoOutput {
        id: viewfinder

        fillMode: VideoOutput.Stretch
//                Stretch
//                PreserveAspectFit
//                PreserveAspectCrop

        anchors.fill: parent
//        focus : visible

        source: camera
        autoOrientation: true
    }

    Timer  {
        id: previewTimer
        interval: 3000
        onTriggered: photoPreview.visible = false;
    }


}
