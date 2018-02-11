#pragma once

// warning: this file is pretty bad

enum MaterialPropertyTypes_t
{
	MATERIAL_PROPERTY_NEEDS_LIGHTMAP = 0, // bool
	MATERIAL_PROPERTY_OPACITY, // int (enum MaterialPropertyOpacityTypes_t)
	MATERIAL_PROPERTY_REFLECTIVITY, // vec3_t
	MATERIAL_PROPERTY_NEEDS_BUMPED_LIGHTMAPS // bool
};

enum ImageFormat
{
	IMAGE_FORMAT_UNKNOWN = -1,
	IMAGE_FORMAT_RGBA8888 = 0,
	IMAGE_FORMAT_ABGR8888,
	IMAGE_FORMAT_RGB888,
	IMAGE_FORMAT_BGR888,
	IMAGE_FORMAT_RGB565,
	IMAGE_FORMAT_I8,
	IMAGE_FORMAT_IA88,
	IMAGE_FORMAT_P8,
	IMAGE_FORMAT_A8,
	IMAGE_FORMAT_RGB888_BLUESCREEN,
	IMAGE_FORMAT_BGR888_BLUESCREEN,
	IMAGE_FORMAT_ARGB8888,
	IMAGE_FORMAT_BGRA8888,
	IMAGE_FORMAT_DXT1,
	IMAGE_FORMAT_DXT3,
	IMAGE_FORMAT_DXT5,
	IMAGE_FORMAT_BGRX8888,
	IMAGE_FORMAT_BGR565,
	IMAGE_FORMAT_BGRX5551,
	IMAGE_FORMAT_BGRA4444,
	IMAGE_FORMAT_DXT1_ONEBITALPHA,
	IMAGE_FORMAT_BGRA5551,
	IMAGE_FORMAT_UV88,
	IMAGE_FORMAT_UVWQ8888,
	IMAGE_FORMAT_RGBA16161616F,
	IMAGE_FORMAT_RGBA16161616,
	IMAGE_FORMAT_UVLX8888,
	IMAGE_FORMAT_R32F, // Single-channel 32-bit floating point
	IMAGE_FORMAT_RGB323232F, // NOTE: D3D9 does not have this format
	IMAGE_FORMAT_RGBA32323232F,
	IMAGE_FORMAT_RG1616F,
	IMAGE_FORMAT_RG3232F,
	IMAGE_FORMAT_RGBX8888,

	IMAGE_FORMAT_NULL, // Dummy format which takes no video memory

	// Compressed normal map formats
	IMAGE_FORMAT_ATI2N, // One-surface ATI2N / DXN format
	IMAGE_FORMAT_ATI1N, // Two-surface ATI1N format

	IMAGE_FORMAT_RGBA1010102, // 10 bit-per component render targets
	IMAGE_FORMAT_BGRA1010102,
	IMAGE_FORMAT_R16F, // 16 bit FP format

	// Depth-stencil texture formats
	IMAGE_FORMAT_D16,
	IMAGE_FORMAT_D15S1,
	IMAGE_FORMAT_D32,
	IMAGE_FORMAT_D24S8,
	IMAGE_FORMAT_LINEAR_D24S8,
	IMAGE_FORMAT_D24X8,
	IMAGE_FORMAT_D24X4S4,
	IMAGE_FORMAT_D24FS8,
	IMAGE_FORMAT_D16_SHADOW, // Specific formats for shadow mapping
	IMAGE_FORMAT_D24X8_SHADOW, // Specific formats for shadow mapping

	// supporting these specific formats as non-tiled for procedural cpu access (360-specific)
	IMAGE_FORMAT_LINEAR_BGRX8888,
	IMAGE_FORMAT_LINEAR_RGBA8888,
	IMAGE_FORMAT_LINEAR_ABGR8888,
	IMAGE_FORMAT_LINEAR_ARGB8888,
	IMAGE_FORMAT_LINEAR_BGRA8888,
	IMAGE_FORMAT_LINEAR_RGB888,
	IMAGE_FORMAT_LINEAR_BGR888,
	IMAGE_FORMAT_LINEAR_BGRX5551,
	IMAGE_FORMAT_LINEAR_I8,
	IMAGE_FORMAT_LINEAR_RGBA16161616,

