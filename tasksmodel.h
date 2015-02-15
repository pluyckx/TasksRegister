#ifndef TASKSMODEL_H
#define TASKSMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QDateTime>

class Task;
class TaskModelIo;

class TasksModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TasksModel(QObject *parent = 0);

    void addTask(Task *task);
    Task *getTask(QModelIndex index) const;
    Task *getTask(int index) const;
    bool setTask(int index, Task *new_task);
    bool setTask(const Task &old_task, Task *new_task);
    bool removeTask(int index);
    int count() const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    friend class TaskModelIo;
    void addTaskNoEmit(Task *task);

signals:
    void modelChanged();

private slots:
    void onTaskChanged(Task *, QString prev_description, QDateTime prev_start, QDateTime prev_end);

private:
    QList<Task*> *m_data;
};

#endif // TASKSMODEL_H
