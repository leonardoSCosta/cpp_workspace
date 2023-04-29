#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString filename = "../../Python_Workspace/Pipeline/python-pipe";
    this->pipe_f.setFileName(filename);

    if(this->pipe_f.open(QIODevice::ReadOnly))
        qDebug() << "File open\n";
    else
        qDebug() << this->pipe_f.errorString();

    this->quit = false;

    this->tmrReadTimer = new QTimer(this);
    connect(this->tmrReadTimer, &QTimer::timeout,
            this, &MainWindow::vTimerRead);
    this->tmrReadTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnQuit_clicked()
{
    exit(0);
    this->pipe_f.close();
}

void MainWindow::vTimerRead()
{
    if(this->pipe_f.size())
    {
        qDebug() << this->pipe_f.readAll();
        this->pipe_f.flush();
    }
    else
        qDebug() << "Nothing" << Qt::endl;
}
