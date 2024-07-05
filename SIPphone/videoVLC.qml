import QtQuick 2.4
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QmlVlc 0.1
import QtMultimedia 5.0

Rectangle {
    width: 480
    height: 288
    VlcPlayer {
        id: vlcPlayer;
        //mrl: "http://download.blender.org/peach/bigbuckbunny_movies/big_buck_bunny_480p_stereo.avi";
        //mrl: "file:///home/james/Programs/EDoorWatch/test.m4v";
        //mrl: "file:///home/james/Programs/build-SIPphone-Desktop_Qt_5_5_1_GCC_64bit-Debug/pipe.mp4v";
        //mrl: "file:///storage/emulated/legacy/Movies/test3.m4v";
        //mrl: "file:///storage/emulated/legacy/Movies/pipe.m4v";
        //mrl: "file:///home/james/Programs/SIPphone/vlc.sdp"
        //mrl: "rtp://@:4100"
        mrl: "file:///data/data/org.qtproject.example.SIPphone/files/vlc.sdp"

    }

    VideoOutput {
        source: vlcPlayer;
        anchors.fill: parent;
        width: parent.width / 2;
        height: parent.height / 2;
        //        opacity: 0.9;
    }
}

