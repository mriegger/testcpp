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

    float4x4 MakeOrthographicMatrixRH(float left, float right, float bottom, float top, float nearDist, float farDist)
    {
        assert(right > left, "right should be greater than left");
        // valid to have matrix invert top/bottom and far/near
        //AZ_Assert(top > bottom, "top should be greater than bottom");
        //AZ_Assert(farDist > nearDist, "far should be greater than near");
        float4x4 out;

        if (!(right > left /*&& top > bottom && farDist > nearDist*/))
        {
            for (int i = 0; i < 4; ++i)
            {
                out.SetRow(i, 0, 0, 0, 0);
            }
            return out;
        }

        out.SetRow(0, 2.f / (right - left), 0.f, 0.f, -(right + left) / (right - left));
        out.SetRow(1, 0.f, 2.f / (top - bottom), 0.f, -(top + bottom) / (top - bottom));
        out.SetRow(2, 0.f, 0.f, 1 / (nearDist - farDist), nearDist / (nearDist - farDist));
        out.SetRow(3, 0.f, 0.f, 0, 1.f);
        return out;
    }

}