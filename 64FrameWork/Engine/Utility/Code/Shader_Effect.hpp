#include "Shader_Base.hpp"

float			g_fAlphaRatio;
float			g_fTime;

texture			g_DepthTexture;
sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};
texture g_NoiseTexture;
sampler NoiseSampler = sampler_state
{
	texture = g_NoiseTexture;
};
struct VS_IN
{
	vector				vPosition	: POSITION;
	float2				vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector				vPosition	: POSITION;
	float2				vTexUV		: TEXCOORD0;
	vector				vProjPos	: TEXCOORD1;

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

	return Out;
}

struct  PS_IN
{
	float2			vTexUV : TEXCOORD0;
	vector			vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	float2		vDepthUV = (float2)0.f;

	// -1 -> 0, 1 -> 1 (������ǥ�� x���� �ؽ�ó uv�� ��ǥ ��ȯ)

	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;

	// 1 -> 0, -1 -> 1(������ǥ�� y���� �ؽ�ó uv�� ��ǥ ��ȯ)
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	float	vViewZ = tex2D(DepthSampler, vDepthUV).y * 1000.f;

	Out.vColor.a = Out.vColor.a * saturate(vViewZ - In.vProjPos.w);
		//Out.vColor.a = (Out.vColor.x + Out.vColor.y + Out.vColor.z) / 3.f * Out.vColor.a;

	return Out;
}

PS_OUT		InsertAlpha(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.a = Out.vColor.r;
	float2		vDepthUV = (float2)0.f;

	// -1 -> 0, 1 -> 1 (������ǥ�� x���� �ؽ�ó uv�� ��ǥ ��ȯ)

	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;

	// 1 -> 0, -1 -> 1(������ǥ�� y���� �ؽ�ó uv�� ��ǥ ��ȯ)
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	float	vViewZ = tex2D(DepthSampler, vDepthUV).y * 1000.f;

	Out.vColor.a = Out.vColor.a * saturate(vViewZ - In.vProjPos.w)*g_fAlphaRatio;

	return Out;
}

PS_OUT		Distortion (PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.a = Out.vColor.r;
	float2		vDepthUV = (float2)0.f;

	// -1 -> 0, 1 -> 1 (������ǥ�� x���� �ؽ�ó uv�� ��ǥ ��ȯ)

	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;

	// 1 -> 0, -1 -> 1(������ǥ�� y���� �ؽ�ó uv�� ��ǥ ��ȯ)
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	float	vViewZ = tex2D(DepthSampler, vDepthUV).y * 1000.f;

	Out.vColor.a = Out.vColor.a * saturate(vViewZ - In.vProjPos.w)*g_fAlphaRatio;


	float2 Trans = In.vTexUV + g_fTime;
	float4 Noise = tex2D(NoiseSampler, Trans);

	//������������ UV �� ���� �ֱ� ���� ���
	// 0.05 �� �� �κ��� Ŀ������ UV ������ ���� �������ϴ�.
	float2 UV = In.vTexUV + Noise.xy * 0.2f;

	float4 Orig = tex2D(BaseSampler, UV);
	Out.vColor = Out.vColor*Orig;

	return Out;

}

technique Default_Device
{
	pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		cullmode = none;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
	pass NotIncludeAlpha
	{
		alpharef = 0xc0;
		cullmode = none;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteEnable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 InsertAlpha();
	}

	pass DistortionShader
	{
		alpharef = 0xc0;
	cullmode = none;

	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	zwriteEnable = false;
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 Distortion();
	}

};