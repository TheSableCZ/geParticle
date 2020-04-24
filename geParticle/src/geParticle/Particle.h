#pragma once

#include <glm/glm.hpp>
#include <geCore/Updatable.h>

namespace ge
{
    namespace particle
    {
        struct Particle {
            glm::vec3 pos, velocity;
            //float r, g, b, a; // Color
            //float size, angle, weight;

            core::time_unit life;
            bool livingFlag = false;
        };
    }
}
