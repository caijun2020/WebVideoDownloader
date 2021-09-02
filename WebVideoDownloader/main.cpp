#include "MainWindow.h"

#include <QApplication>
#include <QTextCodec>
#include <QDesktopWidget>

#define USE_SYS_QTEXTCODEC


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef USE_SYS_QTEXTCODEC
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);

#if 1
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
#else
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
#endif

#endif

    MainWindow w;
    w.show();
    return a.exec();
}
