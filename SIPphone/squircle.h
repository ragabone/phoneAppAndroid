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

#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QTimer>

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

class videoThread;


//! [1]
class SquircleRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    SquircleRenderer(videoThread *);
    ~SquircleRenderer();

    void setT(qreal t) { m_t = t; }
    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void doRendering(bool);

public slots:
    void paint();

private:
    videoThread *m_pVideoSrc;
    QSize m_viewportSize;
    qreal m_t;
    QOpenGLShaderProgram *m_program;
    QOpenGLTexture *textures[6];
    QOpenGLBuffer vbo;
    int mCount;
    void makeObject();
    void paintGL();
    void paintGL2();
    void paintGL3();
    bool mDoRendering;
    QImage* m_img;
    char mPixB[480*272*3];

};
//! [1]

//! [2]
class Squircle : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying WRITE setPlaying)
public:
    Squircle();
    void setPlaying(bool val);
    qreal t() const { return m_t; }
    bool isPlaying() const { return mIsPlaying; }

    void setT(qreal t);

signals:
    void tChanged();

public slots:
    void sync();
    void cleanup();
    void setTimeOut();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    qreal m_t;
    bool mIsPlaying;
    SquircleRenderer *m_renderer;
    videoThread *m_pVideoSrc;
    QTimer *m_pTimer;
};
//! [2]

#endif // SQUIRCLE_H
