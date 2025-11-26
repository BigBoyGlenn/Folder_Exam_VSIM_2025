#ifndef TRACING_H
#define TRACING_H

#include "../../ECS/Entity/EntityManager.h"
#include "../../Core/Utility/gpuresourcemanager.h"
#include <glm/glm.hpp>
#include <vector>

namespace bbl
{

class Tracing
{
public:
    Tracing(EntityManager* entityManager, GPUResourceManager* gpuResources);

    void update(float dt);
    void updateTraceMesh(EntityID entity);

    // B-spline parameters
    void setSplineSegments(int segments) { m_splineSegments = segments; }
    void setSplineDegree(int degree) { m_splineDegree = degree; }

private:
    EntityManager* m_entityManager;
    GPUResourceManager* m_gpuResources;

    // B-spline configuration
    int m_splineSegments = 10;
    int m_splineDegree = 3;

    // B-spline evaluation
    std::vector<glm::vec3> evaluateBSpline(const std::vector<glm::vec3>& controlPoints);
    float basisFunction(int i, int k, float t, const std::vector<float>& knots);
    std::vector<float> generateKnotVector(int numControlPoints, int degree);
};

} // namespace bbl

#endif // TRACING_H
