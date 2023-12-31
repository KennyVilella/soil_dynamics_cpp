.. _body_soil:

Documentation for the update of the soil resting on the body
==============================================================

The second task of the simulator is to move the soil resting on the body following its movement.
This is done by the :code:`UpdateBodySoil` function in the :code:`body_soil.cpp` file.

As the body is moving, the soil resting on the it should also be moved.
Because the body frame is not necessarily aligned with the global frame, this task is extremely challenging.
The current implementation has already been refactored and improved, while still not being good enough for our purpose.
Another round of improvements is therefore needed.

Challenges
----------

Before describing the current algorithm, it is important to list the main challenges of this process in order to better understand the choices that have been made.

(a) In some cases, the body could move by a series of small movements.
Each individual movement would not be enough to move the soil by one cell, but the addition of multiple small movements would move the soil by one cell.

(b) Two or more soil columns could potentially merged into one, and then split if the movement is reversed.
This is particularly true when the body is doing a dumping motion.
In that case, the base of the body, where most of the soil should have landed, would be almost vertical such that a lot of soil should merged to a few lateral cells.

(c) When the soil is close to the edge of the body, two body layers could be close enough such that it becomes difficult to assess in which body layer the soil belongs.
In such cases, some errors may occur and the soil may jump from the inside to the outside of the body, or vice versa.

Note that (a) means that the last body movement is not enough to asses the position of the body soil.
It is necessary to have information about the history of the body soil.
In the current algorithm, (c) is still a problem.
This is clear as soil falls down from the body when moving it laterally.

Determination of the new soil position
--------------------------------------

In the current implementation, for all soil landing on the body across the simulator, the position of the landing as well as the amount of soil is stored in the :code:`body_soil_pos_` field of the :code:`SimOut` class using the :code:`body_soil` struct.
Note that the landing position is converted into the reference body frame (that is the body frame in its reference pose) in order to be independent of the current position of the body.

The :code:`UpdateBodySoil` function iterates over all the soil in :code:`body_soil_pos_` and converts the landing position stored into the current pose of the body.
This gives the estimated position of the body soil in the current pose of the body.
Unfortunately, this estimated position may not always correspond to an actual position on the body.
To tackle this issue, the eight cells neighbouring the estimated position is investigated and soil is moved to the first body layer found.
If no body layer is found, the soil is moved to the neighbouring cells with the lowest vertical difference compared to the estimated position.
The purpose of this is to ensure the continuity in the body soil movement and reduce the impact of the (c) challenge mentionned in the previous section.

Current limitations and alternatives
------------------------------------

The current implementation has several issues that need to be solved:

* The code becomes significantly slower when a high resolution is used due to the number of soil to track in :code:`body_soil_pos_`.
* The algorithm does not always work.
  In particular, soil tends to jump outside the body when close to a vertical body wall.
* Soil resting on the body seems to always move, even when the body is doing a simple translation outside ground.

An alternative way to deal with the soil resting on the body will be to implement a second grid aligned with the body frame.
This will however introduce different types of problems.
For instance, soil relaxation will become very complicated due to the body rotation.

Overall, it seems better to keep the current approach, while improving the continuity in the body soil movement and reducing the number of soil in :code:`body_soil_pos_`.
