#include "GNHair.h"

AI_SHADER_NODE_EXPORT_METHODS(GNHair);

// ‚â‚Á‚½‚ºB
// ‚ñ‚ ‚ ‚ ‚ ‚ I


unsigned int LutgenThreadFunc(void* data)
{
	LutgenThreadData* td = (LutgenThreadData*)data;
	ScatteringParams& sp = *td->sp;

	for (int idx = td->start; idx < td->end; ++idx)
	{
		float a = float(idx) / float(DS_MASTER_LUT_SZ);
		float sigma = -logf(std::max(a, 0.0001f));
		td->ds->_luts[idx] = new ScatteringLut(sp.ior, sp.alpha_R, sp.alpha_TT, sp.alpha_TRT, sp.beta_R2, sp.beta_TT2,
			sp.beta_TRT2, sp.gamma_TT, sp.gamma_g, sp.phi_g, sigma, sp.shape);
	}

	return 0;
}

node_parameters
{
	AiParameterRGB("dyeColor", 1.0f, 1.0f, 1.0f);
AiParameterFlt("melanin", 0.15f);
AiParameterFlt("specularShift", 4.0f);
AiParameterFlt("specularWidth", 3.0f);
AiParameterInt("extraSamplesDiffuse", 0);
AiParameterInt("extraSamplesGlossy", 0);
AiParameterFlt("diffuseStrength", 1.0f);
AiParameterRGB("diffuseColor", 1.0f, 1.0f, 1.0f);
AiParameterEnum("diffuseScatteringMode", DUAL, ScatteringModeNames);
AiParameterFlt("specular1Strength", 1.0f);
AiParameterRGB("specular1Color", 1.0f, 1.0f, 1.0f);
AiParameterFlt("specular2Strength", 1.0f);
AiParameterRGB("specular2Color",1.0f, 1.0f, 1.0f);
AiParameterFlt("glintStrength", 2.5f);
AiParameterFlt("glintRolloff", 5.0f);
AiParameterFlt("transmissionStrength", 1.0f);
AiParameterRGB("transmissionColor", 1.0f, 1.0f, 1.0f);
AiParameterFlt("transmissionRolloff", 10.0f);
AiParameterRGB("opacity", 1.0f, 1.0f, 1.0f);
AiParameterInt("dualDepth", 0);
AiParameterFlt("diffuseForward", 0.7f);
AiParameterFlt("diffuseBack", 0.7f);
AiParameterFlt("specular1WidthScale", 1.0f);
AiParameterFlt("specular2WidthScale", 1.0f);
AiParameterFlt("transmissionWidthScale", 1.0f);
AiParameterFlt("specular1Shift", 0.0f);
AiParameterFlt("specular2Shift", 0.0f);
AiParameterFlt("transmissionShift", 0.0f);
AiParameterFlt("diffuseIndirectStrength", 0.0f);
AiParameterFlt("glossyIndirectStrength", 0.0f);

AiParameterFlt("randomTangent", 0.0f);
AiParameterFlt("randomHue", 0.0f);
AiParameterFlt("randomSaturation", 0.0f);
AiParameterFlt("randomMelanin", 0.08f);

AiParameterStr("uparam", "uparamcoord");
AiParameterStr("vparam", "vparamcoord");

AiParameterRGB("id1", 0.0f, 0.0f, 0.0f);
AiParameterRGB("id2", 0.0f, 0.0f, 0.0f);
AiParameterRGB("id3", 0.0f, 0.0f, 0.0f);
AiParameterRGB("id4", 0.0f, 0.0f, 0.0f);
AiParameterRGB("id5", 0.0f, 0.0f, 0.0f);
AiParameterRGB("id6", 0.0f, 0.0f, 0.0f);
AiParameterRGB("id7", 0.0f, 0.0f, 0.0f);
AiParameterRGB("id8", 0.0f, 0.0f, 0.0f);

AiParameterStr("aov_diffuse_color", "diffuse_color");
AiParameterStr("aov_direct_diffuse", "direct_diffuse");
AiParameterStr("aov_indirect_diffuse", "indirect_diffuse");
AiParameterStr("aov_direct_local", "direct_local");
AiParameterStr("aov_indirect_local", "indirect_local");
AiParameterStr("aov_direct_global", "direct_global");
AiParameterStr("aov_indirect_global", "indirect_global");
AiParameterStr("aov_direct_specular", "direct_specular");
AiParameterStr("aov_indirect_specular", "indirect_specular");
AiParameterStr("aov_direct_specular_2", "direct_specular_2");
AiParameterStr("aov_indirect_specular_2", "indirect_specular_2");
AiParameterStr("aov_direct_glint", "direct_glint");
AiParameterStr("aov_indirect_glint", "indirect_glint");
AiParameterStr("aov_direct_transmission", "direct_transmission");
AiParameterStr("aov_indirect_transmission", "indirect_transmission");
AiParameterStr("aov_depth", "depth");
AiParameterStr("aov_light_group_1", "light_group_1");
AiParameterStr("aov_light_group_2", "light_group_2");
AiParameterStr("aov_light_group_3", "light_group_3");
AiParameterStr("aov_light_group_4", "light_group_4");
AiParameterStr("aov_light_group_5", "light_group_5");
AiParameterStr("aov_light_group_6", "light_group_6");
AiParameterStr("aov_light_group_7", "light_group_7");
AiParameterStr("aov_light_group_8", "light_group_8");
AiParameterStr("aov_light_group_9", "light_group_9");
AiParameterStr("aov_light_group_10", "light_group_10");
AiParameterStr("aov_light_group_11", "light_group_11");
AiParameterStr("aov_light_group_12", "light_group_12");
AiParameterStr("aov_light_group_13", "light_group_13");
AiParameterStr("aov_light_group_14", "light_group_14");
AiParameterStr("aov_light_group_15", "light_group_15");
AiParameterStr("aov_light_group_16", "light_group_16");
AiParameterStr("aov_id_1", "id_1");
AiParameterStr("aov_id_2", "id_2");
AiParameterStr("aov_id_3", "id_3");
AiParameterStr("aov_id_4", "id_4");
AiParameterStr("aov_id_5", "id_5");
AiParameterStr("aov_id_6", "id_6");
AiParameterStr("aov_id_7", "id_7");
AiParameterStr("aov_id_8", "id_8");
AiParameterStr("crypto_asset_override", "");
AiParameterStr("crypto_object_override", "");
AiParameterStr("crypto_material_override", "");
AiParameterStr("aov_crypto_asset", "crypto_asset");
AiParameterStr("aov_crypto_object", "crypto_object");
AiParameterStr("aov_crypto_material", "crypto_material");
}


