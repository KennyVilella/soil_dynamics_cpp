Documentation for soil_dynamics_cpp
===================================

.. note::

    A Julia version of this simulator is available `here`_.

This soil dynamics simulator is a fast first-order model designed to simulate soil displacement caused by the movement of a rigid body.
Currently, the only body supported by this simulator is the typical bucket used by hydraulic excavators.


While excavator buckets have complex shapes to efficiently dig soil, here, for the sake of performance, the bucket is approximated as a simple triangular prism, which is a good representation for its overall shape.
A detailed description of the model used for the bucket is available on the `Bucket <bucket.html>`_ page of this documentation.


In this simulator, the movement of soil is modelled using a cellular automata approach on a regular 3D grid.
The simulation grid is composed of rectangular cells with equal length in the X and Y directions, and a height that is less than or equal to the cell's length (more details are available on the `Grid <grid.html>`_ page of this documentation).
The fundamental idea of cellular automata is to decompose a simulation grid into cells that can have different states.
In this case, the state corresponds to soil, body or empty.
These cells can be moved at every step of the simulation depending on a set of rules.


At the start of each step, the body cells are assigned following the location of the body, and only soil cells are allowed to move.
There are three types of movement for soil cells:


* **Movement of body soil**.
  Soil on the body should move following the body movement.
  This movement is however challenging because the body does not necessarily follows the global grid.
  A best effort is made to track the soil on the body accurately as described in the `Body soil <body_soil.html>`_ page of this documentation.
* **Movement of intersecting soil cells**.
  Intersecting cells refer to cells where both soil and body are present due to the body movement.
  The soil in these intersecting cells has to be moved in response to the body movement.
  The movement of intersecting soil cells follows a complex set of rules that is described in the `Intersecting cells <intersecting_cells.html>`_ page of this documentation.
* **Soil relaxation**.
  This refers to the natural movement of soil.
  Here, it is assumed that soil with a slope greater than the repose angle is unstable and should avalanche to reach a configuration where the slope is equal to the repose angle.
  However, implementing this simple model becomes challenging due to the interaction between soil and the body.
  The set of rules applied for soil relaxation is described in the `Relax <relax.html>`_ page of this documentation.


Please note that the terrain must be updated every time the body moves by more than one cell.
This ensures that the simulator keeps track of how the soil should be moved accurately.


In addition to the core functionalities, the classes used in this simulator are described in the `Types <types.html>`_ page of this documentation.
These classes provide a safe way of passing input arguments to the simulator's public functions, simplifying and accelerating the implementation.
Utility functions are described in the `Utilility <utils.html>`_ page of this documentation.
In particular, the functions used to check the outputs of the simulator or to write the outputs into csv files are described.
An `API <api.html>`_ documentation is available with information on all classes and functions. 

.. toctree::
   :maxdepth: 1
   :hidden:

   Home <self>
   Types <types>
   Grid <grid>
   Bucket <bucket>
   Body soil <body_soil>
   Intersecting cells <intersecting_cells>
   Relax <relax>
   Utility <utils>
   API documentation <api>

.. _here: https://github.com/KennyVilella/soil_dynamics_julia
