#include "../Include/ScalarField2D.h"

// -----------------------------------------
//
//    Constructor & Destructor
//
// -----------------------------------------

ScalarField2D::ScalarField2D()
{
}

ScalarField2D::ScalarField2D(vec2 a, vec2 b, int nx, int ny, std::vector<double> values) : _a(a), _b(b), _nx(nx), _ny(ny), _values(values)
{
    concrete = {"concrete", 0.0, Color(134.0 / 255, 134.0 / 255, 134.0 / 255), 3};
    grass = {"grass", 0.0, Color(36.0 / 255, 182.0 / 255, 66.0 / 255), 2};
    snow = {"snow", 0.0, Color(230.0 / 255, 237.0 / 255, 245.0 / 255), 2};
    water = {"water", 0.0, Color(36.0 / 255, 66.0 / 255, 182.0 / 255), 1};
    itinary = {"itinary", 0.0, Color(182.0 / 255, 66.0 / 255, 36.0 / 255), 1};
}

ScalarField2D::~ScalarField2D()
{
}

// -----------------------------------------
//
//    Getter & Setter
//
// -----------------------------------------

/**
 * getIndex
 * Returns the index in the _values vector corresponding to the point at i,j in the scalar field
 * @param i position on the X-axis
 * @param j position on the Y-axis
 * @return int
 */
int ScalarField2D::getIndex(const int i, const int j) const
{
    return i * _nx + j;
}

/**
 * getValue
 * Returns the value of the corresponding point at i,j in the scalar field
 * @param i position on the X-axis
 * @param j position on the Y-axis
 * @return double
 */
double ScalarField2D::getValue(const int i, const int j) const
{
    return _values.at(getIndex(i, j));
}

/**
 * getGrad
 * Returns a vector containing the gradient on the X-axis and the gradient on the Y-axis
 * @param i position on the X-axis
 * @param j position on the Y-axis
 * @return vec2
 */
vec2 ScalarField2D::getGrad(const int i, const int j) const
{
    double temp_Grad_x = 0;
    double temp_Grad_y = 0;

    if (i - 1 < 0)
    {
        temp_Grad_x = getValue(i + 1, j) - getValue(_nx - 1, j);
    }
    else if (i + 1 >= _nx)
    {
        temp_Grad_x = getValue(0, j) - getValue(i - 1, j);
    }
    else
    {
        temp_Grad_x = getValue(i + 1, j) - getValue(i - 1, j);
    }
    if (j - 1 < 0)
    {
        temp_Grad_y = getValue(i, j + 1) - getValue(i, _ny - 1);
    }
    else if (j + 1 >= _ny)
    {
        temp_Grad_y = getValue(i, 0) - getValue(i, j - 1);
    }
    else
    {
        temp_Grad_y = getValue(i, j + 1) - getValue(i, j - 1);
    }

    double Gradhx = temp_Grad_x / 2 * ((_b.x - _a.x) / (_nx - 1));
    double Gradhy = temp_Grad_y / 2 * ((_b.y - _a.y) / (_ny - 1));
    return vec2(Gradhx, Gradhy);
}

/**
 * getGradXMap
 * Returns a scalar field containing the value of the gradient in the direction of the X-axis for each point
 * @return ScalarField2D
 */
ScalarField2D ScalarField2D::getGradXMap() const
{
    std::vector<double> values = std::vector<double>();
    for (int i = 0; i < _nx; i++)
    {
        for (int j = 0; j < _ny; j++)
        {
            values.push_back(getGrad(i, j).x);
        }
    }
    ScalarField2D sf = ScalarField2D(_a, _b, _nx, _ny, values);
    return sf;
}

/**
 * getGradYMap
 * Returns a scalar field containing the value of the gradient in the direction of the Y-axis for each point
 * @return ScalarField2D
 */
ScalarField2D ScalarField2D::getGradYMap() const
{
    std::vector<double> values = std::vector<double>();
    for (int i = 0; i < _nx; i++)
    {
        for (int j = 0; j < _ny; j++)
        {
            values.push_back(getGrad(i, j).y);
        }
    }
    std::cout << "gradient Y : " << values.size() << std::endl;
    ScalarField2D sf = ScalarField2D(_a, _b, _nx, _ny, values);
    return sf;
}

// -----------------------------------------
//
//    Operators
//
// -----------------------------------------

ScalarField2D ScalarField2D::operator+(ScalarField2D const &sf2d)
{
    ScalarField2D sf;
    if (this->_nx == sf2d._nx && this->_ny == sf2d._ny)
    {
        std::vector<double> values = std::vector<double>();
        for (int i = 0; i < _nx; i++)
        {
            for (int j = 0; j < _ny; j++)
            {
                values.push_back(this->getValue(i, j) + sf2d.getValue(i, j));
            }
        }
        sf = ScalarField2D(this->_a, this->_b, this->_nx, this->_ny, values);
    }
    else
    {
        std::cout << "Error : Adding two ScalarField of different size" << std::endl;
    }
    return sf;
}

