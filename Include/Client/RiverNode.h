#pragma once

namespace client {
    class RiverNode {
    public:
        typedef enum NodeType {
            Tail,
            Segment,
            Mouth
        } eNodeType;

        RiverNode(float x, float y, float z, eNodeType type);

        float getX() const { return m_x; }
        float getY() const { return m_y; }
        float getZ() const { return m_z; }

    private:
        eNodeType m_type;

        float m_x;
        float m_y;
        float m_z;
    };
}