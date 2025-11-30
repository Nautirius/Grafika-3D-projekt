//#include "Camera.h"
//
//
//// convert spherical coords + offsets to world position
//Vec3 Camera::sphericalToCartesian() const {
//    float radYaw = yaw * std::numbers::pi / 180.0f;
//    float radPitch = pitch * std::numbers::pi / 180.0f;
//    Vec3 p;
//    p.x = position.x + distance * std::cos(radPitch) * std::cos(radYaw);
//    p.y = position.y + distance * std::sin(radPitch);
//    p.z = position.z + distance * std::cos(radPitch) * std::sin(radYaw);
//    return p;
//}