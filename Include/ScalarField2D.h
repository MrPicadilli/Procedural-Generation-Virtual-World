#pragma once

#include <vector>

#include <string>
#include "vec.h"
#include "image.h"
#include "image_io.h"
#include <cstring>

#include "../Include/Noise.h"
#include "../Include/utilityMMV.h"

struct layer
{
    std::string label;
    double coef_erosion;
    Color color;
    int priority;

    bool operator<(const layer &a)
    {
        return priority > a.priority;
    }
};

class ScalarField2D
{
protected:
    vec2 _a;                     // X and Y coordinates of one of the points defining the border of the scalar field
    vec2 _b;                     // X and Y coordinates of the other point defining the border of the scalar field
    int _nx;                     // Number of points in the x direction
    int _ny;                     // Number of points in the y direction
    std::vector<double> _values; // Vector containing the values of the scalar field
    std::vector<double> _first_values; // Vector containing the values of the scalar field

    std::vector<layer> _list_layer_per_values;
    layer concrete;
    layer snow;
    layer grass;
    layer water;
    layer itinary;

public:
    ScalarField2D();
    ScalarField2D(vec2 a, vec2 b, int nx, int ny, std::vector<double> values);
    ~ScalarField2D();

    int getIndex(const int i, const int j) const;    // Get the index corresponding to i and j
    double getValue(const int i, const int j) const; // Get the value at i on the X-axis and j on the Y-axis
    vec2 getGrad(const int i, const int j) const;    // Get the gradient at i on the X-axis and j on the Y-axis

    ScalarField2D getGradXMap() const; // Returns a scalar field filled with the gradient on the X-axis
    ScalarField2D getGradYMap() const; // Returns a scalar field filled with the gradient on the Y-axis

    ScalarField2D operator+(ScalarField2D const &sf2d); // Adds two scalar field
    ScalarField2D operator-(ScalarField2D const &sf2d); // Substracts two scalar field
    double min_value() const;                           // Returns the smallest value of the scalar field
    double max_value() const;                           // Returns the biggest value of the scalar field
    void smooth();                                      // Smooth the values in the scalar field
    void blur();                                        // Blurs the values in the scalar field

    std::vector<double> generateNoiseData(FastNoiseLite::NoiseType noiseType, double frequency, double fractalLacunarity, double fractalOctave, double fractalGain, FastNoiseLite::FractalType fractalType);
    void initValues(FastNoiseLite::NoiseType noiseType, double frequency, double fractalLacunarity, double fractalOctave, double fractalGain, FastNoiseLite::FractalType fractalType);
    void initLayer();
    void addNoise(FastNoiseLite::NoiseType noiseType, double frequency, double fractalLacunarity, double fractalOctave, double fractalGain, FastNoiseLite::FractalType fractalType);

    void printValues(std::string fichier) const; // Generates an image representing the valuemap
    void printGradientX() const;                 // Generates an image representing the gradientmap on the X-axis
    void printGradientY() const;                 // Generates an image representing the gradientmap on the Y-axis
};
