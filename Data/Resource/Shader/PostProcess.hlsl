#define DEFINE_ENTRY_POINT(VS_PASS, PS_PASS)\
PS_IN vs_main(VS_IN input)					\
{											\
	return VS_PASS(input);					\
}											\
											\
float4 ps_main(PS_IN input) : SV_TARGET		\
{											\
	return PS_PASS(input);					\
}

#if (XBlur || YBlur)
#include "Blur.hlsli"
DEFINE_ENTRY_POINT(VS_BlurPass, PS_BlurPass)
#endif

#if Luma
#include "Luminous.hlsli"
DEFINE_ENTRY_POINT(VS_LuminousPass, PS_LuminousPass)
#endif

#if Bloom
#include "Bloom.hlsli"
DEFINE_ENTRY_POINT(VS_BloomPass, PS_BloomPass)
#endif