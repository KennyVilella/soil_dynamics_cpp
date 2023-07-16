/*

*/
#ifndef SOIL_DYNAMICS_H
#define SOIL_DYNAMICS_H

namespace soil_simulator
{
    class soil_dynamics
    {
    public:
        void init();
        void step();
        void check();
        void write_outputs();
    };
}
#endif
