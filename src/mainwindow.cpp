#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDirIterator>
#include <QListIterator>
#include <memory>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QSslConfiguration>
#include <QDesktopServices>




MainWindow::MainWindow(QStringList arguments, QWidget *parent) :
    QMainWindow(parent),
    ayarlar(new Settings()),
    settings(new QSettings("HexOpenSource", "GBDPI-GUI", this)),
    trayIcon(new QSystemTrayIcon(this)),
    trayMenu(new QMenu(this)),
    hideAction(new QAction(tr("Gizle"), this)),
    showAction(new QAction(tr("Göster"), this)),
    closeAction(new QAction(tr("Çıkış"), this)),
    startAction(new QAction(QIcon(":/images/images/play-button.png"), tr("Başlat"), this)),
    stopAction(new QAction(QIcon(":/images/images/stop-button.png"), tr("Durdur"), this)),
    settingsAction(new QAction(QIcon(":/images/images/256-256-setings-configuration.png"), tr("Ayarlar"), this)),
    updateAction(new QAction(QIcon(":/images/images/download-2-icon.png"), tr("Yeni sürüm bulundu!"), this)),
    tmpDir(new QTemporaryDir()),
    proc(new QProcess(this)),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QApplication::setApplicationVersion("1.1.3");
    restoreGeometry(mySettings::readSettings("System/Geometry/Main").toByteArray());
    restoreState(mySettings::readSettings("System/WindowState/Main").toByteArray());
    QFile::remove(QApplication::applicationDirPath() + "/dnscrypt-proxy/"+QSysInfo::currentCpuArchitecture()+"/log.txt");
    mySettings::setTheme(mySettings::loadTheme());
    setWindowTitle("GoodByeDPI GUI "+QApplication::applicationVersion());
    setWindowIcon(QIcon(":/images/images/icon.ico"));
    trayIcon->setIcon(QIcon(":/images/images/stopped_icon.ico"));
    setWindowIcon(QIcon(":/images/images/stopped_icon.ico"));
    trayIcon->setToolTip("GoodByeDPI GUI "+QApplication::applicationVersion());
    trayIcon->setVisible(true);
    trayIcon->show();
    ui->labelParameters->setWordWrap(true);

    //For using lambda functions with traymenu
    auto& traymn = trayMenu;

    //Setting traymenu actions.
    connect(startAction, &QAction::triggered, this, &MainWindow::procStart);
    connect(stopAction, &QAction::triggered, this, &MainWindow::procStop);
    connect(closeAction, &QAction::triggered, [this](){
        QCoreApplication::quit();
    });
    connect(hideAction, &QAction::triggered, [this, traymn](){
        this->hide();
        traymn->actions().at(5)->setEnabled(false);
        traymn->actions().at(4)->setEnabled(true);
    });
    connect(settingsAction, &QAction::triggered, this, &MainWindow::onActionAyarlar);
    connect(showAction, &QAction::triggered, [this, traymn](){
        this->show();
        traymn->actions().at(5)->setEnabled(true);
        traymn->actions().at(4)->setEnabled(false);
    });

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(RestoreWindowTrigger(QSystemTrayIcon::ActivationReason)));


    QList<QAction*> actionList;
    actionList << startAction << stopAction << settingsAction << showAction << hideAction << closeAction;

    trayMenu->addActions(actionList);
    trayMenu->insertSeparator(showAction);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
    //Set false Stop and Hide actions
    trayMenu->actions().at(4)->setEnabled(false);
    trayMenu->actions().at(1)->setEnabled(false);

    connect(ui->actionAyarlar, &QAction::triggered, this, &MainWindow::onActionAyarlar);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onActionAbout);

    //Checking if default parameters enabled or not due to enable/disable parameters combo box.
    if(!settings->value("Parametre/defaultParam").toBool())
    {
        ui->comboParametre->setEnabled(false);
    }

    //Capturing state of default parameters checkbox for enable/disable parameters combo box.
    connect(ayarlar, &Settings::defaultParamStateChanged, this, &MainWindow::onDefaultParamCheckState, Qt::QueuedConnection);

    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::procStart);
    connect(ui->btnStop, &QPushButton::clicked, this, &MainWindow::procStop);
    connect(proc, &QProcess::stateChanged, this, &MainWindow::handleState);

    ui->comboParametre->addItem("russia_blacklist", QVariant("-1 --blacklist blacklist.txt"));
    ui->comboParametre->addItem("russia_blacklist_dnsredir", QVariant("-1 --dns-addr 1.1.1.1 --dns-port 1253 --dnsv6-addr 2a02:6b8::feed:0ff --dnsv6-port 1253 --blacklist blacklist.txt"));
    ui->comboParametre->addItem("all", QVariant("-1"));
    ui->comboParametre->addItem(tr("all_dnsredir (Tavsiye Edilen)"), QVariant("-1 --dns-addr 1.1.1.1 --dns-port 1253 --dnsv6-addr 2a02:6b8::feed:0ff --dnsv6-port 1253"));
    ui->comboParametre->addItem("all_dnsredir_hardcore", QVariant("-1 -a -m --dns-addr 1.1.1.1 --dns-port 1253 --dnsv6-addr 2a02:6b8::feed:0ff --dnsv6-port 1253"));

    ui->comboParametre->setCurrentIndex(3);

    ui->btnStop->setEnabled(false);

    connect(ui->comboParametre, QOverload<int>::of(&QComboBox::currentIndexChanged), [this]()
    {
        prepareParameters(true);
    });

    //Capturing ouput of goodbyedpi.exe
    connect(proc, &QProcess::readyReadStandardOutput, this, &MainWindow::processOutput);
    connect(proc, &QProcess::readyReadStandardError, this, &MainWindow::processError);

    if(settings->value("Parametre/defaultParam").toBool())
        prepareParameters(true);
    else
        prepareParameters(false);

    if(!this->isVisible())
    {
        hideAction->setEnabled(false);
        showAction->setEnabled(true);
    }

    connect(proc, &QProcess::errorOccurred, this, &MainWindow::catchError);
    changeDns("dhcp.bat");
    timer();
    ui->actionUpdate->setVisible(false);
    if(settings->value("System/UpdateCheck",true).toBool())
    {
         QTimer::singleShot(500, this, SLOT(checkUpdate()));
    }

}

