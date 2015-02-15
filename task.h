#ifndef TASK_H
#define TASK_H

#include <QObject>

#include <QDateTime>

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(const QDateTime &start, const QDateTime &end, const QString &description, const bool auto_update_end = true, QObject *parent = 0);

    void setDescription(const QString &description);
    void setStartTime(const QDateTime &start);
    void setEndTime(const QDateTime &end);
    void moveStartEndTime(qint64 sec);
    void updateEndTime();
    void disableAutoUpdateEndTime();

    void setTask(const Task *task);

    Task *clone();

    const QString& getDescription() const;
    const QDateTime& getStartTime() const;
    const QDateTime& getEndTime() const;

    const QString toString() const;

    static const QString toString(const QDateTime &time);

    bool operator==(const Task &task);
    bool operator!=(const Task &task);

signals:
    void taskChanged(Task *task, QString prev_description, QDateTime prev_start, QDateTime prev_end);

public slots:

private:
    QDateTime m_start;
    QDateTime m_end;
    QString m_description;
    bool m_auto_update_end;
};

#endif // TASK_H
