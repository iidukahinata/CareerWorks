/**
* @file    ComponentModule.cpp
* @brief
*
* @date	   2022/11/14 2022îNìxèâî≈
*/

#include "ModuleHelper.h"
#include "Subsystem/Scene/World.h"
#include "Subsystem/Resource/Resources/Audio/AudioClip.h"
#include "Subsystem/Resource/Resources/3DModel/Texture.h"
#include "Subsystem/Resource/Resources/3DModel/Material.h"
#include "Subsystem/Resource/Resources/3DModel/Mesh.h"
#include "Subsystem/Resource/Resources/3DModel/Model.h"
#include "Subsystem/Resource/Resources/Scene/Scene.h"
#include "Subsystem/Scene/Component/Transform.h"
#include "SubSystem/Scene/Component/Components/AudioListener.h"
#include "SubSystem/Scene/Component/Components/AudioSpeaker.h"
#include "SubSystem/Scene/Component/Components/Camera.h"
#include "SubSystem/Scene/Component/Components/Collider.h"
#include "SubSystem/Scene/Component/Components/Light.h"
#include "SubSystem/Scene/Component/Components/PostProcessEffect.h"
#include "SubSystem/Scene/Component/Components/RenderObject.h"
#include "SubSystem/Scene/Component/Components/RigidBody.h"
#include "SubSystem/Scene/Component/Components/Script.h"

