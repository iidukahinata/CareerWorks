/**
* @file	   DetailsObjectFactory.cpp
* @brief
*
* @date	   2022/09/13 2022年度初版
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
#include "../DetailsObject/DetailsObjects/ModelRenderDetails.h"
#include "../DetailsObject/DetailsObjects/AudioSpeakerDetails.h"
#include "../DetailsObject/DetailsObjects/AudioListenerDetails.h"

#define CREATE_DETAILS_OBJECT(CLASS, ...) detailsObjects.emplace_back(std::make_unique<CLASS>(detailsWidget, __VA_ARGS__));

Vector<UniquePtr<DetailsObject>> DetailsObjectFactory::Create(DetailsWidget* detailsWidget, GameObject* gameObject) noexcept
{
	Vector<UniquePtr<DetailsObject>> detailsObjects;

	CREATE_DETAILS_OBJECT(GameObjectDetails, gameObject);
	CREATE_DETAILS_OBJECT(TransformDetails , &gameObject->GetTransform());

	// 所持コンポーネント表示用ループ
	for (const auto& component : gameObject->GetAllComponent())
	{
		switch (component.first)
		{
		case GET_HASH(Light)		: CREATE_DETAILS_OBJECT(LightDetails		, component.second.get()); break;
		case GET_HASH(Camera)		: CREATE_DETAILS_OBJECT(CameraDetails		, component.second.get()); break;
		case GET_HASH(ModelRender)	: CREATE_DETAILS_OBJECT(ModelRenderDetails	, component.second.get()); break;
		case GET_HASH(AudioSpeaker)	: CREATE_DETAILS_OBJECT(AudioSpeakerDetails	, component.second.get()); break;
		case GET_HASH(AudioListener): CREATE_DETAILS_OBJECT(AudioListenerDetails, component.second.get()); break;
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
	default				   : CREATE_DETAILS_OBJECT(ResourceDetails, resourceData); break;
	}

	return detailsObjects;
}