

//==================================================


//==================================================
// 조명


#define MAX_LIGHTS		1 
#define POINT_LIGHT		1.0f
#define SPOT_LIGHT		2.0f
#define DIRECTIONAL_LIGHT	3.0f

#define _WITH_LOCAL_VIEWER_HIGHLIGHTING
#define _WITH_THETA_PHI_CONES

struct MATERIAL
{
	float4 m_cAmbient;
	float4 m_cDiffuse;
	float4 m_cSpecular; //a = power
	float4 m_cEmissive;
};

struct LIGHT
{
	float4 m_cAmbient;
	float4 m_cDiffuse;
	float4 m_cSpecular;
	float3 m_vPosition;
	float m_fRange;
	float3 m_vDirection;
	float m_nType;
	float3 m_vAttenuation;
	float m_fFalloff;
	float m_fTheta; //cos(m_fTheta)
	float m_fPhi; //cos(m_fPhi)
	float m_bEnable;
	float padding;
};

//조명을 위한 상수버퍼를 선언한다. 
cbuffer cbLight : register(b0)
{
	LIGHT gLights[MAX_LIGHTS];
	float4 gcLightGlobalAmbient;
	float4 gvCameraPosition;
};

//물질을 위한 상수버퍼를 선언한다. 
cbuffer cbMaterial : register(b1)
{
	MATERIAL gMaterial;
};

struct LIGHTEDCOLOR
{
	float4 m_cAmbient;
	float4 m_cDiffuse;
	float4 m_cSpecular;
};

LIGHTEDCOLOR DirectionalLight(int i, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;
	float3 vToLight = normalize(-gLights[i].m_vDirection);
	float fDiffuseFactor = dot(vToLight, vNormal);
	//조명의 방향이 법선 벡터와 이루는 각도가 예각일 때 직접 조명의 영향을 계산한다.
	if (true)//fDiffuseFactor > 0.0f)
	{
		//물질의 스펙큘러 파워가 0이 아닐 때만 스펙큘러 조명의 영향을 계산한다.
		if (gMaterial.m_cSpecular.a != 0.0f)
		{
#ifdef _WITH_REFLECT
			float3 vReflect = reflect(-vToLight, vNormal);
			float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterial.m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
			float3 vHalf = normalize(vToCamera + vToLight);
#else
			float3 vHalf = float3(0.0f, 1.0f, 0.0f);
#endif
			float fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), gMaterial.m_cSpecular.a);
#endif
			output.m_cSpecular = gMaterial.m_cSpecular * (gLights[i].m_cSpecular * fSpecularFactor);
		}
		output.m_cDiffuse = gMaterial.m_cDiffuse * (gLights[i].m_cDiffuse * fDiffuseFactor);
	}
	output.m_cAmbient = gMaterial.m_cAmbient * gLights[i].m_cAmbient;
	return(output);

}

LIGHTEDCOLOR PointLight(int i, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;

	float3 vToLight = gLights[i].m_vPosition - vPosition;
	float fDistance = length(vToLight);
	//조명까지의 거리가 조명의 유효거리보다 작을 때만 조명의 영향을 계산한다.
	if (fDistance <= gLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		//조명의 방향이 법선 벡터와 이루는 각도가 예각일 때 직접 조명의 영향을 계산한다.
		if (fDiffuseFactor > 0.0f)
		{
			//물질의 스펙큘러 파워가 0이 아닐 때만 스펙큘러 조명의 영향을 계산한다.
			if (gMaterial.m_cSpecular.a != 0.0f)
			{
#ifdef _WITH_REFLECT
				float3 vReflect = reflect(-vToLight, vNormal);
				float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterial.m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
				float3 vHalf = normalize(vToCamera + vToLight);
#else
				float3 vHalf = float3(0.0f, 1.0f, 0.0f);
#endif
				float fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), gMaterial.m_cSpecular.a);
#endif
				output.m_cSpecular = gMaterial.m_cSpecular * (gLights[i].m_cSpecular * fSpecularFactor);
			}
			output.m_cDiffuse = gMaterial.m_cDiffuse * (gLights[i].m_cDiffuse * fDiffuseFactor);
		}
		//조명까지의 거리에 따라 조명의 영향을 계산한다.
		float fAttenuationFactor = 1.0f / dot(gLights[i].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));
		output.m_cAmbient = gMaterial.m_cAmbient * (gLights[i].m_cAmbient * fAttenuationFactor);
		output.m_cDiffuse *= fAttenuationFactor;
		output.m_cSpecular *= fAttenuationFactor;
	}
	return(output);
}

