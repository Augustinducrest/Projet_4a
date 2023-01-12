

#include <GL/glew.h>

#include "scene.hpp"

#include "../../lib/opengl/glutils.hpp"

#include "../../lib/perlin/perlin.hpp"
#include "../../lib/interface/camera_matrices.hpp"

#include "../interface/myWidgetGL.hpp"

#include <cmath>

#include <string>
#include <sstream>
#include "../../lib/mesh/mesh_io.hpp"
#include "../../lib/3d/mat2.hpp"

#include <random>


using namespace cpe;

//LEO
float fract(float x){
    return x-floor(x);
}
//Définition d'un vecteur pseudo aléatoire pour une position (u,v)
float randomfloat(float x){
    return fract(sin(x*(91.3458)) * 47453.5453);
}
//Définition d'un vecteur pseudo aléatoire entre -seuil et seuil pour une position (u,v) avec offset
float randomvec2(vec2 vecx,int seuil,float offsetx,float offsety){
    vec2 off = {offsetx,offsety};
    vecx=vecx+off;
    return (fract(sin(dot(vecx ,vec2(12.9898,78.233))) * 43758.5453))*2*(float)seuil - (float)seuil;
}

float fade(float u){

    return u*u*u*(u * (u * 6 - 15) + 10);
}
float grad(int hash, float x, float y, float z)
{
    switch(hash & 0xF)
    {
        case 0x0: return  x + y;
        case 0x1: return -x + y;
        case 0x2: return  x - y;
        case 0x3: return -x - y;
        case 0x4: return  x + z;
        case 0x5: return -x + z;
        case 0x6: return  x - z;
        case 0x7: return -x - z;
        case 0x8: return  y + z;
        case 0x9: return -y + z;
        case 0xA: return  y - z;
        case 0xB: return -y - z;
        case 0xC: return  y + x;
        case 0xD: return -y + z;
        case 0xE: return  y - x;
        case 0xF: return -y - z;
        default: return 0; // never happens
    }
}
float lerp(float a, float b, float x) {
    return a + x * (b - a);
}
void scene::Perlin(int Nu,int Nv,float frequency,float amplitude,std::vector<int> p,float offsetx,float offsety){
    // Perlin noise

    for(int u=0 ; u<Nu ; u++){
        for(int v=0 ; v<Nv ; v++){
            float x =surface.vertex(u,v).x()*frequency+offsetx;
            float y =surface.vertex(u,v).y()*frequency+offsety;

            int xi = (int) x & 255;
            int yi = (int) y & 255;

            float xf=x-(int)x;
            float yf=y-(int)y;
            float uf = fade(xf);
            float vf = fade(yf);

            int aaa, aba, aab, abb, baa, bba, bab, bbb;
                aaa = p[p[p[xi & 255]+ yi & 255 ] ];
                aba = p[p[p[    xi ]+(yi+1) & 255] ];
                aab = p[(p[p[    xi ]+    yi ]+1) & 255];
                abb = p[(p[p[    xi ]+yi+1]+1) & 255];
                baa = p[p[p[(xi+1)& 255]+    yi & 255 ]];
                bba = p[p[p[(xi+1)& 255]+(yi+1) & 255] ];
                bab = p[(p[p[(xi+1)& 255]+    yi & 255 ]+1) & 255];
                bbb = p[(p[p[(xi+1)& 255]+(yi+1) & 255]+1) & 255];



            float x1, x2, y1, y2;
             x1 = lerp(    grad (aaa, xf  , yf  , 0.0f),
                         grad (baa, xf-1, yf  , 0.0f),
                         uf);
             x2 = lerp(    grad (aba, xf  , yf-1, 0.0f),
                         grad (bba, xf-1, yf-1, 0.0f),
                           uf);
             y1 = lerp(x1, x2, vf);

             x1 = lerp(    grad (aab, xf  , yf  , -1.0f),
                         grad (bab, xf-1.0f, yf  , -1.0f),
                         uf);
             x2 = lerp(    grad (abb, xf  , yf-1.0f, -1.0f),
                           grad (bbb, xf-1.0f, yf-1.0f, -1.0f),
                           uf);
             y2 = lerp (x1, x2, vf);

            surface.vertex(u,v).z() += (lerp (y1, y2, 0.0f)+1.0f/2.0f)*amplitude;
        }
    }
}
void scene::colormap_europe_leo(int Nu,int Nv){
    //Lac et montagnes
    for(int k=0; k<Nu ; ++k ){
        for(int j=0; j<Nv ; ++j){
            float z=surface.vertex(k,j).z()+0.5f;
            vec3 couleur={0.0f,0.0f,0.0f};
            if(z<0.02f){ //lac
                couleur={0.0f,0.0f,0.32f}; //eau
                surface.vertex(k,j).z()=0.1f*(z-0.3f)-0.4f;
            }
            else if(z<0.03f){ //lac
                couleur={0.0f,0.0f,0.35f};//eau
                surface.vertex(k,j).z()=0.1f*(z-0.3f)-0.4f;
            }
            else if(z<0.05f){ //lac
                couleur={0.0f,0.0f,0.4f};//eau
                surface.vertex(k,j).z()=0.1f*(z-0.3f)-0.4f;
            } else if(z<0.08f){ //lac
                couleur={0.0f,0.0f,0.5f};//eau
                surface.vertex(k,j).z()=0.1f*(z-0.3f)-0.4f;

            }
            else if(z<0.1f){ //sable
                couleur={224.0f/255.0f,205.0f/255.0f,169.0f/255.0f};//sable

            }else if(z<0.3f){ //forêt
                couleur={0.2f,-z*0.2f+0.4f,0.2f};
            }
            else if(z<0.7f){ //montagneux
                float g=z;
                couleur={g-0.05f,g-0.05f,g-0.05f};
            }
            else{ //neige
                couleur={1.0f,1.0f,1.0f};
            }
            surface.color(k,j)=couleur;
        }
    }
}
void scene::colormap_volcan(int Nu,int Nv){
    //Lac et montagnes
    for(int k=0; k<Nu ; ++k ){
        for(int j=0; j<Nv ; ++j){
            float z=surface.vertex(k,j).z()+0.5f;
            vec3 couleur={0.0f,0.0f,0.0f};
            if(z<0.09f){ //lave
                surface.vertex(k,j).z()=0.1f*(z-0.1f)-0.4f;
                couleur={1.0f,0.1f,0.0f}; //lave
            }else if(z<0.1){
                surface.vertex(k,j).z()=0.1f*(z-0.1f)-0.4f;
                couleur= {0.8f,0.5f,0.1f};//lave
            }else{ //volcans
                couleur={z,z,z};
                }
            surface.color(k,j)=couleur;
        }
    }
}
void scene::colormap_canyon(int Nu,int Nv){
    //Lac et montagnes
    for(int k=0; k<Nu ; ++k ){
        for(int j=0; j<Nv ; ++j){
            float z=surface.vertex(k,j).z()+0.5f;
            vec3 couleur={1.0f*z+0.15f,0.5f*z+0.09f,0.0f};//roche
            if(z>0.4){
                couleur={1.0f*z+0.15f,0.5f*z+0.09f,0.0f};
                surface.vertex(k,j).z()=0.1f*(z-0.4f)-0.1f;
            }
            surface.color(k,j)=couleur;
            //surface.vertex(k,j).z()=0.0f; //carte
           surface.vertex(k,j).z()=-surface.vertex(k,j).z(); //Canyon
            surface.color(k,j)=couleur;
        }
    }
}
void scene::colormap(int Nu,int Nv){
    if(color==0){
        colormap_europe_leo(Nu,Nv);
    }
    if(color==1){
        colormap_volcan(Nu,Nv);
    }
    if(color==2){
        colormap_canyon(Nu,Nv);
    }
}
void scene::scale_expo(float amplitude_base,int Nu,int Nv,float xmax,float ymax){
    //SCALING
    float max=0.0f;
    float min=0.0f;
    for(int k=0; k<Nu ; ++k ){
        for(int j=0; j<Nv ; ++j){
            float z=surface.vertex(k,j).z();
            if (z>max){
                max = z;
            }
            if (z<min){
               min = z;
            }
        }
    }
    for(int k=0; k<Nu ; ++k ){
        for(int j=0; j<Nv ; ++j){
            surface.vertex(k,j).z()=(surface.vertex(k,j).z() - min)/(max-min);
            surface.vertex(k,j).z()=amplitude_base*powf(surface.vertex(k,j).z(),expo);
            surface.vertex(k,j).x()=surface.vertex(k,j).x()*2*xmax-xmax;
            surface.vertex(k,j).y()=surface.vertex(k,j).y()*2*ymax-ymax;
            surface.vertex(k,j).z()=surface.vertex(k,j).z()-0.5f;
        }
    }
}

