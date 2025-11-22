#include "Terrain.h"
#include "../External/stb_image.h"

#include <fstream>
#include <iostream>
#include <cmath>
#include <QDebug>
#include <sstream>
//#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>


//--------------- Constructor / Destructor -----------------
Terrain::Terrain() : m_width(0), m_height(0),
    m_heightScale(0.02f), m_gridSpacing(0.2f), m_heightPlacement(-5.0f)
{}

Terrain::~Terrain() {}

//----------- Load from point cloud txt file ---------------
bool Terrain::loadFromPointCloud(const std::string& filepath)
{
    m_points.clear();

    std::ifstream file(filepath);
    if (!file.is_open())
    {
        qWarning() << "Failed to open point cloud file:" << QString::fromStdString(filepath);
        return false;
    }

    std::string line;
    std::getline(file, line);
    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        std::istringstream ss(line);
        Point p;
        ss >> p.pos.x >> p.pos.y >> p.pos.z;
        if (ss.fail())
        {
            qWarning() << "Failed to parse point cloud line:" << QString::fromStdString(line);
            continue;
        }
        m_points.push_back(p);
    }

    file.close();

    qDebug() << "Loaded point cloud with" << m_points.size() << "points.";
    return !m_points.empty();
}

//----------- Generate mesh from point cloud -------------------
void Terrain::generateMeshFromPointCloud() {
    // Determine bounds
    float minX = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxZ = std::numeric_limits<float>::lowest();
    for (auto& p : m_points) {
        minX = std::min(minX, p.pos.x);
        minZ = std::min(minZ, p.pos.z);
        maxX = std::max(maxX, p.pos.x);
        maxZ = std::max(maxZ, p.pos.z);
    }

    int gridWidth = static_cast<int>((maxX - minX) / m_gridSpacing) + 1;
    int gridHeight = static_cast<int>((maxZ - minZ) / m_gridSpacing) + 1;
    m_width = gridWidth;
    m_height = gridHeight;
    m_heightData.clear();
    m_heightData.reserve(m_width * m_height);
    std::vector<std::vector<float>> heightGrid(gridHeight, std::vector<float>(gridWidth, m_heightPlacement));

    // Populate grid
    std::vector<std::vector<bool>> hasPoint(gridHeight, std::vector<bool>(gridWidth, false));

    for (auto& p : m_points)
    {
        int gx = static_cast<int>((p.pos.x - minX) / m_gridSpacing);
        int gz = static_cast<int>((p.pos.z - minZ) / m_gridSpacing);

        if (!hasPoint[gz][gx])
        {
            heightGrid[gz][gx] = p.pos.y;
            hasPoint[gz][gx] = true;
        } else
        {
            heightGrid[gz][gx] = std::max(heightGrid[gz][gx], p.pos.y);
        }

    }

    // Fill empty cells with average of neighbors
    for (int z = 0; z < gridHeight; z++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            if (!hasPoint[z][x])
            {
                float sum = 0;
                int count = 0;
                for (int dz = -1; dz <= 1; dz++)
                {
                    for (int dx = -1; dx <= 1; dx++)
                    {
                        int nx = x + dx;
                        int nz = z + dz;
                        if (nx >= 0 && nx < gridWidth && nz >= 0 && nz < gridHeight && hasPoint[nz][nx])
                        {
                            sum += heightGrid[nz][nx];
                            count++;
                        }
                    }
                }
                if (count > 0)
                {
                    heightGrid[z][x] = sum / count;
                    hasPoint[z][x] = true;
                }
            }
        }
    }


    // Create vertices and indices
    m_vertices.clear();
    m_indices.clear();
    for (int z = 0; z < gridHeight; z++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            Vertex v;
            v.pos = glm::vec3(minX + x * m_gridSpacing, heightGrid[z][x], minZ + z * m_gridSpacing);
            v.texCoord = glm::vec2(float(x)/float(gridWidth-1), float(z)/float(gridHeight-1));
            v.color = glm::vec3(0.4f, 0.8f, 0.4f);
            m_vertices.push_back(v);
            m_heightData.push_back(heightGrid[z][x]);
        }
    }

    for (int z = 0; z < gridHeight - 1; z++)
    {
        for (int x = 0; x < gridWidth - 1; x++)
        {
            int topLeft = x + z * gridWidth;
            int topRight = topLeft + 1;
            int bottomLeft = topLeft + gridWidth;
            int bottomRight = bottomLeft + 1;
            // Two triangles per quad (CCW)
            m_indices.push_back(topLeft); m_indices.push_back(bottomLeft); m_indices.push_back(bottomRight);
            m_indices.push_back(topLeft); m_indices.push_back(bottomRight); m_indices.push_back(topRight);

        }
    }

    calculateNormals();
}


