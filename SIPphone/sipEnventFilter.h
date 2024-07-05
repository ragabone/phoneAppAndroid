#include <QObject>
#include <QEvent>

class SIPRequestEater: public QObject
{
    Q_OBJECT
protected:
    bool eventFilter(QObject *obj, QEvent *event)
    {
        if(event->type() == QEvent::FocusIn)
        {
            // filter out FocusIn event
            return true;
        }
        else
        {
            // standard event processing
            return QObject::eventFilter(obj, event);
        }
    }
};