node_loader
{
	if (i>0) return 0;
node->methods = GNHair;
node->output_type = AI_TYPE_RGB;
node->name = "GNHair";
node->node_type = AI_NODE_SHADER;
strcpy(node->version, AI_VERSION);
return true;
}

node_initialize
{
	HairBsdf::ShaderData* data = new HairBsdf::ShaderData;
data->cryptomatte = new CryptomatteData();
AiNodeSetLocalData(node, data);
}

node_finish
{
	if (AiNodeGetLocalData(node))
	{
		HairBsdf::ShaderData* data = (HairBsdf::ShaderData*)AiNodeGetLocalData(node);
		if (data->cryptomatte)
			delete data->cryptomatte;
		delete data;
	}
}

node_update
{
	HairBsdf::ShaderData* data = (HairBsdf::ShaderData*)AiNodeGetLocalData(node);
data->update(params);

AiAOVRegister("diffuse_color", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("direct_specular", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("indirect_specular", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("direct_specular_2", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("indirect_specular_2", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("direct_transmission", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("indirect_transmission", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("direct_glint", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("direct_global", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("indirect_global", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("direct_local", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("indirect_local", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("id_1", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("id_2", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("id_3", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("id_4", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("id_5", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("id_6", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("id_7", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("id_8", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("light_group_1", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("light_group_2", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("light_group_3", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("light_group_4", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("light_group_5", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("light_group_6", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("light_group_7", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("light_group_8", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("light_group_9", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("light_group_10", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("light_group_11", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("light_group_12", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("light_group_13", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("light_group_14", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("light_group_15", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
AiAOVRegister("light_group_16", AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);

data->cryptomatte->setup_all(AiNodeGetStr(node, "aov_crypto_asset"),
	AiNodeGetStr(node, "aov_crypto_object"), AiNodeGetStr(node, "aov_crypto_material"));
// Get all the light nodes in the scene and try and find their light group parameter
// we'll store this based on the light pointer for fast access during rendering
AtNodeIterator* it = AiUniverseGetNodeIterator(AI_NODE_LIGHT);
while (!AiNodeIteratorFinished(it))
{
	AtNode* light = AiNodeIteratorGetNext(it);
	if (AiNodeLookUpUserParameter(light, "lightGroup"))
		data->lightGroups[light] = AiNodeGetInt(light, "lightGroup") - 1;
	else
		data->lightGroups[light] = -1;
}
}

shader_evaluate
{

	// Get shader data
	HairBsdf::ShaderData* data = (HairBsdf::ShaderData*)AiNodeGetLocalData(node);

// Create HairBsdf object 
HairBsdf hb(node, sg, data);
// Get parameters
hb.evaluateParameters(sg, data);

float geo_opacity = 1.0f;
if (AiUDataGetFlt("geo_opacity", &geo_opacity))
{
	hb.opacity *= geo_opacity;
}

const bool transp_early_out = AiShaderGlobalsApplyOpacity(sg, hb.opacity);
data->cryptomatte->do_cryptomattes(sg, node, p_crypto_asset_override, p_crypto_object_override, p_crypto_material_override);
if (transp_early_out)
return;

hb.opacity = sg->out_opacity;

AiStateSetMsgBool("als_hitHair", true);

float als_hairNumIntersections = 0;
AtRGB als_sigma_bar_f = AI_RGB_BLACK;
bool do_dual = false;
if (sg->Rr >= data->dual_depth) do_dual = true;

int als_raytype = ALS_RAY_UNDEFINED;
AiStateGetMsgInt("als_raytype", &als_raytype);

if (do_dual && als_raytype == ALS_RAY_DUAL)
{
	if (AiStateGetMsgFlt("als_hairNumIntersections", &als_hairNumIntersections)
		&& AiStateGetMsgRGB("als_sigma_bar_f", &als_sigma_bar_f))
	{
		float theta_i = AI_PIOVER2 - sphericalTheta(sg->Rd, hb.U);

		als_sigma_bar_f += hb.sp.beta_R2 + hb.sp.beta_TRT2 + hb.sp.beta_TT2;
		AiStateSetMsgRGB("als_sigma_bar_f", als_sigma_bar_f);

		als_hairNumIntersections += minh(hb.opacity);
		AiStateSetMsgFlt("als_hairNumIntersections", als_hairNumIntersections);

		sg->out_opacity = AI_RGB_BLACK;
	}
	else
	{
		sg->out_opacity = AI_RGB_WHITE;
	}

	return; // early out
}

// early-out regardless if we're in a shadow ray, or if opacity is zero
#if AI_VERSION_MINOR_NUM >= 2
if (sg->Rt & AI_RAY_SHADOW || AiColorIsZero(hb.opacity) || AiShaderGlobalsIsObjectMatte(sg)) return;
#else
if (sg->Rt & AI_RAY_SHADOW || AiColorIsZero(hb.opacity)) return;
#endif
// early out if we're a hair-hair glossy ray and the ray depth says we should be calculating dual scattering only
if (sg->Rr_gloss > data->dual_depth && als_raytype == ALS_RAY_HAIR)
{
	sg->out.RGB = AI_RGB_BLACK;
	return;
}

// calculate scattering explicitly up to the dual depth cutoff.
// in other words, do a brute force path trace for x=dual-depth bounces, then fall back to dual scattering for the rest.
if (do_dual && hb.dualImportant && hb.diffuseScatteringMode == DUAL)
{
	hb.integrateDirectDualMis(sg);
	hb.integrateIndirectDual(sg);
}
else
{
	hb.integrateDirectMis(sg);
	hb.integrateIndirect(sg);
}

// Write shader result
hb.writeResult(sg);
sg->out_opacity = hb.opacity;
}