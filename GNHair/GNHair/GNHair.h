// Hair shader based on 
// [1] ISHair: Importance Sampling for Hair Scattering by Ou et al. 2012
// http://www.cs.dartmouth.edu/~ouj/site/Research/Entries/2012/6/21_ISHair__Importance_Sampling_for_Hair_Scattering.html
// [2] Dual Scattering Approximation For Fast Multiple Scattering in Hair by Zinke et al. 2008


#ifdef GNHAIR_EXPORTS
#define GNHAIR_API __declspec(dellexport)
#define GNHAIR_API __declspec(dellimport)
#endif




#include <ai.h>
#include "alUtil.h"
#include "stats.h"
#include "scattering.h"
#include <vector>
#include <algorithm>
#include <map>
#include <cassert>
#include "cryptomatte/cryptomatte.h"


//#define DEBUG_LUTS
#ifdef DEBUG_LUTS
#include "exr.h"
#endif

#define NUM_LIGHT_GROUPS 16
#define NUM_ID_AOVS 8
#define B_WIDTH_SCALE 2.0f
#define F_WIDTH_SCALE 4.0f

enum GNHairParams
{
	p_dyeColor,

	//////////////////////////////
	// yhHair param
	p_eumelanin,
	p_pheomelanin,
	p_bleachingtime,
	p_thickness,
	p_temperature,
	//////////////////////////////
	//////////////////////////////
	


	p_specularShift,
	p_specularWidth,
	p_extraSamplesDiffuse,
	p_extraSamplesGlossy,
	p_diffuseStrength,
	p_diffuseColor,
	p_diffuseScatteringMode,
	p_specular1Strength,
	p_specular1Color,
	p_specular2Strength,
	p_specular2Color,
	p_glintStrength,
	p_glintRolloff,
	p_transmissionStrength,
	p_transmissionColor,
	p_transmissionRolloff,
	p_opacity,
	p_dualDepth,
	p_densityFront,
	p_densityBack,
	p_specular1WidthScale,
	p_specular2WidthScale,
	p_transmissionWidthScale,
	p_specular1Shift,
	p_specular2Shift,
	p_transmissionShift,
	p_diffuseIndirectStrength,
	p_glossyIndirectStrength,

	p_randomTangent,
	p_randomHue,
	p_randomSaturation,
	p_randomMelanin,

	p_uparam,
	p_vparam,

	p_id1,
	p_id2,
	p_id3,
	p_id4,
	p_id5,
	p_id6,
	p_id7,
	p_id8,

	p_aov_diffuse_color,
	p_aov_direct_diffuse,
	p_aov_indirect_diffuse,
	p_aov_direct_local,
	p_aov_indirect_local,
	p_aov_direct_global,
	p_aov_indirect_global,
	p_aov_direct_specular,
	p_aov_indirect_specular,
	p_aov_direct_specular_2,
	p_aov_indirect_specular_2,
	p_aov_direct_glint,
	p_aov_indirect_glint,
	p_aov_direct_transmission,
	p_aov_indirect_transmission,
	p_aov_depth,
	p_aov_light_group_1,
	p_aov_light_group_2,
	p_aov_light_group_3,
	p_aov_light_group_4,
	p_aov_light_group_5,
	p_aov_light_group_6,
	p_aov_light_group_7,
	p_aov_light_group_8,
	p_aov_light_group_9,
	p_aov_light_group_10,
	p_aov_light_group_11,
	p_aov_light_group_12,
	p_aov_light_group_13,
	p_aov_light_group_14,
	p_aov_light_group_15,
	p_aov_light_group_16,
	p_aov_id_1,
	p_aov_id_2,
	p_aov_id_3,
	p_aov_id_4,
	p_aov_id_5,
	p_aov_id_6,
	p_aov_id_7,
	p_aov_id_8,
	p_crypto_asset_override,
	p_crypto_object_override,
	p_crypto_material_override,
	p_aov_crypto_asset,
	p_aov_crypto_object,
	p_aov_crypto_material
};

enum AovIndices
{
	k_diffuse_color = 0,
	k_direct_diffuse,
	k_indirect_diffuse,
	k_direct_local,
	k_indirect_local,
	k_direct_global,
	k_indirect_global,
	k_direct_specular,
	k_indirect_specular,
	k_direct_specular_2,
	k_indirect_specular_2,
	k_direct_glint,
	k_indirect_glint,
	k_direct_transmission,
	k_indirect_transmission,
	k_depth,
	k_light_group_1,
	k_light_group_2,
	k_light_group_3,
	k_light_group_4,
	k_light_group_5,
	k_light_group_6,
	k_light_group_7,
	k_light_group_8,
	k_light_group_9,
	k_light_group_10,
	k_light_group_11,
	k_light_group_12,
	k_light_group_13,
	k_light_group_14,
	k_light_group_15,
	k_light_group_16,
	k_id_1,
	k_id_2,
	k_id_3,
	k_id_4,
	k_id_5,
	k_id_6,
	k_id_7,
	k_id_8
};

enum ScatteringMode
{
	KK = 0,
	DUAL
};

static const char* id_names[NUM_ID_AOVS] =
{
	"id_1",
	"id_2",
	"id_3",
	"id_4",
	"id_5",
	"id_6",
	"id_7",
	"id_8",
};


const char* ScatteringModeNames[] =
{
	"kajiya-kay",
	"dual-scattering",
	NULL
};
struct LutgenThreadData
{
	DualScattering* ds;
	ScatteringParams* sp;
	int start;
	int end;
};


unsigned int LutgenThreadFunc(void* data);