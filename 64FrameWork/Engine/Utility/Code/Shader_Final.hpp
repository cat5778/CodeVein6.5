texture   g_BlendTexture;
sampler BlendSampler = sampler_state
{
	texture = g_BlendTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture g_DistortionTexture;
sampler DistortionSampler = sampler_state
{
	texture = g_DistortionTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

struct PS_IN
{
	float2         vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector         vColor : COLOR0;
};

PS_OUT      PS_MAIN(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	float2 Trans = In.vTexUV + 0.001f;

	float4 Noise = tex2D(DistortionSampler, Trans);
	float2 UV = In.vTexUV + Noise.xz * 0.15f;

	Out.vColor = tex2D(BlendSampler, UV);

	return Out;
}

technique Default_Device
{
	pass   Blend
{

	zwriteEnable = false;
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	
	vertexshader = NULL;
	pixelshader = compile ps_3_0 PS_MAIN();
}

};