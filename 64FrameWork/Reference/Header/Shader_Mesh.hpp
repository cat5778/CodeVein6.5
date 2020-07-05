#include "Shader_Base.hpp"

texture		g_NormalTexture;

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

struct VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	vector		vTangent : TANGENT;
	vector		vBinormal : BINORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	vector		vTangent : TANGENT;
	vector		vBinormal : BINORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
};

// ���ؽ� ���̴� �Լ�
VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vTangent = normalize(mul(vector(In.vTangent.xyz, 0.f), g_matWorld));
	Out.vBinormal = normalize(mul(vector(In.vBinormal.xyz, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN // �ȼ� ���̴� ����ü���� POSITION�̶� Semantic�� ����� �� ����.
{
	float2			vTexUV : TEXCOORD0;
	vector			vProjPos : TEXCOORD1;
	vector			vNormal : NORMAL;
	vector			vTangent : TANGENT;
	vector			vBinormal : BINORMAL;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
	vector			vNormal : COLOR1;
	vector			vDepth : COLOR2;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vNormal = tex2D(NormalSampler, In.vTexUV);

	Out.vNormal = normalize((Out.vNormal * 2.f) - 1.f);

	Out.vNormal = normalize(In.vNormal + Out.vNormal.x * In.vTangent + Out.vNormal.y * In.vBinormal);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,	// z�����⸦ �����Ͽ� 0~1������ ���� z���� �����, �̸� �ؽ��� uv��ǥ�� �Ǵ�
		In.vProjPos.w * 0.001f,			// �佺���̽� ������ z���� �ؽ����� uv�� ��ȯ
		0.f,
		0.f);

	return Out;
}


PS_OUT		PS_ALPHA(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	return Out;
}


technique Default_Device
{
	pass
	{
		alphatestenable = true;
		alphafunc = greater;
		alpharef = 0xc0;
		cullmode = none;
		
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
	
	
	pass AlphaTesting
	{
		alphatestenable = true;
		alphafunc = greater;
		alpharef = 0xc0;
		//cullmode = none;
		
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_ALPHA();
	}
};