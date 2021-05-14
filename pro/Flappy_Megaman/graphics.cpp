#include "graphics.h"
#include <QPainter>
#include <QDebug>
#include <QImage>
#include <math.h>
#include <QPen>
#include <windows.h>
#include <iostream>
#include <string>
#include <QMessageBox>

graphics::graphics(QWidget *parent) :
    QWidget(parent)
{
    imgBackground.load("../background_flappy.png");
    imgMegaman.load("../Megaman.png");
    imgPipe.load("../pipe.png");
    imgMask.load("../mask.png");
    imgCoronaVirus.load("../virus.png");
    imgShield.load("../shield.png");
    background = background.fromImage(imgBackground);
    megaman = megaman.fromImage(imgMegaman);
    pipe = pipe.fromImage(imgPipe);
    mask = mask.fromImage(imgMask);
    coronaVirus = coronaVirus.fromImage(imgCoronaVirus);
    shield = shield.fromImage(imgShield);
    hinh = 0;
    pos_y_megaman = 0;
    random_x_pipe1 = 0;
    random_x_pipe2 = 0;
    random_x_pipe3 = 0;
    random_mask = 0;
    random_x_corona = 0;
    random_y_corona = 0;
    point = -3;
    live = 3;
    checkLive1 = 0;
    checkLive2 = 0;
    checkLive3 = 0;
    checkJump = 0;
    reverse = 0;
    checkShield = 0;
    timeShield = 0;
    checkBullet = 0;
    pos_bullet = 0;
    const_y_bullet = 0;
    valueProgess = 0;
    checkHit = 0;
    timerId = startTimer(50);
}

void graphics::timerEvent(QTimerEvent *){
    if(checkJump == 1){ // khi sự kiện click jump
        pos_y_megaman += 5; // tăng biến y lên cho hàm draw megaman
        if(pos_y_megaman >= 120) reverse = 1; // lên rồi đảo ngược, y giảm
        if(reverse == 1) pos_y_megaman -= 10; // giảm phải nhanh hơn tăng
        if(pos_y_megaman <= 0){ // khi về ban đầu, nhảy lên rồi rơi xuống reset lại
            checkJump = 0;
            reverse = 0;
            pos_y_megaman = 0;
        }
//        qDebug()<<reverse;
    }
    if(checkShield == 1){
        timeShield ++;
        if(timeShield == 100){
            timeShield = 0;
            checkShield = 0;
        }
    }
    hinh ++;
    if(random_x_pipe1 <= 0){
        random_x_pipe1 = random(width()-50, width());
        setPoint();
    }
    if(random_x_pipe2 <= 0){
        random_x_pipe2 = random(width()+50, width()+200);
        setPoint();
    }
        // xử lý 2 pipe trùng của 1 và 2
    while(random_x_pipe2 == random_x_pipe1){
        random_x_pipe2 = random(width(), width()+200);
    }
    if(random_x_pipe3 <= 0){
        random_x_pipe3 = random(width()+200, width()+300);
        setPoint();
    }
        // xử lý 2 pipe trùng của 1 và 3, 2 và 3
    while(random_x_pipe3 == random_x_pipe1 || random_x_pipe3 == random_x_pipe2){
        random_x_pipe3 = random(width(), width()+300);
    }
    if(random_mask <= 0)    random_mask = random(width(), width()+100);
       // xử lý mask trùng với pipe 1, 2, 3
    while(random_mask == random_x_pipe1 || random_mask == random_x_pipe2 || random_mask == random_x_pipe3){
        random_mask = random(random_x_pipe1, random_x_pipe2);
    }
    if(random_x_corona <= 0) random_x_corona = random(width(), width()+100);
    if(random_y_corona >= height()) random_y_corona = 0;
    if(hinh==10)    hinh = 0;
    // check đụng độ giữa megaman và pipe
    if(megaman_pipe(random_x_pipe1) == 1){
        if(live==0){
            QMessageBox::information(this,"End Game!", "You Close! Congratulate your score is: " + QString::number(point) +
                                     " point\n \t@Copyright: DanhUy");
            exit(EXIT_FAILURE);
        }
        else checkLive1 = 1;
    }
    if(megaman_pipe(random_x_pipe2) == 1){
        if(live==0){
            QMessageBox::information(this,"End Game!", "You Close! Congratulate your score is: " + QString::number(point)
                                     + " point\n \t@Copyright: DanhUy");
            exit(EXIT_FAILURE);
        }
        else checkLive2 = 1;
    }
    if(megaman_pipe(random_x_pipe3) == 1){
        if(live==0){
            QMessageBox::information(this,"End Game!", "You Close! Congratulate your score is: " + QString::number(point)
                                     +" point\n \t@Copyright: DanhUy");
            exit(EXIT_FAILURE);
        }
        else checkLive3 = 1;
    }
    // check đụng độ giữa megaman và mask
    if(megaman_mask(random_mask) == 1){
        live++;
        random_mask = random(width(), width()+100);
    }
    if(checkLive1 == 1){
        live --;
        checkLive1 = 0;
        random_x_pipe1 = random(width()-50, width());
    }
    if(checkLive2 == 1){
        live --;
        checkLive2 = 0;
        random_x_pipe2 = random(width()+50, width()+180);
    }
    if(checkLive3 == 1){
        live --;
        checkLive3 = 0;
        random_x_pipe3 = random(width()+200, width()+300);
    }
    // check đụng độ giữa megaman và corona
    if(megaman_corona(random_x_corona, random_y_corona) == 1 && checkShield == 0){
        live--;
        if(live < 0){
            live = 0;
            QMessageBox::information(this,"End Game!", "You Close! Congratulate your score is: " + QString::number(point)
                                     +" point\n \t@Copyright: DanhUy");
            exit(EXIT_FAILURE);
        }
        random_x_corona = random(width()/2, width()+100);
        random_y_corona = 0;
    }
    // check đụng độ giữa bullet và corona
    if(bullet_corona(pos_bullet, const_y_bullet) == 1){
        checkHit = 1;
        random_x_corona = random(width()/2, width()+100);
        random_y_corona = 0;
    }
//    qDebug()<<point;
    repaint();
}

