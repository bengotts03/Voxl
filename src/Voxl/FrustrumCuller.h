//
// Created by Ben Gotts on 24/07/2025.
//

#ifndef FRUSTRUMCULLER_H
#define FRUSTRUMCULLER_H

#include <glm/glm.hpp>
#include "src/Core/Camera.h"
#include "BoundingBox.h"

class FrustrumCuller {
public:
    struct Plane {
        glm::vec3 Normal = { 0.0f, 1.0f, 0.0f };
        float Distance = 0.0f;
    };

    struct Frustrum {
        Plane TopFace;
        Plane BottomFace;

        Plane RightFace;
        Plane LeftFace;

        Plane FarFace;
        Plane NearFace;
    };

    FrustrumCuller(Camera& cam, float aspectRatio, float fov, float zNear, float zFar);
    ~FrustrumCuller();

    bool CubeInFrustum(BoundingBox* bounds);
    bool IsOnOrForwardPlane(const Plane& plane, const BoundingBox& bounds) const;
private:
    Camera _camera;
    Frustrum _frustrum;
};

#endif //FRUSTRUMCULLER_H
