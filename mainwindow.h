#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <vector>


struct P
{
    int x;
    int y;
    P(int _x,int _y)
    {
        x = _x;
        y = _y;
    }
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void closeEvent(QCloseEvent *);

    P judgeRepeat(int i,int j);
    void judgeWin();
    void init();
    //判重，返回的P x分量为颜色标号，y分量为vector索引，c为被判重的颜色标号

public slots:
    void startGame5();

    void startGame6();

    void startGame7();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void author();

private:
    void startGame();//开始游戏
    void drawGrid();//绘制初始的界面
    void readin();//读入关卡信息
    void drawColorLine();//画出路径
    void drawColorRound();//画出圆形光圈
    void showInfo();//在底部显示当前状态

    int level;//关卡
    int n;//规模
    int num;//已占据格子数
    int flows;//已成功颜色数
    int totFlows;//总颜色数
    int interval;//方格宽度
    int mouse_x;
    int mouse_y;//鼠标的xy坐标
    int curColor;//当前选中的颜色
    int data[7][7];//表示每个格子的颜色 0为黑色无颜色

    QPoint pos[7][7];//存放方格左上角的坐标
    std::vector<int> x[8];//颜色路径的x分量
    std::vector<int> y[8];//颜色路径的y分量
    int win[8];//每种颜色是否连接完毕
    QColor transColor[8];//点颜色信息
    QColor transColorX[8];//透明圈颜色信息
    QColor transColorY[8];
    int g[8];//指示是否显示透明路线
    Ui::MainWindow *ui;
    QPainter *painter;
};

#endif // MAINWINDOW_H
