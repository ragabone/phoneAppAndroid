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

#include "videoGL.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLTexture>


Squircle::Squircle()
    : m_t(0)
    , m_renderer(0)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}
//! [7]

//! [8]
void Squircle::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    emit tChanged();
    if (window())
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

SquircleRenderer::~SquircleRenderer()
{
    delete m_texture;
}
//! [6]

//! [9]
void Squircle::sync()
{
    if (!m_renderer) {
        m_renderer = new SquircleRenderer();
        connect(window(), SIGNAL(beforeRendering()), m_renderer, SLOT(paint()), Qt::DirectConnection);
    }
    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setT(m_t);
}
//! [9]

//! [4]
//!
//!
SquircleRenderer::SquircleRenderer(): m_t(0), m_texture(0){
    //paint();
}



void SquircleRenderer::paint()
{
//    QImage (":/images/screensaver_registered.png");
//    m_img = image.convertToFormat(QImage::Format_RGBA8888);
    if (!m_texture) {


        initializeOpenGLFunctions();

        glClearColor(0.0, 0.0, 0.0, 0.0);

        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glOrtho(0, 480, 0, 272, 0, 1);
        glMatrixMode(GL_MODELVIEW);

        glShadeModel(GL_FLAT);

        m_img = QImage("qrc:///images/screensaver_registered.png").mirrored().convertToFormat(QImage::Format_RGBA8888);
        m_texture = new QOpenGLTexture(m_img);
//         m_texture=new QOpenGLTexture(QOpenGLTexture::Target2D);

//         m_texture->setSize(480,272);
//         m_texture->setData(image);
//         m_texture->allocateStorage();
         //QOpenGLTexture *texture = new QOpenGLTexture(white);
         //m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
         //m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
         //m_texture->setSize(480,272,4);
         //m_texture->allocateStorage();

    }
//! [4] //! [5]
//    m_texture->bind();
//    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    glPixelStorei(GL_UNPACK_ROW_LENGTH, m_img.bytesPerLine() / 4);
//    glDrawPixels(m_img.width(), m_img.height(), QOpenGLTexture::RGBA, GL_UNSIGNED_BYTE, m_img.bits());


    m_texture->bind();
    glDrawPixels(m_img.width(), m_img.height(), QOpenGLTexture::RGBA, GL_UNSIGNED_BYTE, m_img.bits());

}