void scene::remise(int Nu,int Nv){
    if(surface.vertex(0,0).z()!=0.0f){
        for(int k=0; k<Nu ; ++k ){
            for(int j=0; j<Nv ; ++j){
               surface.vertex(k,j).z()=0.0f;
               surface.vertex(k,j).x()=surface.vertex(k,j).x()+0.5f;
               surface.vertex(k,j).y()=surface.vertex(k,j).y()+0.5f;
            }
        }
    }
    surface.fill_color({0.5f,0.5f,0.5f});
}

void scene::build_surface_perlin()
{

    std::vector<int> permutation = { 151,160,137,91,90,15,                 // Hash lookup table as defined by Ken Perlin.  This is a randomly
        131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,    // arranged array of all numbers from 0-255 inclusive.
        190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
        88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
        77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
        102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
        135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
        5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
        223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
        129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
        251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
        49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
        138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
    };
    //Perlin MAIN

    int Nu=pow(2,points)+1;
    int Nv=pow(2,points)+1;
    remise(Nu,Nv);
    float frequency = frequency_base;
    float amplitude = amplitude_base;
    for(int i=0;i<octaves;i++) {
        Perlin(Nu,Nv,frequency,amplitude,permutation,offsetx,offsety);
        amplitude *= persistence;
        frequency *= 2.0f;
    }


    scale_expo(amplitude_base,Nu,Nv,0.5f,0.5f);


    //couleurs
    colormap( Nu, Nu);
    surface.fill_normal();
    surface.fill_empty_field_by_default();
    surface_opengl.fill_vbo(surface);
}

