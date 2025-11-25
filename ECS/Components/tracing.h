#ifndef TRACING_H
#define TRACING_H

#include "../../ECS/Entity/EntityManager.h"
#include "../../Core/Utility/gpuresourcemanager.h"

namespace bbl
{


class Tracing
{
public:
    Tracing(EntityManager* entityManager, GPUResourceManager* gpuResources);

    void update(float dt);
    void updateTraceMesh(EntityID entity);

private:
    EntityManager* m_entityManager;
    GPUResourceManager* m_gpuResources;
};

}
#endif // TRACING_H
