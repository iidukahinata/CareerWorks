/**
* @file    pch.h
* @brief
*
* @date	   2022/08/02 2022îNìxèâî≈
*/
#pragma once

// STL
#include <any>
#include <span>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <functional>
#include <d3dcompiler.h>

// Core
#include "SubSystem/Core/Config.h"
#include "SubSystem/Core/Common/Typedef.h"
#include "SubSystem/Core/Common/Macro.h"
#include "SubSystem/Core/Common/Tools/Tools.h"
#include "SubSystem/Core/Common/Tools/Array.h"
#include "SubSystem/Core/Common/Tools/String.h"
#include "SubSystem/Core/Common/ProjectSetting.h"
#include "SubSystem/Core/Common/Tools/Memory/Memory.h"
#include "SubSystem/Log/DebugLog.h"
#include "SubSystem/Timer/Stopwatch.h"

// Context
#include "SubSystem/Core/Context.h"

// Math
#include "SubSystem/Core/Math/Vector2.h"
#include "SubSystem/Core/Math/Vector3.h"
#include "SubSystem/Core/Math/Vector4.h"
#include "SubSystem/Core/Math/Vector2T.h"
#include "SubSystem/Core/Math/Vector3T.h"
#include "SubSystem/Core/Math/Vector4T.h"
#include "SubSystem/Core/Math/Matrix.h"

// File System
#include "SubSystem/Core/IO/FileStream.h"
#include "SubSystem/Core/IO/FileSystem.h"

// Event System
#include "SubSystem/Core/Event/Event.h"
#include "SubSystem/Core/Event/EventListener.h"
#include "SubSystem/Editor/EditorEvents.h"
#include "SubSystem/Input/InputEvents.h"
#include "SubSystem/Scene/SceneEvents.h"

// Job System
#include "SubSystem/JobSystem/Sync/Job.h"
#include "SubSystem/JobSystem/Async/AsyncJob.h"

// Resource System
#include "SubSystem/Resource/ResourceData.h"
#include "SubSystem/Resource/ResourceHandle.h"

// ThirdParty
#include "ThirdParty/directxtex/include/d3dx12.h"
#include "ThirdParty/directxtex/include/DirectXTex.h"
#include "ThirdParty/directxtex/include/DDSTextureLoader.h"
#include "ThirdParty/directxtex/include/WICTextureLoader.h"
#include "ThirdParty/imgui/imgui_impl_dx11.h"
#include "ThirdParty/imgui/imgui_impl_dx12.h"
#include "ThirdParty/imgui/imgui_impl_win32.h"