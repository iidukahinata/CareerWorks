/**
* @file    ComponentModule.cpp
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
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
        PY_CLASS_MAKE_GETSET("position", Transform, GetPosition, PY_RET_REF, SetPosition)
        PY_CLASS_MAKE_GETSET("rotation", Transform, GetRotation, PY_RET_REF, SetRotation)
        PY_CLASS_MAKE_GETSET("scale", Transform, GetScale, PY_RET_REF, SetScale)
        PY_CLASS_MAKE_GET("worldPosition", Transform, GetWoldPosition, PY_RET_REF)
        PY_CLASS_MAKE_GET("localMatrix", Transform, GetLocalMatrix, PY_BY_VALUE)
        PY_CLASS_MAKE_GET("worldMatrix", Transform, GetWorldMatrix, PY_BY_VALUE)
        PY_CLASS_MAKE_GET("right", Transform, GetRight, PY_RET_REF)
        PY_CLASS_MAKE_GET("up", Transform, GetUp, PY_RET_REF)
        PY_CLASS_MAKE_GET("forward", Transform, GetForward, PY_RET_REF)
        PY_CLASS_DEF(Transform, LockAt)
        PY_CLASS_DEF(Transform, LockTo)
        PY_CLASS_DEF(Transform, HasParent)
        PY_CLASS_DEF(Transform, SetParent)
        PY_CLASS_MAKE_GET("childCount", Transform, GetChildCount, PY_BY_VALUE)
        PY_CLASS_MAKE_GET("children", Transform, GetChildren, PY_RET_REF)
        PY_CLASS_MAKE_GET("parent", Transform, GetParent, PY_RET_REF)
        PY_CLASS_MAKE_GET("root", Transform, GetRoot, PY_RET_REF)
        PY_CLASS_MAKE_GET("gameObject", Transform, GetOwner, PY_RET_REF);

    PY_CLASS_NOCOPY(IComponent)
        PY_CLASS_MAKE_GETSET("active", IComponent, GetActive, PY_BY_VALUE, SetActive)
        PY_CLASS_DEF(IComponent, SetTickThreadSafe)
        PY_CLASS_DEF(IComponent, SetTickEnable)
        PY_CLASS_DEF(IComponent, SetTickPriority)
        PY_CLASS_DEF(IComponent, AddTickPrerequisite)
        PY_CLASS_MAKE_GET("transform", IComponent, GetTransform, PY_RET_REF)
        PY_CLASS_MAKE_GET("gameObject", IComponent, GetOwner, PY_RET_REF)
        PY_CLASS_MAKE_GET("scene", IComponent, GetScene, PY_RET_REF)
        PY_CLASS_MAKE_GET("world", IComponent, GetWorld, PY_RET_REF);

    PY_BASE_CLASS_NOCOPY(IAudioListener, IComponent)
        PY_CLASS_MAKE_GETSET("velocity", IAudioListener, GetVelocity, PY_RET_REF, SetVelocity);

    PY_BASE_CLASS_NOCOPY(IAudioSpeaker, IComponent)
        PY_CLASS_DEF(IAudioSpeaker, Play)
        PY_CLASS_DEF_OVERLOADS(IAudioSpeaker, PlayOneShot, PlayOneShotOverloads())
        PY_CLASS_DEF(IAudioSpeaker, Pause)
        PY_CLASS_DEF(IAudioSpeaker, UnPause)
        PY_CLASS_DEF(IAudioSpeaker, Stop)
        PY_CLASS_MAKE_GETSET("clip", IAudioSpeaker, GetAudioClip, PY_RET_REF, SetAudioClip)
        PY_CLASS_GETSET("mute", IAudioSpeaker, GetMute, SetMute)
        PY_CLASS_GETSET("isLoop", IAudioSpeaker, GetIsLoop, SetIsLoop)
        PY_CLASS_GETSET("priority", IAudioSpeaker, GetPriority, SetPriority)
        PY_CLASS_GETSET("volume", IAudioSpeaker, GetVolume, SetVolume)
        PY_CLASS_GETSET("pitch", IAudioSpeaker, GetPitch, SetPitch)
        PY_CLASS_GETSET("pan", IAudioSpeaker, GetPan, SetPan)
        PY_CLASS_GETSET("is2DMode", IAudioSpeaker, GetIs2DMode, SetIs2DMode)
        PY_CLASS_GETSET("maxDistance", IAudioSpeaker, GetMaxDistance, SetMaxDistance)
        PY_CLASS_MAKE_GETSET("outPutMatrix", IAudioSpeaker, GetOutPutMatrix, PY_RET_REF, SetOutPutMatrix);

    PY_BASE_CLASS_NOCOPY(ICamera, IComponent)
        PY_CLASS_GETSET("width", ICamera, GetWidth, SetWidth)
        PY_CLASS_GETSET("height", ICamera, GetHeight, SetHeight)
        PY_CLASS_GETSET("aspect", ICamera, GetAspect, SetAspect)
        PY_CLASS_GETSET("fov", ICamera, SetFov, SetFov)
        PY_CLASS_GETSET("near", ICamera, GetNear, SetNear)
        PY_CLASS_GETSET("far", ICamera, GetFar, SetFar)
        PY_CLASS_MAKE_GET("view", ICamera, GetViewMatrix, PY_RET_REF)
        PY_CLASS_MAKE_GET("projection", ICamera, GetProjectionMatrix, PY_RET_REF)
        PY_CLASS_MAKE_GET("orthographic", ICamera, GetOrthographicMatrix, PY_RET_REF);

    PY_ENUM(ShapeType)
        PY_VALUE(Box)
        PY_VALUE(Sphere)
        PY_VALUE(Capsule);

    PY_BASE_CLASS_NOCOPY(ICollider, IComponent)
        PY_CLASS_MAKE_GETSET("shapeType", ICollider, GetShapeType, PY_BY_VALUE, SetShapeType)
        PY_CLASS_GETSET("isTrigger", ICollider, GetTrigger, SetTrigger)
        PY_CLASS_MAKE_GETSET("scale", ICollider, GetScale, PY_RET_REF, SetScale)
        PY_CLASS_GETSET("restOffset", ICollider, GetRestOffset, SetRestOffset)
        PY_CLASS_GETSET("staticFriction", ICollider, GetStaticFriction, SetStaticFriction)
        PY_CLASS_GETSET("dynamicFriction", ICollider, GetDynamicFriction, SetDynamicFriction)
        PY_CLASS_GETSET("restitution", ICollider, GetRestitution, SetRestitution);

    PY_ENUM(LightType)
        PY_VALUE(DirectionalLight)
        PY_VALUE(PointLight)
        PY_VALUE(SpotLight);

    PY_BASE_CLASS_NOCOPY(ILight, IComponent)
        PY_CLASS_MAKE_GETSET("lightType", ILight, GetLightType, PY_BY_VALUE, SetLightType)
        PY_CLASS_MAKE_GETSET("color", ILight, GetColor, PY_RET_REF, SetColor)
        PY_CLASS_GETSET("intensity", ILight, GetIntensity, SetIntensity)
        PY_CLASS_GETSET("influenceRange", ILight, GetInfluenceRange, SetInfluenceRange)
        PY_CLASS_GETSET("angle", ILight, GetAngle, SetAngle);

    IPostEffect* (IPostProcessEffect::*addPostEffect)(StringView) = &IPostProcessEffect::AddPostEffect;

    PY_BASE_CLASS_NOCOPY(IPostProcessEffect, IComponent)
        .def("AddPostEffect", addPostEffect, PY_RET_REF)
        PY_CLASS_DEF(IPostProcessEffect, RemovePostEffect)
        PY_CLASS_DEF_POLICIE(IPostProcessEffect, GetPostEffect<Bloom>, PY_RET_REF)
        PY_CLASS_DEF_POLICIE(IPostProcessEffect, FindPostEffect, PY_RET_REF);

    PY_BASE_CLASS_NOCOPY(IMeshRender, IComponent)
        PY_CLASS_MAKE_GETSET("mesh", IMeshRender, GetMesh, PY_RET_REF, SetMesh);

    PY_BASE_CLASS_NOCOPY(IModelRender, IComponent)
        PY_CLASS_MAKE_GETSET("model", IModelRender, GetModel, PY_RET_REF, SetModel);

    PY_ENUM(BodyType)
        PY_VALUE(Dynamic)
        PY_VALUE(Static);

    PY_ENUM(ForceMode)
        PY_VALUE(Force)
        PY_VALUE(Impulse);

    PY_BASE_CLASS_NOCOPY(IRigidBody, IComponent)
        PY_CLASS_MAKE_GETSET("bodyType", IRigidBody, GetBodyType, PY_BY_VALUE, SetBodyType)
        PY_CLASS_DEF_OVERLOADS(IRigidBody, AddForce, AddForceOverloads())
        PY_CLASS_DEF_OVERLOADS(IRigidBody, AddTorque, AddTorqueOverloads())
        PY_CLASS_DEF(IRigidBody, AddVelocity)
        PY_CLASS_DEF(IRigidBody, AddAngularVelocity)
        PY_CLASS_MAKE_GETSET("velocity", IRigidBody, GetVelocity, PY_BY_VALUE, SetVelocity)
        PY_CLASS_MAKE_GETSET("angularVelocity", IRigidBody, GetAngularVelocity, PY_BY_VALUE, SetAngularVelocity)
        PY_CLASS_GETSET("mass", IRigidBody, GetMass, SetMass)
        PY_CLASS_GETSET("useGravity", IRigidBody, UseGravity, SetUseGravity)
        PY_CLASS_GETSET("isKinematic", IRigidBody, IsKinematic, SetKinematic)
        PY_CLASS_GET("isDynamic", IRigidBody, IsDynamic)
        PY_CLASS_MAKE_GETSET("positionLock", IRigidBody, GetPositionLock, PY_RET_REF, SetPositionLock)
        PY_CLASS_MAKE_GETSET("rotationLock", IRigidBody, GetRotationLock, PY_RET_REF, SetRotationLock);

    PY_BASE_CLASS_NOCOPY(IScript, IComponent);
}