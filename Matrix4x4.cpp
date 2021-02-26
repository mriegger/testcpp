#include "Matrix4x4.h"
#include <cassert>

namespace Matrix4x4
{
    // From Lumberyard 
    float4x4 MakePerspectiveFovMatrixRH(float fovY, float aspectRatio, float nearDist, float farDist, bool reverseDepth)
    {
        float4x4 out;
        constexpr float FloatEpsilon = 1e-9f;

        assert(nearDist > 0.0f);// , "near distance should be greater than 0.f");
        assert(farDist > nearDist);// , "far distance should be greater than near");
        assert(fovY > FloatEpsilon && aspectRatio > FloatEpsilon);// , "Both field of view in y direction and aspect ratio must be greater than 0");
        if (!(nearDist > 0 && farDist > nearDist && fovY > FloatEpsilon && aspectRatio > FloatEpsilon))
        {
            for (int i = 0; i < 4; ++i)
            {
                out.SetRow(i, 0, 0, 0, 0);
            }
            return out;
        }
        float sinFov = sinf(0.5f * fovY);
        float cosFov = cosf(0.5f * fovY);

        float yScale = cosFov / sinFov; //cot(fovY/2)
        float xScale = yScale / aspectRatio;
        if (reverseDepth)
        {
            std::swap(nearDist, farDist);
        }

        // x right, y up, negative z forward
        out.SetRow(0, xScale, 0.f, 0.f, 0.f);
        out.SetRow(1, 0.f, yScale, 0.f, 0.f);
        out.SetRow(2, 0.f, 0.f, farDist / (nearDist - farDist), nearDist * farDist / (nearDist - farDist));
        out.SetRow(3, 0.f, 0.f, -1.f, 0.f);

        return out;
    }
}