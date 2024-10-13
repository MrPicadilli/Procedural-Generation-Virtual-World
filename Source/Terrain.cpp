#include "../Include/Terrain.h"

// -----------------------------------------
//
//    Constructor & Destructor
//
// -----------------------------------------

Terrain::Terrain(vec2 a, vec2 b, int nx, int ny, std::vector<double> values) : ScalarField2D(a, b, nx, ny, values)
{
}

Terrain::Terrain(ScalarField2D sf2d) : ScalarField2D(sf2d)
{
}
Terrain::Terrain()
{
}

Terrain::~Terrain()
{
}

// -----------------------------------------
//
//    Getter & Setter
//
// -----------------------------------------

/**
 * getPosition
 * Returns the coordinates of the point at i,j
 * @param i position on the X-axis
 * @param j position on the Y-axis
 * @return vec3
 */
vec3 Terrain::getPosition(const int i, const int j) const
{
    double y = getValue(i, j);
    double x = _a.x + ((_b.x - _a.x) * i) / (_nx - 1);
    double z = _a.y + ((_b.y - _a.y) * j) / (_ny - 1);
    return vec3(x, y, z);
}

/**
 * getPosition
 * Returns the coordinates of the point at the coordinates x,y
 * @param x
 * @param y
 * @return vec3
 */
vec3 Terrain::getPosition(const double x, const double y) const
{
    double x_triangle, y_triangle;
    double i0, j0;
    int int_i0, int_j0;
    x_triangle = std::modf((double)x / ((_b.x - _a.x) / _nx), &i0);
    y_triangle = std::modf((double)y / ((_b.y - _a.y) / _ny), &j0);
    int_i0 = (int)i0;
    int_j0 = (int)j0;

    vec3 p1, p2, p3, pos_res;

    if ((x_triangle + y_triangle) > 1)
    {
        p1 = getPosition(int_i0, int_j0 + 1);
        p2 = getPosition(int_i0 + 1, int_j0 + 1);
        p3 = getPosition(int_i0 + 1, int_j0);
    }
    else
    {
        p1 = getPosition(int_i0, int_j0);
        p2 = getPosition(int_i0, int_j0 + 1);
        p3 = getPosition(int_i0 + 1, int_j0);
    }

    pos_res = vec3(x, (p1(1) + p2(1) + p3(1)) / 3.0, y);

    return pos_res;
}

/**
 * getNormal
 * Returns the normal for the point at i,j
 * @param i position on the X-axis
 * @param j position on the Y-axis
 * @return vec3
 */
vec3 Terrain::getNormal(const int i, const int j) const
{
    vec2 g = getGrad(i, j);
    vec3 n = vec3(-g.x, -g.y, 1);
    return vec3(n.x / Norm3(n), n.y / Norm3(n), n.z / Norm3(n));
}

/**
 * getSlope
 * Returns the value of the slope for the point at i,j
 * @param i position on the X-axis
 * @param j position on the Y-axis
 * @return double
 */
double Terrain::getSlope(const int i, const int j) const
{
    return Norm2(getGrad(i, j));
}

/**
 * getSlopeMap
 * Returns a scalar field containing the value of the slope for each point
 * @return ScalarField2D
 */
ScalarField2D Terrain::getSlopeMap() const
{
    std::vector<double> values = std::vector<double>();
    for (int i = 0; i < _nx; i++)
    {
        for (int j = 0; j < _ny; j++)
        {
            values.push_back(getSlope(i, j));
        }
        // std::cout << "taille values : " << values.size() << std::endl;
    }

    ScalarField2D sf = ScalarField2D(_a, _b, _nx, _ny, values);
    return sf;
}

/**
 * GenerateNeighbour
 *
 * @param i
 * @param j
 * @param addition
 * @return std::vector<point_aire>
 */
std::vector<point_aire> Terrain::GenerateNeighbour(const int i, const int j, double &addition) const
{
    std::vector<point_aire> neighbour;

    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            if (j + y >= _ny || i + x >= _nx || j + y < 0 || i + x < 0)
            {
                neighbour.push_back({i + x, j + y, -1});
            }
            else
            {
                neighbour.push_back({i + x, j + y, this->getValue(i + x, j + y)});
                if (this->getValue(i + x, j + y) < getValue(i, j))
                {
                    addition++;
                }
            }
        }
    }

    return neighbour;
}

/**
 * getAreaMap
 * Returns a scalar field containing the value of the slope for each point
 * @return ScalarField2D
 */