	IMAGE_FORMAT_LE_BGRX8888,
	IMAGE_FORMAT_LE_BGRA8888,

	NUM_IMAGE_FORMATS
};

class IMaterialVar;
class ITexture;
class IMaterialProxy;
class Vector;

typedef int64_t VertexFormat_t;

// We're testing 2 normal compression methods
// One compressed normals+tangents into a SHORT2 each (8 bytes total)
// The other compresses them together, into a single UBYTE4 (4 bytes total)
// FIXME: pick one or the other, compare lighting quality in important cases
#define COMPRESSED_NORMALS_SEPARATETANGENTS_SHORT2	0
#define COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4	1
//#define COMPRESSED_NORMALS_TYPE						COMPRESSED_NORMALS_SEPARATETANGENTS_SHORT2
#define COMPRESSED_NORMALS_TYPE						COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4

//-----------------------------------------------------------------------------
// Shader state flags can be read from the FLAGS materialvar
// Also can be read or written to with the Set/GetMaterialVarFlags() call
// Also make sure you add/remove a string associated with each flag below to CShaderSystem::ShaderStateString in ShaderSystem.cpp
//-----------------------------------------------------------------------------
enum MaterialVarFlags_t
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),

	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	//	MATERIAL_VAR_UNUSED					  = (1 << 9),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17), // OBSOLETE
										   //	MATERIAL_VAR_UNUSED					  = (1 << 18),
										   MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19), // OBSOLETE
										   MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
										   MATERIAL_VAR_TRANSLUCENT = (1 << 21),
										   MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
										   MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23), // OBSOLETE
										   MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
										   MATERIAL_VAR_ENVMAPMODE = (1 << 25), // OBSOLETE
										   MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
										   MATERIAL_VAR_HALFLAMBERT = (1 << 27),
										   MATERIAL_VAR_WIREFRAME = (1 << 28),
										   MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
										   MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = (1 << 30),
										   MATERIAL_VAR_VERTEXFOG = (1 << 31),

										   // NOTE: Only add flags here that either should be read from
										   // .vmts or can be set directly from client code. Other, internal
										   // flags should to into the flag enum in IMaterialInternal.h
};

//-----------------------------------------------------------------------------
// Internal flags not accessible from outside the material system. Stored in Flags2
//-----------------------------------------------------------------------------
enum MaterialVarFlags2_t
{
	// NOTE: These are for $flags2!!!!!
	//	UNUSED											= (1 << 0),

	MATERIAL_VAR2_LIGHTING_UNLIT = 0,
	MATERIAL_VAR2_LIGHTING_VERTEX_LIT = (1 << 1),
	MATERIAL_VAR2_LIGHTING_LIGHTMAP = (1 << 2),
	MATERIAL_VAR2_LIGHTING_BUMPED_LIGHTMAP = (1 << 3),
	MATERIAL_VAR2_LIGHTING_MASK =
	(MATERIAL_VAR2_LIGHTING_VERTEX_LIT |
		MATERIAL_VAR2_LIGHTING_LIGHTMAP |
		MATERIAL_VAR2_LIGHTING_BUMPED_LIGHTMAP),