ScalarField2D ScalarField2D::operator-(ScalarField2D const &sf2d)
{
    ScalarField2D sf;
    if (this->_nx == sf2d._nx && this->_ny == sf2d._ny)
    {
        std::vector<double> values = std::vector<double>();
        for (int i = 0; i < _nx; i++)
        {
            for (int j = 0; j < _ny; j++)
            {
                values.push_back(this->getValue(i, j) - sf2d.getValue(i, j));
            }
        }
        sf = ScalarField2D(this->_a, this->_b, this->_nx, this->_ny, values);
    }
    else
    {
        std::cout << "Error : Substracting two ScalarField of different size" << std::endl;
    }
    return sf;
}

/**
 * min_value
 * Returns the minimum value of the scalar field
 * @return double
 */
double ScalarField2D::min_value() const
{
    double min = getValue(1, 1);
    for (int i = 1; i < _nx - 1; i++)
    {
        for (int j = 1; j < _ny - 1; j++)
        {
            if (getValue(i, j) < min)
            {
                min = getValue(i, j);
            }
        }
    }
    return min;
}

/**
 * max_value
 * Returns the maximum value in the scalar field
 * @return double
 */
double ScalarField2D::max_value() const
{
    double max = getValue(1, 1);
    for (int i = 1; i < _nx - 1; i++)
    {
        for (int j = 1; j < _ny - 1; j++)
        {
            if (getValue(i, j) > max)
            {
                max = getValue(i, j);
            }
        }
    }
    return max;
}

void ScalarField2D::smooth()
{
    std::vector<double> smoothed_values = std::vector<double>();
    for (int i = 0; i < _nx; i++)
    {
        for (int j = 0; j < _ny; j++)
        {
            if (i == 0 || j == 0 || i == _nx || j == _ny)
            {
                smoothed_values.push_back(_values[(i * _nx) + j]);
            }
            else
            {
                smoothed_values.push_back((1 / 9) * _values[((i - 1) * _nx) + j - 1] +
                                          (2 / 9) * _values[((i - 1) * _nx) + j] +
                                          (1 / 9) * _values[((i - 1) * _nx) + j + 1] +
                                          (2 / 9) * _values[(i * _nx) + j - 1] +
                                          (4 / 9) * _values[(i * _nx) + j] +
                                          (2 / 9) * _values[(i * _nx) + j + 1] +
                                          (1 / 9) * _values[((i + 1) * _nx) + j - 1] +
                                          (2 / 9) * _values[((i + 1) * _nx) + j] +
                                          (1 / 9) * _values[((i + 1) * _nx) + j + 1]);
            }
        }
    }
}

void ScalarField2D::blur()
{
    std::vector<double> smoothed_values = std::vector<double>();
    for (int i = 0; i < _nx; i++)
    {
        for (int j = 0; j < _ny; j++)
        {
            if (i == 0 || j == 0 || i == _nx || j == _ny)
            {
                smoothed_values.push_back(_values[(i * _nx) + j]);
            }
            else
            {
                smoothed_values.push_back((1 / 9) * _values[((i - 1) * _nx) + j - 1] +
                                          (1 / 9) * _values[((i - 1) * _nx) + j] +
                                          (1 / 9) * _values[((i - 1) * _nx) + j + 1] +
                                          (1 / 9) * _values[(i * _nx) + j - 1] +
                                          (1 / 9) * _values[(i * _nx) + j] +
                                          (1 / 9) * _values[(i * _nx) + j + 1] +
                                          (1 / 9) * _values[((i + 1) * _nx) + j - 1] +
                                          (1 / 9) * _values[((i + 1) * _nx) + j] +
                                          (1 / 9) * _values[((i + 1) * _nx) + j + 1]);
            }
        }
    }
}

// -----------------------------------------
//
//    Values modification
//
// -----------------------------------------

/**
 * generateNoiseData
 * Returns a vector containing the values corresponding to the described noise
 * @param noiseType Type of noise
 * @param frequency Frequency of the noise (the closer to zero, the smoother the terrain)
 * @param fractalLacunarity frequency of apparition of mountain (default 2.0)
 * @param fractalGain will influence the smoothing of the map (default 3.0)
 * @param fractalType
 * @return std::vector<double>
 */
std::vector<double> ScalarField2D::generateNoiseData(FastNoiseLite::NoiseType noiseType, double frequency, double fractalLacunarity, double fractalOctave, double fractalGain, FastNoiseLite::FractalType fractalType)
{
    // Create and configure FastNoise object
    FastNoiseLite noise;
    noise.SetNoiseType(noiseType);

    noise.SetFrequency(frequency);
    noise.SetFractalLacunarity(fractalLacunarity);
    noise.SetFractalOctaves(fractalOctave);
    noise.SetFractalGain(fractalGain);

    noise.SetFractalType(fractalType);

    // Gather noise data
    std::vector<double> noiseData = std::vector<double>();

    for (int i = 0; i < _nx; i++)
    {
        for (int j = 0; j < _ny; j++)
        {
            noiseData.push_back(noise.GetNoise((double)i, (double)j));
        }
    }

    return noiseData;
}

