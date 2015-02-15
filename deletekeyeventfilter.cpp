#include "deletekeyeventfilter.h"

#include <QEvent>
#include <QKeyEvent>

DeleteKeyEventFilter::DeleteKeyEventFilter(QObject *parent) :
    QObject(parent)
{
}

bool DeleteKeyEventFilter::eventFilter(QObject *, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key_event = reinterpret_cast<QKeyEvent*>(event);
        if(key_event->matches(QKeySequence::Delete))
        {
            emit deletePressed();

            return true;
        }
    }

    return false;
}
