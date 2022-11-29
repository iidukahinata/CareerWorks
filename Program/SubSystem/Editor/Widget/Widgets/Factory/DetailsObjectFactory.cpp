/**
* @file	   DetailsObjectFactory.cpp
* @brief
*
* @date	   2022/11/28 2022年度初版
*/


#include "DetailsObjectFactory.h"
#include "SubSystem/Scene/GameObject.h"

// Details Object
#include "../DetailsWidget.h"
#include "../DetailsObject/DetailsObjects/ResourceDetails.h"
#include "../DetailsObject/DetailsObjects/MaterialDetails.h"
#include "../DetailsObject/DetailsObjects/GameObjectDetails.h"
#include "../DetailsObject/DetailsObjects/TransformDetails.h"
#include "../DetailsObject/DetailsObjects/LightDetails.h"
#include "../DetailsObject/DetailsObjects/CameraDetails.h"
#include "../DetailsObject/DetailsObjects/MeshRenderDetails.h"
#include "../DetailsObject/DetailsObjects/ModelRenderDetails.h"
#include "../DetailsObject/DetailsObjects/PostProcessEffectDetails.h"
#include "../DetailsObject/DetailsObjects/AudioSpeakerDetails.h"
#include "../DetailsObject/DetailsObjects/AudioListenerDetails.h"
#include "../DetailsObject/DetailsObjects/ColliderDetails.h"
#include "../DetailsObject/DetailsObjects/RigidBodyDetails.h"
#include "../DetailsObject/DetailsObjects/ScriptDetails.h"
#include "../DetailsObject/DetailsObjects/ShaderDetails.h"

#define CREATE_DETAILS_OBJECT(CLASS, ...) detailsObjects.emplace_back(std::make_unique<CLASS>(detailsWidget, __VA_ARGS__));

Vector<UniquePtr<DetailsObject>> DetailsObjectFactory::Create(DetailsWidget* detailsWidget, GameObject* gameObject) noexcept
{
	Vector<UniquePtr<DetailsObject>> detailsObjects;

	CREATE_DETAILS_OBJECT(GameObjectDetails, gameObject);
	CREATE_DETAILS_OBJECT(TransformDetails , &gameObject->GetTransform());

	// 所持コンポーネント表示用ループ
	for (const auto& component : gameObject->GetAllComponent())
	{
		if (component.second->RequestRemove())
		{
			continue;
		}

		const auto typeData = component.second->GetSuperTypeData();
		switch (typeData.Hash)
		{
		case GET_HASH(ILight)			 : CREATE_DETAILS_OBJECT(LightDetails			 , component.second.get()); break;
		case GET_HASH(ICamera)			 : CREATE_DETAILS_OBJECT(CameraDetails			 , component.second.get()); break;
		case GET_HASH(IMeshRender)		 : CREATE_DETAILS_OBJECT(MeshRenderDetails		 , component.second.get()); break;
		case GET_HASH(IModelRender)		 : CREATE_DETAILS_OBJECT(ModelRenderDetails		 , component.second.get()); break;
		case GET_HASH(IPostProcessEffect): CREATE_DETAILS_OBJECT(PostProcessEffectDetails, component.second.get()); break;
		case GET_HASH(IAudioSpeaker)	 : CREATE_DETAILS_OBJECT(AudioSpeakerDetails	 , component.second.get()); break;
		case GET_HASH(IAudioListener)	 : CREATE_DETAILS_OBJECT(AudioListenerDetails	 , component.second.get()); break;
		case GET_HASH(ICollider)		 : CREATE_DETAILS_OBJECT(ColliderDetails		 , component.second.get()); break;
		case GET_HASH(IRigidBody)		 : CREATE_DETAILS_OBJECT(RigidBodyDetails		 , component.second.get()); break;
		case GET_HASH(IScript)			 : CREATE_DETAILS_OBJECT(ScriptDetails			 , component.second.get()); break;
		default: break;
		}
	}

    return detailsObjects;
}

Vector<UniquePtr<DetailsObject>> DetailsObjectFactory::Create(DetailsWidget* detailsWidget, ResourceData* resourceData) noexcept
{
	const auto type = resourceData->m_resourcePath.m_type;

	Vector<UniquePtr<DetailsObject>> detailsObjects;

	switch (type)
	{
	case GET_HASH(Material): CREATE_DETAILS_OBJECT(MaterialDetails, resourceData); break;
	case GET_HASH(Shader)  : CREATE_DETAILS_OBJECT(ShaderDetails  , resourceData); break;
	default				   : CREATE_DETAILS_OBJECT(ResourceDetails, resourceData); break;
	}

	return detailsObjects;
}