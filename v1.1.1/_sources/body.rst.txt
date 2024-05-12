.. _body:

Documentation for body objects
==============================

The simulator is able to model the interaction between the soil and multiple body types.
Currently, two body types are supported:

* Excavator `bucket <bucket.html>`_, approximated as a triangular prism.
* Bulldozer `blade <blade.html>`_, approximated as the combination of two rectangular prisms.

For each body type, an associated class is implemented derived from the :code:`Body` base class.
This creates an interface between the simulator and the considered body to make the implementation of new body types easier.
The only functions that are specific to the body type are :code:`CalcBodyPos` and :code:`WriteBody` as they depends on the body shape.

.. toctree::
   :maxdepth: 1
   :hidden:

   Introduction <self>
   Bucket <bucket>
   Blade <blade>
   Body position <body_pos>
