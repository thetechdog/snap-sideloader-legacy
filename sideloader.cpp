#include "sideloader.h"
#include "./ui_sideloader.h"
#include <string>
#include <cstdio>
#include <QMessageBox>
#include <QFileDialog>
#include <cstdlib>
#include <memory>
//#include <QProcess>
//#include <QThread>


using namespace std;
//int status=0;
Sideloader::Sideloader(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Sideloader)
{
    ui->setupUi(this);
}

Sideloader::~Sideloader()
{
    delete ui;
}
//0-unloaded
//1-armed
//2-installed
void Sideloader::on_ActionButton_clicked()
{   if (status==0){
        //ui->RemoveButton->setEnabled(true); to willingly break program

        try {

            string user="";
            user=getenv("USER");
            QString filepath=QFileDialog::getOpenFileName(NULL, "Select a package file","/home/"+QString::fromStdString(user),"Snap package (*.snap)");
            path="snap info "+filepath.toStdString();
            comresult=pipeTheCommand(path);//DO NOT EDIT

            //cout<<comresult;
            //QMessageBox::warning(this, "info",QString::fromStdString(comresult ));
            ui->PackName->setText(QString::fromStdString(getName()));
            ui->ActionButton->setText("Install Package");
            ui->Summary->setText(QString::fromStdString(getSummary()));
            ui->Version->setText("Version: "+QString::fromStdString(getVersion()));
            ui->License->setText("License: "+QString::fromStdString(getLicense()));
            ui->Description->setPlainText(QString::fromStdString(getDescription()));
            ui->Path->setText("Path: "+QString::fromStdString(getPath()));

            //check if package is already installed and set to status 2 instead

            string check="[ -f /snap/bin/"+Sideloader::getName()+" ] && echo 0 || echo 1";
            string result = pipeTheCommand(check);
            if (result.compare("0\n")==0){
                ui->ActionButton->setText("Reinstall");
                ui->RemoveButton->setEnabled(true);
                status=2;
            }
            else status=1;
        } catch (...) {
            QMessageBox::critical(this,"Error","An error occurred");
        }
        return;
    }
    if (status==1){
        try {
            string actualPath=Sideloader::getPath();
            string install="pkexec snap install "+actualPath+" --dangerous";
            //QMessageBox* installBox=new QMessageBox(this);
            /*installBox->setText("Installing package...");
            installBox->setStandardButtons(QMessageBox::Ok);
            installBox->setWindowTitle("WORK IN PROGRESS");
            installBox->setIcon(QMessageBox::Warning);
            installBox->show();
            installBox->raise();
            installBox->activateWindow();
            QThread* installThread=new QThread;
            QObject::connect(installThread, &QThread::finished, [installBox]()){
                installBox->close();
            }
            //will have to move install task to a separate thread, but I don't want to bang my head against a wall for now*/
            QMessageBox::information(this,"WIP","Package will install. Please be Patient.");
            int result = system(install.c_str());
            //QThread::sleep(1);
            if (result == 0) {

                ui->ActionButton->setText("Reinstall");
                ui->RemoveButton->setEnabled(true);
                status=2;
                QMessageBox::information(this,"Success","Installation complete!");
            } else {
                QMessageBox::critical(this,"Error","Installation failed!");
            }

        } catch (...) {
            QMessageBox::critical(this,"Error","An error occurred");
        }
        return;
    }

    if(status==2){
        try {
            string actualPath=Sideloader::getPath();
            QMessageBox::information(this,"WIP","Package will be removed and then reinstalled.");
            string reinstall="pkexec snap remove "+Sideloader::getName();
            int result = system(reinstall.c_str());
            //QThread::sleep(1);
            if (result == 0) {

                ui->ActionButton->setText("Install package");
                ui->RemoveButton->setEnabled(false);
                status=1;
                QMessageBox::information(this,"Success","Package will now be reinstalled.");
                reinstall="pkexec snap install "+actualPath+" --dangerous";
                result = system(reinstall.c_str());
                //QThread::sleep(1);
                if (result==0){
                    QMessageBox::information(this,"Success","Reinstallation complete!");
                    ui->ActionButton->setText("Reinstall");
                    ui->RemoveButton->setEnabled(true);
                    status=2;
                }
                else{
                    QMessageBox::critical(this,"Error","Reinstallation failed!");

                }

            } else {
                QMessageBox::critical(this,"Error","Removal failed!");
            }

        } catch (...) {
            QMessageBox::critical(this,"Error","An error occurred");
        }
        return;


    }

}



