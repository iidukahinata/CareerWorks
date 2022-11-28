/**
* @file	   AudioSpeakerDetails.cpp
* @brief
*
* @date	   2022/11/28 2022年度初版
*/

#include "AudioSpeakerDetails.h"
#include "SubSystem/Resource/Resources/Audio/AudioClip.h"
#include "SubSystem/Scene/Component/IAudioSpeaker.h"

AudioSpeakerDetails::AudioSpeakerDetails(DetailsWidget* detailsWidget, IComponent* component) :
	ComponentDetails(detailsWidget)
{
	m_audioSpeaker = dynamic_cast<IAudioSpeaker*>(component);
	ASSERT(m_audioSpeaker);
}

void AudioSpeakerDetails::Draw()
{
	constexpr int offsetPos = 130;

	if (ShowComponentHeader(m_audioSpeaker))
	{
		auto audioSpeaker = m_audioSpeaker;
		auto audioClip	  = m_audioSpeaker->GetAudioClip();
		auto playOnAwake  = m_audioSpeaker->GetPlayOnAwake();
		auto is2DMode	  = m_audioSpeaker->GetIs2DMode();
		auto isLoop		  = m_audioSpeaker->GetIsLoop();
		auto isMute		  = m_audioSpeaker->GetMute();
		auto priority	  = m_audioSpeaker->GetPriority();
		auto volume		  = m_audioSpeaker->GetVolume();
		auto pitch		  = m_audioSpeaker->GetPitch();
		auto pan		  = m_audioSpeaker->GetPan();
		auto maxDistance  = m_audioSpeaker->GetMaxDistance();
		auto outPutMatrix = m_audioSpeaker->GetOutPutMatrix();

		auto clipPath = ConvertToJapanese(audioClip ? audioClip->GetFilePath() : String());

		ImGui::Text("AudioClip"); ImGui::SameLine(offsetPos);

		constexpr auto itemWidth = 250;
		ImGui::PushItemWidth(itemWidth);
		ImGui::InputText("##", clipPath.data(), clipPath.size());
		ImGui::PopItemWidth();

		// ドラッグアンドドロップでの AudioClip 切り替え
		const auto hoverd = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
		if (ImGui::IsMouseReleased(0) && hoverd)
		{
			if (auto catchAudioClip = CatchDragObject<AudioClip>())
			{
				RegisterEditorCommand([audioSpeaker](auto data) { audioSpeaker->SetAudioClip(data); }, catchAudioClip, audioClip);
			}
		}

		ShowDragDropHelper<AudioClip>(hoverd, 221, 0, 0);

		ImGui::SameLine(offsetPos + itemWidth + 5);

		// リソースの検索を始める
		OpenResourceHelper("AudioSpeaker");
		if (auto resourceData = ShowSearchResourceHelper<AudioClip>())
		{
			auto clip = LoadResource<AudioClip>(resourceData);
			audioSpeaker->SetAudioClip(clip);
		}

		// 改行用
		ImGui::Text("");

		ImGui::Text("PlayOnAwake"); ImGui::SameLine(offsetPos);
		auto inputPlayOnAwake = ImGui::Checkbox("##PlayOnAwake", &playOnAwake);

		ImGui::Text("Is2DMode"); ImGui::SameLine(offsetPos);
		auto inputIs2DMode = ImGui::Checkbox("##Is2DMode", &is2DMode);

		ImGui::Text("IsLoop"); ImGui::SameLine(offsetPos);
		auto inputIsLoop = ImGui::Checkbox("##IsLoop", &isLoop);

		ImGui::Text("IsMute"); ImGui::SameLine(offsetPos);
		auto inputIsMute = ImGui::Checkbox("##IsMute", &isMute);

		ImGui::Text("Priority"); ImGui::SameLine(offsetPos);
		auto inputPriority = ImGui::SliderInt("##Priority", &priority, 0, 256);

		ImGui::Text("Volume"); ImGui::SameLine(offsetPos);
		auto inputVolume = ImGui::SliderFloat("##Volume", &volume, 0.f, 1.f);

		ImGui::Text("Pitch"); ImGui::SameLine(offsetPos);
		auto inputPitch = ImGui::SliderFloat("##Pitch", &pitch, 0.f, 2.f);

		ImGui::Text("Pan"); ImGui::SameLine(offsetPos);
		auto inputPan = ImGui::SliderFloat("##Pan", &pan, -1.f, 1.f);

		ImGui::Text("MaxDistance"); ImGui::SameLine(offsetPos);
		auto inputMaxDistance = ImGui::SliderFloat("##MaxDistance", &maxDistance, 1.f, 10000);

		ImGui::Text(""); // 改行

		ImGui::Text("OutPutMatrix"); ImGui::Text(""); // 改行

		auto inputOutPutMatrix = false;
		for (int i = 0; i < outPutMatrix.max_size(); ++i)
		{
			auto&& lavel = "##OutPutMatrix" + std::to_string(i);

			ImGui::SameLine();
			inputOutPutMatrix |= ImGui::VSliderFloat(lavel.c_str(), ImVec2(30, 160), &outPutMatrix[i], 0.f, 1.f);
		}

		if (inputPlayOnAwake)  RegisterEditorCommand([audioSpeaker](auto data) { audioSpeaker->SetPlayOnAwake(data);  }, playOnAwake  , m_audioSpeaker->GetPlayOnAwake());
		if (inputIs2DMode)	   RegisterEditorCommand([audioSpeaker](auto data) { audioSpeaker->SetIs2DMode(data);	  }, is2DMode	  , m_audioSpeaker->GetIs2DMode());
		if (inputIsLoop)	   RegisterEditorCommand([audioSpeaker](auto data) { audioSpeaker->SetIsLoop(data);		  }, isLoop	      , m_audioSpeaker->GetIsLoop());
		if (inputIsMute)	   RegisterEditorCommand([audioSpeaker](auto data) { audioSpeaker->SetMute(data);		  }, isMute	      , m_audioSpeaker->GetMute());
		if (inputPriority)	   RegisterEditorCommand([audioSpeaker](auto data) { audioSpeaker->SetPriority(data);	  }, priority	  , m_audioSpeaker->GetPriority());
		if (inputVolume)	   RegisterEditorCommand([audioSpeaker](auto data) { audioSpeaker->SetVolume(data);		  }, volume	      , m_audioSpeaker->GetVolume());
		if (inputPitch)		   RegisterEditorCommand([audioSpeaker](auto data) { audioSpeaker->SetPitch(data);		  }, pitch	      , m_audioSpeaker->GetPitch());
		if (inputPan)		   RegisterEditorCommand([audioSpeaker](auto data) { audioSpeaker->SetPan(data);		  }, pan		  , m_audioSpeaker->GetPan());
		if (inputMaxDistance)  RegisterEditorCommand([audioSpeaker](auto data) { audioSpeaker->SetMaxDistance(data);  }, maxDistance  , m_audioSpeaker->GetMaxDistance());
		if (inputOutPutMatrix) RegisterEditorCommand([audioSpeaker](auto data) { audioSpeaker->SetOutPutMatrix(data); }, outPutMatrix , m_audioSpeaker->GetOutPutMatrix());
	}
}