#pragma once

#include <math.h>
#include <iostream>
#include <vector>
#include <string>
#include <list>

#include <limits> // for numeric_limits

#include <set>
#include <utility> // for pair
#include <algorithm>
#include <iterator>
#include "vec.h"

double Norm2(const vec2 u); // Compute the norm of a vector containing 2 values

double Norm3(const vec3 u); // Compute the norm of a vector containing 3 values

double InterpolationLin(const double x, const double y); // Compute the linear interpolation of the x and y values

typedef int vertex_t;
typedef double weight_t;

const weight_t max_weight = std::numeric_limits<double>::infinity();

struct neighbor
{
    vertex_t target;
    weight_t weight;
    neighbor(vertex_t arg_target, weight_t arg_weight)
        : target(arg_target), weight(arg_weight) {}
};

typedef std::vector<std::vector<neighbor>> adjacency_list_t;

void DijkstraComputePaths(vertex_t source,
                          const adjacency_list_t &adjacency_list,
                          std::vector<weight_t> &min_distance,
                          std::vector<vertex_t> &previous);

std::list<vertex_t> DijkstraGetShortestPathTo(
    vertex_t vertex, const std::vector<vertex_t> &previous);