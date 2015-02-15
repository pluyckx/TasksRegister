#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TimeContainer;
class QWidget;
class QTableView;
class QPushButton;
class QModelIndex;
class HotkeyEventFilter;
class QDir;
class QItemSelection;
class TaskModelIo;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

private slots:
    void onNextClicked();
    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void onEndOfDayClicked();
    void onPaste();

    void onDeleteSelectedTask();

private:
    QWidget *m_content;
    TimeContainer *m_time_container;

    QTableView *m_task_list;

    QPushButton *m_next;
    QPushButton *m_end;

    HotkeyEventFilter *m_hotkey;

    TaskModelIo *m_task_model_io;

    static const QDir m_file_location;
    static const QString m_file_name;
};

#endif // MAINWINDOW_H