ScalarField2D Terrain::getAreaMap() const
{

    std::vector<point_aire> array;
    std::vector<double> _areas; // Vector containing the values of the scalar field

    for (int i = 0; i < _nx; i++)
    {
        for (int j = 0; j < _ny; j++)
        {
            // etape 1 : on met tout les heightfield a 1
            _areas.push_back(1);

            // etape 2 : faire une liste de tout les points avec leur hauteur et leur pente
            array.push_back({i, j, getValue(i, j)});
        }
    }
    // etape 3 : trier la liste dans l'ordre décroissant par rapport aux hauteur
    std::sort(array.begin(), array.end());

    // etape 4 : parcourir la liste tout en incrémentant les cases qui sont a une hauteur inferier
    // faire un tableau des 8 voisins
    double addition = 0;
    for (size_t j = 0; j < array.size(); j++)
    {
        addition = 0;
        std::vector<point_aire> neighbour = GenerateNeighbour(array[j].i, array[j].j, addition);

        for (size_t p = 0; p < neighbour.size(); p++)
        {

            if (neighbour[p].h != -1)
            {
                if (neighbour[p].h < array[j].h)
                {
                    _areas[getIndex(neighbour[p].i, neighbour[p].j)] += _areas[getIndex(array[j].i, array[j].j)] / addition;
                }
            }
        }
    }

    ScalarField2D sf = ScalarField2D(_a, _b, _nx, _ny, _areas);
    return sf;
}

double Terrain::Get4NeighbourHeight(const int i, const int j, double &taille) const
{
    double somme = 0;
    taille = 0;
    if (i - 1 >= 0)
    {
        somme += getValue(i - 1, j);
        taille++;
    }

    if (j - 1 >= 0)
    {
        somme += getValue(i, j - 1);
        taille++;
    }

    if (i + 1 < _nx)
    {
        somme += getValue(i + 1, j);
        taille++;
    }

    if (j + 1 < _ny)
    {
        somme += getValue(i, j + 1);
        taille++;
    }

    return somme;
}

ScalarField2D Terrain::getLaplacianMap() const
{
    std::vector<double> laplacian; // Vector containing the values of the scalar field

    double somme, difference;
    double h = 1;
    double taille;

    for (int i = 0; i < _nx; i++)
    {
        for (int j = 0; j < _ny; j++)
        {

            somme = Get4NeighbourHeight(i, j, taille);
            difference = taille * getValue(i, j);
            laplacian.push_back((somme - difference) / (h * h));
        }
    }

    ScalarField2D sf = ScalarField2D(_a, _b, _nx, _ny, laplacian);
    return sf;
};

ScalarField2D Terrain::getWetnessMap(Terrain drain_area, Terrain slope) const
{
    std::vector<double> wetness; // Vector containing the values of the scalar field

    for (int i = 0; i < _nx; i++)
    {
        for (int j = 0; j < _ny; j++)
        {
            wetness.push_back(std::log(drain_area.getValue(i, j) / slope.getValue(i, j)));
        }
    }

    ScalarField2D sf = ScalarField2D(_a, _b, _nx, _ny, wetness);
    return sf;
};

/**
 * getMesh
 * Returns the mesh representing the terrain
 * @return Mesh
 */
Mesh Terrain::getMesh() const
{
    Mesh m = Mesh(GL_TRIANGLES);
    std::vector<int> id_points = std::vector<int>();
    std::cout << "_list_layer_per_values" << _list_layer_per_values.size() << std::endl;
    std::cout << "_values" << _values.size() << std::endl;

    for (int i = 0; i < _nx; i++)
    {
        for (int j = 0; j < _ny; j++)
        {
            m.color(this->_list_layer_per_values[this->getIndex(i, j)].color);
            id_points.push_back(m.vertex(getPosition(i, j)));
        }
    }

    for (int i = 0; i < _nx - 1; i++)
    {
        for (int j = 0; j < _ny - 1; j++)
        {
            m.triangle(id_points.at(i * _nx + (j + 1)), id_points.at((i + 1) * _nx + j), id_points.at(i * _nx + j));
            m.triangle(id_points.at(i * _nx + (j + 1)), id_points.at((i + 1) * _nx + (j + 1)), id_points.at((i + 1) * _nx + j));
        }
    }

    return m;
}

