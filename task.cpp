#ifdef DEBUG
#include <QDebug>
#endif

#include "task.h"

Task::Task(const QDateTime &start, const QDateTime &end, const QString &description, const bool auto_update_end,  QObject *parent) :
    QObject(parent), m_start(start), m_end(end), m_description(description), m_auto_update_end(auto_update_end)
{
    m_start = m_start.addSecs(-m_start.time().second());
    m_end = m_end.addSecs(m_end.time().second());
}

void Task::setDescription(const QString &description)
{
    if(m_description != description)
    {
        QString prev = m_description;
        m_description = description;
        emit taskChanged(this, prev, m_start, m_end);
    }
}

void Task::setStartTime(const QDateTime &start)
{
    QDateTime tmp = start.addSecs(-start.time().second());
    if(m_start != tmp)
    {
        QDateTime prev = m_start;
        m_start = tmp;
        emit taskChanged(this, m_description, prev, m_end);
    }
}

void Task::setEndTime(const QDateTime &end)
{
    QDateTime tmp = end.addSecs(-end.time().second());
    if(m_end != tmp)
    {
        QDateTime prev = m_start;
        m_end = tmp;
        emit taskChanged(this, m_description, m_start, prev);
    }
}

void Task::moveStartEndTime(qint64 sec)
{
    sec = (sec / 60) * 60;
    if(qAbs(sec) >= 60)
    {
#ifdef DEBUG
        qDebug() << "Delta > 60:" << sec;
#endif
        QDateTime prev_start = m_start;
        QDateTime prev_end = m_end;

        m_start = m_start.addSecs(sec);
        m_end = m_end.addSecs(sec);

        emit taskChanged(this, m_description, prev_start, prev_end);
    }
}

void Task::updateEndTime()
{
    if(m_auto_update_end)
    {
        setEndTime(QDateTime::currentDateTime());
    }
}

void Task::disableAutoUpdateEndTime()
{
    m_auto_update_end = false;
}

void Task::setTask(const Task *task)
{
    QDateTime prev_start = m_start;
    QDateTime prev_end = m_end;
    QString prev_description = m_description;

    m_start = task->m_start;
    m_end = task->m_end;
    m_description = task->m_description;
    m_auto_update_end = task->m_auto_update_end;

    emit taskChanged(this, prev_description, prev_start, prev_end);
}

Task *Task::clone()
{
    return new Task(m_start, m_end, m_description);
}

const QString& Task::getDescription() const
{
    return m_description;
}

const QDateTime& Task::getStartTime() const
{
    return m_start;
}

const QDateTime& Task::getEndTime() const
{
    return m_end;
}


const QString Task::toString() const
{
    QString s = m_start.toString() + tr(" - ") + m_end.toString() + ": " + m_description;

    return s;
}

const QString Task::toString(const QDateTime &time)
{
    static const QString format("dd/MM/yyyy hh:mm");

    return time.toString(format);
}

bool Task::operator==(const Task &task)
{
    if(&task == this)
    {
        return true;
    }
    else
    {
        return (task.m_start == m_start &&
                task.m_end == m_end &&
                task.m_description == m_description);
    }
}

bool Task::operator !=(const Task &task)
{
    return !(*this == task);
}
