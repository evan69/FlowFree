#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QPoint>
#include <QMessageBox>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <QSound>

#include <fstream>
#include <iostream>

const int size = 420;
const int leftside = 90;
const int rightside = 90;
const int up = 60;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    n = 1;
    level = 1;
    curColor = 0;
    interval = 420 / n;
    mouse_x = -1000;
    mouse_y = -1000;
    flows = 0;
    num = 0;
    readin();
    transColor[0] = QColor(0,0,0);//黑色
    transColor[1] = QColor(255,0,0);//红色
    transColor[2] = QColor(255,255,0);//黄色
    transColor[3] = QColor(0,255,0);//绿色
    transColor[4] = QColor(0,0,255);//蓝色
    transColor[5] = QColor(255,127,0);//橘色
    transColor[6] = QColor(0,255,255);//青色
    transColor[7] = QColor(160,32,240);//紫色
    transColorX[0] = QColor(0,0,0,160);//黑色
    transColorX[1] = QColor(255,0,0,160);//红色
    transColorX[2] = QColor(255,255,0,160);//黄色
    transColorX[3] = QColor(0,255,0,160);//绿色
    transColorX[4] = QColor(0,0,255,160);//蓝色
    transColorX[5] = QColor(255,127,0,160);//橘色
    transColorX[6] = QColor(0,255,255,160);//青色
    transColorX[7] = QColor(160,32,240,160);//紫色
    transColorY[0] = QColor(0,0,0,100);//黑色
    transColorY[1] = QColor(255,0,0,100);//红色
    transColorY[2] = QColor(255,255,0,100);//黄色
    transColorY[3] = QColor(0,255,0,100);//绿色
    transColorY[4] = QColor(0,0,255,100);//蓝色
    transColorY[5] = QColor(255,127,0,100);//橘色
    transColorY[6] = QColor(0,255,255,100);//青色
    transColorY[7] = QColor(160,32,240,100);//紫色
    for(int i = 1;i < 8;i++)
    {
        win[i] = 0;
    }//初始化每种颜色的胜利信息
    this->setFixedSize(this->width(),this->height());
    connect(ui->action5_5,SIGNAL(triggered(bool)),this,SLOT(startGame5()));
    connect(ui->action6_9,SIGNAL(triggered(bool)),this,SLOT(startGame6()));
    connect(ui->action7_9,SIGNAL(triggered(bool)),this,SLOT(startGame7()));
    connect(ui->actionAuthor,SIGNAL(triggered(bool)),this,SLOT(author()));
}

void MainWindow::init()
{
    curColor = 0;
    flows = 0;
    num = 0;
    mouse_x = -1000;
    mouse_y = -1000;
    for(int i = 0;i < 8;i++)
    {
        x[i].clear();
        y[i].clear();
    }
    for(int i = 0;i < 8;i++)
    {
        win[i] = 0;
        g[i] = 0;
    }
    readin();
}

MainWindow::~MainWindow()
{
    delete ui;
    //delete painter;
}

void MainWindow::startGame5()
{
    n = 5;
    level = 1;
    init();
    update();
}

void MainWindow::startGame6()
{
    n = 6;
    level = 1;
    init();
    update();
}

void MainWindow::startGame7()
{
    n = 7;
    level = 1;
    init();
    update();
}

void MainWindow::author()
{
    QMessageBox::information(0,"Author","Author:Evan",QMessageBox::Ok);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    //qDebug() << curColor;
    if(n == 1) return;
    painter = new QPainter(this);
    drawGrid();
    drawColorLine();
    drawColorRound();
    showInfo();
}

void MainWindow::readin()
{
    if(n == 1) return;
    QString file = QString::number(n);
    file += QString::number(level);
    //关卡文件的名称
    std::ifstream fin(file.toStdString().c_str());
    char t;
    fin >> t;
    totFlows = t - '0';
    for(int i = 0;i < n;i++)
    {
        for(int j = 0;j < n;j++)
        {
            char c;
            fin >> c;
            data[i][j] = c - '0';//读入颜色点位置
        }
    }
    fin.close();
}

