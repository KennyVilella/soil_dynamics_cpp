.. _body_pos:

Determination of the body position
==================================

The first task of the simulator is to determine the position of the simulated body within the 3D grid.
This is done by the :code:`CalcBodyPos` function in the :code:`body_pos.cpp` file.
Specifically, it aims to identify, for each XY position in the grid, the minimum and maximum height of the body walls if present at that location.
The algorithm used for this task has been optimized for performance, resulting in an efficient implementation.
However, due to the optimization, the algorithm may appear complex or unintuitive.

The basic idea under the implementation is to decompose the body into its different rigid surfaces, and to identify for each XY position of the grid the minimum and maximum height of the given body wall.
The results for the various surfaces are aggregated and sorted.
It is then straightforward to determine for each XY position the minimum and maximum height of the body using this information.
Note that only triangular and rectangular surfaces are currently supported.

A different implementation is present for rectangular and triangular surfaces.
However, as the algorithm behind these two implementations is almost identical, it can be described independently of the type of surface considered.
The position of the body wall is determined using a three-steps process:

* Each XY position within the bounding box of the body wall is decomposed into the reference vectors of the considered surface.
  This allows to easily determine whether a given XY position falls inside or outside the projection of the body wall into the horizontal plane.
* For each XY position (:code:`ii`, :code:`jj`) within the projection of the body wall into the horizontal plane, the height of the body wall at the cell corner (:code:`ii + 0.5`, :code:`jj + 0.5`) is calculated and added to the tally.
  By iterating over all the cells, the algorithm ensures that the height of the body is calculated at the 4 corners of each cell within the projection of the body wall into the horizontal plane.
  This is crucial because the minimum and maximum height of a plane intersecting laterally a cell is occurring necessarily at one of the cell's corner.
* An issue of this algorithm is that it does not handle the case where the body wall is either not fully intersecting the cell or intersecting vertically the cell.
  Those cases are handled separately by identifying all the cells where the edges of the body wall are located.

The algorithm to identify all the cells that lie on the edge of a body wall follow the implementations proposed by Amanatides and Woo in the article "A Fast Voxel Traversal Algorithm for Ray Tracing".
The reader is invited to read this article for a detailed explanation of the algorithm, here only a general description will be given.
In this implementation, the gradient of the straight line is used to determine how long it is necessary to travel along the line before to cross a cell boundary in the three directions.
Knowing this information, it is then possible to travel along the line while identifying all the cell boundaries that are crossed.
