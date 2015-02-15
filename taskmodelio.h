#ifndef TASKMODELIO_H
#define TASKMODELIO_H

#include <QObject>

class TasksModel;
class QFile;
class QTimer;

class TaskModelIo : public QObject
{
    Q_OBJECT
public:
    explicit TaskModelIo(TasksModel *model, QString file_path, QObject *parent = 0);
    ~TaskModelIo();

signals:

public slots:
    bool write();
    bool read();

private slots:
    void writeOnChange();

private:
    TasksModel *m_model;
    QFile *m_file;
    QTimer *m_delayed_write;
};

#endif // TASKMODELIO_H