void MainWindow::drawGrid()
{
    if(n == 1) return;
    painter->setBrush(Qt::black);
    painter->setPen(Qt::white);
    interval = 420 / n;
    for(int i = 0;i < n;i++)
    {
        for(int j = 0;j < n;j++)
        {
            pos[i][j].setX(leftside + j * interval + interval / 2);
            pos[i][j].setY(up + i * interval + interval / 2);
        }
    }

    //绘制黑色矩形作为游戏区域
    QPoint pointList[4] = {QPoint(leftside,up),
                             QPoint(600 - rightside,up),
                            QPoint(600 - rightside,up + 420),
                            QPoint(leftside,up + 420)};
    painter->drawPolygon(pointList,4);

    //绘制方格线
    for(int i = 1;i < n;i++)
    {
        painter->drawLine(QPoint(leftside,up+i*interval),
                          QPoint(leftside+420,up+i*interval));
    }
    for(int i = 1;i < n;i++)
    {
        painter->drawLine(QPoint(leftside+i*interval,up),
                          QPoint(leftside+i*interval,up+420));
    }

    //把存好的关卡在屏幕上显示
    for(int i = 0;i < n;i++)
    {
        for(int j = 0;j < n;j++)
        {
            painter->setPen(Qt::black);//黑色
            if(data[i][j] == 0) continue;
            painter->setBrush(transColor[data[i][j]]);
            painter->drawEllipse(pos[i][j],interval/3,interval/3);
            update();
        }
    }
}

