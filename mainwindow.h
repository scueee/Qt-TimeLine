#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include <QDebug>
#include <QSettings>
#include <QCloseEvent>
#include <QScrollBar>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    QSettings *ini;
    double scale = 1.0;
    int size = 80;
    QVector<QPushButton*> btnVec;
    QVector<int> xPos;
    QVector<int> yPos;
    int now_pos[2];
    int last_move[2] = {0,0};
    int vector_max_plus1(QVector<int>);
    void creat_btn(int,int);
    void update_background();

private slots:
    void btn_card_clicked(const int *);
    void btn_contextMenu_requested(const int *);
    void on_btn_row_clicked();
    void on_btn_col_clicked();
    void on_btn_setting_clicked();
    void on_btn_show_clicked();
    void on_horizontalSlider_valueChanged(int);
    void getTitle(QString);

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void closeEvent(QCloseEvent *) override;
};
#endif // MAINWINDOW_H