MainWindow::~MainWindow()
{
    //dnsCrypt(" -service stop");
    //dnsCrypt(" -service uninstall");
    procDnsCrypt.close();
    changeDns("dhcp.bat");
    mySettings::writeSettings("System/Geometry/Main", saveGeometry());
    mySettings::writeSettings("System/WindowState/Main", saveState());
    delete ui;
    proc->kill();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    MyMessageBox msgBox;
    QPushButton *tray=msgBox.addButton(tr("Sistem Tepsisine Küçült"),QMessageBox::ActionRole);
    QPushButton *close=msgBox.addButton(tr("Kapat"),QMessageBox::ActionRole);
    QCheckBox *dontAsk = new QCheckBox(tr("Tekrar sorma"),this);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(tr("Programdan çıkmak ya da sistem tepsisine küçültmek mi istiyorsunuz?"));
    msgBox.setCheckBox(dontAsk);
    if(settings->value("System/dontAsk").isNull())
        settings->setValue("System/dontAsk",false);
    if(settings->value("System/dontAsk").toBool())
    {
        settings->setValue("System/dontAsk",true);
        if(settings->value("System/systemTray").toString() == "true" && (this->isTopLevel() || this->isVisible()))
        {
            this->hide();
            trayIcon->show();
            trayMenu->actions().at(4)->setEnabled(true);
            trayMenu->actions().at(5)->setEnabled(false);

            if(!settings->value("System/disableNotifications").toBool())
            {
                qDebug() << "Message will shown";
                QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
                trayIcon->showMessage("GoodByeDPI GUI", tr("Arka planda çalışıyor."), icon, 1000);
            }
        }
        else
        {
            //dnsCrypt(" -service stop");
            //dnsCrypt(" -service uninstall");
            procDnsCrypt.close();
            changeDns("dhcp.bat");
            ayarlar->close();
            hakkinda.close();
            event->accept();
        }
    }
    else
    {
        msgBox.exec();
        if(dontAsk->isChecked())
            settings->setValue("System/dontAsk",true);
        if(msgBox.clickedButton()==tray)
        {
            settings->setValue("System/systemTray",true);
            this->hide();
            trayIcon->show();
            trayMenu->actions().at(4)->setEnabled(true);
            trayMenu->actions().at(5)->setEnabled(false);
            if(!settings->value("System/disableNotifications").toBool())
            {
                qDebug() << "Message will shown";
                QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
                trayIcon->showMessage("GoodByeDPI GUI", tr("Arka planda çalışıyor."), icon, 1000);
            }
        }
        else
        {
            settings->setValue("System/systemTray",false);
            //dnsCrypt(" -service stop");
            //dnsCrypt(" -service uninstall");
            procDnsCrypt.close();
            changeDns("dhcp.bat");
            ayarlar->close();
            hakkinda.close();
            event->accept();
        }
    }
}

