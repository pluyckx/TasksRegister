#include "hotkeyeventfilter.h"


#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifdef DEBUG
#include <QDebug>
#endif

HotkeyEventFilter::HotkeyEventFilter(int id, int key, int flags, QObject *parent) :
    QObject(parent)
{
#if defined(_WIN32) || defined(_WIN64)
    BOOL ret = RegisterHotKey(0, id, flags, key);
#endif
}

HotkeyEventFilter::~HotkeyEventFilter()
{
#if defined(_WIN32) || defined(_WIN64)
    UnregisterHotKey(0, 1);
#endif
}


bool HotkeyEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);
    Q_UNUSED(message);

#if defined(_WIN32) || defined(_WIN64)
    MSG *msg = static_cast<MSG*>(message);

    if(msg->message == WM_HOTKEY)
    {
#ifdef DEBUG
        qDebug() << "Hotkey received:" << msg->lParam << msg->wParam;
#endif
        emit hotkeyReceived();

        return true;
    }
#endif

    return false;
}
