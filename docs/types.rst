.. _types:

Documentation for the implemented types
=======================================

Introductory remarks
--------------------

The simulator relies on class and struct to organize and manage the various parameters and properties.
These new types are implemented in the :code:`types.cpp` file and are designed to provide a clear and efficient way of passing input arguments to the simulator's public functions.

To ensure the integrity and validity of the input parameters, the classes are constructed using constructors that perform appropriate checks.
This guarantees that the simulator operates under the assumption of correct and consistent input data.
If incorrect parameters are provided, the user will be alerted through appropriate warnings or error messages.

The current implementation includes four distinct classes:

* :code:`Grid`: Aggregates all the properties related to the simulation grid.
* :code:`Bucket`: Aggregates all the properties associated with the bucket.
* :code:`SimParam`: Aggregates the general parameters and settings for the simulation.
* :code:`SimOut`: Aggregates and manages the output data generated during the simulation.

These classes play a crucial role in organizing and encapsulating the various aspects of the simulator, providing a convenient and structured way to work with the simulation inputs and outputs.

In addition to these classes, the :code:`body_soil` struct is used to store information related to the soil resting on the bucket.
This is used to populate the field :code:`body_soil_pos_` of the :code:`SimOut` class.