	// FIXME: Should this be a part of the above lighting enums?
	MATERIAL_VAR2_DIFFUSE_BUMPMAPPED_MODEL = (1 << 4),
	MATERIAL_VAR2_USES_ENV_CUBEMAP = (1 << 5),
	MATERIAL_VAR2_NEEDS_TANGENT_SPACES = (1 << 6),
	MATERIAL_VAR2_NEEDS_SOFTWARE_LIGHTING = (1 << 7),
	// GR - HDR path puts lightmap alpha in separate texture...
	MATERIAL_VAR2_BLEND_WITH_LIGHTMAP_ALPHA = (1 << 8),
	MATERIAL_VAR2_NEEDS_BAKED_LIGHTING_SNAPSHOTS = (1 << 9),
	MATERIAL_VAR2_USE_FLASHLIGHT = (1 << 10),
	MATERIAL_VAR2_USE_FIXED_FUNCTION_BAKED_LIGHTING = (1 << 11),
	MATERIAL_VAR2_NEEDS_FIXED_FUNCTION_FLASHLIGHT = (1 << 12),
	MATERIAL_VAR2_USE_EDITOR = (1 << 13),
	MATERIAL_VAR2_NEEDS_POWER_OF_TWO_FRAME_BUFFER_TEXTURE = (1 << 14),
	MATERIAL_VAR2_NEEDS_FULL_FRAME_BUFFER_TEXTURE = (1 << 15),
	MATERIAL_VAR2_IS_SPRITECARD = (1 << 16),
	MATERIAL_VAR2_USES_VERTEXID = (1 << 17),
	MATERIAL_VAR2_SUPPORTS_HW_SKINNING = (1 << 18),
	MATERIAL_VAR2_SUPPORTS_FLASHLIGHT = (1 << 19),
	MATERIAL_VAR2_USE_GBUFFER0 = (1 << 20),
	MATERIAL_VAR2_USE_GBUFFER1 = (1 << 21),
	MATERIAL_VAR2_SELFILLUMMASK = (1 << 22),
	MATERIAL_VAR2_SUPPORTS_TESSELLATION = (1 << 23)
};

//-----------------------------------------------------------------------------
// Preview image return values
//-----------------------------------------------------------------------------
enum PreviewImageRetVal_t
{
	MATERIAL_PREVIEW_IMAGE_BAD = 0,
	MATERIAL_PREVIEW_IMAGE_OK,
	MATERIAL_NO_PREVIEW_IMAGE,
};

//-----------------------------------------------------------------------------
// material interface
//-----------------------------------------------------------------------------
class IMaterial
{
public:
	// Get the name of the material.  This is a full path to 
	// the vmt file starting from "hl2/materials" (or equivalent) without
	// a file extension.
	virtual const char* GetName() const = 0;
	virtual const char* GetTextureGroupName() const = 0;

	// Get the preferred size/bitDepth of a preview image of a material.
	// This is the sort of image that you would use for a thumbnail view
	// of a material, or in WorldCraft until it uses materials to render.
	// separate this for the tools maybe
	virtual PreviewImageRetVal_t GetPreviewImageProperties(int* width, int* height, ImageFormat* imageFormat, bool* isTranslucent) const = 0;

	// Get a preview image at the specified width/height and bitDepth.
	// Will do resampling if necessary.(not yet!!! :) )
	// Will do color format conversion. (works now.)
	virtual PreviewImageRetVal_t GetPreviewImage(unsigned char* data, int width, int height, ImageFormat imageFormat) const = 0;
	// 
	virtual int GetMappingWidth() = 0;
	virtual int GetMappingHeight() = 0;

	virtual int GetNumAnimationFrames() = 0;

	// For material subrects (material pages).  Offset(u,v) and scale(u,v) are normalized to texture.
	virtual bool InMaterialPage(void) = 0;
	virtual void GetMaterialOffset(float* pOffset) = 0;
	virtual void GetMaterialScale(float* pScale) = 0;
	virtual IMaterial* GetMaterialPage(void) = 0;

	// find a vmt variable.
	// This is how game code affects how a material is rendered.
	// The game code must know about the params that are used by
	// the shader for the material that it is trying to affect.
	virtual IMaterialVar* FindVar(const char* varName, bool* found, bool complain = true) = 0;

	// The user never allocates or deallocates materials.  Reference counting is
	// used instead.  Garbage collection is done upon a call to 
	// IMaterialSystem::UncacheUnusedMaterials.
	virtual void IncrementReferenceCount(void) = 0;
	virtual void DecrementReferenceCount(void) = 0;

	inline void AddRef()
	{
		IncrementReferenceCount();
	}

	inline void Release()
	{
		DecrementReferenceCount();
	}

	// Each material is assigned a number that groups it with like materials
	// for sorting in the application.
	virtual int GetEnumerationID(void) const = 0;

