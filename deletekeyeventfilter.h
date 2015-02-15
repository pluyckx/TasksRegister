#ifndef DELETEKEYEVENTFILTER_H
#define DELETEKEYEVENTFILTER_H

#include <QObject>

class DeleteKeyEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit DeleteKeyEventFilter(QObject *parent = 0);

    bool eventFilter(QObject *, QEvent *);

signals:
    void deletePressed();

public slots:

};

#endif // DELETEKEYEVENTFILTER_H
