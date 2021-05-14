#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QWidget>

class graphics : public QWidget
{
    Q_OBJECT
public:
    explicit graphics(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    void drawBackground(QPainter& painter);
    void drawMegaman(QPainter& painter, int h);
    void drawPipe(QPainter& painter, int x);
    void drawMask(QPainter& painter, int x);
    void drawCoronaVirus(QPainter& painter, int x, int y);
    void drawLive(QPainter& painter);
    void drawStar(QPainter& painter, int x, int y);
    void drawShield(QPainter& painter);
    void drawBullet(QPainter& painter);

    QPointF quay(QPointF p, QPointF c, float huong);
    QPointF dichuyen(QPointF p, double kc, int huong);
    int random(int minN, int maxN);

    int megaman_pipe(int random_x_pipe);
    void setPoint();
    int megaman_mask(int random_mask);
    int megaman_corona(int random_x_corona, int random_y_corona);
    int bullet_corona(int pos_bullet, int const_y_bullet);

    // variable
        // chứa ảnh
    QImage imgBackground; // load trước để tăng tốc độ
    QImage imgMegaman; // load
    QImage imgPipe;
    QImage imgMask;
    QImage imgCoronaVirus;
    QImage imgShield;
        // hiển thị ảnh
    QPixmap background;
    QPixmap megaman;
    QPixmap pipe;
    QPixmap mask;
    QPixmap coronaVirus;
    QPixmap shield;

    int hinh;
    int timerId;
    int pos_bullet;
    int const_y_bullet;
    int pos_y_megaman;
    int random_x_pipe1;
    int random_x_pipe2;
    int random_x_pipe3;
    int random_mask;
    int random_x_corona;
    int random_y_corona;
    int point;
    int live;
    int checkLive1;
    int checkLive2;
    int checkLive3;

    int checkJump;
    int reverse;
    int checkShield;
    int timeShield ;
    int checkBullet;
    int valueProgess;
    int checkHit;

    
signals:
    
public slots:
    
};

#endif // GRAPHICS_H
