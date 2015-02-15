#ifndef HOTKEYEVENTFILTER_H
#define HOTKEYEVENTFILTER_H

#include <QObject>
#include <QAbstractNativeEventFilter>

class HotkeyEventFilter : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit HotkeyEventFilter(int id, int key, int flags, QObject *parent = 0);
    ~HotkeyEventFilter();

    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
signals:
    void hotkeyReceived();

public slots:

};

#endif // HOTKEYEVENTFILTER_H
