#pragma once

#include <GameEngine/Core/MathTypes.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

namespace GameEngine {
    // Hack for correct(LH like Unity3D) coordinate system specific to opengl
    const glm::mat4 P = glm::scale(glm::vec3(1,-1,1));
    
    class Transform {
    public:
        enum Space { WORLD, SELF };

        inline constexpr Transform* GetParent() const { return parent; }
        void SetParent(Transform* p){
            if (p == nullptr) {
                if (parent != nullptr) {
                    parent->children.erase(std::find(parent->children.begin(), parent->children.end(), this));
                    parent == nullptr;
                }
            } else {
                if (std::find(p->children.begin(), p->children.end(), this) == p->children.end()){
                    p->children.push_back(this);
                    parent = p;
                }
            }
        }

        inline constexpr void Translate(M::Vec3 xyz, Space relativeTo = Space::SELF){
            position += relativeTo == Space::SELF ? xyz : TransformPoint(xyz, Space::SELF);
        }
        
        constexpr void Rotate(M::Vec3 eulerRads, Space relativeTo = Space::SELF){
            if (relativeTo == Space::SELF)
                rotation = M::QuatF(eulerRads) * rotation;
            else {
                // TODO
                //glm::vec3 const forward = {1.0f, 0.0f, 0.0f};
            }
        }

        inline Transform* GetChild(int idx) const { return children[idx]; }
        inline const size_t GetChildCount() const { return children.size(); }
        
        inline M::Vec3 GetWorldPosition() const { return TransformPoint(position, Space::WORLD); }
        inline M::Vec3 GetRotationEuler() const { return glm::eulerAngles(rotation); }

        M::Mat4 GetMatrix() const { 
            auto m = GetLocalMatrix();
            if (parent != nullptr) m = parent->GetMatrix() * m;
            return m;
        }

        inline M::Mat4 GetLocalMatrix() const { 
            return  glm::translate(P, position) * glm::toMat4(rotation) * glm::scale(P, scale);
        }

        /**
         * Transforms point from local space to wold space and vice versa
         * RelativeTo WORLD => Local to World (position is assumed to be in local space)
         * RelativeTo SELF  => World to local (position is assumed to be in world space) (Inverse Transform)
         * If transform has no parent World and local point coincide.
         */
        M::Vec3 TransformPoint(M::Vec3 pnt, Space transformTo = Space::WORLD) const { 
            auto pos = M::Vec4(pnt, 1);
            if (transformTo == Space::WORLD){
                pos = GetMatrix() * pos;
            } else {
                pos = glm::inverse(GetMatrix()) * pos;
            }
            return M::Vec3(pos);
        }

        /**
         * Transforms direction from local space to wold space and vice versa
         * RelativeTo WORLD => Local to World (direction is assumed to be in local space)
         * RelativeTo SELF  => World to local (direction is assumed to be in world space) (Inverse Transform)
         * If transform has no parent World and local directions coincide.
         */
        M::Vec3 TransformDirection(M::Vec3 direction, Space transformTo = Space::WORLD) const { 
            if (transformTo == Space::WORLD){
                return glm::mat3(GetMatrix()) * direction;
            } else {
                return glm::inverse(glm::mat3(GetMatrix())) * direction;
            }
        }

        M::Vec3 position = { 0.0f, 0.0f, 0.0f};
        M::Vec3 scale = { 1.0f, 1.0f, 1.0f};
        M::QuatF rotation = glm::quat(1,0,0,0);
    private:
        Transform* parent = nullptr;
        std::vector<Transform*> children;
    };
}