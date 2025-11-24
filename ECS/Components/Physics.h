#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "../../ECS/Entity/EntityManager.h"
#include "../../Game/Terrain.h"

#include <glm/glm.hpp>

namespace bbl
{

class PhysicsSystem
{
public:
    PhysicsSystem(EntityManager* entityManager);

    void update(float dt);

    void setGravity(const glm::vec3& g);
    void setTerrain(Terrain* terrain);

private:
    EntityManager* m_entityManager;
    Terrain* m_terrain;
    glm::vec3 m_gravity{0.0f, -9.81f, 0.0f};
};
}

#endif // PHYSICSSYSTEM_H
