#include "mainwindow.h"

#if defined(DEBUG)
#include <QDebug>
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <QWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QClipboard>
#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include <QItemSelectionModel>
#include "timecontainer.h"
#include "deletekeyeventfilter.h"

#include "tasksmodel.h"
#include "taskmodelio.h"
#include "task.h"
#include "hotkeyeventfilter.h"

const QDir MainWindow::m_file_location = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/TaskRegister");
const QString MainWindow::m_file_name = "tasks.list";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_content = new QWidget(this);
    m_content->setMinimumSize(1024, 500);
    setCentralWidget(m_content);

    QLayout *content = new QVBoxLayout();
    m_content->setLayout(content);

    QLayout *input_container = new QHBoxLayout();
    content->addItem(input_container);

    m_time_container = new TimeContainer(m_content);
    input_container->addWidget(m_time_container);

    DeleteKeyEventFilter *filter = new DeleteKeyEventFilter();
    m_task_list = new QTableView(this);
    TasksModel *task_model = new TasksModel(m_task_list);
    m_task_list->setModel(task_model);
    m_task_list->setSelectionMode(QTableView::SingleSelection);
    m_task_list->setSelectionBehavior(QTableView::SelectRows);
    m_task_list->installEventFilter(filter);
    input_container->addWidget(m_task_list);

    QHBoxLayout *buttons = new QHBoxLayout();
    content->addItem(buttons);

    m_next = new QPushButton("Next", m_content);
    buttons->addWidget(m_next);

    m_end = new QPushButton("End of day", m_content);
    buttons->addWidget(m_end);

    m_hotkey = new HotkeyEventFilter(1, 0x56, MOD_CONTROL | MOD_SHIFT);

    connect(m_next, &QPushButton::clicked, this, &MainWindow::onNextClicked);
    connect(m_task_list->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onSelectionChanged);
    connect(m_end, &QPushButton::clicked, this, &MainWindow::onEndOfDayClicked);
    connect(m_hotkey, &HotkeyEventFilter::hotkeyReceived, this, &MainWindow::onPaste);
    connect(filter, &DeleteKeyEventFilter::deletePressed, this, &MainWindow::onDeleteSelectedTask);

    QApplication::instance()->installNativeEventFilter(m_hotkey);

    QString file_path = m_file_location.absoluteFilePath(m_file_name);
    m_task_model_io = new TaskModelIo(task_model, file_path);
    m_task_model_io->read();

    onNextClicked();
}

MainWindow::~MainWindow()
{
    delete m_time_container;
    delete m_content;
    delete m_hotkey;
}

void MainWindow::onNextClicked()
{
    TasksModel *model = reinterpret_cast<TasksModel*>(m_task_list->model());
    QDateTime now = QDateTime::currentDateTime();
    Task *task = new Task(now, now, "");

    model->addTask(task);

    m_task_list->selectRow(model->count()-1);
}

void MainWindow::onDeleteSelectedTask()
{
    QModelIndexList list = m_task_list->selectionModel()->selectedRows();
    if(list.count() > 0)
    {
        int index = list.at(0).row();
        TasksModel *model = reinterpret_cast<TasksModel*>(m_task_list->model());

        m_time_container->setTask(0);

        model->removeTask(index);

        if(index >= model->count())
        {
            index--;
        }
        m_task_list->selectRow(index);

        if(model->count() == 0)
        {
            onNextClicked();
        }
    }
}

void MainWindow::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

#ifdef DEBUG
    qDebug() << "Row selected:" << selected.indexes().at(0).row();
#endif

    if(selected.indexes().count() > 0)
    {
        TasksModel *model = reinterpret_cast<TasksModel*>(m_task_list->model());
        int index = selected.indexes().at(0).row();

        Task *task = model->getTask(index);

        if(task)
        {
            m_time_container->setTask(task);
        }
    }
    else
    {
        m_time_container->setTask(0);
    }
}

void MainWindow::onEndOfDayClicked()
{
    m_task_list->setEnabled(false);
    m_task_list->setSelectionBehavior(QTableView::SelectItems);
    m_time_container->setEnabled(false);

    TasksModel *model = reinterpret_cast<TasksModel*>(m_task_list->model());
    model->getTask(model->count()-1)->disableAutoUpdateEndTime();

    onPaste();

    m_end->setEnabled(false);
}

void MainWindow::onPaste()
{
    static int item_index = -1;
    static int item_column = -1;

    const Task *task = 0;
    TasksModel *model = reinterpret_cast<TasksModel*>(m_task_list->model());

    if(item_index == -1)
    {
        item_index = 0;
        item_column = 0;
    }
    else
    {
        item_column++;
        if(item_column == 3)
        {
            item_index++;
            item_column = 0;
        }
    }

    if(item_index < model->count())
    {
        QModelIndex index = model->index(item_index, item_column);
        m_task_list->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);

        task = model->getTask(item_index);
        switch(item_column)
        {
        case 0:
            QApplication::clipboard()->setText(task->getStartTime().toString("dd/MM/yyyy hh:mm"));
            break;
        case 1:
            QApplication::clipboard()->setText(task->getEndTime().toString("dd/MM/yyyy hh:mm"));
            break;
        case 2:
            QApplication::clipboard()->setText(task->getDescription());
            break;
        }
    }
    else
    {
        item_index = -1;
        m_end->setEnabled(true);
        m_time_container->setEnabled(true);
        m_task_list->setSelectionBehavior(QTableView::SelectRows);
        m_task_list->setEnabled(true);
    }
}