	virtual void GetLowResColorSample(float s, float t, float* color) const = 0;

	// This computes the state snapshots for this material
	virtual void RecomputeStateSnapshots() = 0;

	// Are we translucent?
	virtual bool IsTranslucent() = 0;

	// Are we alphatested?
	virtual bool IsAlphaTested() = 0;

	// Are we vertex lit?
	virtual bool IsVertexLit() = 0;

	// Gets the vertex format
	virtual VertexFormat_t GetVertexFormat() const = 0;

	// returns true if this material uses a material proxy
	virtual bool HasProxy(void) const = 0;

	virtual bool UsesEnvCubemap(void) = 0;

	virtual bool NeedsTangentSpace(void) = 0;

	virtual bool NeedsPowerOfTwoFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool NeedsFullFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;

	// returns true if the shader doesn't do skinning itself and requires
	// the data that is sent to it to be preskinned.
	virtual bool NeedsSoftwareSkinning(void) = 0;

	// Apply constant color or alpha modulation
	virtual void AlphaModulate(float alpha) = 0;
	virtual void ColorModulate(float r, float g, float b) = 0;

	// Material Var flags...
	virtual void SetMaterialVarFlag(MaterialVarFlags_t flag, bool on) = 0;
	virtual bool GetMaterialVarFlag(MaterialVarFlags_t flag) = 0;

	// Gets material reflectivity
	virtual void GetReflectivity(Vector& reflect) = 0;

	// Gets material property flags
	virtual bool GetPropertyFlag(MaterialPropertyTypes_t type) = 0;

	// Is the material visible from both sides?
	virtual bool IsTwoSided() = 0;

	// Sets the shader associated with the material
	virtual void SetShader(const char* pShaderName) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int GetNumPasses(void) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int GetTextureMemoryBytes(void) = 0;

	// Meant to be used with materials created using CreateMaterial
	// It updates the materials to reflect the current values stored in the material vars
	virtual void Refresh() = 0;

	// GR - returns true is material uses lightmap alpha for blending
	virtual bool NeedsLightmapBlendAlpha(void) = 0;

	// returns true if the shader doesn't do lighting itself and requires
	// the data that is sent to it to be prelighted
	virtual bool NeedsSoftwareLighting(void) = 0;

	// Gets at the shader parameters
	virtual int ShaderParamCount() const = 0;
	virtual IMaterialVar** GetShaderParams(void) = 0;

	// Returns true if this is the error material you get back from IMaterialSystem::FindMaterial if
	// the material can't be found.
	virtual bool IsErrorMaterial() const = 0;

	virtual void Unused() = 0;

	// Gets the current alpha modulation
	virtual float GetAlphaModulation() = 0;
	virtual void GetColorModulation(float* r, float* g, float* b) = 0;

	// Is this translucent given a particular alpha modulation?
	virtual bool IsTranslucentUnderModulation(float fAlphaModulation = 1.0f) const = 0;

	// fast find that stores the index of the found var in the string table in local cache
	virtual IMaterialVar* FindVarFast(char const* pVarName, unsigned int* pToken) = 0;

	// Sets new VMT shader parameters for the material
	virtual void SetShaderAndParams(void* pKeyValues) = 0;
	virtual const char* GetShaderName() const = 0;

	virtual void DeleteIfUnreferenced() = 0;

	virtual bool IsSpriteCard() = 0;

	virtual void CallBindProxy(void* proxyData) = 0;

	virtual void RefreshPreservingMaterialVars() = 0;

	virtual bool WasReloadedFromWhitelist() = 0;

	virtual bool SetTempExcluded(bool bSet, int nExcludedDimensionLimit) = 0;

	virtual int GetReferenceCount() const = 0;
};


// all below is bad

#define DECLARE_POINTER_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
#define MAXSTUDIOSKINS	32

