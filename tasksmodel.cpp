#include "tasksmodel.h"

#include <QFile>
#include "task.h"

#if defined(DEBUG)
#include <QDebug>
#endif

TasksModel::TasksModel(QObject *parent) :
    QAbstractItemModel(parent), m_data(new QList<Task*>())
{
}

void TasksModel::addTask(Task *task)
{
    addTaskNoEmit(task);
    emit modelChanged();
}

void TasksModel::addTaskNoEmit(Task *task)
{
    emit beginResetModel();
    m_data->append(task);
    connect(task, &Task::taskChanged, this, &TasksModel::onTaskChanged);
    emit endResetModel();
}

Task *TasksModel::getTask(QModelIndex index) const
{
    return getTask(index.row());
}

Task *TasksModel::getTask(int index) const
{
    if(index < m_data->count())
    {
        return m_data->at(index);
    }
    else
    {
        return 0;
    }
}

bool TasksModel::setTask(int index, Task *new_task)
{
    if(index >= 0 && index < m_data->count())
    {
        disconnect(m_data->at(index), &Task::taskChanged, this, &TasksModel::onTaskChanged);
        m_data->replace(index, new_task);
        connect(new_task, &Task::taskChanged, this, &TasksModel::onTaskChanged);

        QModelIndex tl = createIndex(index, 0);
        QModelIndex br = createIndex(index, 3);

        emit dataChanged(tl, br);

        emit modelChanged();

        return true;
    }
    else
    {
        return false;
    }
}

bool TasksModel::setTask(const Task &old_task, Task *new_task)
{
    int index = 0;
    while(index < m_data->count() && *m_data->at(index) != old_task)
    {
        index++;
    }

    if(index < m_data->count())
    {
        return setTask(index, new_task);
    }
    else
    {
        return false;
    }
}

bool TasksModel::removeTask(int index)
{
    if(index < m_data->count())
    {
        emit beginResetModel();
        m_data->removeAt(index);
        emit endResetModel();

        emit modelChanged();

        return true;
    }
    else
    {
        return false;
    }
}

int TasksModel::count() const
{
    return m_data->count();
}

int TasksModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return count();
}


int TasksModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant TasksModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::DisplayRole && index.isValid() && index.row() < m_data->count())
    {
        switch(index.column())
        {
        case 0:
            return Task::toString(m_data->at(index.row())->getStartTime());
        case 1:
            return Task::toString(m_data->at(index.row())->getEndTime());
        case 2:
            return m_data->at(index.row())->getDescription();
        default:
            return QVariant();
        }
    }
    else
    {
        return QVariant();
    }
}

QVariant TasksModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            switch(section)
            {
            case 0:
                return QVariant("Start time");
            case 1:
                return QVariant("End time");
            case 2:
                return QVariant("Description");
            default:
                return QVariant();
            }
        }
        else
        {
            return QVariant(section+1);
        }
    }
    else
    {
        return QVariant();
    }
}

QModelIndex TasksModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return createIndex(row, column);
}

QModelIndex TasksModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}


bool TasksModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    return false;
}

Qt::ItemFlags TasksModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}

void TasksModel::onTaskChanged(Task *task, QString prev_description, QDateTime prev_start, QDateTime prev_end)
{
    int index = m_data->indexOf(task);
    if(index != -1)
    {
#ifdef DEBUG
        qDebug() << "task changed: " << index;
#endif
        QModelIndex mi_tl = createIndex(index, 0);
        QModelIndex mi_br = createIndex(index, 3);

        emit dataChanged(mi_tl, mi_br);
        emit modelChanged();

        if(index > 0)
        {
            Task *prev_task = m_data->at(index-1);
            if(prev_task->getEndTime() > task->getStartTime())
            {
                qint64 delta = task->getStartTime().secsTo(prev_task->getEndTime());
#ifdef DEBUG
                qDebug().nospace() << "prev_task_end < cur_task_start: with " << delta << "seconds";
#endif
                prev_task->moveStartEndTime(delta);
            }
            else if(prev_start < task->getStartTime() && prev_start == prev_task->getEndTime())
            {
                qint64 delta = prev_start.secsTo(task->getStartTime());
#ifdef DEBUG
                qDebug().nospace() << "prev_start < cur_task_start: with " << delta << "seconds";
#endif
                prev_task->moveStartEndTime(delta);
            }
        }

        if(index+1 < m_data->count())
        {
            Task *next_task = m_data->at(index+1);
            if(next_task->getStartTime() < task->getEndTime())
            {
                qint64 delta = next_task->getStartTime().secsTo(task->getEndTime());
#ifdef DEBUG
                qDebug().nospace() << "next_task_start < cur_task_end: with " << delta << "seconds";
#endif
                next_task->moveStartEndTime(delta);
            }
            else if(prev_end > task->getEndTime() && prev_end == next_task->getStartTime())
            {
                qint64 delta = task->getEndTime().secsTo(prev_end);
#ifdef DEBUG
                qDebug().nospace() << "prev_end > cur_task_end: with " << delta << "seconds";
#endif
                next_task->moveStartEndTime(delta);
            }
        }
    }
}