void MainWindow::timer()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkTime()));
    timer->start(10000);
}


void MainWindow::addItemListWidget()
{
    if(procDnsCrypt.canReadLine())
    {
        ui->listWidget_2->addItem(QString::fromLocal8Bit(procDnsCrypt.readLine()).trimmed());
        ui->listWidget_2->scrollToBottom();
        if(QString::fromLocal8Bit(procDnsCrypt.readLine()).contains("live servers", Qt::CaseInsensitive))
        {
            logtimer->stop();
            ui->listWidget_2->addItem("\n");
        }
        if(QString::fromLocal8Bit(procDnsCrypt.readLine()).contains("FATAL", Qt::CaseInsensitive))
        {
            QProcess killDns;
            killDns.start("TASKKILL /F /IM dnscrypt-proxy.exe");
            killDns.waitForFinished(500);
            killDns.start("TASKKILL /F /IM goodbyedpi.exe");
            killDns.waitForFinished(500);
            logtimer->stop();
            procStop();
            procStart();
        }
    }
}


void MainWindow::changeDns(QString dns)
{
        QProcess procDns;
        procDns.setNativeArguments("start /min "+QApplication::applicationDirPath()+"/dnscrypt-proxy/"+dns);
        procDns.start("cmd.exe /c ",QProcess::ReadOnly);
        //procDns.waitForFinished(2000);
        //procDns.close();
}

void MainWindow::dnsCrypt(QString arg)
{
    procDnsCrypt.setNativeArguments(arg);
    procDnsCrypt.setReadChannelMode(QProcess::MergedChannels);
    procDnsCrypt.start(QApplication::applicationDirPath()+"/dnscrypt-proxy/"+QSysInfo::currentCpuArchitecture()+"/dnscrypt-proxy.exe",QProcess::ReadOnly);
    procDnsCrypt.waitForReadyRead();

}


void MainWindow::procStart()
{
    proc->setArguments(prepareParameters(ui->comboParametre->isEnabled()));
    //ui->debugArea->appendPlainText("[*] " + ui->comboParametre->currentText());
    //ui->debugArea->appendPlainText("Exe Path: " + QDir::currentPath() + "/goodbyedpi/goodbyedpi.exe");
    proc->start(QApplication::applicationDirPath()+"/goodbyedpi/"+QSysInfo::currentCpuArchitecture()+"/goodbyedpi.exe", QProcess::ReadOnly);
    proc->waitForStarted(1000);

    if(!settings->value("System/disableNotifications").toBool())
    {
        qDebug() << "Message will shown";
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
        trayIcon->showMessage("GoodByeDPI GUI", tr("Başlatıldı."), icon, 1000);
    }
    setWindowIcon(QIcon(":/images/images/icon.ico"));
    trayIcon->setIcon(QIcon(":/images/images/icon.ico"));
    changeDns("localhost.bat");
    //dnsCrypt(" -service install");
    dnsCrypt("");
    //dnsCrypt(" -logfile=log.txt");



}

