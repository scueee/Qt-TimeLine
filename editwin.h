#ifndef EDITWIN_H
#define EDITWIN_H

#include <QWidget>
#include <QDebug>
#include <QSettings>

namespace Ui {
class EditWin;
}

class EditWin : public QWidget
{
    Q_OBJECT

public:
    explicit EditWin(QWidget *parent = nullptr);
    ~EditWin();
    void setPos(int const[2]);

private:
    Ui::EditWin *ui;
    QSettings *ini;
    int pos[2];

private slots:
    void on_lineEdit_textChanged(const QString &text);

protected:
    void closeEvent(QCloseEvent *);

signals:
    void sendTitle(QString);
};

#endif // EDITWIN_H
