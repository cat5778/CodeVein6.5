#include "Shader_Base.hpp"

texture			g_DepthTexture;
float			g_TextureU;
float			g_TextureV;
float			g_TextureA;
float			g_TextureUScale;
float			g_TextureFilterVal;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
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
	vector				vColor : COLOR0;


};

VS_OUT	VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV  = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);



	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vTexUV.x *=g_TextureUScale;


	Out.vProjPos = Out.vPosition;
	Out.vTexUV.x += g_TextureU;
	Out.vTexUV.y += g_TextureV;

	//Out.vColor = tex2D(BaseSampler, Out.vTexUV);
	//if ((Out.vColor.r + Out.vColor.g + Out.vColor.b) < g_TextureFilterVal)
	//	Out.vColor.a = 0.f;

	return Out;
}

struct  PS_IN
{
	float2			vTexUV : TEXCOORD0;
	vector			vProjPos : TEXCOORD1;
	vector			vColor	: COLOR0;

};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	//if ((In.vColor.r + In.vColor.g + In.vColor.b) < g_TextureFilterVal)
	//	In.vColor.a = 0.f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);


	float2		vDepthUV = (float2)0.f;

	// -1 -> 0, 1 -> 1 (투영좌표인 x값이 텍스처 uv로 좌표 변환)


	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	// 1 -> 0, -1 -> 1(투영좌표인 y값이 텍스처 uv로 좌표 변환)
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	float	vViewZ = tex2D(DepthSampler, vDepthUV).y * 1000.f;

	Out.vColor.a = Out.vColor.a * saturate(vViewZ - In.vProjPos.w);
	Out.vColor.a *= g_TextureA;

	//if ((Out.vColor.r + Out.vColor.g + Out.vColor.b) < g_TextureFilterVal)
	//	Out.vColor.a = 0.f;

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
	}

};