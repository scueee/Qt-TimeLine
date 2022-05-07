#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editwin.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->scrollAreaWidgetContents->installEventFilter(this);
    ui->widget_main->setStyleSheet(
                "QPushButton{border:0px;background:rgba(250, 250, 250, 255);border-radius:10px;}"
                "QPushButton:hover{border:1px dashed;background:rgba(244, 244, 244, 255);}"
                "QPushButton:pressed{background:rgba(240, 240, 240, 255);}");
    xPos.append(0);
    yPos.append(0);
    ini = new QSettings("config.ini",QSettings::IniFormat);
    ui->btn_row->setFixedSize(size,size);
    ui->btn_col->setFixedSize(size,size);
    // init btn
    btn_init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event){
    if(obj == ui->scrollAreaWidgetContents){
        // 实现鼠标拖拽移动视图
        if(event->type() == QEvent::MouseMove){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(last_move[0]==0)last_move[0]=mouseEvent->pos().x();
            if(last_move[1]==0)last_move[1]=mouseEvent->pos().y();
            QScrollBar *hBar = ui->scrollArea->horizontalScrollBar();
            QScrollBar *vBar = ui->scrollArea->verticalScrollBar();
            hBar->setValue(hBar->value()-(mouseEvent->pos().x()-last_move[0])/2);
            vBar->setValue(vBar->value()-(mouseEvent->pos().y()-last_move[1])/2);
            last_move[0] = mouseEvent->pos().x();
            last_move[1] = mouseEvent->pos().y();
            return true;
        }else if(event->type() == QEvent::MouseButtonRelease){
            last_move[0] = 0;
            last_move[1] = 0;
            return true;
        }else{
            return false;
        }
    }else{
        return QMainWindow::eventFilter(obj, event);
    }
}

void MainWindow::closeEvent(QCloseEvent *event){
    ini->setValue("Width",vector_max_plus1(xPos));
    ini->setValue("Height",vector_max_plus1(yPos));
    event->accept();
}

void MainWindow::btn_init(){
    if(ini->contains("Width")&&ini->contains("Height")){
        int w = ini->value("Width").toInt();
        int h = ini->value("Height").toInt();
        for(int i=0;i<w;i++){
            for(int j=0;j<h;j++){
                if(i!=0||j!=0)creat_btn(i,j);
                QString key = QString{"%1%2"}.arg(i).arg(j);
                if(ini->value(key+"/Hide").toBool())btnVec.last()->hide();
                if(ini->contains(key+"/Title"))btnVec.last()->setText(ini->value(key+"/Title").toString());
            }
        }
        ui->scrollAreaWidgetContents->setMinimumSize(
                    (int(scale*(vector_max_plus1(xPos)+4)*size)),
                    int(scale*(vector_max_plus1(yPos)+4)*size));
    }
}

void MainWindow::btn_card_clicked(const int *pos){
    EditWin *newWin = new EditWin;
    now_pos[0] = pos[0];
    now_pos[1] = pos[1];
    newWin->setPos(pos);
    connect(newWin,SIGNAL(sendTitle(QString)),this,SLOT(getTitle(QString)));
    newWin->show();
}

void MainWindow::btn_contextMenu_requested(const int *pos)
{
    int n = 0;
    for(int i=0;i<btnVec.size();i++){
        if(xPos[i+1]==pos[0] && yPos[i+1]==pos[1]){
            n = i;
        }
    }
    QMenu *cmenu = new QMenu(btnVec[n]);
    QAction *act_edit = cmenu->addAction("编辑");
    QAction *act_hide = cmenu->addAction("隐藏");
    connect(act_edit,&QAction::triggered,[=]{btn_card_clicked(pos);});
    connect(act_hide,&QAction::triggered,[=]{
        btnVec[n]->hide();
        QString key = QString{"%1%2/Hide"}.arg(pos[0]).arg(pos[1]);
        ini->setValue(key,true);
    });
    cmenu->exec(QCursor::pos());//呼出菜单
}

void MainWindow::getTitle(QString title){
    for(int i=0;i<btnVec.size();i++){
        if(xPos[i+1]==now_pos[0] && yPos[i+1]==now_pos[1]){
            btnVec[i]->setText(title);
        }
    }
}

