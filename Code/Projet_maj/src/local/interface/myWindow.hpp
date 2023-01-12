#pragma once

#ifndef MY_WINDOW_HPP
#define MY_WINDOW_HPP


#include <QMainWindow>




//forward declaration
namespace Ui
{
class MainWindow;
}
class myWidgetGL;

/** Declaration of the Window class */
class myWindow: public QMainWindow
{
    Q_OBJECT

public:

    myWindow(QWidget *parent=NULL);
    ~myWindow();



private slots:

    /** Quit the application */
    void action_quit();
    /** Set the Wireframe mode for the meshes */
    void action_wireframe();
    void action_draw();
    /** fonctions*/
    void DiamondSquare();
    void Perlin();
    void Faulting();
    /** Signaux */
    void changer_ampli(int ampli);
    void changer_expo(int expo);
    void changer_offsetx(int offsetx);
    void changer_offsety(int offsety);
    void changer_octaves(int offsety);
    void changer_persistence(int offsety);
    void changer_freq(int offsety);
    void changer_color(int color);
    void changer_it(int k);
    void changer_rayon(int k);
    void changer_hourst(int k);
    void changer_a(int k);

private:

    /** Layout for the Window */
    Ui::MainWindow *ui;
    /** The OpenGL Widget */
    myWidgetGL *glWidget;





};

#endif
