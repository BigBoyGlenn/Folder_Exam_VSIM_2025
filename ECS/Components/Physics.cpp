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
        physics->acceleration = m_gravity;

        // Project gravity along the terrain slope
        if (collision->isGrounded && m_terrain)
        {
            glm::vec3 terrainNormal = m_terrain->getNormalAt(transform->position.x, transform->position.z);
            glm::vec3 gravityAlongSurface = m_gravity - glm::dot(m_gravity, terrainNormal) * terrainNormal;
            physics->acceleration = gravityAlongSurface;
        }

        // Integrate velocity and position
        physics->velocity += physics->acceleration * dt;
        transform->position += physics->velocity * dt;

        // Rolling friction
        if (collision->isGrounded)
        {
            physics->velocity *= 0.98f;
        }
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

