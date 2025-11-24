#include "Physics.h"

using namespace bbl;

PhysicsSystem::PhysicsSystem(EntityManager* entityManager)
    : m_entityManager(entityManager)
{
}

void PhysicsSystem::update(float dt)
{
    if (!m_entityManager) return;

    auto entities = m_entityManager->getEntitiesWith<Physics, Transform, Collision>();

    for (auto entity : entities)
    {
        Physics* physics = m_entityManager->getComponent<Physics>(entity);
        Transform* transform = m_entityManager->getComponent<Transform>(entity);
        Collision* collision = m_entityManager->getComponent<Collision>(entity);

        if (!physics || !transform || !collision) continue;

        // Gravity
        glm::vec3 a = m_gravity;

        // Project gravity along the terrain slope
        if (collision->isGrounded && m_terrain)
        {
            glm::vec3 n = m_terrain->getNormalAt(transform->position.x, transform->position.z);

            float nx=n.x;
            float ny=n.y;
            float nz=n.z;
            float g = glm::length(m_gravity);
            //(9.14)
            a= glm::vec3(g*(nx*ny), g*(ny*ny-1.0f), g*(nz*ny));
        }
        //(9.16)
        physics->velocity = physics->velocity +a*dt;
        //(9.17)
        transform->position=transform->position+physics->velocity*dt;
    }
}

void PhysicsSystem::setGravity(const glm::vec3& g)
{
    m_gravity = g;
}

void PhysicsSystem::setTerrain(Terrain* terrain)
{
    m_terrain = terrain;
}