/**
 * initValues
 * Set the values inside the scalar field
 * @param noiseType Type of noise
 * @param frequency Frequency of the noise (the closer to zero, the smoother the terrain)
 * @param fractalLacunarity frequency of apparition of mountain (default 2.0)
 * @param fractalGain will influence the smoothing of the map (default 3.0)
 * @param fractalType
 */
void ScalarField2D::initValues(FastNoiseLite::NoiseType noiseType, double frequency, double fractalLacunarity, double fractalOctave, double fractalGain, FastNoiseLite::FractalType fractalType)
{
    _values = this->generateNoiseData(noiseType, frequency, fractalLacunarity, fractalOctave, fractalGain, fractalType);
}

void ScalarField2D::initLayer()
{
    double min_val = min_value();
    double max_val = max_value();
    double echelon = std::abs(min_val) + std::abs(max_val);

    std::cout << "min_val" << min_val << std::endl;
    std::cout << "max_val" << max_val << std::endl;
    std::cout << "echelon" << echelon << std::endl;

    for (int i = 0; i < _nx; i++)
    {
        for (int j = 0; j < _ny; j++)
        {
            if (std::abs(min_val) + getValue(i, j) > 0.8 * echelon)
            {
                this->_list_layer_per_values.push_back(snow);
            }
            else if (std::abs(min_val) + getValue(i, j) <= 0.8 * echelon && std::abs(min_val) + getValue(i, j) > 0.45 * echelon)
            {
                this->_list_layer_per_values.push_back(concrete);
            }
            else if (std::abs(min_val) + getValue(i, j) > 0.1 * echelon && std::abs(min_val) + getValue(i, j) <= 0.45 * echelon)
            {
                this->_list_layer_per_values.push_back(grass);
            }
            else
            {
                this->_list_layer_per_values.push_back(water);
            }
        }
    }
}

/**
 * addNoise
 * Adds the described noise to the scalar field
 * @param noiseType Type of noise
 * @param frequency Frequency of the noise (the closer to zero, the flatter the terrain)
 * @param fractalLacunarity
 * @param fractalGain
 * @param fractalType
 */
void ScalarField2D::addNoise(FastNoiseLite::NoiseType noiseType, double frequency, double fractalLacunarity, double fractalOctave, double fractalGain, FastNoiseLite::FractalType fractalType)
{
    // Create and configure FastNoise object
    FastNoiseLite noise;
    noise.SetNoiseType(noiseType);

    noise.SetFrequency(frequency); // entre 0 et 1, plus on est proche de 0 plus le terrain devient plat
    noise.SetFractalLacunarity(fractalLacunarity);
    noise.SetFractalOctaves(fractalOctave); // default 3
    noise.SetFractalGain(fractalGain);

    noise.SetFractalType(fractalType);

    // Gather noise data
    std::vector<double> noiseData = std::vector<double>();

    for (int i = 0; i < this->_nx; i++)
    {
        for (int j = 0; j < this->_ny; j++)
        {
            _values[i * _ny + j] += noise.GetNoise((double)i, (double)j);
        }
    }
}

// -----------------------------------------
//
//    Print map
//
// -----------------------------------------

/**
 * printValues
 * Generates an image of the valuemap of the scalar field
 */
void ScalarField2D::printValues(std::string fichier) const
{
    Image img = Image(_nx, _ny);
    double min_val = min_value();
    double max_val = max_value();
    std::cout << "min_val" << min_val << std::endl;
    std::cout << "max_val" << max_val << std::endl;

    for (int i = 0; i < _nx; i++)
    {
        for (int j = 0; j < _ny; j++)
        {
            img(i * _ny + j) = Color((getValue(i, j) - min_val) / (max_val - min_val)); // mettre le résultat en std::sqrt pour avoir des joli map de drainage
        }
    }
    const char *file = fichier.c_str();
    const char *path = "./tp_mmv/GeneratedData/";
    const char *extension = ".png";

    char *s = new char[strlen(path) + strlen(extension) + strlen(file) + 1];

    std::strcpy(s, path);
    std::strcat(s, file);
    std::strcat(s, extension);

    write_image(img, s);
}

/**
 * printGradientX
 * Generates an image of the gradientmap of the scalar field on the X-axis
 */
void ScalarField2D::printGradientX() const
{
    ScalarField2D sf_gradX = getGradXMap();
    sf_gradX.printValues("gradientX_map");
}

/**
 * printGradientY
 * Generates an image of the gradientmap of the scalar field on the Y-axis
 */
void ScalarField2D::printGradientY() const
{
    ScalarField2D sf_gradY = getGradYMap();
    sf_gradY.printValues("gradientY_map");
}