void MainWindow::on_btn_row_clicked(){
    int col = vector_max_plus1(xPos);
    int row = vector_max_plus1(yPos);
    for(int i=0;i<col;i++){
        creat_btn(i,row);
    }
    ui->scrollAreaWidgetContents->setMinimumHeight(int(scale*(row+4)*size)); // 更新scrollArea高度
    update_background(); // 重绘背景
}

void MainWindow::on_btn_col_clicked(){
    int col = vector_max_plus1(xPos);
    int row = vector_max_plus1(yPos);
    for(int i=0;i<row;i++){
        creat_btn(col,i);
    }
    ui->scrollAreaWidgetContents->setMinimumWidth(int(scale*(col+4)*size)); // 更新scrollArea宽度
    update_background(); // 重绘背景
}

int MainWindow::vector_max_plus1(QVector<int> vec){
    QVector<int>::iterator max = std::max_element(std::begin(vec), std::end(vec));
    return (*max) + 1;
}

void MainWindow::creat_btn(int x,int y){
    xPos.append(x);yPos.append(y);
    btnVec.push_back(new QPushButton);
    int pos[2] = {x,y};
    btnVec.last()->setText("UNTITLE");
    btnVec.last()->setFixedSize(int(size*scale),int(size*scale));
    btnVec.last()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(btnVec.last(),&QPushButton::clicked,[=]{btn_card_clicked(pos);});
    connect(btnVec.last(),&QPushButton::customContextMenuRequested,[=]{btn_contextMenu_requested(pos);});
    ui->gridLayout->addWidget(btnVec.last(),y,x);    
    // read ini
    QString key = QString{"%1%2"}.arg(pos[0]).arg(pos[1]);
    if(ini->contains(key)){
        btnVec.last()->setText(ini->value(key+"/Title").toString());
    }
    if(!ini->value(key+"/Hide").toBool())ini->setValue(key+"/Hide",false);
}

void MainWindow::on_horizontalSlider_valueChanged(int value){
    scale = 1 + (value-50)/100.0;
    QSize btn_size = QSize(int(size*scale),int(size*scale));
    for(int i=0;i<btnVec.size();i++){
        btnVec[i]->setFixedSize(btn_size);
    }
    ui->btn_row->setFixedSize(btn_size);
    ui->btn_col->setFixedSize(btn_size);
    // update scrollArea size
    ui->scrollAreaWidgetContents->setMinimumSize(
                (int(scale*(vector_max_plus1(xPos)+4)*size)),
                int(scale*(vector_max_plus1(yPos)+4)*size));
    update_background();
}

void MainWindow::on_btn_setting_clicked(){
    qDebug()<<"setting";
}

void MainWindow::on_btn_show_clicked(){
    for(int i=0;i<btnVec.size();i++){
        btnVec[i]->show();
    }
    QStringList key_list = ini->allKeys();
    foreach(QString key,key_list){
        if(key.contains("/Hide"))ini->setValue(key,false);
    }
}

void MainWindow::update_background(){
    ui->widget_main->size = size;
    ui->widget_main->scale = scale;
    ui->widget_main->row = vector_max_plus1(yPos);
    ui->widget_main->update();

}

void MainWindow::on_actionExport_triggered(){
    QString file;
    QString path = QFileDialog::getSaveFileName(this, tr("保存INI文件"), "./", tr("File(*.ini)"), &file, QFileDialog::ShowDirsOnly);
    QFile::copy("./config.ini",path);
}

void MainWindow::on_actionOpen_triggered(){
    QString file;
    QString path = QFileDialog::getOpenFileName(this, tr("打开INI文件"), "./", tr("File(*.ini)"), &file, QFileDialog::ShowDirsOnly);
    on_actionNew_triggered();
    QFile::copy(path,"./config.ini");
    btn_init();
}

void MainWindow::on_actionNew_triggered(){
    for(QPushButton *btn:btnVec){
        ui->widget_main->layout()->removeWidget(btn);
        btn->setParent(nullptr);
        btn->deleteLater();
    }
    xPos.clear();
    yPos.clear();
    xPos.append(0);
    yPos.append(0);
    btnVec.clear();
    QFile::remove("./config.ini");
}
