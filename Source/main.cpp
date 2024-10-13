#include "wavefront.h"
#include "texture.h"

#include "orbiter.h"
#include "draw.h"
#include "app_camera.h"

#include "mat.h"
#include "mesh.h"

#include <vector>
#include <cfloat>
#include <chrono>

#include "program.h"
#include "uniforms.h"

#include "gltf.h"

#include "../Include/Terrain.h"

class TP : public AppCamera
{
private:
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    TP() : AppCamera(1024, 640) {}
    // creation des objets de l'application
    int init()
    {
        std::vector<double> values = std::vector<double>();
        size_terrain_x = 25.0;
        size_terrain_y = 25.0;
        resolution_terrain = 500;
        Terrain t1 = Terrain(vec2(0.0, 0.0), vec2(size_terrain_x, size_terrain_y), resolution_terrain, resolution_terrain, values);
        t1.initValues(FastNoiseLite::NoiseType_OpenSimplex2, 0.01, 2.5, 4.0, 0.2, FastNoiseLite::FractalType_FBm);
        // t1.addNoise(FastNoiseLite::NoiseType_OpenSimplex2, 0.01, 1.0, 4.0, 0.4, FastNoiseLite::FractalType_FBm);
        t1.initLayer();
        t = t1;
        // Terrain area = t.getAreaMap();
        // area.printValues("coco");
        t.printValues("hauteur");
        Terrain laplacian = t.getLaplacianMap();
        laplacian.printValues("laplacian");
        Terrain Area_drain = t.getAreaMap();
        Area_drain.printValues("Area_drain");
        Terrain Slope = t.getSlopeMap();
        Slope.printValues("Slope");

        t.Dijkstra(0, 249999);
        Terrain wetness = t.getWetnessMap(Area_drain, Slope);
        wetness.printValues("Wetness");
        std::cout << "coucou";
        // t.printValues("dijkstra");
        t.printGradientX();
        t.printGradientY();
        t.printSlope();

        m_terrain = t.getMesh();
        pos_cube = t.getPosition(4.73, 6.84);
        // m_cube = read_mesh("data/cube.obj");

        Point pmin, pmax;
        m_terrain.bounds(pmin, pmax);
        camera().lookat(pmin, pmax);

        // etat openGL par defaut
        glClearColor(0.2f, 0.2f, 0.2f, 1.f); // couleur par defaut de la fenetre

        glClearDepth(1.f);       // profondeur par defaut
        glDepthFunc(GL_LESS);    // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST); // activer le ztest
        // make_texture()

        return 0; // pas d'erreur, sinon renvoyer -1
    }

    // destruction des objets de l'application
    int quit()
    {
        m_cube.release();
        m_terrain.release();
        glDeleteTextures(1, &concrete);
        glDeleteTextures(1, &snow);
        glDeleteTextures(1, &grass);
        glDeleteTextures(1, &water);

        return 0; // pas d'erreur
    }

    // dessiner une nouvelle image
    int render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (key_state(SDLK_e) && click == false)
        {
            std::cout << "applyThermicErosion" << std::endl;
            click = true;
            t.applyThermicErosion();
            m_terrain = t.getMesh();
        }
        if (key_state(SDLK_r))
        {
            std::cout << "applyTectonicErosion" << std::endl;
            double u = 0.01;
            double k = 0.05;
            double l = 0.001;
            double n = 1.0;
            t.applyTectonicErosion(u, k, l, n, 100);
            m_terrain = t.getMesh();
        }
        if (key_state(SDLK_u))
        {
            click = false;
        }

        /*
        for (size_t i = 0; i < 500 * 500; i++)
        {

            glUseProgram(m_program);

            for (unsigned int k = 0; k < m_objet[i].size(); k++)
            {

                if (m_colors_texture[m_colors_index[i][k]] != -1)
                {
                    program_use_texture(m_program, "texture", 0, texture);
                }

                glBindVertexArray(m_objet[i][k].vao);
                glDrawElements(GL_TRIANGLES, m_objet[i][k].nb_indice, GL_UNSIGNED_INT, 0);
            }
        }
        */

        // m_terrain.color(Color(0.2,0.4,0.5));
        // m_terrain.ve
        // draw(m_terrain,camera(),grass);
        //  dessine le terrain, placé au centre du monde
        draw(m_terrain, /* model */ Identity(), camera());

        // dessine le cube, placé à une position sur le terrain
        // draw(m_cube, Translation(pos_cube(0), pos_cube(1), pos_cube(2)) * Scale(0.1), camera());

        return 1;
    }

protected:
    Mesh m_terrain;
    bool click = false;

    Mesh m_cube;
    Terrain t;

    vec3 pos_cube;

    double size_terrain_x;
    double size_terrain_y;
    double resolution_terrain;

    GLuint concrete;
    GLuint snow;
    GLuint grass;
    GLuint water;
    // GLuint m_texture1;
};

int main(int argc, char **argv)
{
    // il ne reste plus qu'a creer un objet application et la lancer
    TP tp;
    tp.run();

    return 0;
}
