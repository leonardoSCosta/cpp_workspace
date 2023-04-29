#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDebug>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnQuit_clicked();
public slots:
    void vTimerRead();

private:
    Ui::MainWindow *ui;
    bool quit;
    QFile pipe_f;
    QTimer *tmrReadTimer;
};
#endif // MAINWINDOW_H
