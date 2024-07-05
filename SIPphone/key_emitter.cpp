#include <QCoreApplication>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QQuickItem>
#include <QtGui/QInputMethod>
#include "key_emitter.h"

KeyEmitter::KeyEmitter()
{
}

KeyEmitter::~KeyEmitter()
{
}

void KeyEmitter::emitKey(Qt::Key key)
{
    disableAndroidKB();
	QQuickItem* receiver = qobject_cast<QQuickItem*>(QGuiApplication::focusObject());
	if(!receiver) {
		return;
	}
	QKeyEvent pressEvent = QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier, QKeySequence(key).toString());
	QKeyEvent releaseEvent = QKeyEvent(QEvent::KeyRelease, key, Qt::NoModifier);
	QCoreApplication::sendEvent(receiver, &pressEvent);
	QCoreApplication::sendEvent(receiver, &releaseEvent);

}

void KeyEmitter::disableAndroidKB(){
     QGuiApplication::inputMethod()->setVisible(false);
}
