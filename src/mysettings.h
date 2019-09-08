#ifndef MYSETTINGS_H
#define MYSETTINGS_H
/*This class can r/w settings, set themes*/
/************created by m231**************/
/**************m231@tnctr.com*************/

#include <QApplication>
#include <QStyleFactory>
#include <QStyle>
#include <QSettings>
#include <QDesktopWidget>
#include <QToolTip>


class mySettings
{
public:
    enum Theme
    {
        vista=0, classic=1, lightFusion=2, darkFusion=3,
    };

    static int loadTheme()
    {
        int theme;
        QSettings s("HexOpenSource", "GBDPI-GUI");  // ayarlar registryde saklanmasi icin -> QSettings s("m231",QApplication::applicationName());
        theme=s.value("System/Skin",lightFusion).toInt(); //default theme is standard system theme
        s.endGroup();
        return theme;
    }

    static void setTheme(int theme)
    {
        QSettings s("HexOpenSource", "GBDPI-GUI");  // ayarlar registryde saklanmasi icin -> QSettings s("m231",QApplication::applicationName());
        s.setValue("System/Skin",theme);
        s.endGroup();

        switch (theme)
        {
        case vista:
        {
            qApp->setStyle(QStyleFactory::create("windowsvista"));
            qApp->setPalette(qApp->style()->standardPalette());
            QToolTip::setPalette(qApp->style()->standardPalette());
            break;
        }
        case classic:
        {
            qApp->setStyle(QStyleFactory::create("windows"));
            qApp->setPalette(qApp->style()->standardPalette());
            QToolTip::setPalette(qApp->style()->standardPalette());
            break;
        }
        case lightFusion:
        {
            qApp->setStyle(QStyleFactory::create("Fusion"));
            qApp->setPalette(qApp->style()->standardPalette());
            QToolTip::setPalette(qApp->style()->standardPalette());
            break;
        }
        case darkFusion:
        {
            qApp->setStyle(QStyleFactory::create("Fusion"));
            QPalette darkPalette;
            darkPalette.setColor(QPalette::Window, QColor(53,53,53));
            darkPalette.setColor(QPalette::WindowText, Qt::white);
            darkPalette.setColor(QPalette::Base, QColor(25,25,25));
            darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
            darkPalette.setColor(QPalette::ToolTipBase, QColor(53,53,53));
            darkPalette.setColor(QPalette::ToolTipText, Qt::white);
            darkPalette.setColor(QPalette::Text, Qt::white);
            darkPalette.setColor(QPalette::Button, QColor(53,53,53));
            darkPalette.setColor(QPalette::ButtonText, Qt::white);
            darkPalette.setColor(QPalette::BrightText, Qt::red);
            darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
            darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
            darkPalette.setColor(QPalette::HighlightedText, Qt::black);
            darkPalette.setColor(QPalette::Disabled,QPalette::Text,QColor(164,166,168));
            darkPalette.setColor(QPalette::Disabled,QPalette::WindowText,QColor(164,166,168));
            darkPalette.setColor(QPalette::Disabled,QPalette::ButtonText,QColor(164,166,168));
            darkPalette.setColor(QPalette::Disabled,QPalette::HighlightedText,QColor(164,166,168));
            darkPalette.setColor(QPalette::Disabled,QPalette::Base,QColor(68,68,68));
            darkPalette.setColor(QPalette::Disabled,QPalette::Window,QColor(68,68,68));
            darkPalette.setColor(QPalette::Disabled,QPalette::Highlight,QColor(68,68,68));
            QToolTip::setPalette(darkPalette);
            qApp->setPalette(darkPalette);
            break;
            }
        default:
            break;
        }
    }

    template <class T>
    static void writeSettings(QString key, T option)
    {
        QSettings s("HexOpenSource", "GBDPI-GUI");  // ayarlar registryde saklanmasi icin -> QSettings s("m231",QApplication::applicationName());
        s.setValue(key, option);
        s.endGroup();
    }

    static QVariant readSettings(QString key)
    {
        QVariant val;
        QSettings s("HexOpenSource", "GBDPI-GUI");  // ayarlar registryde saklanmasi icin -> QSettings s("m231",QApplication::applicationName());
        val=s.value(key);
        s.endGroup();
        return val;
    }
};


#endif // MYSETTINGS_H
