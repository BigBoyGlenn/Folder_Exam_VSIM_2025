#include "GameWorld.h"
#include "../Editor/MainWindow.h"

bbl::GameWorld::GameWorld()
{
    m_terrain = std::make_unique<Terrain>();
}


void bbl::GameWorld::Setup()
{
    if (m_terrain->loadFromPointCloud("../../Assets/Mapping/lasdata.txt"))
    {
        m_terrain->generateMeshFromPointCloud();
        m_terrainLoaded = true;
        qDebug() << "Point cloud terrain loaded successfully!";
    } else
    {
        qWarning() << "Failed to load point cloud terrain!";
    }
}

void bbl::GameWorld::initializeSystems(EntityManager* entityManager, GPUResourceManager *gpuResources)
{
    if (!entityManager) {
        qWarning() << "Cannot initialize systems: EntityManager is null!";
        return;
    }
    if(!gpuResources)
    {
        qWarning() << "Cannot initialize systems: GPUResourceManager is null!";
        return;
    }

    // Physics System
    m_physicsSystem = std::make_unique<PhysicsSystem>(entityManager);
    m_physicsSystem->setGravity(glm::vec3(0.0f, -9.81f, 0.0f));
    m_physicsSystem->setTerrain(m_terrain.get());

    // Collision System
    m_collisionSystem = std::make_unique<CollisionSystem>(entityManager, m_terrain.get());
    m_collisionSystem->setTerrainCollisionEnabled(true);
    m_collisionSystem->setEntityCollisionEnabled(true);

    // Tracing system
   m_tracing = std::make_unique<Tracing>(entityManager, gpuResources);
}

void bbl::GameWorld::update(float dt)
{
    if(!m_isPlaying) return;

    if (m_physicsSystem) {
        m_physicsSystem->update(dt);
    }
    if (m_collisionSystem) {
        m_collisionSystem->update(dt);
    }
    if (m_tracing){
        m_tracing->update(dt);
    }
}