void graphics::paintEvent(QPaintEvent *){
    QPainter painter(this);
    // draw Background
    drawBackground(painter);
    // draw Megaman
    if(checkJump == 0)
        drawMegaman(painter,0); // y bằng 0, giữ nguyên vị trí
    else{
        drawMegaman(painter,pos_y_megaman);
//        qDebug()<<pos_y_megaman;
    }
    // draw Pipe
                        // tăng giảm tốc độ nhờ vào giảm đi biến random
    drawPipe(painter, random_x_pipe1); random_x_pipe1 -= 3;
    drawPipe(painter, random_x_pipe2); random_x_pipe2 -= 3;
    drawPipe(painter, random_x_pipe3); random_x_pipe3 -= 3;

    // draw Mask
    drawMask(painter,random_mask); random_mask -= 3;

    // draw Corona virus
    drawCoronaVirus(painter,random_x_corona,random_y_corona);
    random_x_corona -= 3;
    random_y_corona += 3;

    // draw text Live
    drawLive(painter);
    if(checkShield == 1)
        drawShield(painter);
    if(checkBullet == 1){
        drawBullet(painter);
        pos_bullet += 5;
        if(pos_bullet == width()) checkBullet = 0;
    }

}

int graphics::random(int minN, int maxN){
    return minN + rand() % (maxN + 1 - minN);
}

void graphics::drawBackground(QPainter &painter){
    int x = 0, y = 0; // góc tọa độ
    painter.drawPixmap(x, y, width(), height(), background);
}

void graphics::drawMegaman(QPainter &painter, int h){
    double wHinh = megaman.width() / 10;
    double hHinh = megaman.height();
    int x_megaman = 60;
    int d_flat = 60;
    // cắt ảnh
    QRect displayWindow(wHinh*hinh, 0,wHinh,hHinh);
    // vị trí hiển thị
    QRect pos(x_megaman,height() - d_flat - h,wHinh,hHinh);
//    painter.setPen(Qt::red);
//    painter.drawRect(x_megaman,height()-d_flat-h,33,40);
    painter.drawPixmap(pos,megaman,displayWindow);
}

void graphics::drawPipe(QPainter &painter, int x){
    int d_flat = 70;
    int wHinh = 50;
    int hHinh = 50;
    int x_pipe = x;
    int y_pipe = height() - d_flat;
//    painter.setPen(Qt::red);
//    painter.drawRect(x_pipe+17,y_pipe,15,50);
    painter.drawPixmap(x_pipe, y_pipe, wHinh, hHinh, pipe);
}

void graphics::drawMask(QPainter &painter, int x){
    int d_flat = 60;
    int wHinh = 50;
    int hHinh = 50;
    int x_mask = x;
    int y_mask = height() - d_flat;
//    painter.setPen(Qt::red);
//    painter.drawRect(x_mask+5,y_mask+10,40,30);
    painter.drawPixmap(x_mask, y_mask, wHinh, hHinh, mask);
}

