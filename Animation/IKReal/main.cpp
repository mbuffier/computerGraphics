/*****************************************************************************

Projet IGR204 - Lena Petersen, Shane Nana Yemming, Maud Buffier 

******************************************************************************/


#include <QApplication>
#include "mywindow.h"

int main(int argc, char *argv[]){

    QApplication app(argc, argv);
    myWindow myWin; //Ajout de notre classe myWindow
    myWin.show();   //Exécution de notre fenêtre de rendu OpenGL

    return app.exec();

}

