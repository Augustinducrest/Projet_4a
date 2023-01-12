
/** TP 4ETI - CPE Lyon - 2013/2014 */

#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

//#include <GL/gl.h>
#include <GL/glew.h>

#include "../../lib/3d/mat3.hpp"
#include "../../lib/3d/vec4.hpp"
#include "../../lib/mesh/mesh_parametric.hpp"
#include "../../lib/opengl/mesh_opengl.hpp"
#include "../../lib/interface/camera_matrices.hpp"

#include <vector>


class myWidgetGL;

class scene
{
public:

    scene();


    void build_surface();


    /** \brief Method called only once at the beginning (load off files ...) */
    void load_scene();

    /** \brief Method called at every frame */
    void draw_scene();

    /** Set the pointer to the parent Widget */
    void set_widget(myWidgetGL* widget_param);

    /** Load basic data for the scene */
    void load_common_data();

    /** PERLIN */
    void Perlin(int Nu,int Nv,float frequency,float amplitude,std::vector<int> p,float offsetx,float offsety);
    void scale_expo(float amplitude_base,int Nu,int Nv,float xmax,float ymax);
    void build_surface_perlin();
    void remise(int Nu,int Nv);
    void colormap(int Nu,int Nv);
    void colormap_europe_leo(int Nu,int Nv);
    void colormap_volcan(int Nu,int Nv);
    void colormap_canyon(int Nu,int Nv);

    /** DIAMOND */
    void build_surface_diamond();

    /** Faulting*/
    void build_surface_faulting();
    float line(float theta,float x0,float y0,float x);

    int color=0;
    int points=9;
    float expo=1.0f;
    float amplitude_base = 0.0f;
    float offsetx=0.0f;
    float offsety=0.0f;
    /** Perlin */
    int octaves=1;
    float persistence=0.0f;
    float frequency_base = 1.0f;
    /** faulting */
    int K= 1;
    float R= 0.0f;
    float H = 0.0f;
    float a_base = 0.0f;
private:

    /** Load a texture from a given file and returns its id */
    GLuint load_texture_file(std::string const& filename);

    /** Access to the parent object */
    myWidgetGL* pwidget;

    /** Default id for the texture (white texture) */
    GLuint texture_default;

    /** The id of the shader do draw meshes */
    GLuint shader_program_id;


    cpe::mesh_parametric surface;
    cpe::mesh_opengl surface_opengl;





};

#endif
