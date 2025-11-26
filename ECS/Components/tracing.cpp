// tracing.cpp - Implementation
#include "tracing.h"
#include <algorithm>
#include <cmath>

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

            // Keep only last maxPoints
            if (trace->positions.size() > trace->maxPoints)
            {
                trace->positions.erase(trace->positions.begin());
            }

            trace->timeSinceLastSample = 0.0f;
            trace->needsUpdate = true;
        }

        // Update mesh if enough points
        if (trace->needsUpdate && trace->positions.size() >= 4)
        {
            updateTraceMesh(entity);
            trace->needsUpdate = false;
        }
    }
}

void Tracing::updateTraceMesh(EntityID entity)
{
    Trace* trace = m_entityManager->getComponent<Trace>(entity);
    if (!trace || trace->positions.size() < 4) return;

    // Evaluate B-spline curve from control points
    std::vector<glm::vec3> curvePoints = evaluateBSpline(trace->positions);

    if (curvePoints.size() < 2) return;

    // Create mesh data
    MeshData lineMesh;
    lineMesh.vertices.clear();
    lineMesh.indices.clear();

    // Create vertices from B-spline curve points
    for (size_t i = 0; i < curvePoints.size(); i++)
    {
        Vertex v;
        v.pos = curvePoints[i];

        // Gradient color
        float t = static_cast<float>(i) / static_cast<float>(curvePoints.size() - 1);
        v.color = glm::vec3(1.0f, t, 0.0f);
        v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        v.texCoord = glm::vec2(0.0f);

        lineMesh.vertices.push_back(v);
    }

    // Create line indices
    for (size_t i = 0; i < curvePoints.size() - 1; i++)
    {
        lineMesh.indices.push_back(i);
        lineMesh.indices.push_back(i + 1);
    }

    // Release old mesh
    if (trace->meshResourceID != 0)
    {
        m_gpuResources->releaseMeshResources(trace->meshResourceID);
    }

    // Upload new mesh
    trace->meshResourceID = m_gpuResources->uploadMesh(lineMesh);
}

std::vector<glm::vec3> Tracing::evaluateBSpline(const std::vector<glm::vec3>& controlPoints)
{
    std::vector<glm::vec3> result;

    int n = controlPoints.size();
    if (n < m_splineDegree + 1) return result;

    // Knot vector
    std::vector<float> knots = generateKnotVector(n, m_splineDegree);

    // Curve points
    int numSegments = (n - m_splineDegree) * m_splineSegments;

    // Regular intervals
    float tMin = knots[m_splineDegree];
    float tMax = knots[n];
    float tStep = (tMax - tMin) / numSegments;

    for (int i = 0; i <= numSegments; i++)
    {
        float t = tMin + i * tStep;

        // Clamp to valid range
        t = std::max(tMin, std::min(t, tMax - 0.0001f));

        // Use B-spline basis functions
        glm::vec3 point(0.0f);

        for (int j = 0; j < n; j++)
        {
            float basis = basisFunction(j, m_splineDegree, t, knots);
            point += basis * controlPoints[j];
        }

        result.push_back(point);
    }

    return result;
}

float Tracing::basisFunction(int i, int k, float t, const std::vector<float>& knots)
{
    if (k == 0)
    {
        // Base case
        if (knots[i] <= t && t < knots[i + 1]) return 1.0f;
        else return 0.0f;
    }

    // Recursive case
    float left = 0.0f;
    float right = 0.0f;

    float denomLeft = knots[i + k] - knots[i];
    if (denomLeft > 0.0001f)
    {
        left = ((t - knots[i]) / denomLeft) * basisFunction(i, k - 1, t, knots);
    }

    float denomRight = knots[i + k + 1] - knots[i + 1];
    if (denomRight > 0.0001f)
    {
        right = ((knots[i + k + 1] - t) / denomRight) * basisFunction(i + 1, k - 1, t, knots);
    }

    return left + right;
}

std::vector<float> Tracing::generateKnotVector(int numControlPoints, int degree)
{
    int n = numControlPoints;
    int p = degree;
    int m = n + p + 1;

    std::vector<float> knots(m);

    // Clamped B-spline knot vector
    // First (p+1) knots are 0
    for (int i = 0; i <= p; i++)
    {
        knots[i] = 0.0f;
    }

    // Middle knots are uniformly spaced
    for (int i = p + 1; i < n; i++)
    {
        knots[i] = static_cast<float>(i - p) / static_cast<float>(n - p);
    }

    // Last (p+1) knots are 1
    for (int i = n; i < m; i++)
    {
        knots[i] = 1.0f;
    }

    return knots;
}