#define TEXTURE_GROUP_LIGHTMAP						        "Lightmaps"
#define TEXTURE_GROUP_WORLD							          "World textures"
#define TEXTURE_GROUP_MODEL							          "Model textures"
#define TEXTURE_GROUP_VGUI							          "VGUI textures"
#define TEXTURE_GROUP_PARTICLE						        "Particle textures"
#define TEXTURE_GROUP_DECAL							          "Decal textures"
#define TEXTURE_GROUP_SKYBOX						          "SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				      "ClientEffect textures"
#define TEXTURE_GROUP_OTHER							          "Other textures"
#define TEXTURE_GROUP_PRECACHED						        "Precached"
#define TEXTURE_GROUP_CUBE_MAP						        "CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					      "RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED					        "Unaccounted textures"
//#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER		  "Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			    "Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			  "Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			  "Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					      "DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					        "ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					      "Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				      "Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			  "RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					      "Morph Targets"

class IMaterial;
class IMesh;
class IVertexBuffer;
class IIndexBuffer;
struct MaterialSystem_Config_t;
class matrix3x4_t;
class ITexture;
struct MaterialSystemHWID_t;
class KeyValues;
class IShader;
class IVertexTexture;
class IMorph;
class IMatRenderContext;
class ICallQueue;
struct MorphWeight_t;
class IFileList;
struct VertexStreamSpec_t;
struct ShaderStencilState_t;
struct MeshInstanceData_t;
class IClientMaterialSystem;
class CPaintMaterial;
class IPaintMapDataManager;
class IPaintMapTextureManager;
class GPUMemoryStats;
struct AspectRatioInfo_t;
struct CascadedShadowMappingState_t;

class IMaterialProxyFactory;
class ITexture;
class IMaterialSystemHardwareConfig;
class CShadowMgr;

enum CompiledVtfFlags
{
	TEXTUREFLAGS_POINTSAMPLE = 0x00000001,
	TEXTUREFLAGS_TRILINEAR = 0x00000002,
	TEXTUREFLAGS_CLAMPS = 0x00000004,
	TEXTUREFLAGS_CLAMPT = 0x00000008,
	TEXTUREFLAGS_ANISOTROPIC = 0x00000010,
	TEXTUREFLAGS_HINT_DXT5 = 0x00000020,
	TEXTUREFLAGS_PWL_CORRECTED = 0x00000040,
	TEXTUREFLAGS_NORMAL = 0x00000080,
	TEXTUREFLAGS_NOMIP = 0x00000100,
	TEXTUREFLAGS_NOLOD = 0x00000200,
	TEXTUREFLAGS_ALL_MIPS = 0x00000400,
	TEXTUREFLAGS_PROCEDURAL = 0x00000800,
	TEXTUREFLAGS_ONEBITALPHA = 0x00001000,
	TEXTUREFLAGS_EIGHTBITALPHA = 0x00002000,
	TEXTUREFLAGS_ENVMAP = 0x00004000,
	TEXTUREFLAGS_RENDERTARGET = 0x00008000,
	TEXTUREFLAGS_DEPTHRENDERTARGET = 0x00010000,
	TEXTUREFLAGS_NODEBUGOVERRIDE = 0x00020000,
	TEXTUREFLAGS_SINGLECOPY = 0x00040000,
	TEXTUREFLAGS_PRE_SRGB = 0x00080000,
	TEXTUREFLAGS_UNUSED_00100000 = 0x00100000,
	TEXTUREFLAGS_UNUSED_00200000 = 0x00200000,
	TEXTUREFLAGS_UNUSED_00400000 = 0x00400000,
	TEXTUREFLAGS_NODEPTHBUFFER = 0x00800000,
	TEXTUREFLAGS_UNUSED_01000000 = 0x01000000,
	TEXTUREFLAGS_CLAMPU = 0x02000000,
	TEXTUREFLAGS_VERTEXTEXTURE = 0x04000000,
	TEXTUREFLAGS_SSBUMP = 0x08000000,
	TEXTUREFLAGS_UNUSED_10000000 = 0x10000000,
	TEXTUREFLAGS_BORDER = 0x20000000,
	TEXTUREFLAGS_UNUSED_40000000 = 0x40000000,
	TEXTUREFLAGS_UNUSED_80000000 = 0x80000000
};