void MainWindow::drawColorLine()
{
    for(int i = 1;i < 8;i++)
    {
        if(x[i].size() < 1) continue;
        painter->setPen(QPen(transColor[i],20,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
        for(int j = 0;j < x[i].size() - 1;j++)
        {
            painter->drawLine(pos[x[i][j]][y[i][j]],
                    pos[x[i][j+1]][y[i][j+1]]);
        }
        painter->setPen(QPen(transColorY[i],interval));

        for(int j = 0;j < x[i].size() - 1;j++)
        {
            if(g[i] == 1)
            painter->drawLine(pos[x[i][j]][y[i][j]],
                    pos[x[i][j+1]][y[i][j+1]]);
        }
    }
}

void MainWindow::drawColorRound()
{
    if(curColor == 0) return;
    painter->setBrush(transColorX[curColor]);
    painter->setPen(transColorX[curColor]);
    painter->drawEllipse(QPoint(mouse_x,mouse_y),interval/2,interval/2);
}

void MainWindow::showInfo()
{
    QString info = "size:" + QString::number(n);
    info += "       level:" + QString::number(level);
    info += "       flows:";
    info += QString::number(flows);
    info += "/";
    info += QString::number(totFlows);
    info += "       pipe:";
    info += QString::number(100 * num / (n * n));
    info += "%";
    this->ui->statusBar->showMessage(info,0);
}

void MainWindow::mousePressEvent(QMouseEvent * e)//按下鼠标的事件
{
    if(e->button() != Qt::LeftButton) return;
    mouse_x = e->x();
    mouse_y = e->y();
    for(int i = 0;i < n;i++)
    {
        for(int j = 0;j < n;j++)
        {
            if(mouse_x > (pos[i][j].x() - interval / 2)
                    && mouse_x < (pos[i][j].x() + interval / 2)
                    && mouse_y > (pos[i][j].y() - interval / 2)
                    && mouse_y < (pos[i][j].y() + interval / 2))
                {
                    P myP = judgeRepeat(i,j);//判断点到的点是否是某条管道的一部分
                    if(myP.x != 0)//如果是
                    {
                        curColor = myP.x;//设置当前颜色
                        int c = myP.y + 1;
                        while(x[curColor].size() > c)
                        {
                            x[curColor].pop_back();
                            y[curColor].pop_back();
                            win[curColor] = 1;
                        }//把这条管道之后的部分去掉
                        g[curColor] = 0;
                        judgeWin();
                        return;
                    }
                    if(data[i][j] == 0) return;//如果落点是一个黑色区域，不做操作
                    //落点是某个色点
                    curColor = data[i][j];
                    win[curColor] = 1;//激活
                    x[curColor].clear();
                    y[curColor].clear();
                    x[curColor].push_back(i);
                    y[curColor].push_back(j);
                    //加入到点列当中去
                    g[curColor] = 0;
                    repaint();
                    return;
                }

        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)//移动鼠标的事件
{
    //if(e->button() != Qt::LeftButton) return;
    if(curColor == 0) return;
    mouse_x = e->x();
    mouse_y = e->y();
    for(int i = 0;i < n;i++)
    {
        for(int j = 0;j < n;j++)
        {
            if(mouse_x > (pos[i][j].x() - interval / 2)
                    && mouse_x < (pos[i][j].x() + interval / 2)
                    && mouse_y > (pos[i][j].y() - interval / 2)
                    && mouse_y < (pos[i][j].y() + interval / 2))//定位鼠标所在区域
                {
                    if(i == x[curColor][x[curColor].size() - 1]
                            && j == y[curColor][y[curColor].size() - 1])
                        return;//如果没有移动鼠标到原区域外，无需响应
                    if(data[i][j] != curColor && data[i][j] > 0) return;
                    //不允许移动到其他颜色的点
                    P flag = judgeRepeat(i,j);
                    //获取新区域的信息，是否是已有路径的一部分
                    //x-路径颜色 y-格子位置的vec索引值
                    if(abs(x[curColor][x[curColor].size()-1]-i) +
                            abs(y[curColor][y[curColor].size()-1]-j) != 1) return;
                    //不连续则不允许切断管道
                    if(flag.x != 0)//如果移动到了已有的管道上
                    {
                        win[flag.x] = 1;
                        int tmp = x[flag.x].size();
                        if(flag.x == curColor || win[curColor] < 2)
                            //同种颜色自环要断掉管道，但若该颜色连接完成则不能再断开其他颜色管道
                        {
                            if(curColor != flag.x)
                            {
                                QSound::play(":/sound/forward.wav");
                            }
                            for(int u = flag.y;u < tmp;u = u + 1)
                            {
                                x[flag.x].pop_back();
                                y[flag.x].pop_back();
                            }//把重复的路径后半截去除，无论是同种还是不同种颜色
                        }
                        if(flag.x == curColor)//如果同种颜色构成了环
                        {
                            win[curColor] = 1;//需要把完成值设为1即未完成
                            if(x[curColor][0] == i &&
                                    y[curColor][0] == j)//如果回到了出发点
                            {
                                x[curColor].clear();
                                y[curColor].clear();
                                //该颜色管道清除
                                x[curColor].push_back(i);
                                y[curColor].push_back(j);
                                //重新加入该点坐标
                            }
                        }
                    }
                    if(win[curColor] < 2 &&
                            abs(x[curColor][x[curColor].size()-1]-i) +
                            abs(y[curColor][y[curColor].size()-1]-j) == 1)
                        //如果这种颜色的线还没有完成连接，并且相差1格
                    {
                        x[curColor].push_back(i);
                        y[curColor].push_back(j);
                        if(data[i][j] == curColor && x[curColor].size() > 1)
                        {
                           win[curColor] = 2;//完成这个折线段
                           QSound::play(":/sound/forward.wav");
                        }
                    }
                    repaint();                    
                    judgeWin();
                    return;
                }
        }
    }
}

P MainWindow::judgeRepeat(int r, int s)//r,s为被判重的方格的标号
//判重，返回的P x分量为颜色标号，y分量为vector索引
{
    P g = P(0,0);
    for(int i = 1;i < 8;i++)
    {
        for(int j = 0;j < x[i].size();j++)
        {
            if(x[i][j] == r && y[i][j] == s)
            {
                return P(i,j);
            }
        }
    }
    return g;
}

void MainWindow::judgeWin()
//判断是否胜利
{
    int tot1 = 0;
    int tot2 = 0;
    for(int i = 1;i < 8;i++)
    {
        tot1 = tot1 + x[i].size();
        if(win[i] == 2) tot2++;
    }
    num = tot1;
    flows = tot2;
    //更新num flow变量
    if(tot1 == n*n)//判断通关
    {
        QSound::play(":/sound/leak.wav");//播放胜利的音效
        int ret = QMessageBox::information(0,tr("Win"),tr("You have won the game\nTry next level!"),QMessageBox::Ok);
        ui->pushButton_2->click();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    g[curColor] = 1;
    curColor = 0;
    update();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::warning(0, tr("MainWindow"), tr("Do you really want to exit?"), QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        event->accept(); //确认关闭
    } else {
        event->ignore(); //不关闭
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(level == 1) return;
    level = 1;
    init();
    update();
}

void MainWindow::on_pushButton_2_clicked()
{
    if(level == 2) return;
    level = 2;
    init();
    update();
}

void MainWindow::on_pushButton_3_clicked()
{
    init();
    update();
}
