/*
This file declares the functions used to calculate the bucket position.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <tuple>
#include <vector>
#include "soil_simulator/types.hpp"

namespace soil_simulator {

/// \brief This function determines all the cells where the bucket is located.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param pos: Cartesian coordinates of the bucket origin. [m]
/// \param ori: Orientation of the bucket. [Quaternion]
/// \param grid: Class that stores information related to the simulation grid.
/// \param bucket: Class that stores information related to the bucket object.
/// \param sim_param: Class that stores information related to the simulation.
/// \param tol: Small number used to handle numerical approximation errors.
void CalcBucketPos(
    SimOut* sim_out, std::vector<float> pos, std::vector<float> ori, Grid grid,
    Bucket* bucket, SimParam sim_param, float tol);

/// \brief This function determines the cells where a rectangle surface is
///        located.
///
/// \param a: Cartesian coordinates of one vertex of the rectangle. [m]
/// \param b: Cartesian coordinates of one vertex of the rectangle. [m]
/// \param c: Cartesian coordinates of one vertex of the rectangle. [m]
/// \param d: Cartesian coordinates of one vertex of the rectangle. [m]
/// \param delta: Spatial increment used to decompose the edges of the
///               rectangle. [m]
/// \param grid: Class that stores information related to the simulation grid.
/// \param tol: Small number used to handle numerical approximation errors.
///
/// \return Collection of cells indices where the rectangle is located.
///         Result is not sorted and duplicates may be present.
std::vector<std::vector<int>> CalcRectanglePos(
    std::vector<float> a, std::vector<float> b, std::vector<float> c,
    std::vector<float> d, Grid grid, float tol);

/// \brief This function performs a vector decomposition on a portion of the
///        horizontal plane where a rectangle ABCD is located.
///
/// \param ab_ind: Indices representing the edge AB of the rectangle.
/// \param ad_ind: Indices representing the edge AD of the rectangle.
/// \param a_ind: Indices of the vertex A from which the edges AB and AD start.
/// \param area_min_x: Minimum index in the X direction of the specified area.
/// \param area_min_y: Minimum index in the Y direction of the specified area.
/// \param area_length_x: Number of grid elements in the X direction of the
///                       specified area.
/// \param area_length_y: Number of grid elements in the Y direction of the
///                       specified area.
/// \param tol: Small number used to handle numerical approximation errors.
///
/// \return A tuple composed of a vector of vector giving the vector
///         decomposition in terms of the AB component, a vector of vector
///         giving the vector decomposition in terms of the AD component, a
///         vector of vector indicating whether the cell is inside the rectangle
///         area, and the number of cells inside the rectangle area.
std::tuple<
    std::vector<std::vector<float>>, std::vector<std::vector<float>>,
    std::vector<std::vector<bool>>, int>
DecomposeVectorRectangle(
    std::vector<float> ab_ind, std::vector<float> ad_ind,
    std::vector<float> a_ind, int area_min_x, int area_min_y,
    int area_length_x, int area_length_y, float tol);

/// \brief This function determines the cells where a triangle surface is
///        located.
///
/// \param a: Cartesian coordinates of one vertex of the triangle. [m]
/// \param b: Cartesian coordinates of one vertex of the triangle. [m]
/// \param c: Cartesian coordinates of one vertex of the triangle. [m]
/// \param delta: Spatial increment used to decompose the edges of the
///               triangle. [m]
/// \param grid: Class that stores information related to the simulation grid.
/// \param tol: Small number used to handle numerical approximation errors.
///
/// \return Collection of cells indices where the triangle is located.
///         Result is not sorted and duplicates may be present.
std::vector<std::vector<int>> CalcTrianglePos(
    std::vector<float> a, std::vector<float> b, std::vector<float> c,
    Grid grid, float tol);

/// \brief This function performs a vector decomposition on a portion of the
///        horizontal plane where a tritangle ABC is located.
///
/// \param ab_ind: Indices representing the edge AB of the triangle.
/// \param ac_ind: Indices representing the edge AC of the triangle.
/// \param a_ind: Indices of the vertex A from which the edges AB and AC start.
/// \param area_min_x: Minimum index in the X direction of the specified area.
/// \param area_min_y: Minimum index in the Y direction of the specified area.
/// \param area_length_x: Number of grid elements in the X direction of the
///                       specified area.
/// \param area_length_y: Number of grid elements in the Y direction of the
///                       specified area.
/// \param tol: Small number used to handle numerical approximation errors.
///
/// \return A tuple composed of a vector of vector giving the vector
///         decomposition in terms of the AB component, a vector of vector
///         giving the vector decomposition in terms of the AC component, a
///         vector of vector indicating whether the cell is inside the triangle
///         area, and the number of cells inside the triangle area.
std::tuple<
    std::vector<std::vector<float>>, std::vector<std::vector<float>>,
    std::vector<std::vector<bool>>, int>
DecomposeVectorTriangle(
    std::vector<float> ab_ind, std::vector<float> ac_ind,
    std::vector<float> a_ind, int area_min_x, int area_min_y,
    int area_length_x, int area_length_y, float tol);

/// \brief This function determines all the cells that lie on a straight line
///        between two Cartesian coordinates.
///
/// \param a: Cartesian coordinates of the first extremity of the line. [m]
/// \param b: Cartesian coordinates of the second extremity of the line. [m]
/// \param delta: Spatial increment used to decompose the line. [m]
/// \param grid: Class that stores information related to the simulation grid.
///
/// \return Collection of cells indices where the line is located.
///         Result is not sorted and duplicates should be expected.
std::vector<std::vector<int>> CalcLinePos(
    std::vector<float> a, std::vector<float> b, Grid grid);

/// \brief This function updates the bucket position in `body` following the
///        cells composing `area_pos`.
///
/// \param area_pos: A collection of cell indices specifying where a bucket wall
///                  is located.
/// \param sim_out: Class that stores simulation outputs.
/// \param grid: Class that stores information related to the simulation grid.
/// \param tol: Small number used to handle numerical approximation errors.
void UpdateBody(std::vector<std::vector<int>> area_pos, SimOut* sim_out,
    Grid grid, float tol);

/// \brief This function updates the bucket position in `body` at the
///        coordinates (`ii`, `jj`).
///
/// \param sim_out: Class that stores simulation outputs.
/// \param ii: Index of the considered position in the X direction.
/// \param jj: Index of the considered position in the Y direction.
/// \param min_h: Minimum height of the bucket. [m]
/// \param max_h: Maximum height of the bucket. [m]
/// \param tol: Small number used to handle numerical approximation errors.
void IncludeNewBodyPos(SimOut* sim_out, int ii, int jj, float min_h,
    float max_h, float tol);

}  // namespace soil_simulator
