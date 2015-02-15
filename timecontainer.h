#ifndef TIMECONTAINER_H
#define TIMECONTAINER_H

#include <QGroupBox>

class QDateTimeEdit;
class QLabel;
class QTimer;
class QTextEdit;
class Task;

class TimeContainer : public QGroupBox
{
    Q_OBJECT
public:
    explicit TimeContainer(QWidget *parent = 0);
    ~TimeContainer();

    void setTask(Task *task);

signals:

public slots:

private slots:
    void scheduleUpdate();
    void updateEndTime();

    void onStartTimeChanged();
    void onEndTimeChanged();
    void onDescriptionChanged();

private:
    QLabel *m_l_start;
    QDateTimeEdit *m_dte_start;
    QLabel *m_l_end;
    QDateTimeEdit *m_dte_end;
    QTextEdit *m_te_description;

    QTimer *m_t_endtime_updater;

    Task *m_task;

    bool m_updateEndTime;
};

#endif // TIMECONTAINER_H
