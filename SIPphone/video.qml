/*******************************************************************************
* Copyright (c) 2014, Sergey Radionov <rsatom_gmail.com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.

* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtMultimedia 5.0

Rectangle {
    id: rec
    width: parent.width/1.6
    height: parent.height/1.6
    color: "grey"
    anchors.centerIn: parent

    signal callActive()

    Rectangle {
        id: recVideo
        width: rec.width/1.4
        height: rec.height/1.6
        x: 20

        anchors.top: rec.top
        anchors.topMargin: 20
        color: Qt.lighter(rec.color,1.4)

        transform: Scale {
            id: scaleTransformVideo
            property real scale: 1
            xScale: scale
            yScale: scale
        }


        NumberAnimation {
            id: scaleAnimationVideo
            target: scaleTransformVideo
            properties: "scale"
            from: 1.0
            to: 1.3
            duration: 1000
        }


        Loader {
            id: videoLoader
            anchors.fill: recVideo
            //sourceComponent: imageComp
            sourceComponent: videoComp
        }

    }

    Component {
        id: videoComp
        Rectangle {
            color: 'grey';
            VlcPlayer {
                id: vlcPlayer;
                mrl: "stream:///home/james/Programs/EDoorWatch/vlc.sdp";
                //mrl: "http://download.blender.org/peach/bigbuckbunny_movies/big_buck_bunny_480p_stereo.avi";
            }

            VideoOutput {
                source: vlcPlayer;
                anchors.fill: parent;
                width: parent.width / 2;
                height: parent.height / 2;
                //        opacity: 0.9;
            }

        }
    }
}