void scene::build_surface_diamond()
{

    int const Nu=pow(2,points)+1; //doit être impair
    remise(Nu,Nu);

    //algo diamant-carré
    surface.vertex(0,0).z()= randomvec2({surface.vertex(0,0).x(),surface.vertex(0,0).y()},Nu,offsetx,offsety);
    surface.vertex(0,Nu-1).z()=randomvec2({surface.vertex(0,0).x(),surface.vertex(0,0).y()},Nu,offsetx,offsety);
    surface.vertex(Nu-1,0).z()=randomvec2({surface.vertex(0,0).x(),surface.vertex(0,0).y()},Nu,offsetx,offsety);
    surface.vertex(Nu-1,Nu-1).z()=randomvec2({surface.vertex(0,0).x(),surface.vertex(0,0).y()},Nu,offsetx,offsety);


    int i=Nu-1;
    int id=0;
    while(i>1){
        id=i/2;

        for(int k=id; k<Nu ; k=k+i ){ //Début de la phase du diamant
            for(int j=id; j<Nu ; j=j+i){
                float moyenne=(surface.vertex(k-id,j-id).z()+surface.vertex(k-id,j+id).z()+surface.vertex(k+id,j+id).z()+surface.vertex(k+id,j-id).z())/4.0f;
                surface.vertex(k,j).z()=moyenne + randomvec2({surface.vertex(k,j).x(),surface.vertex(k,j).y()},id,offsetx,offsety);
            }
        }
        int decalage=0;
        for(int k=0 ; k<Nu ; k=k+id){ // Début de la phase du carré
            if(decalage==0){
                decalage=id;
            }else{
                decalage=0;
            }
            for(int j=decalage ; j<Nu ; j=j+i){
                float somme=0.0f;
                float n=0.0f;
                if(k>=id){
                    somme=somme+surface.vertex(k-id,j).z();
                    n=n+1.0f;
                }
                if(k+id<Nu){
                    somme=somme+surface.vertex(k+id,j).z();
                    n=n+1.0f;
                }
                if(j>=id){
                    somme=somme+surface.vertex(k,j-id).z();
                    n=n+1.0f;
                }
                if(j+id<Nu){
                    somme=somme+surface.vertex(k,j+id).z();
                    n=n+1.0f;
                }

                surface.vertex(k,j).z()=somme / n + randomvec2({surface.vertex(k,j).x(),surface.vertex(k,j).y()},id,offsetx,offsety);

            }

        }
            i=id;
    }

    scale_expo(amplitude_base,Nu,Nu,0.5f,0.5f);
    colormap( Nu, Nu);

    surface.fill_normal();
    surface.fill_empty_field_by_default();
    surface_opengl.fill_vbo(surface);
}
float scene::line(float theta,float x0,float y0,float x)
{
    return tanf(theta)*x + y0 - tanf(theta)*x0;
}
void scene::build_surface_faulting()
{
    int Nu=pow(2,points)+1;
    int Nv=pow(2,points)+1;
    remise(Nu,Nv);
    /** K iterations */
    std::default_random_engine generator(offsetx*offsety);
    std::uniform_real_distribution<float> thetaDistribution(-M_PI/2,M_PI/2);
    std::uniform_real_distribution<float> yDistribution(0.0f,1.0f);
    std::uniform_real_distribution<float> xDistribution(0.0f,1.0f);
    float x =0.0f;
    float y =0.0f;
    float theta = 0.0f;
    float x0 = 0.0f;
    float y0 = 0.0f;
    float a = a_base;
    float z =0.0f;
    float d = 0.0f;
    float b = 0.0f;
     for (int k=0;k<K;k++)
     {
         /** Phi generation */

         theta = thetaDistribution(generator);
         x0 = xDistribution(generator);
         y0 = yDistribution(generator);

         for (int ku = 0;ku < Nu;ku++)
         {
             for (int kv = 0;kv < Nv;kv++)
             {
                 x = surface.vertex(ku,kv).x();
                 y = surface.vertex(ku,kv).y();
                 z =0.0f;
                 /** Calculation of the distance between the point end the function phi(x) = tan(theta) + b */

                 if (fabs(theta) == M_PI/2)
                 {
                     d = fabs(x-x0);
                 } else
                 {
                     b = line(theta,x0,y0,0.0f);
                     d=fabs(tanf(theta)*x-y+b)/sqrtf(powf(tanf(theta),2)+1);
                 }

                 if (d < R)
                 {
                     if (y >= line(theta,x0,y0,x))
                     {
                         z = surface.vertex(ku,kv).z() + a*powf(1-powf(d/R,2),2);
                     } else
                     {
                         z = surface.vertex(ku,kv).z() - a*powf(1-powf(d/R,2),2);
                     }


                 } else
                 {
                     z =  surface.vertex(ku,kv).z();
                 }
                 surface.vertex(ku,kv).z() = z;
             }
         }
         a *= powf(0.5,H);
         //std::cout<<"a = "<<a<<std::endl;

     }

    scale_expo(amplitude_base,Nu,Nv,0.5f,0.5f);
    colormap( Nu, Nu);

    surface.fill_normal();
    surface.fill_empty_field_by_default();
    surface_opengl.fill_vbo(surface);
}
void scene::build_surface()
{

    int Nu=pow(2,points)+1;
    int Nv=pow(2,points)+1;
    surface.set_plane_xy_unit(Nu,Nv);

}


