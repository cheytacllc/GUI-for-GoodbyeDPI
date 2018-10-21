#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTemporaryDir>
#include <QProcess>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QMenu>
#include "settings.h"
#include "about.h"
#include "mysettings.h"
#include <QFile>
#include <windows.h>
#include "mymessagebox.h"
#include <QCheckBox>
#include <QPushButton>
#include <QListWidget>
#include <QTime>
#include <QTimer>
#include <QSysInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QStringList, QWidget *parent = 0);
    QProcess procDnsCrypt;
    QTimer *logtimer;
    qint64 seeklog=0;
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void addItemListWidget();
    void changeDns(int dns);
    void dnsCrypt(QString arg);
    void procStart();
    void procStop();
    void checkTime();
    void timer();
    void processOutput();
    void processError();
    void handleState();
    void RestoreWindowTrigger(QSystemTrayIcon::ActivationReason RW);
    void onActionAyarlar();
    void onActionAbout();

    void onDefaultParamCheckState(Qt::CheckState state);

    QStringList prepareParameters(bool isComboParametreEnabled);

    void catchError(QProcess::ProcessError err);

signals:


private:
    Ui::MainWindow *ui;

    QTemporaryDir *tmpDir;
    QSettings *settings;

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;

    QAction *hideAction;
    QAction *showAction;
    QAction *closeAction;
    QAction *startAction;
    QAction *stopAction;
    QAction *settingsAction;

    QProcess *proc;

    QStringList args;

    Settings *ayarlar;
    About hakkinda;

    enum ParameterList{
        russia_blacklist,
        russia_blacklist_dnsredir,
        all,
        all_dnsredir,
        all_dnsredir_hardcore
    };
};

#endif // MAINWINDOW_H
