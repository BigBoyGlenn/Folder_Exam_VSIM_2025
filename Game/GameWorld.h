#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "../Game/Terrain.h"
#include "../ECS/Components/Physics.h"
#include "../ECS/Components/CollisionSystem.h"
#include "../ECS/Entity/EntityManager.h"
#include "../ECS/Components/tracing.h"

#include <memory>

namespace bbl
{

class GameWorld
{
public:
    GameWorld();

    void Setup();
    void update(float dt);


    Terrain* getTerrain() const { return m_terrain.get(); }
    bool isTerrainLoaded() const { return m_terrainLoaded; }
    void setTerrainEntity(EntityID terrainID) {
        if (m_collisionSystem) {
            m_collisionSystem->setTerrainEntity(terrainID);
        }
    }
    void initializeSystems(EntityManager* entityManager, GPUResourceManager* gpuResources);
    void setPlaying(bool playing){m_isPlaying=playing;}

private:
    std::unique_ptr<Terrain> m_terrain;
    std::unique_ptr<PhysicsSystem> m_physicsSystem;
    std::unique_ptr<CollisionSystem> m_collisionSystem;
    std::unique_ptr<Tracing> m_tracing;

    bool m_terrainLoaded{false};
    bool m_isPlaying = false;
};

} // namespace bbl

#endif // GAMEWORLD_H