void scene::load_scene()
{
    load_common_data();


    build_surface();


    surface.fill_normal();
    surface.fill_empty_field_by_default();

    surface_opengl.fill_vbo(surface);

}

void scene::draw_scene()
{
    //Setup uniform parameters
    glUseProgram(shader_program_id);                                                                           PRINT_OPENGL_ERROR();

    //Get cameras parameters (modelview,projection,normal).
    camera_matrices const& cam=pwidget->camera();

    //Set Uniform data to GPU
    glUniformMatrix4fv(get_uni_loc(shader_program_id,"camera_modelview"),1,false,cam.modelview.pointer());     PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(shader_program_id,"camera_projection"),1,false,cam.projection.pointer());   PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(shader_program_id,"normal_matrix"),1,false,cam.normal.pointer());           PRINT_OPENGL_ERROR();


    glBindTexture(GL_TEXTURE_2D,texture_default);  PRINT_OPENGL_ERROR();
    surface_opengl.draw();

}


scene::scene()
    :shader_program_id(0)
{}


GLuint scene::load_texture_file(std::string const& filename)
{
    return pwidget->load_texture_file(filename);
}

void scene::set_widget(myWidgetGL* widget_param)
{
    pwidget=widget_param;
}


void scene::load_common_data()
{
    texture_default=load_texture_file("data/white.jpg");
    shader_program_id = read_shader("shaders/shader_mesh.vert",
                                    "shaders/shader_mesh.frag"); PRINT_OPENGL_ERROR();
}


