#include "tracing.h"

using namespace bbl;

Tracing::Tracing(EntityManager* entityManager, GPUResourceManager* gpuResources)
    : m_entityManager(entityManager)
    , m_gpuResources(gpuResources)
{
}

void Tracing::update(float dt)
{
    if (!m_entityManager) return;

    auto entities = m_entityManager->getEntitiesWith<Trace, Transform>();

    for (auto entity : entities)
    {
        Trace* trace = m_entityManager->getComponent<Trace>(entity);
        Transform* transform = m_entityManager->getComponent<Transform>(entity);

        if (!trace || !transform) continue;

        trace->timeSinceLastSample += dt;

        // Sample position at fixed intervals
        if (trace->timeSinceLastSample >= trace->sampleInterval)
        {
            trace->positions.push_back(transform->position);

            // Keeps only last maxpoints
            if (trace->positions.size() > trace->maxPoints)
            {
                trace->positions.erase(trace->positions.begin());
            }
            trace->timeSinceLastSample = 0.0f;
            trace->needsUpdate = true;
        }

        // Update mesh
        if (trace->needsUpdate && trace->positions.size() >=2)
        {
            updateTraceMesh(entity);
            trace->needsUpdate=false;
        }
    }
}

void Tracing::updateTraceMesh(EntityID entity)
{
    Trace* trace = m_entityManager->getComponent<Trace>(entity);
    if (!trace || trace->positions.size() < 2) return;

    // Clear
    MeshData lineMesh;
    lineMesh.vertices.clear();
    lineMesh.indices.clear();

    // Creates line from position
    for (size_t i=0; i<trace->positions.size(); i++)
    {
        Vertex v;
        v.pos = trace->positions[i];
        v.color = glm::vec3(1.0f, 0.0f, 0.0f);
        v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        v.texCoord = glm::vec2(0.0f);
        lineMesh.vertices.push_back(v);
    }

    // Create line indices
    for(size_t i =0; i<trace->positions.size() -1; i++)
    {
        lineMesh.indices.push_back(i);
        lineMesh.indices.push_back(i+1);
    }

    // Release old mesh
    if(trace->meshResourceID !=0)
    {
        m_gpuResources->releaseMeshResources(trace->meshResourceID);
    }

    // Upload mesh
    trace->meshResourceID = m_gpuResources->uploadMesh(lineMesh);
}
