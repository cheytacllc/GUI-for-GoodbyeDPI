#ifndef UPDATER_H
#define UPDATER_H

#include <QtWidgets>
#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QProgressDialog>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include "mysettings.h"


//class QDialogButtonBox;
//class QFile;
//class QLabel;
//class QLineEdit;
//class QProgressDialog;
//class QPushButton;
//class QSslError;
//class QAuthenticator;
//class QNetworkReply;

namespace Ui {
class Updater;
}

class Updater : public QDialog
{
    Q_OBJECT

public:
    explicit Updater(QWidget *parent = 0);
    ~Updater();
    void closeEvent(QCloseEvent *event);
    void startRequest(QUrl url);
    QProcess *prog;

private slots:
    void downloadFile();
    void cancelDownload();
    void httpFinished();
    void httpReadyRead();
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
    void enableDownloadButton();
    void returnApp();
   // void slotAuthenticationRequired(QNetworkReply*,QAuthenticator *);
#ifndef QT_NO_SSL
    //void sslErrors(QNetworkReply*,const QList<QSslError> &errors);
#endif

private:
    QLabel *statusLabel;
    QLabel *urlLabel;
    QLineEdit *urlLineEdit;
    QProgressDialog *progressDialog;
    QPushButton *downloadButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;
    //Updater *ui;
    Ui::Updater *ui;

};

#endif