#include "SubSystem/Renderer/PostEffect/Bloom.h"

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(PlayOneShotOverloads, PlayOneShot, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(AddForceOverloads, AddForce, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(AddTorqueOverloads, AddTorque, 1, 2)

void SetUpComponentModule()
{
    PY_CLASS(Transform)
        PY_CLASS_DEF_INIT(GameObject*)
        PY_CLASS_MAKE_GETSET("position", Transform, GetPosition, PY_COPY_CONST_REF, SetPosition)
        PY_CLASS_MAKE_GETSET("rotation", Transform, GetRotation, PY_COPY_CONST_REF, SetRotation)
        PY_CLASS_MAKE_GETSET("scale", Transform, GetScale, PY_COPY_CONST_REF, SetScale)
        PY_CLASS_MAKE_GET("worldPosition", Transform, GetWoldPosition, PY_COPY_CONST_REF)
        PY_CLASS_MAKE_GET("localMatrix", Transform, GetLocalMatrix, PY_BY_VALUE)
        PY_CLASS_MAKE_GET("worldMatrix", Transform, GetWorldMatrix, PY_BY_VALUE)
        PY_CLASS_MAKE_GET("right", Transform, GetRight, PY_COPY_CONST_REF)
        PY_CLASS_MAKE_GET("up", Transform, GetUp, PY_COPY_CONST_REF)
        PY_CLASS_MAKE_GET("forward", Transform, GetForward, PY_COPY_CONST_REF)
        PY_CLASS_DEF(Transform, LockAt)
        PY_CLASS_DEF(Transform, LockTo)
        PY_CLASS_DEF(Transform, HasParent)
        PY_CLASS_DEF(Transform, SetParent)
        PY_CLASS_MAKE_GET("childCount", Transform, GetChildCount, PY_BY_VALUE)
        PY_CLASS_MAKE_GET("children", Transform, GetChildren, PY_COPY_CONST_REF)
        PY_CLASS_MAKE_GET("parent", Transform, GetParent, PY_OPAQUE_PTR)
        PY_CLASS_MAKE_GET("root", Transform, GetRoot, PY_OPAQUE_PTR)
        PY_CLASS_MAKE_GET("gameObject", Transform, GetOwner, PY_OPAQUE_PTR);

    PY_CLASS_NOCOPY(IComponent)
        PY_CLASS_MAKE_GETSET("active", AudioSpeaker, GetActive, PY_BY_VALUE, SetActive)
        PY_CLASS_DEF(IComponent, SetTickThreadSafe)
        PY_CLASS_DEF(IComponent, SetTickEnable)
        PY_CLASS_DEF(IComponent, SetTickPriority)
        PY_CLASS_DEF(IComponent, AddTickPrerequisite)
        PY_CLASS_MAKE_GET("transform", IComponent, GetTransform, PY_NOT_CONST_REF)
        PY_CLASS_MAKE_GET("gameObject", IComponent, GetOwner, PY_OPAQUE_PTR)
        PY_CLASS_MAKE_GET("scene", IComponent, GetScene, PY_OPAQUE_PTR)
        PY_CLASS_MAKE_GET("world", IComponent, GetWorld, PY_OPAQUE_PTR);

    PY_BASE_CLASS_NOCOPY(AudioListener, IComponent)
        PY_CLASS_MAKE_GETSET("velocity", AudioListener, GetVelocity, PY_COPY_CONST_REF, SetVelocity);

    PY_BASE_CLASS_NOCOPY(AudioSpeaker, IComponent)
        PY_CLASS_DEF(AudioSpeaker, Play)
        PY_CLASS_DEF_OVERLOADS(AudioSpeaker, PlayOneShot, PlayOneShotOverloads())
        PY_CLASS_DEF(AudioSpeaker, Pause)
        PY_CLASS_DEF(AudioSpeaker, UnPause)
        PY_CLASS_DEF(AudioSpeaker, Stop)
        PY_CLASS_MAKE_GETSET("clip", AudioSpeaker, GetAudioClip, PY_OPAQUE_PTR, SetAudioClip)
        PY_CLASS_GETSET("mute", AudioSpeaker, GetMute, SetMute)
        PY_CLASS_GETSET("isLoop", AudioSpeaker, GetIsLoop, SetIsLoop)
        PY_CLASS_GETSET("priority", AudioSpeaker, GetPriority, SetPriority)
        PY_CLASS_GETSET("volume", AudioSpeaker, GetVolume, SetVolume)
        PY_CLASS_GETSET("pitch", AudioSpeaker, GetPitch, SetPitch)
        PY_CLASS_GETSET("pan", AudioSpeaker, GetPan, SetPan)
        PY_CLASS_GETSET("is2DMode", AudioSpeaker, GetIs2DMode, SetIs2DMode)
        PY_CLASS_GETSET("maxDistance", AudioSpeaker, GetMaxDistance, SetMaxDistance)
        PY_CLASS_MAKE_GETSET("outPutMatrix", AudioSpeaker, GetOutPutMatrix, PY_COPY_CONST_REF, SetOutPutMatrix);

    PY_BASE_CLASS_NOCOPY(Camera, IComponent)
        PY_CLASS_GETSET("width", Camera, GetWidth, SetWidth)
        PY_CLASS_GETSET("height", Camera, GetHeight, SetHeight)
        PY_CLASS_GETSET("aspect", Camera, GetAspect, SetAspect)
        PY_CLASS_GETSET("fov", Camera, SetFov, SetFov)
        PY_CLASS_GETSET("near", Camera, GetNear, SetNear)
        PY_CLASS_GETSET("far", Camera, GetFar, SetFar)
        PY_CLASS_MAKE_GET("view", Camera, GetViewMatrix, PY_COPY_CONST_REF)
        PY_CLASS_MAKE_GET("projection", Camera, GetProjectionMatrix, PY_COPY_CONST_REF)
        PY_CLASS_MAKE_GET("orthographic", Camera, GetOrthographicMatrix, PY_COPY_CONST_REF);

    PY_ENUM(ShapeType)
        PY_VALUE(Box)
        PY_VALUE(Sphere)
        PY_VALUE(Capsule);

    PY_BASE_CLASS_NOCOPY(Collider, IComponent)
        PY_CLASS_MAKE_GETSET("shapeType", Collider, GetShapeType, PY_COPY_CONST_REF, SetShapeType)
        PY_CLASS_GETSET("isTrigger", Collider, GetTrigger, SetTrigger)
        PY_CLASS_MAKE_GETSET("scale", Collider, GetScale, PY_COPY_CONST_REF, SetScale)
        PY_CLASS_GETSET("restOffset", Collider, GetRestOffset, SetRestOffset)
        PY_CLASS_GETSET("staticFriction", Collider, GetStaticFriction, SetStaticFriction)
        PY_CLASS_GETSET("dynamicFriction", Collider, GetDynamicFriction, SetDynamicFriction)
        PY_CLASS_GETSET("restitution", Collider, GetRestitution, SetRestitution);

    PY_ENUM(LightType)
        PY_VALUE(DirectionalLight)
        PY_VALUE(PointLight)
        PY_VALUE(SpotLight);

    PY_BASE_CLASS_NOCOPY(Light, IComponent)
        PY_CLASS_MAKE_GETSET("lightType", Light, GetLightType, PY_BY_VALUE, SetLightType)
        PY_CLASS_MAKE_GETSET("color", Light, GetColor, PY_COPY_CONST_REF, SetColor)
        PY_CLASS_GETSET("intensity", Light, GetIntensity, SetIntensity)
        PY_CLASS_GETSET("influenceRange", Light, GetInfluenceRange, SetInfluenceRange)
        PY_CLASS_GETSET("angle", Light, GetAngle, SetAngle);

    PostEffect* (PostProcessEffect::*addPostEffect)(StringView) = &PostProcessEffect::AddPostEffect;

    PY_BASE_CLASS_NOCOPY(PostProcessEffect, IComponent)
        .def("AddPostEffect", addPostEffect, PY_OPAQUE_PTR)
        PY_CLASS_DEF(PostProcessEffect, RemovePostEffect)
        PY_CLASS_DEF_POLICIE(PostProcessEffect, GetPostEffect<Bloom>, PY_OPAQUE_PTR)
        PY_CLASS_DEF_POLICIE(PostProcessEffect, FindPostEffect, PY_OPAQUE_PTR);

    PY_BASE_CLASS_NOCOPY(MeshRender, IComponent)
        PY_CLASS_MAKE_GETSET("mesh", MeshRender, GetMesh, PY_OPAQUE_PTR, SetMesh);

    PY_BASE_CLASS_NOCOPY(ModelRender, IComponent)
        PY_CLASS_MAKE_GETSET("model", ModelRender, GetModel, PY_OPAQUE_PTR, SetModel);

    PY_ENUM(BodyType)
        PY_VALUE(Dynamic)
        PY_VALUE(Static);

    PY_ENUM(ForceMode)
        PY_VALUE(Force)
        PY_VALUE(Impulse);

    PY_BASE_CLASS_NOCOPY(RigidBody, IComponent)
        PY_CLASS_MAKE_GETSET("bodyType", RigidBody, GetBodyType, PY_COPY_CONST_REF, SetBodyType)
        PY_CLASS_DEF_OVERLOADS(RigidBody, AddForce, AddForceOverloads())
        PY_CLASS_DEF_OVERLOADS(RigidBody, AddTorque, AddTorqueOverloads())
        PY_CLASS_DEF(RigidBody, AddVelocity)
        PY_CLASS_DEF(RigidBody, AddAngularVelocity)
        PY_CLASS_MAKE_GETSET("velocity", RigidBody, GetVelocity, PY_BY_VALUE, SetVelocity)
        PY_CLASS_MAKE_GETSET("angularVelocity", RigidBody, GetAngularVelocity, PY_BY_VALUE, SetAngularVelocity)
        PY_CLASS_GETSET("mass", RigidBody, GetMass, SetMass)
        PY_CLASS_GETSET("useGravity", RigidBody, UseGravity, SetUseGravity)
        PY_CLASS_GETSET("isKinematic", RigidBody, IsKinematic, SetKinematic)
        PY_CLASS_GET("isDynamic", RigidBody, IsDynamic)
        PY_CLASS_MAKE_GETSET("positionLock", RigidBody, GetPositionLock, PY_COPY_CONST_REF, SetPositionLock)
        PY_CLASS_MAKE_GETSET("rotationLock", RigidBody, GetRotationLock, PY_COPY_CONST_REF, SetRotationLock);

    PY_BASE_CLASS_NOCOPY(Script, IComponent);
}