void Sideloader::setStatus(int status){
    this->status=status;
}

string Sideloader::getName(){
    int location=comresult.find("name:");
    string name=comresult.substr(location);
    name=name.substr(name.find(":")+1);
    int i;
    for(i=0;i<name.length() && name[i]==' ';i++){
    }
    name=name.substr(i);
    for(i=0;i<name.length();i++){
        if(name[i]=='\n') break;
    }
    name=name.substr(0,i);
    //cout<<name;
    return name;
}

string Sideloader::getSummary(){
    int location=comresult.find("summary:");
    string summary=comresult.substr(location);
    summary=summary.substr(summary.find(":")+1);
    int i;
    for(i=0;i<summary.length() && summary[i]==' ';i++){
    }
    summary=summary.substr(i);
    for(i=0;i<summary.length();i++){
        if(summary[i]=='\n') break;
    }
    summary=summary.substr(0,i);
    //cout<<summary;
    return summary;
}

string Sideloader::getVersion(){
    int location=comresult.find("version:");
    string version=comresult.substr(location);
    version=version.substr(version.find(":")+1);
    int i;
    for(i=0;i<version.length() && version[i]==' ';i++){
    }
    version=version.substr(i);
    for(i=0;i<version.length();i++){
        if(version[i]=='\n') break;
    }
    version=version.substr(0,i);
    version=version.substr(0,version.find_last_of('-'));
    //cout<<version;
    return version;
}

string Sideloader::getLicense(){
    int location=comresult.find("license:");
    string license=comresult.substr(location);
    license=license.substr(license.find(":")+1);
    int i;
    for(i=0;i<license.length() && license[i]==' ';i++){
    }
    license=license.substr(i);
    for(i=0;i<license.length();i++){
        if(license[i]=='\n') break;
    }
    license=license.substr(0,i);
    //cout<<license;
    return license;
}

string Sideloader::getDescription(){
    int location=comresult.find("description:");
    string description=comresult.substr(location);
    description=description.substr(description.find(":")+1);
    int i;
    for(i=0;i<description.length() && (description[i]==' ' || description[i]=='|');i++){
    }
    description=description.substr(i);
    description=description.substr(0,description.find("\ncommands:"));
    return description;
}

string Sideloader::getPath(){
    int location=comresult.find("path:");
    string path=comresult.substr(location);
    path=path.substr(path.find(":")+1);
    int i;
    for(i=0;i<path.length() && path[i]==' ';i++){
    }
    path=path.substr(i);
    for(i=0;i<path.length();i++){
        if(path[i]=='\n') break;
    }
    path=path.substr(0,i);
    //cout<<path;
    return path;
}

void Sideloader::whoopsie(){
    QMessageBox::warning(nullptr,"Aw, Snap!","If you see this then you were able to do something you shouldn't have been able to and this is a bug... Either that or snapd is not installed or is inaccessible. Now the program will close.");
    exit(1);
}

void Sideloader::on_RemoveButton_clicked()
{   try {
        if (status==2){//check validity
            string actualPath=Sideloader::getPath();
            QMessageBox::information(this,"WIP","Package and its data will be removed.");
            string purge="pkexec snap remove --purge "+Sideloader::getName();
            int result = system(purge.c_str());
            //QThread::sleep(1);
            if (result == 0) {

                ui->ActionButton->setText("Install Package");
                ui->RemoveButton->setEnabled(false);
                status=1;
                QMessageBox::information(this,"Success","Package removal complete!");

            }
            else{
                QMessageBox::critical(this,"Error","Removal failed!");
            }

        }
        else{
            whoopsie();
        }
    } catch (...) {
        QMessageBox::critical(this,"Error","An error occurred");
    }
    return;
}

string Sideloader::pipeTheCommand(string command){
    //buffer for storing command output lines
    array<char,256> buffer;
    string result="";
    //open pipe, run command, close pipe
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    //read from pipe into buffer line by line, then append to command result
    while(fgets(buffer.data(),buffer.size(),pipe.get())!=nullptr){
        result=result+buffer.data();
    }
    return result;
}

void Sideloader::on_About_clicked()
{
    QMessageBox::about(nullptr,"About program", "Snap Sideloader Beta v0.7\n(C) Andrei Ionel - 2024\nLicensed under the GPLv3 license.\nHomepage: https://github.com/thetechdog/snap-sideloader");
    //QMessageBox::aboutQt(nullptr,"About Qt");
}