void MainWindow::procStop()
{
    proc->close();
    proc->waitForFinished(2000);
    if(!settings->value("System/disableNotifications").toBool())
    {
        qDebug() << "Message will shown";
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
        trayIcon->showMessage("GoodByeDPI GUI", tr("Durduruldu."), icon, 1000);
    }
    trayIcon->setIcon(QIcon(":/images/images/stopped_icon.ico"));
    setWindowIcon(QIcon(":/images/images/stopped_icon.ico"));
    //    dnsCrypt(" -service stop");
    //    dnsCrypt(" -service uninstall");
    QProcess stopWinDivert;
    //stopWinDivert.setNativeArguments("sc stop windivert1.3");
    stopWinDivert.start("sc stop windivert1.3", QProcess::ReadOnly);
    stopWinDivert.waitForFinished(500);
    procDnsCrypt.close();
    changeDns("dhcp.bat");
    ui->listWidget->scrollToBottom();
}

void MainWindow::checkTime()
{
    if(settings->value("System/systemSchedule").toBool())
    {
        if(settings->value("System/D"+QString::number(QDate::currentDate().dayOfWeek())+"/Enabled").toBool())
        {
            if((QTime::fromString(settings->value("System/D"+QString::number(QDate::currentDate().dayOfWeek())+"/systemScheduleStart").toString()).hour()<=QTime::currentTime().hour()&&QTime::fromString(settings->value("System/D"+QString::number(QDate::currentDate().dayOfWeek())+"/systemScheduleStart").toString()).minute()<=QTime::currentTime().minute())&&(QTime::fromString(settings->value("System/D"+QString::number(QDate::currentDate().dayOfWeek())+"/systemScheduleEnd").toString()).hour()>=QTime::currentTime().hour()&&QTime::fromString(settings->value("System/D"+QString::number(QDate::currentDate().dayOfWeek())+"/systemScheduleEnd").toString()).minute()>=QTime::currentTime().minute()))
            {
                if(proc->state()==QProcess::NotRunning)
                    ui->btnStart->click();
            }
            else
                if(proc->state()==QProcess::Running)
                    ui->btnStop->click();
        }
    }
}

void MainWindow::processOutput()
{
    proc->setReadChannel(QProcess::StandardOutput);
    QString output = proc->readAllStandardOutput();

    if(!output.isEmpty())
    {
        ui->listWidget->addItem(output);
    }
    ui->listWidget->scrollToBottom();

    logtimer = new QTimer(this);
    logtimer->start(500);
    connect(logtimer, SIGNAL(timeout()), this, SLOT(addItemListWidget()));

}


void MainWindow::processError()
{
    proc->setReadChannel(QProcess::StandardError);
    QString errout = proc->readAllStandardError();
    if(!errout.isEmpty())
    {
        ui->listWidget->addItem(proc->errorString());
    }
    ui->listWidget->scrollToBottom();
}
void MainWindow::handleState()
{
    if(proc->state() == QProcess::NotRunning)
    {
        ui->listWidget->addItem(tr("[-] Durduruldu"));
        ui->btnStart->setEnabled(true);
        ui->btnStop->setEnabled(false);
        trayMenu->actions().at(1)->setEnabled(false);
        trayMenu->actions().at(0)->setEnabled(true);
    }
    else if(proc->state() == QProcess::Running)
    {
        ui->listWidget->addItem(tr("[+] Başlatıldı\n[+] PID:") + QString::number(proc->processId()));
        ui->btnStart->setEnabled(false);
        ui->btnStop->setEnabled(true);
        trayMenu->actions().at(0)->setEnabled(false);
        trayMenu->actions().at(1)->setEnabled(true);
    }
    ui->listWidget->scrollToBottom();
}