void graphics::drawCoronaVirus(QPainter &painter, int x, int y){
    int wHinh = 100;
    int hHinh = 50;
    int x_corona = x;
    int y_corona = y;
//    painter.setPen(Qt::red);
//    painter.drawRect(x_corona+22,y_corona,55,50);
    painter.drawPixmap(x_corona, y_corona, wHinh, hHinh, coronaVirus);
}

void graphics::drawLive(QPainter &painter){
    int x = 10;
    int y = 20;
    int wText = 55;
    int hText = 7;
    // Set font chữ, cỡ chữ và màu cho painter
    QFont font = painter.font() ;
    font.setPointSize(12);
    font.setWeight(QFont::DemiBold);
    painter.setFont(font);
    painter.setPen(Qt::red);
    painter.drawText(x,y,"Live: ");
    // draw star
    drawStar(painter, x+wText, y-hText);
    // draw number
    painter.setPen(Qt::yellow);
    painter.drawText(x+wText+20, y, QString::number(live));
    painter.setPen(Qt::red);
    painter.drawText(x,y*2, "Score: ");
    painter.setPen(Qt::yellow);
    // covert
    painter.drawText(x+wText+20,y*2,QString::number(point));
}

QPointF graphics::quay(QPointF p, QPointF c, float deta) {
    QPointF pnew;
    double goc = M_PI * deta / 180;
    pnew.setX(c.x() + (p.x() - c.x()) * cos(goc) - (p.y() - c.y()) * sin(goc));
    pnew.setY(c.y() + (p.x() - c.x()) * sin(goc) + (p.y() - c.y()) * cos(goc));
    return pnew;
}

QPointF graphics::dichuyen(QPointF p, double kc, int huong){
    QPointF pnew;
    pnew.setX(p.x()+kc*cos(M_PI*huong/180));
    pnew.setY(p.y()+kc*sin(M_PI*huong/180));
    return pnew;
}

void graphics::drawStar(QPainter &painter, int x, int y){
    int d = 10;
    for(int i=d;i>=0;i--){
        QPolygonF poly;
        QPointF c(x, y);
        QPointF p(x, y-i);
        QPointF pnew;
        poly<<QPointF(p.x(),p.y());
        for(int i=1;i<=4;i++){
            pnew = quay(p,c,-i*72);
            poly<<QPointF(pnew.x(),pnew.y());
        }
        painter.setPen(Qt::red);
        painter.drawLine(poly.value(0),poly.value(2));
        painter.drawLine(poly.value(0),poly.value(3));
        painter.drawLine(poly.value(1),poly.value(3));
        painter.drawLine(poly.value(1),poly.value(4));
        painter.drawLine(poly.value(2),poly.value(4));
    }
}

/*--------------------Megaman--------------*/
   /*
       - position x: 60
       - position y: height() - 60 - pos_y_megaman;
       - width: 33
       - height: 40
       - middle_x: 60 + 16
       - middle_y = height() - 60 - pos_y_megaman + 20

   */

void graphics::drawShield(QPainter &painter){
    painter.drawPixmap(70, height()-60-pos_y_megaman-30, 80, 80, shield);
//    painter.setPen(Qt::red);
//    QPointF tt(70,height()-5-pos_y_megaman);
//    QPointF bd;
//    QPointF pnew;
//    QPolygonF poly;
//    bd = tt;
//    int d = 20;
//    int huong = 0;
//    poly<<bd;
//    for(int i=0;i<12;i++){
//        pnew = dichuyen(bd,d,huong);
//        poly<<pnew;
//        painter.drawLine(bd,pnew);
//        huong += -30;
//        bd = pnew;
//    }

//    for(int i=0; i<poly.count(); i++){
//        for(int j=0; j<poly.count(); j++){
//            painter.drawLine(poly.value(i), poly.value(j));
//        }
//    }
}

void graphics::drawBullet(QPainter &painter){
    QPoint center(pos_bullet, const_y_bullet);
//    painter.drawLine(pos_bullet, height()-40-pos_y_megaman, pos_bullet+20, height()-40-pos_y_megaman);
    painter.setBrush(Qt::red);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(center, 5, 5);
//        painter.setPen(Qt::yellow);
//        painter.drawRect(center.x()-5,center.y()-5,10,10);

}



void graphics::setPoint(){
    point ++;
}

// Xử lí đụng độ
 /*--------------------Megaman--------------*/
    /*
        - position x: 60
        - position y: height() - 60 - pos_y_megaman;
        - width: 33
        - height: 40

    */

/*--------------------Pipe 1--------------*/
   /*
       - position x: random_x_pipe1 + 17
       - position y: height() - 70;
       - width: 15
       - height: 50

   */