enum StandardLightmap_t
{
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_WHITE = -1,
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_WHITE_BUMP = -2,
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_USER_DEFINED = -3
};


struct MaterialSystem_SortInfo_t
{
	IMaterial	*material;
	int			lightmapPageID;
};

enum MaterialThreadMode_t
{
	MATERIAL_SINGLE_THREADED,
	MATERIAL_QUEUED_SINGLE_THREADED,
	MATERIAL_QUEUED_THREADED
};

enum MaterialContextType_t
{
	MATERIAL_HARDWARE_CONTEXT,
	MATERIAL_QUEUED_CONTEXT,
	MATERIAL_NULL_CONTEXT
};

enum
{
	MATERIAL_ADAPTER_NAME_LENGTH = 512
};

struct MaterialTextureInfo_t
{
	int iExcludeInformation;
};

struct ApplicationPerformanceCountersInfo_t
{
	float msMain;
	float msMST;
	float msGPU;
	float msFlip;
	float msTotal;
};

struct ApplicationInstantCountersInfo_t
{
	uint32_t m_nCpuActivityMask;
	uint32_t m_nDeferredWordsAllocated;
};
struct MaterialAdapterInfo_t
{
	char m_pDriverName[MATERIAL_ADAPTER_NAME_LENGTH];
	unsigned int m_VendorID;
	unsigned int m_DeviceID;
	unsigned int m_SubSysID;
	unsigned int m_Revision;
	int m_nDXSupportLevel;			// This is the *preferred* dx support level
	int m_nMinDXSupportLevel;
	int m_nMaxDXSupportLevel;
	unsigned int m_nDriverVersionHigh;
	unsigned int m_nDriverVersionLow;
};

struct MaterialVideoMode_t
{
	int m_Width;			// if width and height are 0 and you select 
	int m_Height;			// windowed mode, it'll use the window size
	ImageFormat m_Format;	// use ImageFormats (ignored for windowed mode)
	int m_RefreshRate;		// 0 == default (ignored for windowed mode)
};
enum HDRType_t
{
	HDR_TYPE_NONE,
	HDR_TYPE_INTEGER,
	HDR_TYPE_FLOAT,
};

enum RestoreChangeFlags_t
{
	MATERIAL_RESTORE_VERTEX_FORMAT_CHANGED = 0x1,
	MATERIAL_RESTORE_RELEASE_MANAGED_RESOURCES = 0x2,
};

enum RenderTargetSizeMode_t
{
	RT_SIZE_NO_CHANGE = 0,
	RT_SIZE_DEFAULT = 1,
	RT_SIZE_PICMIP = 2,
	RT_SIZE_HDR = 3,
	RT_SIZE_FULL_FRAME_BUFFER = 4,
	RT_SIZE_OFFSCREEN = 5,
	RT_SIZE_FULL_FRAME_BUFFER_ROUNDED_UP = 6
};

enum MaterialRenderTargetDepth_t
{
	MATERIAL_RT_DEPTH_SHARED = 0x0,
	MATERIAL_RT_DEPTH_SEPARATE = 0x1,
	MATERIAL_RT_DEPTH_NONE = 0x2,
	MATERIAL_RT_DEPTH_ONLY = 0x3,
};

typedef void(*MaterialBufferReleaseFunc_t)(int nChangeFlags);	// see RestoreChangeFlags_t
typedef void(*MaterialBufferRestoreFunc_t)(int nChangeFlags);	// see RestoreChangeFlags_t
typedef void(*ModeChangeCallbackFunc_t)(void);
typedef void(*EndFrameCleanupFunc_t)(void);
typedef bool(*EndFramePriorToNextContextFunc_t)(void);
typedef void(*OnLevelShutdownFunc_t)(void *pUserData);

typedef unsigned short MaterialHandle_t;
DECLARE_POINTER_HANDLE(MaterialLock_t);
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

class IMaterialSystem : public IAppSystem
{
public:

	virtual CreateInterfaceFn               Init(char const* pShaderAPIDLL, IMaterialProxyFactory *pMaterialProxyFactory, CreateInterfaceFn fileSystemFactory, CreateInterfaceFn cvarFactory = NULL) = 0;
	virtual void                            SetShaderAPI(char const *pShaderAPIDLL) = 0;
	virtual void                            SetAdapter(int nAdapter, int nFlags) = 0;
	virtual void                            ModInit() = 0;
	virtual void                            ModShutdown() = 0;
	virtual void                            SetThreadMode(MaterialThreadMode_t mode, int nServiceThread = -1) = 0;
	virtual MaterialThreadMode_t            GetThreadMode() = 0;
	virtual void                            ExecuteQueued() = 0;
	virtual void                            OnDebugEvent(const char *pEvent) = 0;
	virtual IMaterialSystemHardwareConfig*  GetHardwareConfig(const char *pVersion, int *returnCode) = 0;
	virtual void                            __unknown() = 0;
	virtual bool                            UpdateConfig(bool bForceUpdate) = 0; //20
	virtual bool                            OverrideConfig(const MaterialSystem_Config_t &config, bool bForceUpdate) = 0;
	virtual const MaterialSystem_Config_t&  GetCurrentConfigForVideoCard() const = 0;
	virtual bool                            GetRecommendedConfigurationInfo(int nDXLevel, KeyValues * pKeyValues) = 0;
	virtual int                             GetDisplayAdapterCount() const = 0;
	virtual int                             GetCurrentAdapter() const = 0;
	virtual void                            GetDisplayAdapterInfo(int adapter, MaterialAdapterInfo_t& info) const = 0;
	virtual int                             GetModeCount(int adapter) const = 0;
	virtual void                            GetModeInfo(int adapter, int mode, MaterialVideoMode_t& info) const = 0;
	virtual void                            AddModeChangeCallBack(ModeChangeCallbackFunc_t func) = 0;
	virtual void                            GetDisplayMode(MaterialVideoMode_t& mode) const = 0; //30
	virtual bool                            SetMode(void* hwnd, const MaterialSystem_Config_t &config) = 0;
	virtual bool                            SupportsMSAAMode(int nMSAAMode) = 0;
	virtual const MaterialSystemHWID_t&     GetVideoCardIdentifier(void) const = 0;
	virtual void                            SpewDriverInfo() const = 0;
	virtual void                            GetBackBufferDimensions(int &width, int &height) const = 0;
	virtual ImageFormat                     GetBackBufferFormat() const = 0;
	virtual const AspectRatioInfo_t&        GetAspectRatioInfo() const = 0;
	virtual bool                            SupportsHDRMode(HDRType_t nHDRModede) = 0;
	virtual bool                            AddView(void* hwnd) = 0;
	virtual void                            RemoveView(void* hwnd) = 0; //40
	virtual void                            SetView(void* hwnd) = 0;
	virtual void                            BeginFrame(float frameTime) = 0;
	virtual void                            EndFrame() = 0;
	virtual void                            Flush(bool flushHardware = false) = 0;
	virtual unsigned int                    GetCurrentFrameCount() = 0;
	virtual void                            SwapBuffers() = 0;
	virtual void                            EvictManagedResources() = 0;
	virtual void                            ReleaseResources(void) = 0;
	virtual void                            ReacquireResources(void) = 0;
	virtual void                            AddReleaseFunc(MaterialBufferReleaseFunc_t func) = 0; //50
	virtual void                            RemoveReleaseFunc(MaterialBufferReleaseFunc_t func) = 0;
	virtual void                            AddRestoreFunc(MaterialBufferRestoreFunc_t func) = 0;
	virtual void                            RemoveRestoreFunc(MaterialBufferRestoreFunc_t func) = 0;
	virtual void                            AddEndFrameCleanupFunc(EndFrameCleanupFunc_t func) = 0;
	virtual void                            RemoveEndFrameCleanupFunc(EndFrameCleanupFunc_t func) = 0;
	virtual void                            OnLevelShutdown() = 0;
	virtual bool                            AddOnLevelShutdownFunc(OnLevelShutdownFunc_t func, void *pUserData) = 0;
	virtual bool                            RemoveOnLevelShutdownFunc(OnLevelShutdownFunc_t func, void *pUserData) = 0;
	virtual void                            OnLevelLoadingComplete() = 0;
	virtual void                            ResetTempHWMemory(bool bExitingLevel = false) = 0; //60
	virtual void                            HandleDeviceLost() = 0;
	virtual int                             ShaderCount() const = 0;
	virtual int                             GetShaders(int nFirstShader, int nMaxCount, IShader **ppShaderList) const = 0;
	virtual int                             ShaderFlagCount() const = 0;
	virtual const char*                     ShaderFlagName(int nIndex) const = 0;
	virtual void                            GetShaderFallback(const char *pShaderName, char *pFallbackShader, int nFallbackLength) = 0;
	virtual IMaterialProxyFactory*          GetMaterialProxyFactory() = 0;
	virtual void                            SetMaterialProxyFactory(IMaterialProxyFactory* pFactory) = 0;
	virtual void                            EnableEditorMaterials() = 0;
	virtual void                            EnableGBuffers() = 0; //70
	virtual void                            SetInStubMode(bool bInStubMode) = 0;
	virtual void                            DebugPrintUsedMaterials(const char *pSearchSubString, bool bVerbose) = 0;
	virtual void                            DebugPrintUsedTextures(void) = 0;
	virtual void                            ToggleSuppressMaterial(char const* pMaterialName) = 0;
	virtual void                            ToggleDebugMaterial(char const* pMaterialName) = 0;
	virtual bool                            UsingFastClipping(void) = 0;
	virtual int                             StencilBufferBits(void) = 0; //number of bits per pixel in the stencil buffer
	virtual void                            UncacheAllMaterials() = 0;
	virtual void                            UncacheUnusedMaterials(bool bRecomputeStateSnapshots = false) = 0;
	virtual void                            CacheUsedMaterials() = 0; //80
	virtual void                            ReloadTextures() = 0;
	virtual void                            ReloadMaterials(const char *pSubString = NULL) = 0;
	virtual IMaterial*                      CreateMaterial(const char *pMaterialName, KeyValues *pVMTKeyValues) = 0;
	virtual IMaterial*                      FindMaterial(char const* pMaterialName, const char *pTextureGroupName, bool complain = true, const char *pComplainPrefix = NULL) = 0;
	virtual void						    LoadKeyValuesFromVMTFile(KeyValues& values, const char* name, bool somebool) const = 0;
	virtual MaterialHandle_t                FirstMaterial() const = 0;
	virtual MaterialHandle_t                NextMaterial(MaterialHandle_t h) const = 0;
	virtual MaterialHandle_t                InvalidMaterial() const = 0;
	virtual IMaterial*                      GetMaterial(MaterialHandle_t h) const = 0;
	virtual int                             GetNumMaterials() const = 0;
	virtual ITexture*                       FindTexture(char const* pTextureName, const char *pTextureGroupName, bool complain = true) = 0;
	virtual bool                            IsTextureLoaded(char const* pTextureName) const = 0;
	virtual ITexture*                       CreateProceduralTexture(const char	*pTextureName, const char *pTextureGroupName, int w, int h, ImageFormat fmt, int nFlags) = 0;
	virtual void                            BeginRenderTargetAllocation() = 0;
	virtual void                            EndRenderTargetAllocation() = 0; // Simulate an Alt-Tab in here, which causes a release/restore of all resources
	virtual ITexture*                       CreateRenderTargetTexture(int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat	format, MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED) = 0;
	virtual ITexture*                       CreateNamedRenderTargetTextureEx(const char *pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED, unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT, unsigned int renderTargetFlags = 0) = 0;
	virtual ITexture*                       CreateNamedRenderTargetTexture(const char *pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED, bool bClampTexCoords = true, bool bAutoMipMap = false) = 0;
	virtual ITexture*                       CreateNamedRenderTargetTextureEx2(const char *pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED, unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT, unsigned int renderTargetFlags = 0) = 0;
};
