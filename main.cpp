#include "sideloader.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Sideloader_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    Sideloader w;
    w.show();
    std::string check="whereis snapd"; //snapd not installed
    std::string result=w.pipeTheCommand(check);

    if (result.compare("snapd:\n")==0){
        QMessageBox::information(nullptr,"Package manager not installed","Are you sure snapd is installed? You can't use this program without it!");
        w.whoopsie();
    }
    check="snap";
    int intresult = system(check.c_str());
    if(intresult==127 ||intresult==32512 || intresult==126 ||intresult==1) w.whoopsie(); //command not found or not executable or some other error
    return a.exec();
}