int graphics::megaman_pipe(int random_x_pipe){
    int xMegaman = 60;
    int yMegaman = height() - 60 - pos_y_megaman;
    int wMegaman = 33;
    int hMegaman = 40;
    int xPipe = random_x_pipe + 17;
    int yPipe = height() - 70;
    int wPipe = 15;
    int hPipe = 50;

    if((xMegaman+wMegaman <= xPipe+wPipe) && (xMegaman+wMegaman >= xPipe)){
        if((yMegaman+hMegaman <= yPipe+hPipe) && (yMegaman+hMegaman >= yPipe))  return 1;
        if((yMegaman <= yPipe+hPipe) && (yMegaman >= yPipe)) return 1;
    }
    if((xMegaman<= xPipe+wPipe) && (xMegaman >= xPipe)){
        if((yMegaman+hMegaman <= yPipe+hPipe) && (yMegaman+hMegaman >= yPipe))  return 1;
        if((yMegaman <= yPipe+hPipe) && (yMegaman >= yPipe)) return 1;
    }
}

/*--------------------Mask--------------*/
   /*
       - position x: random_mask + 5
       - position y: height() - 60 + 10;
       - width: 40
       - height: 30

   */
int graphics::megaman_mask(int random_mask){
    int xMegaman = 60;
    int yMegaman = height() - 60 - pos_y_megaman;
    int wMegaman = 33;
    int hMegaman = 40;
    int xMask = random_mask + 5;
    int yMask = height() - 50;
    int wMask = 40;
    int hMask = 30;

    if((xMegaman+wMegaman <= xMask+wMask) && (xMegaman+wMegaman >= xMask)){
        if((yMegaman+hMegaman <= yMask+hMask) && (yMegaman+hMegaman >= yMask))  return 1;
        if((yMegaman <= yMask+hMask) && (yMegaman >= yMask)) return 1;
    }
    if((xMegaman<= xMask+wMask) && (xMegaman >= xMask)){
        if((yMegaman+hMegaman <= yMask+hMask) && (yMegaman+hMegaman >= yMask))  return 1;
        if((yMegaman <= yMask+hMask) && (yMegaman >= yMask)) return 1;
    }
}


/*--------------------Corona--------------*/
   /*
       - position x: random_x_corona + 22
       - position y: random_y_corona
       - width: 55
       - height: 50

   */
int graphics::megaman_corona(int random_x_corona, int random_y_corona){
    int xMegaman = 60;
    int yMegaman = height() - 60 - pos_y_megaman;
    int wMegaman = 33;
    int hMegaman = 40;
    int xCorona = random_x_corona + 22;
    int yCorona = random_y_corona;
    int wCorona = 55;
    int hCorona = 50;

    if((xMegaman+wMegaman <= xCorona+wCorona) && (xMegaman+wMegaman >= xCorona)){
        if((yMegaman+hMegaman <= yCorona+hCorona) && (yMegaman+hMegaman >= yCorona))  return 1;
        if((yMegaman <= yCorona+hCorona) && (yMegaman >= yCorona)) return 1;
    }
    if((xMegaman<= xCorona+wCorona) && (xMegaman >= xCorona)){
        if((yMegaman+hMegaman <= yCorona+hCorona) && (yMegaman+hMegaman >= yCorona))  return 1;
        if((yMegaman <= yCorona+hCorona) && (yMegaman >= yCorona)) return 1;
    }
}

//     QPoint center(pos_bullet, const_y_bullet);
//        painter.setPen(Qt::yellow);
//        painter.drawRect(center.x()-5,center.y()-5,10,10);
/*--------------------Corona--------------*/
   /*
       - position x: pos_bullet - 5
       - position y: const_y_bullet - 5
       - width: 10
       - height: 10

   */
int graphics::bullet_corona(int pos_bullet, int const_y_bullet){
    int xBullet = pos_bullet - 5;
    int yBullet = const_y_bullet - 5;
    int wBullet = 10;
    int hBullet = 10;
    int xCorona = random_x_corona + 22;
    int yCorona = random_y_corona;
    int wCorona = 55;
    int hCorona = 50;

    if((xBullet+wBullet <= xCorona+wCorona) && (xBullet+wBullet >= xCorona)){
        if((yBullet+hBullet <= yCorona+hCorona) && (yBullet+hBullet >= yCorona))  return 1;
        if((yBullet <= yCorona+hCorona) && (yBullet >= yCorona)) return 1;
    }
    if((xBullet<= xCorona+wCorona) && (xBullet >= xCorona)){
        if((yBullet+hBullet <= yCorona+hCorona) && (yBullet+hBullet >= yCorona))  return 1;
        if((yBullet <= yCorona+hCorona) && (yBullet >= yCorona)) return 1;
    }
}