LIGHTEDCOLOR SpotLight(int i, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;
	float3 vToLight = gLights[i].m_vPosition - vPosition;
	float fDistance = length(vToLight);
	//조명까지의 거리가 조명의 유효거리보다 작을 때만 조명의 영향을 계산한다.
	if (fDistance <= gLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		//조명의 방향이 법선 벡터와 이루는 각도가 예각일 때 직접 조명의 영향을 계산한다.
		if (fDiffuseFactor > 0.0f)
		{
			//물질의 스펙큘러 파워가 0이 아닐 때만 스펙큘러 조명의 영향을 계산한다.
			if (gMaterial.m_cSpecular.a != 0.0f)
			{
#ifdef _WITH_REFLECT
				float3 vReflect = reflect(-vToLight, vNormal);
				float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterial.m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
				float3 vHalf = normalize(vToCamera + vToLight);
#else
				float3 vHalf = float3(0.0f, 1.0f, 0.0f);
#endif
				float fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), gMaterial.m_cSpecular.a);
#endif
				output.m_cSpecular = gMaterial.m_cSpecular * (gLights[i].m_cSpecular * fSpecularFactor);
			}
			output.m_cDiffuse = gMaterial.m_cDiffuse * (gLights[i].m_cDiffuse * fDiffuseFactor);
		}
#ifdef _WITH_THETA_PHI_CONES
		float fAlpha = max(dot(-vToLight, gLights[i].m_vDirection), 0.0f);
		float fSpotFactor = pow(max(((fAlpha - gLights[i].m_fPhi) / (gLights[i].m_fTheta - gLights[i].m_fPhi)), 0.0f), gLights[i].m_fFalloff);
#else
		float fSpotFactor = pow(max(dot(-vToLight, gLights[i].m_vDirection), 0.0f), gLights[i].m_fFalloff);
#endif
		float fAttenuationFactor = 1.0f / dot(gLights[i].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));
		output.m_cAmbient = gMaterial.m_cAmbient * (gLights[i].m_cAmbient * fAttenuationFactor * fSpotFactor);
		output.m_cDiffuse *= fAttenuationFactor * fSpotFactor;
		output.m_cSpecular *= fAttenuationFactor * fSpotFactor;
	}
	return(output);
}


float4 Lighting(float3 vPosition, float3 vNormal)
{
	int i;
	float3 vCameraPosition = float3(gvCameraPosition.x, gvCameraPosition.y, gvCameraPosition.z);
	float3 vToCamera = normalize(float3(0.0f, 0.0f, -100.0f));//vCameraPosition - vPosition);
	LIGHTEDCOLOR LightedColor = (LIGHTEDCOLOR)0;
	float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (i = 0; i < MAX_LIGHTS; i++)
	{
		//활성화된 조명에 대하여 조명의 영향을 계산한다.
		if (gLights[i].m_bEnable == 1.0f)
		{
			//조명의 유형에 따라 조명의 영향을 계산한다.
			if (gLights[i].m_nType == DIRECTIONAL_LIGHT)
			{
				LightedColor = DirectionalLight(i, vNormal, vToCamera);
				cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
			}
			if (gLights[i].m_nType == POINT_LIGHT)
			{
				LightedColor = PointLight(i, vPosition, vNormal, vToCamera);
				cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
			}
			if (gLights[i].m_nType == SPOT_LIGHT)
			{
				LightedColor = SpotLight(i, vPosition, vNormal, vToCamera);
				cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
			}
		}
	}
	//글로벌 주변 조명의 영향을 최종 색상에 더한다.
	cColor += (gcLightGlobalAmbient * gMaterial.m_cAmbient);
	//최종 색상의 알파값은 물질의 디퓨즈 색상의 알파값으로 설정한다.
	cColor.a = gMaterial.m_cDiffuse.a;
	return(cColor);
}


