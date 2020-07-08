//#include "Shader_Base.hpp"
//
//
//float4x4 WVPM : WORLDVIEWPROJECTION;
//float4x4 WM : WORLD;
//float4x4 WITM : WORLDINVERSETRANSPOSE;
//float4x4 eyePosition : VIEWINVERSE;
//float time : TIME;
//
//float a = 0.9;
//float3 Color = { 1.0f, 0.f,0.f };
////<
////	string UlName = "Color";
////	string UlWidget= "Color";
////
////> =  {1.0f, 0.0f, 0.0f};
//
////texture DiffuseTexture : DIFFUSE
////<
////	string ResourceName = "bbk01.dds";
////	string UlName		= "Diffuse_Texture.dds";
////	string ResourceType = "2D";
////>;
//
//sampler2D DiffuseSampler = sampler_state
//{
//	Texture = DiffuseTexture;
//	FILTER = MIN_MAG_MIP_LINEAR;
//	AddressU = Wrap;
//	AddressV = Wrap;
//};
//
//
////texture DiffuseTexture2 : DIFFUSE
////<
////	string ResourceName = "color_sm.tag";
////	string UlName = "Diffuse_Texture2";
////	string ResourceType = "2D";
////> ;
////
//sampler2D DiffuseSampler2 = sampler_state
//{
//	Texture = DiffuseTexture2;
//	FILTER = MIN_MAG_MIP_LINEAR;
//	AddressU = Wrap;
//	AddressV = Wrap;
//};
//
//struct VS_INPUT
//{
//	float4 position : POSITION;
//	float2 uv		: TEXCOORD0;
//	float3 normal	: NORMAL;
//};
//
//struct VS_OUTPUT
//{
//	float4 pposition : POSITION;
//	float2 ouv		 : TEXCOORD0;
//	float4 wposition : TEXCOORD1;
//	float3 onomal	 : TEXCOORD2;
//};
//
//VS_OUTPUT VS_MAIN(VS_INPUT Input)
//{
//	VS_OUTPUT Output;
//	Output.pposition.xyz = mul(Input.position.xyz, WVPM);
//	Output.wposition = mul(Input.position, WM);
//	Output.onomal	= mul(Input.normal, WITM);
//
//	Output.ouv.x = Input.uv.x;
//	Output.ouv.y = -Input.uv.y;
//	return Output;
//}
//float4 PS_MAIN(VS_OUTPUT Input) :COLOR
//{
//	float3 diffuseM = tex2D(DiffuseSampler, Input.ouv);
//	float3 diffuseM2 = tex2D(DiffuseSampler2, Input.ouv);
//	
//	float multi1 = ((diffuseM2.r * sin(time))*2.8);
//	float multi2 = (diffuseM2.r * sin(time));
//	float b = saturate(pow(multi1 + multi2, 20));
//
//	float c = pow(multi1 + multi2, 20);
//
//	float3 Ke;
//	if (a >= b)
//		Ke = (100.0f, 1.0f, 1.0f);
//	else
//		Ke = (0.f, 0.f, 0.f);
//	float diffuse = (Ke * Color + diffuseM);
//	return float4(diffuse, c);
//
//}
//float4 PS_MAIN2(VS_INPUT Input):COLOR
//{
//	float3 diffuseM = tex2D(DiffuseSampler , Input.ouv);
//	float3 diffuseM2 = tex2D(DiffuseSampler, Input.ouv);
//
//	float multi1 = ((diffuseM2.r * sin(time))*2.8);
//	float multi2 = (diffuseM2.r * sin(time));
//	float b = saturate(pow(multi1 + multi2, 20));
//
//	float c = pow(multi1 + multi2, 100)*10;
//	 
//	float3 Ke;
//	if (a >= b)
//		Ke = (0.8f, 0.1f, 0.1f);
//	else
//		Ke = (0.f, 0.f, 0.f);
//	float3 diffuse = (Ke * Color);
//	return float4(diffuse, c);
//
//}
//
//technique technuque0()
//{
//	pass p0 
//	{
//		Cullmode = None;
//		AlphaBlendEnable = true;
//		DestBlend = InvsrcAlpha;
//		SrcBlend = SrcAlpha;
//		VertexShader = compile vs_3_0 VS_MAIN();
//		PixelShader = compile ps_3_0 PS_MAIN();
//	}
//	
//	pass p1
//	{
//		Cullmode = None;
//		AlphaBlendEnable = true;
//		DestBlend = One;
//		SrcBlend = SrcAlpha;
//		VertexShader = compile vs_3_0 VS_MAIN();
//		PixelShader = compile ps_3_0 PS_MAIN1();
//	}
//
//}