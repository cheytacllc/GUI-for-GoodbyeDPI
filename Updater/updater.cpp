#include "updater.h"
#include "ui_updater.h"

Updater::Updater(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::Updater)

{
    ui->setupUi(this);
    mySettings::setTheme(mySettings::loadTheme());
    setWindowIcon(QIcon(":/icons/icon.ico"));
    this->setWindowFlags(Qt::CustomizeWindowHint);

//    urlLineEdit = new QLineEdit("http://qt-project.org/");


//    urlLabel = new QLabel(tr("&URL:"));
//    urlLabel->setBuddy(urlLineEdit);
//    statusLabel = new QLabel(tr("Please enter the URL of a file you want to "
//                                "download."));
//    statusLabel->setWordWrap(true);

//    downloadButton = new QPushButton(tr("Download"));
//    downloadButton->setDefault(true);
//    quitButton = new QPushButton(tr("Quit"));
//    quitButton->setAutoDefault(false);

//    buttonBox = new QDialogButtonBox;
//    buttonBox->addButton(downloadButton, QDialogButtonBox::ActionRole);
//    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

//    progressDialog = new QProgressDialog(this);

//    connect(urlLineEdit, SIGNAL(textChanged(QString)),
//            this, SLOT(enableDownloadButton()));

//    connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelDownload()));
//    connect(downloadButton, SIGNAL(clicked()), this, SLOT(downloadFile()));
//    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

//    QHBoxLayout *topLayout = new QHBoxLayout;
//    topLayout->addWidget(urlLabel);
//    topLayout->addWidget(urlLineEdit);

//    QVBoxLayout *mainLayout = new QVBoxLayout;
//    mainLayout->addLayout(topLayout);
//    mainLayout->addWidget(statusLabel);
//    mainLayout->addWidget(buttonBox);
//    setLayout(mainLayout);

//    setWindowTitle(tr("HTTP"));
//    urlLineEdit->setFocus();
    QTimer::singleShot(500, this, SLOT(downloadFile()));

}

Updater::~Updater()
{

        delete ui;
}

void Updater::closeEvent(QCloseEvent *event)
{

}



void Updater::startRequest(QUrl url)
{
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(updateDataReadProgress(qint64,qint64)));
}

void Updater::downloadFile()
{
    QString urlString = "https://cheytacllc.github.io/GUI-for-GoodbyeDPI/version.html";
    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setUrl(QUrl(urlString));
    QNetworkReply *response = manager.get(request);
    QEventLoop event;
    connect(response, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();
    QString content = response->readAll();

    if(content.trimmed()!=mySettings::readSettings("Version").toString())
    {
        url = QUrl("https://github.com/cheytacllc/GUI-for-GoodbyeDPI/releases/download/"+content.trimmed()+"/GoodByeDPI_GUI.exe");

        QFileInfo fileInfo(url.path());
        QString fileName = fileInfo.fileName();
        if (fileName.isEmpty())
            fileName = "index.html";

        if (QFile::exists(fileName)) {

            //QFile::remove(mySettings::readSettings("Dir").toString()+"/GoodByeDPI_GUI_old.exe");
            QFile::rename(mySettings::readSettings("Dir").toString()+"/GoodByeDPI_GUI.exe",mySettings::readSettings("Dir").toString()+"/GoodByeDPI_GUI_"+mySettings::readSettings("Version").toString()+".exe");
        }

        file = new QFile(mySettings::readSettings("Dir").toString()+'/'+fileName);
        if (!file->open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("HTTP"),
                                     tr("Unable to save the file %1: %2.")
                                         .arg(fileName).arg(file->errorString()));
            delete file;
            file = 0;
            return;
        }

        //    progressDialog->setWindowTitle(tr("HTTP"));
        //    progressDialog->setLabelText(tr("Downloading %1.").arg(fileName));
        //    downloadButton->setEnabled(false);

        // schedule the request
        httpRequestAborted = false;
        startRequest(url);
    }

    else if(content.trimmed()==mySettings::readSettings("Version").toString())
    {
        QMessageBox::information(this,"Information","You are using the last version");
        QApplication::quit();
    }

    else
    {
        QMessageBox::critical(this,"Error","Network error");
        QApplication::quit();
    }


}

void Updater::cancelDownload()
{
    statusLabel->setText(tr("Download canceled."));
    httpRequestAborted = true;
    reply->abort();
//    downloadButton->setEnabled(true);
}

void Updater::httpFinished()
{
    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
//        progressDialog->hide();
        return;
    }


//    progressDialog->hide();
    file->flush();
    file->close();
    QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(returnApp()));
        timer->start(1000);


    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        file->remove();
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Download failed: %1.")
                                 .arg(reply->errorString()));
//        downloadButton->setEnabled(true);
    } else if (!redirectionTarget.isNull()) {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
            url = newUrl;
            reply->deleteLater();
            file->open(QIODevice::WriteOnly);
            file->resize(0);
            startRequest(url);
            return;
        }
     else {
        return;
//        statusLabel->setText(tr("Downloaded %1 to %2.").arg(fileName).arg(QDir::currentPath()));
//        downloadButton->setEnabled(true);
    }

    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;



}

void Updater::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (file)
        file->write(reply->readAll());
}

void Updater::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    if (httpRequestAborted)
        return;

    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesRead);
}

void Updater::enableDownloadButton()
{
    //    downloadButton->setEnabled(!urlLineEdit->text().isEmpty());
}

void Updater::returnApp()
{
    if(ui->progressBar->value()==ui->progressBar->maximum()&&file->size()>0)
    {
        if(file->size()>2048)
            QFile::rename(mySettings::readSettings("Dir").toString()+"/GoodByeDPI_GUI.exe",mySettings::readSettings("Dir").toString()+"/GoodByeDPI_GUI_sfx.exe");

        if(QFile::exists("GoodByeDPI_GUI_sfx.exe"))
            QProcess::startDetached(mySettings::readSettings("Dir").toString()+"/GoodByeDPI_GUI_sfx.exe");
        else
            QProcess::startDetached(mySettings::readSettings("Dir").toString()+"/GoodByeDPI_GUI.exe");
        this->close();
    }

}
/*
#ifndef QT_NO_SSL
void Updater::sslErrors(QNetworkReply*,const QList<QSslError> &errors)
{
    QString errorString;
    foreach (const QSslError &error, errors) {
        if (!errorString.isEmpty())
            errorString += ", ";
        errorString += error.errorString();
    }

    if (QMessageBox::warning(this, tr("HTTP"),
                             tr("One or more SSL errors has occurred: %1").arg(errorString),
                             QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {
        reply->ignoreSslErrors();
    }
}
#endif
*/