//=================================================

//쉐이더 변수는 구조체로 선언할 수 있다. 구조체의 멤버 변수는 전역변수로 사용된다. 

//카메라 변환 행렬과 투영 변환 행렬을 위한 쉐이더 변수를 선언한다(슬롯 0을 사용).

// ===============================================
//		상수 버퍼
// 뷰, 투영변환행렬
cbuffer cbViewProjectionMatrix : register(b0)
{
	matrix gmtxView: packoffset(c0);
	matrix gmtxProjection: packoffset(c4);
};

// 월드변환행렬
cbuffer cbWorldMatrix : register(b1)
{
	matrix gmtxWorld: packoffset(c0);
};

// 애니메이션 행렬
cbuffer cbSkinned : register(b2)
{
	// 한캐릭터당 최대 뼈대 개수는 96.
	matrix gBoneTransforms[96];
};

Texture2D gtxtTexture[9] : register(t0); // 텍스쳐
Texture2D gtxBump[9] : register(t9); // 범프맵
SamplerState gSamplerState : register(s0); // 텍스쳐 샘플러
SamplerState gSamplerState_NORMAL : register(s1); // 범프 샘플러

//=============================================
// 구조체
//----------------------------------------------
// Diffuse
struct VS_DIFFUSED_COLOR_INPUT
{
	float3 position: POSITION;
	float3 normal : NORMAL;
	float4 color: COLOR0;
};

struct VS_DIFFUSED_COLOR_OUTPUT
{
	float4 position: SV_POSITION;
	float3 positionW: POSITION;
	float3 normalW : NORMAL;
	float4 color: COLOR;
};

//Texture
struct VS_TEXTURED_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 tex2dcoord : TEXCOORD0;
};

struct VS_TEXTURED_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 tex2dcoord : TEXCOORD0;
};

//Texture UVW
struct VS_TEXTUREDUVW_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 tex2dcoord : TEXCOORD;
	int textureNum : TEXNUM;
};

struct VS_TEXTUREDUVW_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 tex2dcoord : TEXCOORD0;
	int textureNum : TEXNUM;
};


// Animate
struct SkinnedVertexIn
{
	float3 PosL       : POSITION;
	float3 NormalL    : NORMAL;
	float3 BiNormalL  : BINORMAL;
	float3 TangentL   : TANGENT;
	float2 Tex        : TEXCOORD;
	float3 Weights    : WEIGHTS;
	float4 BoneIndices : BONEINDICES;
	int TextureNumber : TEXNUM;
};

struct SkinnedVertexOut
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float3 binormalW : BINORMAL;
	float3 tangentW : TANGENT;
	float2 tex2dcoord : TEXCOORD0;
	int textureNum : TEXNUMBER;
	float4 color : TEXCOORD1;
};

// ================================================


//==================================================
// 버텍스 쉐이더
VS_DIFFUSED_COLOR_OUTPUT VSLightingColor(VS_DIFFUSED_COLOR_INPUT input)
{
	VS_DIFFUSED_COLOR_OUTPUT output = (VS_DIFFUSED_COLOR_OUTPUT)0;
	
	output.color = input.color;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul(input.position, (float3x3)gmtxWorld);
	output.positionW += float3(gmtxWorld._41, gmtxWorld._42, gmtxWorld._43);

	matrix mtxWorldViewProjection = mul(gmtxWorld, gmtxView);
	mtxWorldViewProjection = mul(mtxWorldViewProjection, gmtxProjection);


	output.position = mul(float4(input.position, 1.0f), mtxWorldViewProjection);
	return(output);
}

VS_TEXTURED_LIGHTING_OUTPUT VSTexturedLighting(VS_TEXTURED_LIGHTING_INPUT input)
{
	VS_TEXTURED_LIGHTING_OUTPUT output = (VS_TEXTURED_LIGHTING_OUTPUT)0;

	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul((float4)(input.position,1.0f), gmtxWorld).xyz;
	//output.positionW += float3(gmtxWorld._41, gmtxWorld._42, gmtxWorld._43);

	matrix mtxWorldViewProjection = mul(gmtxWorld, gmtxView);
	mtxWorldViewProjection = mul(mtxWorldViewProjection, gmtxProjection);
	
	
	output.position = mul(float4(input.position, 1.0f), mtxWorldViewProjection);
	output.tex2dcoord = input.tex2dcoord;

	return(output);
}

