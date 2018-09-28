#include "about.h"
#include "ui_about.h"
#include <QLabel>
#include <QDesktopServices>
#include <QUrl>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle("Hakkında");

    QPixmap pix(":/images/images/icon.png");
    ui->iconLabel->setPixmap(pix);
    ui->iconLabel->setScaledContents(true);
    ui->iconLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    ui->aboutLabel->setTextFormat(Qt::RichText);
    ui->aboutLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    //ui->aboutLabel->setOpenExternalLinks(true);

}

About::~About()
{
    delete ui;
}


void About::on_aboutLabel_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link, QUrl::TolerantMode));
    if(link=="qt")
        QApplication::aboutQt();

}
