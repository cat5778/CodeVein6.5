#include "Shader_Base.hpp"

texture			g_DepthTexture;
texture			g_DepthAlphaTexture;
texture			g_DepthMaskTexture;
cbuffer NoiseBuffer
{
	float	frameTime;
	float3	scrollSpeeds;
	float3	scales;
	float	padding;
};

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};
sampler AlphaSampler = sampler_state
{
	texture = g_DepthAlphaTexture;
};

sampler MaskSampler = sampler_state
{
	texture = g_DepthMaskTexture;
};

struct VS_IN
{
	vector				vPosition	: POSITION;
	float2				vTexUV		: TEXCOORD0;

};

struct VS_OUT
{
	vector				vPosition	: POSITION;
	float2				vTexUV : TEXCOORD0;
	vector				vProjPos : TEXCOORD1;
	float2				vTexCoords2 : TEXCOORD2;
	float2				vTexCoords3 : TEXCOORD3;
	float2				vTexCoords4 : TEXCOORD4;

};

VS_OUT	VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV  = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;


	Out.vTexCoords2 = (In.vTexUV * scales.x);
	Out.vTexCoords2.y = Out.vTexCoords2.y + (frameTime*scrollSpeeds.x);

	Out.vTexCoords3 = (In.vTexUV * scales.x);
	Out.vTexCoords3.y = Out.vTexCoords4.y + (frameTime*scrollSpeeds.x);

	Out.vTexCoords4 = (In.vTexUV * scales.x);
	Out.vTexCoords4.y = Out.vTexCoords4.y + (frameTime*scrollSpeeds.x);

	return Out;
}

struct  PS_IN
{
	float2			vTexUV : TEXCOORD0;
	vector			vProjPos : TEXCOORD1;
	float2			vTexCoords2 : TEXCOORD2;
	float2			vTexCoords3 : TEXCOORD3;
	float2			vTexCoords4 : TEXCOORD4;

};

struct PS_OUT
{
	vector			vColor : COLOR0;
};
cbuffer DistotionBuffer
{
	float2	distortion1;
	float2	distortion2;
	float2	distortion3;
	float	distortionScale;
	float	distortionBias;
};
PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	float2		vDepthUV = (float2)0.f;

	// -1 -> 0, 1 -> 1 (투영좌표인 x값이 텍스처 uv로 좌표 변환)
	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	// 1 -> 0, -1 -> 1(투영좌표인 y값이 텍스처 uv로 좌표 변환)
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;
	float	vViewZ = tex2D(DepthSampler, vDepthUV).y * 1000.f;

	vector vAlpha =tex2D(AlphaSampler, In.vTexCoords2);
	vector vAlpha2 = tex2D(MaskSampler, In.vTexUV);


	Out.vColor.a = vAlpha2.r;
	Out.vColor.a = (Out.vColor.a/** vAlpha.a*/)* saturate(vViewZ - In.vProjPos.w);

	return Out;
}

struct PixelInputType
{
	float4 position		: POSITION;
	float2 tex			: TEXCOORD0;
	vector vProjPos		: TEXCOORD1;
	float2 texCoords1	: TEXCOORD2;
	float2 texCoords2	: TEXCOORD3;
	float2 texCoords3	: TEXCOORD4;
};

PS_OUT FirePixelShader(PixelInputType Input)
{
	PS_OUT			Out = (PS_OUT)0;
	float4 noise1;
	float4 noise2;
	float4 noise3;
	float4 finalNoise;
	float	perturb;
	float2 noiseCoords;
	float4 fireColor;
	float4 alphaColor;

	noise1 = tex2D(DepthSampler, Input.texCoords1);
	noise2 = tex2D(AlphaSampler, Input.texCoords2);
	noise3 = tex2D(MaskSampler, Input.texCoords1);

	noise1 = (noise1 - 0.5f)*2.0f;
	noise2 = (noise2 - 0.5f)*2.0f;
	noise3 = (noise3 - 0.5f)*2.0f;

	noise1.xy = noise1.xy*distortion1.xy;
	noise2.xy = noise2.xy*distortion2.xy;
	noise3.xy = noise3.xy*distortion1.xy;

	finalNoise = noise1 + noise2 + noise3;

	perturb = ((1.f - Input.tex.y)*distortionScale) + distortionBias;

	noiseCoords.xy = (finalNoise.xy*perturb) + Input.tex.xy;
	fireColor = tex2D(DepthSampler, noiseCoords.xy);
	alphaColor = tex2D(MaskSampler, noiseCoords.xy);
	
	fireColor.a = alphaColor;
	Out.vColor = fireColor;
	return Out;
}
technique Default_Device
{
	pass
	{
		alphablendenable = true;
		srcblend = srcalpha;	
		destblend = invsrcalpha;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
		//pixelshader = compile ps_3_0 FirePixelShader();
}

};