VS_TEXTUREDUVW_LIGHTING_OUTPUT VSTexturedUVWLighting(VS_TEXTUREDUVW_LIGHTING_INPUT input)
{
	VS_TEXTUREDUVW_LIGHTING_OUTPUT output = (VS_TEXTUREDUVW_LIGHTING_OUTPUT)0;

	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul((float4)(input.position, 1.0f), gmtxWorld).xyz;
	//output.positionW += float3(gmtxWorld._41, gmtxWorld._42, gmtxWorld._43);

	matrix mtxWorldViewProjection = mul(gmtxWorld, gmtxView);
	mtxWorldViewProjection = mul(mtxWorldViewProjection, gmtxProjection);


	output.position = mul(float4(input.position, 1.0f), mtxWorldViewProjection);

	output.tex2dcoord = input.tex2dcoord;
	output.textureNum = input.textureNum;

	return(output);
}

SkinnedVertexOut SkinnedVS(SkinnedVertexIn vin)
{
	SkinnedVertexOut vout;
	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	weights[0] = vin.Weights.x;
	weights[1] = vin.Weights.y;
	weights[2] = vin.Weights.z;

	if (weights[0] == 0.0f && weights[1] == 0.0f && weights[2] == 0.0f && vin.BoneIndices[3] == 0)
		weights[3] = 0.0f;
	else
		weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);
	float3 binormalL = float3(0.0f, 0.0f, 0.0f);
	float3 TangentL = float3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 4; i++)
	{
		//Assume no nonuniform scaling when transforming normals, so 
		//that we do not have to use the inverse-transpose.
		posL += weights[i] * mul(float4(vin.PosL, 1.0f), gBoneTransforms[vin.BoneIndices[i]]).xyz;
		normalL += weights[i] * mul(float4(vin.NormalL, 1.0f), (float3x3)gBoneTransforms[vin.BoneIndices[i]]).xyz;
		binormalL += weights[i] * mul(float4(vin.BiNormalL, 1.0f), (float3x3)gBoneTransforms[vin.BoneIndices[i]]).xyz;
		TangentL += weights[i] * mul(float4(vin.TangentL, 1.0f), (float3x3)gBoneTransforms[vin.BoneIndices[i]]).xyz;

	}
	//posL = vin.PosL;
	vout.positionW = mul(float4(posL, 1.0f), gmtxWorld).xyz;
	vout.normalW = mul(normalL,(float3x3)gmtxWorld);
	vout.binormalW = mul(binormalL,(float3x3)gmtxWorld);
	vout.tangentW = mul(TangentL,(float3x3)gmtxWorld);

	//월드뷰투영변환행렬 생성
	matrix mtxWorldViewProjection = mul(gmtxWorld, gmtxView);
	mtxWorldViewProjection = mul(mtxWorldViewProjection, gmtxProjection);


	vout.position = mul(float4(posL, 1.0f), mtxWorldViewProjection).xyzw;
	


	float a = 53;
	vout.color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	if (vin.BoneIndices[0] == a || vin.BoneIndices[1] == a || vin.BoneIndices[2] == a || vin.BoneIndices[3] == a)
		vout.color.x = 1.0f;

	vout.tex2dcoord = vin.Tex;
	vout.textureNum = vin.TextureNumber;


	return vout;

};
//==================================================


//==================================================
// 픽셀 쉐이더

float4 PSLightingColor(VS_DIFFUSED_COLOR_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	return( input.color * cIllumination);
}

float4 PSTexturedLighting(VS_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
	if (input.normalW.x == 0.0f && input.normalW.y == 0.0f && input.normalW.z == 0.0f)
	{
		float4 cColor = gtxtTexture[0].Sample(gSamplerState, input.tex2dcoord);
		return cColor;
	}
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	float4 cColor = gtxtTexture[0].Sample(gSamplerState, input.tex2dcoord) * cIllumination;
	return cColor;
}

