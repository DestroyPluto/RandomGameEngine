#pragma once

namespace core{
class Entity{
    public:
        Entity();
    private:
    //TODO: these should be vectors
        double m_position;
        double m_rotation;
        double m_scale;
};
}