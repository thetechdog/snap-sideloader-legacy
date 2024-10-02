#ifndef SIDELOADER_H
#define SIDELOADER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Sideloader;
}
QT_END_NAMESPACE

class Sideloader : public QWidget
{
    Q_OBJECT

public:
    Sideloader(QWidget *parent = nullptr);
    ~Sideloader();
    void setStatus(int status);
    void whoopsie();
    std::string getName();
    std::string getSummary();
    std::string getVersion();
    std::string getLicense();
    std::string getDescription();
    std::string getPath();
    std::string pipeTheCommand(std::string command);

private slots:
    void on_ActionButton_clicked();


    void on_RemoveButton_clicked();

    void on_About_clicked();

private:
    Ui::Sideloader *ui;
    int status=0;
    std::string comresult="";
    std::string path;
};
#endif // SIDELOADER_H
