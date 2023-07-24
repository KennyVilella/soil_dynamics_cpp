/*
This file declares the functions used to calculate the bucket position.

Copyright, 2023,  Vilella Kenny.
*/
#pragma once

namespace soil_simulator {

void CalcBucketPos();
void CalcRectanglePos();
void DecomposeVectorRectangle();
void CalcTrianglePos();
void DecomposeVectorTriangle();
void CalcLinePos();
void UpdateBody();
void IncludeNewBodyPos();

}  // namespace soil_simulator
