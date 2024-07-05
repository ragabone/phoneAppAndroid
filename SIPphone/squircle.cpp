/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "squircle.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QOpenGLTexture>
#include "mpeg4decoder.h"
#include "videothread.h"


//! [7]
Squircle::Squircle()
    : m_t(0)
    , m_renderer(0)
{
    m_pVideoSrc = new videoThread();
    m_pVideoSrc->start();
    QMetaObject::invokeMethod(m_pVideoSrc, "threadStart");

    m_pTimer = new QTimer();
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(setTimeOut()));


    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));


}
//! [7]

//! [8]
void Squircle::setT(qreal t)
{
//    if (t == m_t)
//        return;
//    m_t = t;
//    emit tChanged();
//    if (window())
//        window()->update();
}

void Squircle::setPlaying(bool val){
    if (m_renderer) {
        m_renderer->doRendering(val);
    }
    mIsPlaying=val;
    if(mIsPlaying)
        m_pTimer->start(40);
    else
        m_pTimer->stop();
}

void Squircle::setTimeOut(){
    if (window() && mIsPlaying)
        window()->update();

}

//! [8]

//! [1]
void Squircle::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
        //! [1]
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        //! [3]
        win->setClearBeforeRendering(false);
    }
}
//! [3]

//! [6]
void Squircle::cleanup()
{
    if (m_renderer) {
        delete m_renderer;
        m_renderer = 0;
    }
}

SquircleRenderer::SquircleRenderer(videoThread *videoSrc) : m_t(0), m_program(0),m_pVideoSrc(videoSrc),mDoRendering(false),m_img(0) {  }

SquircleRenderer::~SquircleRenderer()
{
    delete m_program;
}
//! [6]

//! [9]
//!
//!
void SquircleRenderer::doRendering(bool val)
{
    mDoRendering = val;

}

void Squircle::sync()
{
    if (!m_renderer) {
        m_renderer = new SquircleRenderer(m_pVideoSrc);
        connect(window(), SIGNAL(beforeRendering()), m_renderer, SLOT(paint()), Qt::DirectConnection);
    }
    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setT(m_t);
}
//! [9]

//! [4]
//!
//!
//!
void SquircleRenderer::paint()
{
    if(!m_program){


        initializeOpenGLFunctions();

        //makeObject();

        //    glEnable(GL_DEPTH_TEST);
        //    glEnable(GL_CULL_FACE);

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

        QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
        const char *vsrc =
                "attribute highp vec4 vertex;\n"
                "attribute mediump vec4 texCoord;\n"
                "varying mediump vec4 texc;\n"
                "uniform mediump mat4 matrix;\n"
                "void main(void)\n"
                "{\n"
                "    gl_Position = matrix * vertex;\n"
                "    texc = texCoord;\n"
                "}\n";
        vshader->compileSourceCode(vsrc);

        QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
        const char *fsrc =
                "uniform sampler2D texture;\n"
                "varying mediump vec4 texc;\n"
                "void main(void)\n"
                "{\n"
                "    gl_FragColor = texture2D(texture, texc.st);\n"
                "}\n";
        fshader->compileSourceCode(fsrc);

        m_program = new QOpenGLShaderProgram;
        m_program->addShader(vshader);
        m_program->addShader(fshader);
        m_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
        m_program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
        m_program->link();

        m_program->bind();
        m_program->setUniformValue("texture", 0);
        glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());

        paintGL();
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        m_program->release();
        mCount=0;
    }
    else{


        if(!m_program->isLinked())
            m_program->link();

        m_program->bind();
        m_program->setUniformValue("texture", 0);
        glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
//        if(mCount>40){
//            paintGL3();
//            mCount++;
//            if(mCount>80)
//                mCount=0;
//        }
//        else{
//            paintGL2();
//            mCount++;
//        }

        paintGL2();


        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        m_program->release();
        //paintGL2();
        //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
      //  QImage m_img = QImage("qrc:///images/screensaver_registered.png").convertToFormat(QImage::Format_RGBA8888);
       // textures[0]->bind();
//            //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//            glDrawPixels(m_img.width(), m_img.height(), QOpenGLTexture::RGBA, GL_UNSIGNED_BYTE, m_img.bits());
       // textures[0]->setData(QOpenGLTexture::RGBA, QOpenGLTexture::Int8,m_img.bits());


       // QImage m_img = QImage(QString(":/images/screensaver_registered.png"));
        //textures[0]->bind();
    //            //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    //            glDrawPixels(m_img.width(), m_img.height(), QOpenGLTexture::RGBA, GL_UNSIGNED_BYTE, m_img.bits());
        //textures[0]->setData(m_img);
        //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

       // textures[0]->setData(m_img);
       // glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
     //delete textures[0];
//        makeObject();

        //textures[0] = new QOpenGLTexture(QImage(QString(":/images/screensaver_registered.png")).mirrored());

        // textures[0]->setMinificationFilter(QOpenGLTexture::NearestMipMapNearest);
        // textures[0]->setMagnificationFilter(QOpenGLTexture::Nearest);
        // textures[0]->setSize(480,272);
        //textures[0]->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8);

//        QImage m_img = QImage(":/images/screensaver_registered.png").convertToFormat(QImage::Format_RGBA8888);

//        QImage* img = new QImage(480, 272, QImage::Format_RGBA8888);
//        for (int y = 0; y < img->height(); y++)
//        {
//            memcpy(img->scanLine(y), m_img.scanLine(y), img->bytesPerLine());
//        }


//        textures[0]->setData(*img);
//        textures[0]->bind();
//        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    }

}

