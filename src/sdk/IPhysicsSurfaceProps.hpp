struct surfacedata_t;

class IPhysicsSurfaceProps
{
public:

	virtual ~IPhysicsSurfaceProps(void) {}

	virtual int	ParseSurfaceData(const char *pFilename, const char *pTextfile) = 0;

	virtual int	SurfacePropCount(void) = 0;

	virtual int GetSurfaceIndex(const char *pSurfacePropName) = 0;
	virtual void GetPhysicsProperties(int surfaceDataIndex, float *density, float *thickness, float *friction, float *elasticity) = 0;

	virtual surfacedata_t* GetSurfaceData(int surfaceDataIndex) = 0;
};

struct surfacephysicsparams_t
{
	float			friction;
	float			elasticity;				// collision elasticity - used to compute coefficient of restitution
	float			density;				// physical density (in kg / m^3)
	float			thickness;				// material thickness if not solid (sheet materials) in inches
	float			dampening;
};

struct surfaceaudioparams_t
{
	float			reflectivity;		// like elasticity, but how much sound should be reflected by this surface
	float			hardnessFactor;	// like elasticity, but only affects impact sound choices
	float			roughnessFactor;	// like friction, but only affects scrape sound choices
	float			roughThreshold;	// surface roughness > this causes "rough" scrapes, < this causes "smooth" scrapes
	float			hardThreshold;	// surface hardness > this causes "hard" impacts, < this causes "soft" impacts
	float			hardVelocityThreshold;	// collision velocity > this causes "hard" impacts, < this causes "soft" impacts
	float			highPitchOcclusion;       //a value betweeen 0 and 100 where 0 is not occluded at all and 100 is silent (except for any additional reflected sound)
	float			midPitchOcclusion;
	float			lowPitchOcclusion;
};

struct surfacesoundnames_t
{
	unsigned short		walkStepLeft;
	unsigned short		walkStepRight;
	unsigned short		runStepLeft;
	unsigned short		runStepRight;
	unsigned short		impactSoft;
	unsigned short		impactHard;
	unsigned short		scrapeSmooth;
	unsigned short		scrapeRough;
	unsigned short		bulletImpact;
	unsigned short		rolling;
	unsigned short		breakSound;
	unsigned short		strainSound;
};

struct surfacegameprops_t
{
	float		maxspeedfactor;
	float		jumpfactor;
	float		penetrationmodifier;
	float		damagemodifier;
	uint16_t	material;
	uint8_t		climbable;
};

struct surfacedata_t
{
	surfacephysicsparams_t	physics;	// physics parameters
	surfaceaudioparams_t	audio;		// audio parameters
	surfacesoundnames_t		sounds;		// names of linked sounds
	surfacegameprops_t		game;		// Game data / properties
};
