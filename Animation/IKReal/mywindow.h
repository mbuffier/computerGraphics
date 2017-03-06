#ifndef MYWINDOW_H
#define MYWINDOW_H

#include "myglwidget.h"
#include <QMouseEvent>
#include <QTimer>

class myWindow : public myGLWidget
{
    Q_OBJECT
public:
    QTime timer;
    explicit myWindow(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void update();
    //void draw();
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    int mainLoop();
    //void terminateApp();

};

#endif // MYWINDOW_H