void SquircleRenderer::paintGL2()
{
    char *pixelBuffer,*pixB;




    pixelBuffer = m_pVideoSrc->getLatestVideobuffer();


    if(pixelBuffer<=0){
        pixelBuffer = mPixB;
       //return;
      // mDoRendering = false;
    }
    else{
        memcpy(mPixB,pixelBuffer,480*272*3);
    }

//    if(mDoRendering == false){
//        m_pVideoSrc->releaseLatestBuffer(pixelBuffer);

//    }


    pixB=pixelBuffer;

    makeObject();
    qDebug("Frame %d",mCount++);
    QMatrix4x4 m;
    m.ortho(-0.5f, +0.5f, +0.5f, -0.5f, 4.0f, 15.0f);
    m.translate(0.0f, 0.0f, -10.0f);

    m_program->setUniformValue("matrix", m);
    m_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    m_program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    m_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    m_program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));




    if(m_img == 0)
        m_img = new QImage(480, 272, QImage::Format_RGB888);



    for (int y = 0; y < m_img->height(); y++)
    {
        memcpy(m_img->scanLine(y), pixelBuffer, m_img->bytesPerLine());
        pixelBuffer+=m_img->bytesPerLine();

    }
    m_pVideoSrc->releaseLatestBuffer(pixB);


    textures[0]->setFormat(QOpenGLTexture::RGB8_UNorm);
    textures[0]->setMinificationFilter(QOpenGLTexture::Nearest);
    textures[0]->setMagnificationFilter(QOpenGLTexture::Nearest);
    //textures[0]->setData(m_img, QOpenGLTexture::GenerateMipMaps);

        // QImage m_img = QImage(":/images/screensaver_registered.png").mirrored().convertToFormat(QImage::Format_RGBA8888);

        QImage m_imgMirror = m_img->mirrored();
        textures[0]->setData(m_imgMirror, QOpenGLTexture::GenerateMipMaps);

         textures[0]->bind();


}




void SquircleRenderer::paintGL()
{

    makeObject();
    QMatrix4x4 m;
    m.ortho(-0.5f, +0.5f, +0.5f, -0.5f, 4.0f, 15.0f);
    m.translate(0.0f, 0.0f, -10.0f);

    m_program->setUniformValue("matrix", m);
    m_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    m_program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    m_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    m_program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));


//    textures[0] = new QOpenGLTexture(QImage(QString(":/images/screensaver_registered.png")).mirrored());

