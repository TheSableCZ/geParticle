#pragma once

#include <glm/glm.hpp>
#include <geCore/Updatable.h>

namespace ge
{
    namespace particle
    {
        struct Particle {
            glm::vec3 pos, velocity;

            core::time_unit life;
            bool livingFlag = false;
        };
    }
}
