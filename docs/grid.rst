.. _grid:

Documentation for the simulation grid
=====================================

The simulation takes place on a rectangular 3D grid made of rectangular prism cells.
An illustration of the 3D grid is given below along with the representation of a grid element.

.. image:: _asset/grid.png

The grid is created by the :code:`Grid` class by providing the maximum size in the XYZ directions (:code:`grid_size_x`, :code:`grid_size_y`, and :code:`grid_size_z`, respectively) and the size of a grid element (:code:`cell_size_xy` and :code:`cell_size_z`).

Grid elements
-------------

The grid elements have equal size in the X and Y direction (:code:`cell_size_xy_`), while their height (:code:`cell_size_z_`) should be lower or equal than their lateral extent.
By convention, the origin of each cell is assumed to be in the center of their top surface (red point in the illustration).
This is particularly important when associating 3D positions to cell indices.

Grid geometry
-------------

The extent of the grid in meter is:
- [:code:`-grid_size_x`, :code:`grid_size_x`] in the X direction, corresponding to :code:`2 * half_length_x_ + 1` grid elements.
- [:code:`-grid_size_y`, :code:`grid_size_y`] in the Y direction, corresponding to :code:`2 * half_length_y_ + 1` grid elements.
- [:code:`-grid_size_z`, :code:`grid_size_z`] in the Z direction, corresponding to :code:`2 * half_length_z_ + 1` grid elements.
The grid is therefore centered in 0.
Note that the the right-hand rule is used so that the Z direction is pointing upward.