//     textures[0]->setMinificationFilter(QOpenGLTexture::NearestMipMapNearest);
//     textures[0]->setMagnificationFilter(QOpenGLTexture::Nearest);
////     textures[0]->setSize(480,272);
////    textures[0]->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8);
//    textures[0]->allocateStorage();
//    bool isStorage = textures[0]->isStorageAllocated();

        QImage m_img = QImage(":/images/screensaver_registered.png").convertToFormat(QImage::Format_RGB888);
        textures[0] = new QOpenGLTexture(QOpenGLTexture::Target2D);
        textures[0]->create();
        textures[0]->setSize(m_img.width(),m_img.height(),m_img.depth());
        textures[0]->setFormat(QOpenGLTexture::RGB8_UNorm);
        textures[0]->setMinificationFilter(QOpenGLTexture::Nearest);
        textures[0]->setMagnificationFilter(QOpenGLTexture::Nearest);
        textures[0]->setData(m_img, QOpenGLTexture::GenerateMipMaps);

    textures[0]->bind();

//   QImage m_img = QImage(":/images/screensaver_registered.png").convertToFormat(QImage::Format_RGBA8888);

//   QImage* img = new QImage(480, 272, QImage::Format_RGBA8888);
//   for (int y = 0; y < img->height(); y++)
//   {
//       memcpy(img->scanLine(y), m_img.scanLine(y), img->bytesPerLine());
//   }
//   textures[0]->setData(*img);
   //glDrawArrays(GL_TRIANGLES, 0, 4);


//   m_program->disableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
//   m_program->disableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);

//   m_program->release();


}

//QOpenGLTexture *oglt = new QOpenGLTexture(QOpenGLTexture::Target2D);
//oglt->setMinificationFilter(QOpenGLTexture::NearestMipMapNearest);
//oglt->setMagnificationFilter(QOpenGLTexture::Nearest);
////oglt->setFormat(QOpenGLTexture::RGB32F); // works
//oglt->setFormat(QOpenGLTexture::RGB16U); // now works with integer images (unsigned)
//oglt->setSize(naxis1, naxis2);

////oglt->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::Float32); // works
////oglt->setData(QOpenGLTexture::RGB, QOpenGLTexture::Float32, tempImageRGB.data); // works
//oglt->allocateStorage(QOpenGLTexture::RGB_Integer, QOpenGLTexture::UInt16); // now works with integer images (unsigned)
//oglt->setData(QOpenGLTexture::RGB_Integer, QOpenGLTexture::UInt16, tempImageRGB.data); // now works with integer images (unsigned)

void SquircleRenderer::makeObject()
{
    static const int coords[6][4][3] = {
        { { +1, -1, -1 }, { -1, -1, -1 }, { -1, +1, -1 }, { +1, +1, -1 } },
        { { +1, +1, -1 }, { -1, +1, -1 }, { -1, +1, +1 }, { +1, +1, +1 } },
        { { +1, -1, +1 }, { +1, -1, -1 }, { +1, +1, -1 }, { +1, +1, +1 } },
        { { -1, -1, -1 }, { -1, -1, +1 }, { -1, +1, +1 }, { -1, +1, -1 } },
        { { +1, -1, +1 }, { -1, -1, +1 }, { -1, -1, -1 }, { +1, -1, -1 } },
        { { -1, -1, +1 }, { +1, -1, +1 }, { +1, +1, +1 }, { -1, +1, +1 } }
    };



    QVector<GLfloat> vertData;
    for (int i = 0; i < 1; ++i) {
        for (int j = 0; j < 4; ++j) {
            // vertex position
            vertData.append(0.4 * coords[i][j][0]);
            vertData.append(0.4 * coords[i][j][1]);
            vertData.append(0.4 * coords[i][j][2]);
            // texture coordinate
            vertData.append(j == 0 || j == 3);
            vertData.append(j == 0 || j == 1);
        }
    }


    vbo.create();
    vbo.bind();
    vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
}
//! [5]
//!
//!
//!
