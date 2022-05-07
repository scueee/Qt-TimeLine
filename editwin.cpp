#include "editwin.h"
#include "ui_editwin.h"

EditWin::EditWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditWin)
{
    ui->setupUi(this);
    ini = new QSettings("config.ini",QSettings::IniFormat);
}

EditWin::~EditWin()
{
    delete ui;
}

void EditWin::on_lineEdit_textChanged(const QString &text){
    emit sendTitle(text);
}

void EditWin::closeEvent(QCloseEvent *event){
    QString key = QString{"%1%2"}.arg(pos[0]).arg(pos[1]);
    ini->setValue((key+"/Title"),ui->lineEdit->text());
    ini->setValue((key+"/Content"),ui->plainTextEdit->toPlainText());
    event->accept();
}

void EditWin::setPos(const int *Pos){
    pos[0] = Pos[0];
    pos[1] = Pos[1];
    QString key = QString{"%1%2"}.arg(pos[0]).arg(pos[1]);
    if(ini->contains(key+"/Title")){
        ui->lineEdit->setText(ini->value(key+"/Title").toString());
    }
    if(ini->contains(key+"/Content")){
        ui->plainTextEdit->setPlainText(ini->value(key+"/Content").toString());
    }
}
