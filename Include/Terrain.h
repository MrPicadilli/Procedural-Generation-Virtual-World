#pragma once

#include "../Include/ScalarField2D.h"

#include "../Include/utilityMMV.h"

#include "mesh.h"

struct point_aire
{
    int i, j;
    double h;

    bool operator<(const point_aire &a)
    {
        return h > a.h;
    }
};

class Terrain : public ScalarField2D
{
private:
public:
    Terrain(vec2 a, vec2 b, int nx, int ny, std::vector<double> values);
    Terrain(ScalarField2D sf2d);
    Terrain();

    ~Terrain();

    vec3 getPosition(const int i, const int j) const;       // Get the coordinates of the point in the terrain corresponding to the point at i,j in the scalar field
    vec3 getPosition(const double x, const double y) const; // Get the coordinates of the point in the terrain corresponding to the point at x,y using interpolation
    vec3 getNormal(const int i, const int j) const;         // Get the normal of the terrain at the point corresponding to the point at i,j in the scalar field
    double getSlope(const int i, const int j) const;        // Get the value of the slope of the terrain at the point corresponding to the point at i,j in the scalar field
    ScalarField2D getSlopeMap() const;                      // Returns a scalar field filled with the slope values of the terrain
    ScalarField2D getAreaMap() const;                       // Returns a scalar field filled with the slope values of the terrain
    ScalarField2D getLaplacianMap() const;
    ScalarField2D getWetnessMap(Terrain drain_area, Terrain slope) const;

    std::vector<point_aire> GenerateNeighbour(const int i, const int j, double &addition) const;
    double Get4NeighbourHeight(const int i, const int j, double &taille) const;

    Mesh getMesh() const; // Get the mesh representing the terrain

    void printSlope() const;     // Generates an image representing the slope of the terrain
    void printArea() const;      // Generates an image representing the drainage area of the terrain
    void printLaplacian() const; // Generates an image representing the Laplacian of the terrain

    std::vector<double> Generate8NeighbourSlope(const int i, const int j, double &nb_bas);
    void applyThermicErosion();
    void applyTectonicErosion(double u, double k, double l, double n, int iteration);

    adjacency_list_t Dijkstra(int source, int end);
};
