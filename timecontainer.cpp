#include "timecontainer.h"
#include "task.h"

#if defined(DEBUG)
#include <QDebug>
#endif

#include <QHBoxLayout>
#include <QDateTimeEdit>
#include <QLabel>
#include <QTimer>
#include <QTextEdit>

TimeContainer::TimeContainer(QWidget *parent) :
    QGroupBox("Time", parent), m_task(0), m_updateEndTime(false)
{
    QLayout *layout = new QVBoxLayout(this);
    QLayout *time_layout = new QHBoxLayout();
    layout->addItem(time_layout);

    setLayout(layout);

    QDateTime now = QDateTime::currentDateTime();

    m_l_start = new QLabel("Start", this);
    m_dte_start = new QDateTimeEdit(now, this);
    m_dte_start->setEnabled(false);

    m_l_end = new QLabel("End", this);
    m_dte_end = new QDateTimeEdit(now, this);
    m_dte_end->setEnabled(false);

    time_layout->addWidget(m_l_start);
    time_layout->addWidget(m_dte_start);
    time_layout->addWidget(m_l_end);
    time_layout->addWidget(m_dte_end);

    m_t_endtime_updater = new QTimer(this);
    m_t_endtime_updater->setSingleShot(true);

    m_te_description = new QTextEdit(this);
    m_te_description->setMinimumSize(QSize(350, 200));
    m_te_description->setEnabled(false);
    layout->addWidget(m_te_description);

    connect(m_t_endtime_updater, &QTimer::timeout, this, &TimeContainer::updateEndTime);
    connect(m_t_endtime_updater, &QTimer::timeout, this, &TimeContainer::scheduleUpdate);

    connect(m_dte_start, &QDateTimeEdit::editingFinished, this, &TimeContainer::onStartTimeChanged);
    connect(m_dte_end, &QDateTimeEdit::editingFinished, this, &TimeContainer::onEndTimeChanged);
    connect(m_te_description, &QTextEdit::textChanged, this, &TimeContainer::onDescriptionChanged);

    scheduleUpdate();
}

TimeContainer::~TimeContainer()
{
    delete m_l_start;
    delete m_dte_start;
    delete m_l_end;
    delete m_dte_end;

    delete m_t_endtime_updater;
}

void TimeContainer::setTask(Task *task)
{
    m_task = task;

    if(task != 0)
    {
        m_dte_start->setDateTime(task->getStartTime());
        m_dte_end->setDateTime(task->getEndTime());
        m_te_description->setText(task->getDescription());
    }

    m_dte_start->setEnabled(task != 0);
    m_dte_end->setEnabled(task != 0);
    m_te_description->setEnabled(task != 0);
}

void TimeContainer::scheduleUpdate()
{
    if(m_updateEndTime)
    {
        if(!m_t_endtime_updater->isActive())
        {
            QDateTime now = QDateTime::currentDateTime();
            int seconds = now.time().second();
            int delay = (60 - seconds) * 1000;

            m_t_endtime_updater->start(delay);

#if defined(DEBUG)
            qDebug().nospace() << "Update in " << delay << "ms";
#endif
        }
#if defined(DEBUG)
        else
        {
            qDebug() << "Timer already running";
        }
#endif
    }
}

void TimeContainer::updateEndTime()
{
    if(m_task)
    {
        m_task->updateEndTime();
        m_dte_end->setDateTime(m_task->getEndTime());
    }
}

void TimeContainer::onStartTimeChanged()
{
    if(m_task)
    {
        m_task->setStartTime(m_dte_start->dateTime());
    }

#ifdef DEBUG
    qDebug() << "onStartTimeChanged, m_task =" << m_task;
#endif
}

void TimeContainer::onEndTimeChanged()
{
    if(m_task)
    {
        m_task->setEndTime(m_dte_end->dateTime());
    }

#ifdef DEBUG
    qDebug() << "onEndTimeChanged, m_task =" << m_task;
#endif
}

void TimeContainer::onDescriptionChanged()
{
    if(m_task)
    {
        m_task->setDescription(m_te_description->document()->toPlainText());
    }

#ifdef DEBUG
    qDebug() << "onDescriptionChanged, m_task =" << m_task;
#endif
}