float4 PSTexturedUVWLighting(VS_TEXTUREDUVW_LIGHTING_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	if (input.normalW.x == 0.0f && input.normalW.y == 0.0f && input.normalW.z == 0.0f)
	{
		cIllumination = float4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	float4 cColor = { 1.0f, 1.0f, 0.0f, 1.0f };
	int n = input.textureNum;

	if (n == 0)
	{
		cColor = gtxtTexture[0].Sample(gSamplerState, input.tex2dcoord);
	}
	else if (n == 1)
	{
		cColor = gtxtTexture[1].Sample(gSamplerState, input.tex2dcoord);
	}
	else if (n == 2)
	{
		cColor = gtxtTexture[2].Sample(gSamplerState, input.tex2dcoord);
	}
	else if (n == 3)
	{
		cColor = gtxtTexture[3].Sample(gSamplerState, input.tex2dcoord);
	}
	else if (n == 4)
	{
		cColor = gtxtTexture[4].Sample(gSamplerState, input.tex2dcoord);
	}
	else if (n == 5)
	{
		cColor = gtxtTexture[5].Sample(gSamplerState, input.tex2dcoord);
	}
	else if (n == 6)
	{
		cColor = gtxtTexture[6].Sample(gSamplerState, input.tex2dcoord);
	}
	else if (n == 7)
	{
		cColor = gtxtTexture[7].Sample(gSamplerState, input.tex2dcoord);
	}
	else
	{
		cColor = gtxtTexture[0].Sample(gSamplerState, input.tex2dcoord);
	}
	//float4 cColor = gtxtTexture[0].Sample(gSamplerState, input.tex2dcoord) * cIllumination;
	return cColor *cIllumination;
}


float4 SkinnedPS(SkinnedVertexOut input) : SV_Target
{
	int n = input.textureNum;
	float4 cColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	float3 BumpMap = { 0.0f, 0.0f, 0.0f};
	
	if (n == 0)
	{
		cColor = gtxtTexture[0].Sample(gSamplerState, input.tex2dcoord);
		BumpMap = gtxBump[0].Sample(gSamplerState, input.tex2dcoord);
	}
	else if (n == 1)
	{
		cColor = gtxtTexture[1].Sample(gSamplerState, input.tex2dcoord);
		BumpMap = gtxBump[1].Sample(gSamplerState, input.tex2dcoord);
	}
	else if (n == 2)
	{
		cColor = gtxtTexture[2].Sample(gSamplerState, input.tex2dcoord);
		BumpMap = gtxBump[2].Sample(gSamplerState, input.tex2dcoord);
	}
	else if (n == 3)
	{
		cColor = gtxtTexture[3].Sample(gSamplerState, input.tex2dcoord);
		BumpMap = gtxBump[3].Sample(gSamplerState, input.tex2dcoord);
	} 
	else if (n == 4)
	{
		cColor = gtxtTexture[4].Sample(gSamplerState, input.tex2dcoord);
		BumpMap = gtxBump[4].Sample(gSamplerState, input.tex2dcoord);
	}
	else if (n == 5)
	{
		cColor = gtxtTexture[5].Sample(gSamplerState, input.tex2dcoord);
		BumpMap = gtxBump[5].Sample(gSamplerState, input.tex2dcoord);
	}
	else if (n == 6)
	{
		cColor = gtxtTexture[6].Sample(gSamplerState, input.tex2dcoord);
		BumpMap = gtxBump[6].Sample(gSamplerState, input.tex2dcoord);
	}
	else if (n == 7)
	{
		cColor = gtxtTexture[7].Sample(gSamplerState, input.tex2dcoord);
		BumpMap = gtxBump[7].Sample(gSamplerState, input.tex2dcoord);
	}
	input.normalW = normalize(input.normalW);
	input.binormalW = normalize(input.binormalW);
	input.tangentW = normalize(input.tangentW);
	BumpMap = (BumpMap * 2.0f) - 1.0f;
	float3 BumpNormal = input.normalW + (BumpMap.x * input.tangentW) + (BumpMap.y * input.binormalW);
	BumpNormal = normalize(BumpNormal);
	float4 cIllumination = Lighting(input.positionW, BumpNormal);


	cColor = cColor *cIllumination;
	
	return(cColor);
}