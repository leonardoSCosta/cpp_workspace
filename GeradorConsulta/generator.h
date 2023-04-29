#ifndef GENERATOR_H
#define GENERATOR_H

#include <QMainWindow>
#include <QDateTime>
#include <QSpinBox>
#include <QCheckBox>
#include <QClipboard>

#define SCHEDULE_TYPE 0
#define FROM_TO_DATE_WEEK 1

QT_BEGIN_NAMESPACE
namespace Ui { class Generator; }
QT_END_NAMESPACE

class Generator : public QMainWindow
{
    Q_OBJECT

public:
    Generator(QWidget *parent = nullptr);
    ~Generator();

private:
    Ui::Generator *ui;

    QDateTime dtFromDate;
    QDateTime dtToDate;
    QString strScheduleType;
    qint8 iWeeks;

    QDateTime dtCurrentDate;
    qint8 iDaysBetween;
    QStringList strAvailableTimes;

    QStringList strScheduleMessage;
    QVector<QCheckBox*> ckTimes;

    void vInitDateLines();
    void vUpdatePreamble();

private slots:
    void vUpdateScheduleType_(const QString &_text);
    void vUpdateWeeks_(const int &_value);
    void vUpdateFromDate_();
    void vUpdateToDate_();
    void vUpdateAvailableTimes_();
    void vNextDate_();
    void vPreviousDate_();
    void vUpdateText_();
    void vClearText_();
    void vCopy_();
};
#endif // GENERATOR_H
