#include "myWindow.hpp"

#include "myWidgetGL.hpp"
#include "../../lib/common/error_handling.hpp"
#include "ui_mainwindow.h"
#include <cmath>
#include <iostream>

myWindow::myWindow(QWidget *parent)
    :QMainWindow(parent),ui(new Ui::MainWindow)
{
    try
    {
        //Setup window layout
        ui->setupUi(this);

        //Create openGL context
        QGLFormat qglFormat;
        qglFormat.setVersion(1,2);

        //Create OpenGL Widget renderer
        glWidget=new myWidgetGL(qglFormat);

        //Add the OpenGL Widget into the layout
        ui->layout_scene->addWidget(glWidget);
    }
    catch(cpe::exception_cpe const& e)
    {
        std::cout<<std::endl<<e.report_exception()<<std::endl;
    }

    //Connect slot and signals
    connect(ui->quit,SIGNAL(clicked()),this,SLOT(action_quit()));
    connect(ui->Button_fault,SIGNAL(clicked()),this,SLOT(Faulting()));
    connect(ui->Button_diamond,SIGNAL(clicked()),this,SLOT(DiamondSquare()));
    connect(ui->Button_perlin,SIGNAL(clicked()),this,SLOT(Perlin()));
    connect(ui->draw,SIGNAL(clicked()),this,SLOT(action_draw()));
    connect(ui->wireframe,SIGNAL(clicked()),this,SLOT(action_wireframe()));

    connect(ui->slid_ampli, SIGNAL(valueChanged(int)), this, SLOT(changer_ampli(int)));
    connect(ui->slid_expo, SIGNAL(valueChanged(int)), this, SLOT(changer_expo(int)));
    connect(ui->slid_oct, SIGNAL(valueChanged(int)), this, SLOT(changer_octaves(int)));
    connect(ui->slid_offx, SIGNAL(valueChanged(int)), this, SLOT(changer_offsetx(int)));
    connect(ui->slid_offy, SIGNAL(valueChanged(int)), this, SLOT(changer_offsety(int)));
    connect(ui->slid_persi, SIGNAL(valueChanged(int)), this, SLOT(changer_persistence(int)));
    connect(ui->slid_freq, SIGNAL(valueChanged(int)), this, SLOT(changer_freq(int)));
    connect(ui->Box_color, SIGNAL(valueChanged(int)), this, SLOT(changer_color(int)));
    connect(ui->slid_it, SIGNAL(valueChanged(int)), this, SLOT(changer_it(int)));
    connect(ui->slid_rayon, SIGNAL(valueChanged(int)), this, SLOT(changer_rayon(int)));
    connect(ui->slid_hourst, SIGNAL(valueChanged(int)), this, SLOT(changer_hourst(int)));
    connect(ui->slid_a, SIGNAL(valueChanged(int)), this, SLOT(changer_a(int)));

}

myWindow::~myWindow()
{}

void myWindow::action_quit()
{
    close();
}
void myWindow::Faulting()
{
    glWidget->scene_3d.build_surface_faulting();
}
void myWindow::DiamondSquare()
{
    glWidget->scene_3d.build_surface_diamond();
}
void myWindow::Perlin()
{
    glWidget->scene_3d.build_surface_perlin();
}
void myWindow::action_draw()
{
    glWidget->change_draw_state();
}

void myWindow::action_wireframe()
{
    bool const state_wireframe=ui->wireframe->isChecked();
    glWidget->wireframe(state_wireframe);
}

void myWindow::changer_ampli(int ampli)
{
    glWidget->scene_3d.amplitude_base=ampli/99.0f;
}
void myWindow::changer_expo(int expo)
{
    glWidget->scene_3d.expo=(expo/20.0f) +1;
}
void myWindow::changer_offsetx(int offsetx)
{
    glWidget->scene_3d.offsetx=offsetx;
}
void myWindow::changer_offsety(int offsety)
{
    glWidget->scene_3d.offsety=offsety;
}
void myWindow::changer_octaves(int oct)
{
    glWidget->scene_3d.octaves=floor((oct+10)/10.0f);
}
void myWindow::changer_persistence(int pers)
{
    glWidget->scene_3d.persistence=pers/99.0f;
}
void myWindow::changer_freq(int freq)
{
    glWidget->scene_3d.frequency_base=freq*4.0f/99.0f + 1;
}
void myWindow::changer_color(int color)
{
    glWidget->scene_3d.color=color;
}
void myWindow::changer_it(int k)
{
    glWidget->scene_3d.K=(k+1)*4-3;
}
void myWindow::changer_rayon(int r)
{
    glWidget->scene_3d.R=r*sqrtf(2.0f)/99.0f ;
}
void myWindow::changer_hourst(int h)
{
    glWidget->scene_3d.H=h/99.0f;
}
void myWindow::changer_a(int a)
{
    glWidget->scene_3d.a_base=a*5.0f/99.0f;
}