//---------------- Normals for heightmap mesh -----------------
void Terrain::calculateNormals()
{
    // Reset normals
    for (auto& v : m_vertices)
    {
        v.normal = glm::vec3(0.0f);
    }

    // Compute triangle normals
    for (size_t i = 0; i < m_indices.size(); i += 3)
    {
        uint32_t i0 = m_indices[i];
        uint32_t i1 = m_indices[i + 1];
        uint32_t i2 = m_indices[i + 2];

        glm::vec3 v0 = m_vertices[i0].pos;
        glm::vec3 v1 = m_vertices[i1].pos;
        glm::vec3 v2 = m_vertices[i2].pos;

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 normal = glm::cross(edge1, edge2);

        m_vertices[i0].normal += normal;
        m_vertices[i1].normal += normal;
        m_vertices[i2].normal += normal;
    }

    // Normalize
    for (auto& v : m_vertices)
    {
        if (glm::length(v.normal) > 0.0f)
            v.normal = glm::normalize(v.normal);
        else
            v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    }
}

//--------------- Barycentric interpolation -----------------------
float Terrain::barycentric(const glm::vec2& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) const
{
    glm::vec2 a2D(a.x, a.z);
    glm::vec2 b2D(b.x, b.z);
    glm::vec2 c2D(c.x, c.z);
    glm::vec2 v0 = b2D - a2D;
    glm::vec2 v1 = c2D - a2D;
    glm::vec2 v2 = p - a2D;

    float d00 = glm::dot(v0, v0);
    float d01 = glm::dot(v0, v1);
    float d11 = glm::dot(v1, v1);
    float d20 = glm::dot(v2, v0);
    float d21 = glm::dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    if (denom == 0.0f) return a.y;

    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;

    return u * a.y + v * b.y + w * c.y;
}


//-------------- Terrain center ---------------------
glm::vec3 Terrain::getCenter() const
{
    return glm::vec3(0.0f); // Centered at origin
}

//------------ Terrain height ------------------------
float Terrain::getHeightAt(float worldX, float worldZ, const glm::vec3& terrainPosition) const
{
    if (m_vertices.empty() || m_heightData.empty())
        return m_heightPlacement;

    // Centering the terrain
    float offsetX = -m_width * m_gridSpacing / 2.0f;
    float offsetZ = -m_height * m_gridSpacing / 2.0f;

    // Convert world coordinates to local terrain coordinates
    float localX = worldX - terrainPosition.x - offsetX;
    float localZ = worldZ - terrainPosition.z - offsetZ;

    // Determine which grid cell we are in
    int gridX = static_cast<int>(std::floor(localX / m_gridSpacing));
    int gridZ = static_cast<int>(std::floor(localZ / m_gridSpacing));

    // Clamp to edges
    gridX = glm::clamp(gridX, 0, m_width - 2);
    gridZ = glm::clamp(gridZ, 0, m_height - 2);

    // Local coordinates within the cell
    float xCoord = (localX - gridX * m_gridSpacing) / m_gridSpacing;
    float zCoord = (localZ - gridZ * m_gridSpacing) / m_gridSpacing;

    // Get indices of the corners
    int topLeftIndex = gridX + gridZ * m_width;
    glm::vec3 a, b, c;

    // Determine which triangle
    if (xCoord + zCoord <= 1.0f)
    {
        a = m_vertices[topLeftIndex].pos;
        b = m_vertices[topLeftIndex + 1].pos;
        c = m_vertices[topLeftIndex + m_width].pos;
    }
    else
    {
        a = m_vertices[topLeftIndex + 1 + m_width].pos;
        b = m_vertices[topLeftIndex + m_width].pos;
        c = m_vertices[topLeftIndex + 1].pos;
    }

    // Smooth out height on terrain
    return barycentric(glm::vec2(worldX - terrainPosition.x, worldZ - terrainPosition.z), a, b, c);
}
