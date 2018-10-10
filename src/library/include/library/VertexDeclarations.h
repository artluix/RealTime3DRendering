#pragma once
#include "library/Math.h"
#include "library/Color.h"

namespace library
{
    namespace vertex
    {
        struct Position
        {
            math::Vector4f position;

            explicit Position(const math::Vector4f& position)
                : position(position)
            {
            }
        };

        /////////////////////////////////////////////////////////////////

        struct PositionColor
        {
            math::Vector4f position;
            Color color;

            explicit PositionColor(const math::Vector4f& position, const Color& color)
                : position(position)
                , color(color)
            {
            }
        };

        /////////////////////////////////////////////////////////////////

        struct PositionTexture
        {
            math::Vector4f position;
            math::Vector2f textureCoordinates;

            explicit PositionTexture(const math::Vector4f& position, const math::Vector2f& textureCoordinates)
                : position(position)
                , textureCoordinates(textureCoordinates)
            {
            }
        };

        /////////////////////////////////////////////////////////////////

        using PositionSize = PositionTexture;

        /////////////////////////////////////////////////////////////////

        struct PositionNormal
        {
            math::Vector4f position;
            math::Vector3f normal;

            explicit PositionNormal(const math::Vector4f& position, const math::Vector3f& normal)
                : position(position)
                , normal(normal)
            {
            }
        };

        /////////////////////////////////////////////////////////////////

        struct PositionTextureNormal
        {
            math::Vector4f position;
            math::Vector2f textureCoordinates;
            math::Vector3f normal;

            explicit PositionTextureNormal(
                const math::Vector4f& position,
                const math::Vector2f& textureCoordinates,
                const math::Vector3f& normal
            )
                : position(position)
                , textureCoordinates(textureCoordinates)
                , normal(normal)
            {
            }
        };

        /////////////////////////////////////////////////////////////////

        struct SkinnedPositionTextureNormal
        {
            math::Vector4f position;
            math::Vector2f textureCoordinates;
            math::Vector3f normal;
            math::Vector4u boneIndices;
            math::Vector4f boneWeights;

            explicit SkinnedPositionTextureNormal(
                const math::Vector4f& position,
                const math::Vector2f& textureCoordinates,
                const math::Vector3f& normal,
                const math::Vector4u& boneIndices,
                const math::Vector4f& boneWeights
            )
                : position(position)
                , textureCoordinates(textureCoordinates)
                , normal(normal)
                , boneIndices(boneIndices)
                , boneWeights(boneWeights)
            {
            }
        };
    } // namespace vertex
} // namespace library
