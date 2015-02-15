#include "taskmodelio.h"

#include <QFile>
#include <QDir>
#include <QTimer>
#include "tasksmodel.h"
#include "task.h"

TaskModelIo::TaskModelIo(TasksModel *model, QString file_path, QObject *parent)
    : QObject(parent), m_model(model), m_file(new QFile(file_path)), m_delayed_write(new QTimer())
{
    if(!m_file->exists())
    {
        QDir dir(QDir(file_path).dirName());
        if(!dir.exists())
        {
            if(!dir.mkpath(dir.absolutePath()))
            {
                delete m_file;
                m_file = 0;
            }
        }
    }

    m_delayed_write->setSingleShot(true);

    if(m_file)
    {
        connect(model, &TasksModel::modelChanged, this, &TaskModelIo::writeOnChange);
        connect(m_delayed_write, &QTimer::timeout, this, &TaskModelIo::write);
    }
}

TaskModelIo::~TaskModelIo()
{
    if(m_file)
    {
        delete m_file;
    }

    if(m_delayed_write)
    {
        delete m_delayed_write;
    }

    /* do not delete the model, we do not own it, we just use it */
    m_model = 0;
    m_file = 0;
    m_delayed_write = 0;
}

bool TaskModelIo::write()
{
    if(m_file->open(QFile::WriteOnly | QFile::Text))
    {
        for(int i=0; i<m_model->m_data->count(); i++)
        {
            QByteArray data;
            QString description = m_model->m_data->at(i)->getDescription();

            data += m_model->m_data->at(i)->getStartTime().toString("yyyy/MM/dd hh:mm");
            data += " - ";
            data += m_model->m_data->at(i)->getEndTime().toString("yyyy/MM/dd hh:mm");
            data += ": ";
            data += description.replace("\\", "\\\\").replace("\n", "\\n");
            data += "\n";

            m_file->write(data);
        }

        m_file->close();

        return true;
    }
    else
    {
        return false;
    }
}

bool TaskModelIo::read()
{
    if(m_file->open(QFile::ReadOnly | QFile::Text))
    {
        while(m_file->pos() != m_file->size())
        {
            QString line = m_file->readLine();
            QString start = line.mid(0, 16);
            QString end = line.mid(19, 16);
            QString description = line.mid(37).replace("\\n", "\n").replace("\\\\", "\\");
            description = description.left(description.length()-1);

            QDateTime dt_start = QDateTime::fromString(start, "yyyy/MM/dd hh:mm");
            QDateTime dt_end = QDateTime::fromString(end, "yyyy/MM/dd hh:mm");

            m_model->addTaskNoEmit(new Task(dt_start, dt_end, description));
        }

        m_file->close();

        return true;
    }
    else
    {
        return false;
    }
}

void TaskModelIo::writeOnChange()
{
    if(m_delayed_write->isActive())
    {
        m_delayed_write->stop();
    }

    /* delay 10 seconds before write */
    m_delayed_write->start(10000);
}
