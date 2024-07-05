#include <QtGui/QGuiApplication>
//#include <QAndroidJniObject>
//#include <QtAndroid>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>

#include <QDir>
#include "squircle.h"
#include "registrarui.h"
#include "sipstackif.h"
#include "key_emitter.h"
#include "sipEnventFilter.h"
#include "mpeg4decoder.h"
#include "videoGL.h"
#ifdef ANDROID
#include "lockhelper.h"
#endif
#include <QDebug>
#include <qstandardpaths.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

//int main(int argc, char* argv[])
//{
//    QGuiApplication app(argc, argv);
//    SIPRequestEater *myFilter = new SIPRequestEater;
//    app.installEventFilter(myFilter);
//    QQmlApplicationEngine engine;
//    KeyEmitter keyEmitter;
//    QQmlContext* ctx = engine.rootContext();
//    ctx->setContextProperty("keyEmitter", &keyEmitter);
//    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
//    return app.exec();
//}

//void keepScreenOn()
//{
//    QAndroidJniObject activity = QtAndroid::androidActivity();
//    if (activity.isValid()) {
//        QAndroidJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");

//        if (window.isValid()) {
//            const int FLAG_KEEP_SCREEN_ON = 128;
//            window.callObjectMethod("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
//        }
//    }
//}


int main(int argc, char *argv[])
{

    bool status;
   // QString dataAbsPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    //QFile::copy(":/vlc.sdp", dataAbsPath + "/vlc.sdp");


    QDir root("/");
#ifdef ANDROID
    KeepAwakeHelper helper;
    QDir location(QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation));
    //keepScreenOn();
#else
    QDir location(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
#endif


    QString fileCopyName = location.absolutePath() + "/vlc.sdp";
    if(QFile(fileCopyName).exists() == false){
        if(location.exists()==false){
            status=root.mkpath(location.absolutePath());
        }
        //status= QFile::copy(":/data/test.m4v", fileCopyName);
        status= QFile::copy(":/vlc.sdp", fileCopyName);
        //status= QFile::setPermissions(fileCopyName,QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther);
    }

    QString videoStream = QObject::tr("file://") + fileCopyName;

    setVideoDecoderFile(videoStream.toLatin1().data());

    qDebug() << fileCopyName ;


    qmlRegisterType<Squircle>("VideoGL", 1, 0, "Squircle");
//    RegisterQmlVlc();
//    QmlVlcConfig& config = QmlVlcConfig::instance();
//    config.enableAdjustFilter( true );
//    config.enableMarqueeFilter( true );
//    config.enableLogoFilter( true );
//    config.enableDebug( true );
//    config.setNetworkCacheTime(300);

    KeyEmitter keyEmitter;

    QGuiApplication app(argc, argv);

    SIPRequestEater *myFilter = new SIPRequestEater;
    app.installEventFilter(myFilter);
    QQuickView view;

    QObject::connect((QObject *)view.engine(),SIGNAL(quit()),&app,SLOT(quit()));
    //HWND hWnd = reinterpret_cast<HWND>(view.winId());
    view.setClearBeforeRendering( false );
    view.setMinimumSize( QSize( 640, 480 ) );
    view.show();
    view.setResizeMode( QQuickView::SizeRootObjectToView );
    view.setSource( QStringLiteral( "qrc:///skin1.qml" ) );
    //view.setSource( QStringLiteral( "qrc:///make_call.qml" ) );
    //view.setSource( QStringLiteral( "qrc:///main.qml" ) );
    //view.setSource( QStringLiteral("qrc:///incoming.qml" ) );
    //view.setSource( QStringLiteral("qrc:///active.qml" ) );
    QQmlContext *ctxt = view.rootContext();
    QObject *object = view.rootObject();
    registrarUI *registrar = new registrarUI(object,ctxt);
    sipStackIf *sip = new sipStackIf(object,registrar);


    ctxt->setContextProperty("registrar", registrar);
    ctxt->setContextProperty("sip", sip);
    ctxt->setContextProperty("keyEmitter", &keyEmitter);

#ifdef ANDROID
   // keepScreenOn();
#endif
    return app.exec();
}