std::vector<double> Terrain::Generate8NeighbourSlope(const int i, const int j, double &nb_bas)
{
    std::vector<double> neigh;
    double distance = _b.x / _nx;
    double pente = 0.0;
    double hauteur_base = _first_values[getIndex(i, j)];
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            if (j + y >= _ny || i + x >= _nx || j + y < 0 || i + x < 0)
            {
                pente = -1;
                // neighbour.push_back(-1);
            }
            else if (abs(x) == abs(y) && x != 0)
            {
                pente = (_first_values[getIndex(i, j)] - _first_values[getIndex(i + x, j + y)]) / std::sqrt(2 * distance);
                pente = std::atan(pente);
                pente = (pente * 180) / M_PI;
            }
            else
            {
                pente = (_first_values[getIndex(i, j)] - _first_values[getIndex(i + x, j + y)]) / distance;
                pente = std::atan(pente);
                pente = (pente * 180) / M_PI;
            }
            if (pente > 30)
            {
                nb_bas++;
            }
            neigh.push_back(pente);
        }
    }

    return neigh;
}

void Terrain::applyThermicErosion()
{
    _first_values = this->_values;
    for (int i = 0; i < _nx - 1; i++)
    {
        for (int j = 0; j < _ny - 1; j++)
        {
            double nb_bas=0;
            std::vector<double> neigh = Generate8NeighbourSlope(i, j, nb_bas);
            int ite = 0;
            double hauteur_base = _first_values[getIndex(i, j)];
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    if (neigh[ite] > 30)
                    {
                        //std::cout << "changement" << std::endl;
                        _values[getIndex(i + x, j + y)] += (0.01 / nb_bas) * hauteur_base;
                        _values[getIndex(i, j)] -= (0.01 / nb_bas) * hauteur_base;
                    }
                    ite++;
                }
            }
        }
    }
}

void Terrain::applyTectonicErosion(double u, double k, double l, double n, int iteration)
{
    ScalarField2D area_drain = getAreaMap();
    ScalarField2D laplacian = getLaplacianMap();
    ScalarField2D slope = getSlopeMap();
    for (int nb_iteration = 0; nb_iteration < iteration; nb_iteration++)
    {
        for (int i = 0; i < _nx - 1; i++)
        {
            for (int j = 0; j < _ny - 1; j++)
            {
                _values[getIndex(i, j)] += (u + k * std::pow(area_drain.getValue(i, j), n / 2) * std::pow(slope.getValue(i, j), n) + 0* laplacian.getValue(i, j));
                //std::cout << "on ajoute : " <<  (u + k * std::pow(area_drain.getValue(i, j), n / 2) * std::pow(slope.getValue(i, j), n) + l * laplacian.getValue(i, j)) << std::endl;
            }
        }
    }
}

adjacency_list_t Terrain::Dijkstra(int source, int end)
{
    adjacency_list_t adjacency_list(_nx * _ny);
    int ite = 0;
    int ite_neighbour = 0;
    _first_values = this->_values;

    for (int i = 0; i < _nx; i++)
    {
        for (int j = 0; j < _ny; j++)
        {
            double nb_bas;
            std::vector<double> neighbour = Generate8NeighbourSlope(i, j, nb_bas);
            ite_neighbour = 0;
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    if (j + y >= _ny || i + x >= _nx || j + y < 0 || i + x < 0)
                    {
                    }
                    else if(x == 0 && y == 0){

                    }
                    else
                    {
                        adjacency_list[this->getIndex(i, j)].push_back(neighbor(this->getIndex(i + x, j + y), std::abs(neighbour[ite_neighbour])));
                    }
                    ite_neighbour++;
                }
            }
            ite++;
        }
    }

    std::cout << "adjacency_list:  " << adjacency_list.size() << std::endl;
   // std::cout << "adjacency_list:  " << adjacency_list[0].size() << std::endl;



    std::cout << "fin du parcour " << std::endl;

    std::vector<weight_t> min_distance;
    std::vector<vertex_t> previous;
    std::cout << "debut dijkstra " << std::endl;

    DijkstraComputePaths(source, adjacency_list, min_distance, previous);
    std::cout << "fin dijkstra " << std::endl;

    std::cout << "Distance from 0 to 4: " << min_distance[1200] << std::endl;
    std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
    std::cout << "Path : ";
    std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));

    for (int n : path)
        this->_list_layer_per_values[n] = itinary;
    
    std::cout << std::endl;
    return adjacency_list;
}

// -----------------------------------------
//
//    Operators
//
// -----------------------------------------

// -----------------------------------------
//
//    Print map
//
// -----------------------------------------

/**
 * printSlope
 * Generates an image of the slope of the terrain
 */
void Terrain::printSlope() const
{
    ScalarField2D sf_slope = getSlopeMap();
    sf_slope.printValues("slope_map");
}

void Terrain::printArea() const
{
    ScalarField2D sf_area = getAreaMap();
    sf_area.printValues("drainage_area_map");
}

void Terrain::printLaplacian() const
{
    ScalarField2D sf_laplacian = getLaplacianMap();
    sf_laplacian.printValues("laplacian_map");
}