void MainWindow::RestoreWindowTrigger(QSystemTrayIcon::ActivationReason RW)
{
    if(RW == QSystemTrayIcon::DoubleClick)
    {
        if(this->isHidden())
        {
            this->show();
            this->activateWindow();
            trayMenu->actions().at(5)->setEnabled(true);
            trayMenu->actions().at(4)->setEnabled(false);
        }
        else
        {
            this->hide();
            trayMenu->actions().at(5)->setEnabled(false);
            trayMenu->actions().at(4)->setEnabled(true);
        }
    }
}

void MainWindow::onActionAyarlar()
{
    ayarlar->show();

}


void MainWindow::onActionAbout()
{
    hakkinda.exec();
}

void MainWindow::checkUpdate()
{
    QString url = "https://cheytacllc.github.io/GUI-for-GoodbyeDPI/version.html";
    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setUrl(QUrl(url));
    QNetworkReply *response = manager.get(request);
    QEventLoop event;
    connect(response, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();
    content = response->readAll();
    if(content.trimmed()!=QApplication::applicationVersion()&&content.trimmed()!="")
        ui->actionUpdate->setVisible(true);
    else
    {
        QTimer::singleShot(60000, this, SLOT(checkUpdate()));
    }
        //QTimer::singleShot(60000, this, SLOT(checkUpdate()));
        //QMessageBox::information(this,"New version found","<body><html><a href='https://github.com/cheytacllc/GUI-for-GoodbyeDPI/releases/download/"+content.trimmed()+"/GoodByeDPI_GUI.zip'>Click here to download new version</a></body></html>");

}


void MainWindow::onDefaultParamCheckState(Qt::CheckState state)
{
    if(state == Qt::Checked)
    {
        ui->comboParametre->setEnabled(true);
        prepareParameters(true);
    }
    else
    {
        ui->comboParametre->setEnabled(false);
        prepareParameters(false);
    }

}

QStringList MainWindow::prepareParameters(bool isComboParametreEnabled)
{
    QStringList defaultparameters;
    QStringList customParameters;
    QStringList quickParameters;
    QStringList param2Box;
    QStringList param1Box;

    //PARAMBOX1
    if(settings->value("Parametre/paramP").toBool())
        param1Box << "-p";
    if(settings->value("Parametre/paramR").toBool())
        param1Box << "-r";
    if(settings->value("Parametre/paramS").toBool())
        param1Box << "-s";
    if(settings->value("Parametre/paramM").toBool())
        param1Box << "-m";
    if(settings->value("Parametre/paramF").toString() != "false")
        param1Box << settings->value("Parametre/paramF").toString();
    if(settings->value("Parametre/paramK").toString() != "false")
        param1Box << settings->value("Parametre/paramK").toString();
    if(settings->value("Parametre/paramN").toBool())
        param1Box << "-n";
    if(settings->value("Parametre/paramE").toString() != "false")
        param1Box << settings->value("Parametre/paramE").toString();
    if(settings->value("Parametre/paramA").toBool())
        param1Box << "-a";
    if(settings->value("Parametre/paramW").toBool())
        param1Box << "-w";
    if(settings->value("Parametre/paramPort").toString() != "false")
        param1Box << settings->value("Parametre/paramPort").toString();
    if(settings->value("Parametre/paramIpId").toString() != "false")
        param1Box << settings->value("Parametre/paramIpId").toString();

    //PARAMBOX2
    if(settings->value("Parametre/paramDnsAddr").toString() != "false")
        param2Box << settings->value("Parametre/paramDnsAddr").toString();
    if(settings->value("Parametre/paramDnsPort").toString() != "false")
        param2Box << settings->value("Parametre/paramDnsPort").toString();
    if(settings->value("Parametre/paramDnsPort").toString() != "false")
        param2Box << settings->value("Parametre/paramDnsPort").toString();
    if(settings->value("Parametre/paramDnsv6Addr").toString() != "false")
        param2Box << settings->value("Parametre/paramDnsv6Addr").toString();
    if(settings->value("Parametre/paramDnsv6Port").toString() != "false")
        param2Box << settings->value("Parametre/paramDnsv6Port").toString();
    if(settings->value("Parametre/paramBlacklist").toString() != "false")
        param2Box << "--blacklist blacklist.txt";

    //QUICKSETTINGS
    if(settings->value("Parametre/paramQuick").toString() == "-1")
        quickParameters << "-p -r -s -f 2 -k 2 -n -e 2" << param2Box;
    if(settings->value("Parametre/paramQuick").toString() == "-2")
        quickParameters << "-p -r -s -f 2 -k 2 -n -e 40" << param2Box;
    else if(settings->value("Parametre/paramQuick").toString() == "-3")
        quickParameters << "-p -r -s -e 40" << param2Box;
    else if(settings->value("Parametre/paramQuick").toString() == "-4")
        quickParameters << "-p -r -s" << param2Box;

    //DEFAULTPARAMETERS
    switch (ui->comboParametre->currentIndex()) {
    case 0:
        defaultparameters << "-1 --blacklist blacklist.txt";
        break;
    case 1:
        defaultparameters << "-1 --dns-addr 1.1.1.1 --dns-port 1253 --dnsv6-addr 2a02:6b8::feed:0ff --dnsv6-port 1253 --blacklist blacklist.txt";
        break;
    case 2:
        defaultparameters << "-1";
        break;
    case 3:
        defaultparameters << "-1 --dns-addr 1.1.1.1 --dns-port 1253 --dnsv6-addr 2a02:6b8::feed:0ff --dnsv6-port 1253";
        break;
    case 4:
        defaultparameters << "-1 -a -m --dns-addr 1.1.1.1 --dns-port 1253 --dnsv6-addr 2a02:6b8::feed:0ff --dnsv6-port 1253";
    }

    //CUSTOMPARAMETERS
    customParameters << param1Box << param2Box;

    //UPDATE Parameter Label
    if(isComboParametreEnabled)
    {
        ui->labelParameters->setText("goodbyedpi.exe " + defaultparameters.join(" "));
        return defaultparameters;
    }
    else if(settings->value("Parametre/customParam").toString() == "true" && settings->value("Parametre/quickSettings").toString() == "false")
    {
        ui->labelParameters->setText("goodbyedpi.exe " + customParameters.join(" "));
        return customParameters;
    }
    else if(settings->value("Parametre/customParam").toString() == "false" && settings->value("Parametre/quickSettings").toString() == "false")
    {
        ui->labelParameters->setText("goodbyedpi.exe");
        return QStringList();
    }
    else if(settings->value("Parametre/customParam").toString() == "true" && settings->value("Parametre/quickSettings").toString() == "true")
    {
        ui->labelParameters->setText("goodbyedpi.exe" + quickParameters.join(" "));
        return QStringList();
    }
    else
    {
        ui->labelParameters->setText("goodbyedpi.exe " + quickParameters.join(" "));
        return quickParameters;
    }

}

void MainWindow::catchError(QProcess::ProcessError err)
{
    ui->listWidget->addItem(proc->errorString());
    ui->listWidget->scrollToBottom();
}


void MainWindow::on_actionUpdate_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/cheytacllc/GUI-for-GoodbyeDPI/releases/download/"+content.trimmed()+"/GoodByeDPI_GUI.zip"));
}
