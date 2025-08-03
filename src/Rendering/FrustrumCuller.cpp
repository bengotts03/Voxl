//
// Fixed FrustrumCuller.cpp
//

#include "FrustrumCuller.h"
#include <spdlog/spdlog.h>
#include "../Voxl/BoundingBox.h"

FrustrumCuller::FrustrumCuller(Camera& cam, float aspectRatio, float fov, float zNear, float zFar) : _camera(cam)
{
    _frustrum = Frustrum();

    const float halfVSide = zFar * tanf(fov * 0.5f);
    const float halfHSide = halfVSide * aspectRatio;

    glm::vec3 forward = cam.Forward;
    glm::vec3 right = cam.Right;
    glm::vec3 up = cam.Up;
    glm::vec3 position = cam.Position;

    const glm::vec3 frontMultFar = zFar * forward;

    glm::vec3 nearPoint = position + zNear * forward;
    glm::vec3 farPoint = position + frontMultFar;
    _frustrum.NearFace = {forward, glm::dot(forward, nearPoint)};
    _frustrum.FarFace = {-forward, glm::dot(-forward, farPoint)};

    glm::vec3 farTopLeft = position + frontMultFar + up * halfVSide - right * halfHSide;
    glm::vec3 farTopRight = position + frontMultFar + up * halfVSide + right * halfHSide;
    glm::vec3 farBottomLeft = position + frontMultFar - up * halfVSide - right * halfHSide;
    glm::vec3 farBottomRight = position + frontMultFar - up * halfVSide + right * halfHSide;

    glm::vec3 leftNormal = glm::normalize(glm::cross(farBottomLeft - position, farTopLeft - position));
    _frustrum.LeftFace = {leftNormal, glm::dot(leftNormal, position)};

    glm::vec3 rightNormal = glm::normalize(glm::cross(farTopRight - position, farBottomRight - position));
    _frustrum.RightFace = {rightNormal, glm::dot(rightNormal, position)};

    glm::vec3 topNormal = glm::normalize(glm::cross(farTopLeft - position, farTopRight - position));
    _frustrum.TopFace = {topNormal, glm::dot(topNormal, position)};

    glm::vec3 bottomNormal = glm::normalize(glm::cross(farBottomRight - position, farBottomLeft - position));
    _frustrum.BottomFace = {bottomNormal, glm::dot(bottomNormal, position)};
}

FrustrumCuller::~FrustrumCuller() {
}

bool FrustrumCuller::CubeInFrustum(BoundingBox* bounds) {
    bool bottom = IsOnOrForwardPlane(_frustrum.BottomFace, *bounds);
    bool top = IsOnOrForwardPlane(_frustrum.TopFace, *bounds);
    bool left = IsOnOrForwardPlane(_frustrum.LeftFace, *bounds);
    bool right = IsOnOrForwardPlane(_frustrum.RightFace, *bounds);
    bool near = IsOnOrForwardPlane(_frustrum.NearFace, *bounds);
    bool far = IsOnOrForwardPlane(_frustrum.FarFace, *bounds);

    // spdlog::info("Cube In Frustrum: bottom={}, top={}, left={}, right={}, near={}, far={}", bottom, top, left, right, near, far);

    return bottom && top && left && right && near && far;
}

bool FrustrumCuller::IsOnOrForwardPlane(const Plane& plane, const BoundingBox& bounds) const {
    const float intersectionRadius =    bounds.Extents.x * plane.Normal.x +
                                        bounds.Extents.y * plane.Normal.y +
                                        bounds.Extents.z * plane.Normal.z;

    const float distanceToCenter = glm::dot(plane.Normal, bounds.Centre) - plane.Distance;

    return distanceToCenter + intersectionRadius >= 0.0f;
}