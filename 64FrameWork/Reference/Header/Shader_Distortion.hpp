#include "Shader_Base.hpp"
uniform texture _pTex_Distortion = NULL;
uniform texture _pTex_Orig = NULL;
texture g_AlphaTexture;
float	g_frameTime;



sampler AlphaSampler = sampler_state
{
	texture = g_AlphaTexture;
};

sampler2D Samp_Texture_Distortion =
sampler_state
{
	Texture = <_pTex_Distortion>;

//4.0
Filter = MIN_MAG_MIP_LINEAR;
//AddressU = Wrap;
//AddressV = Wrap;

};

sampler2D Samp_Texture_Orig =
sampler_state
{
	Texture = <_pTex_Orig>;

//4.0
Filter = MIN_MAG_MIP_LINEAR;
//AddressU = Wrap;
//AddressV = Wrap;

};



struct VS_INPUT
{
	float4 pos	: POSITION;
	float4 diff : COLOR0;
	float2 uv	: TEXCOORD0;

};



struct VS_OUTPUT
{
	float4 pos	: POSITION;
	float4 diff : COLOR0;
	float2 uv	: TEXCOORD0;

};



VS_OUTPUT _VS(VS_INPUT _In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.pos = _In.pos;
	Out.uv = _In.uv;
	Out.diff = _In.diff;


	return Out;
}



float4 _PS_Distortion(VS_OUTPUT _In) : COLOR
{
	float4 Out = 0;
	//uv trans
	// 0.001f �� �׽�Ʈ�� �־��� ���̸�, tick, timer ���� ������ ��ü�ϸ� �˴ϴ�.
	float2 Trans = _In.uv + g_frameTime;
	float4 Noise = tex2D(Samp_Texture_Distortion, Trans);
	
	//������������ UV �� ���� �ֱ� ���� ���
	// 0.05 �� �� �κ��� Ŀ������ UV ������ ���� �������ϴ�.
	float2 UV = _In.uv + Noise.xy * 0.05f;
	
	
	float4 Orig = tex2D(Samp_Texture_Orig, UV);
	Out = Orig;
	
	return Out;
}







technique Default_Device
{
	pass
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	vertexshader = compile vs_3_0 _VS();
	pixelshader = compile ps_3_0 _PS_Distortion();
	//pixelshader = compile ps_3_0 FirePixelShader();
	}

};