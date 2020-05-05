#ifndef LWPP_BXDF_H
#define LWPP_BXDF_H

#include <lwbxdf.h>
#include <lwpp/global.h>

#define BXDF_DIFFUSE (LWBXDF_REFLECTION | LWBXDF_DIFFUSE)
#define BXDF_SPECULAR (LWBXDF_REFLECTION | LWBXDF_GLOSSY)
#define BXDF_REFLECTION (LWBXDF_REFLECTION | LWBXDF_SPECULAR)

#define BXDF_TRANSLUCENCY (LWBXDF_TRANSMISSION | LWBXDF_DIFFUSE)
#define BXDF_SOFTREFRACTION (LWBXDF_TRANSMISSION | LWBXDF_GLOSSY)
#define BXDF_REFRACTION (LWBXDF_TRANSMISSION | LWBXDF_SPECULAR)

namespace lwpp
{

  class BSDFFuncs : public GlobalBase<LWBSDFFuncs>
  {
  public:
	  LWBSDF createBSDF()
	  {
		  return globPtr->createBSDF();
	  }
	  void destroyBSDF(LWBSDF bsdf)
	  {
		  globPtr->destroyBSDF(bsdf);
	  }
    /*
    void resetBSDF (LWBSDF bsdf);

    LWBxDF createBxDF(LWBxDF_F* f, LWBxDF_SampleF* sampleF, size_t memorySize, unsigned int flags, const char* label);
    void destroyBxDF (LWBxDF);

    LWBSSRDF createBSSRDF (const char* label);
    void destroyBSSRDF (LWBSSRDF);

    LWBTDFVolume(*createBTDFVolume)(LWBTDFVolume_Extinction* extinctionF,
                                    LWBTDFVolume_Sample* sampleF,
                                    LWBTDFVolume_Phase* phaseF,
                                    LWBTDFVolume_SamplePhase* samplePhaseF,
                                    size_t memorySize);

    void(*destroyBTDFVolume)(LWBTDFVolume);
	*/
    // Add a BxDF to the BSDF. Returns the LWBxDF that was added, 0 if failed.
	LWBxDF addBxDF (LWBSDF bsdf, LWBxDF bxdf, LWUserData data, const LWDVector weight, double roughness = 0.0)
	{
#if (LWSDKVER_MAJOR < 2020)
	  return globPtr->addBxDF(bsdf, bxdf, data, weight);
#else
		return globPtr->addBxDF(bsdf, bxdf, data, weight, roughness);
#endif
	}
	/*
    // Only use addVolume for BxDFs returned by addBxDF and BxDFs which are BTDFs.
    int(*addVolume)(LWBSDF bsdf, LWBxDF bxdf, LWBTDFVolume btdfVolume, LWUserData data, LWBTDFVolumeType type, double stepSize);
    */

    unsigned int numBxDF (LWBSDF bsdf, unsigned int flags)
    {
      return globPtr->numBxDF(bsdf, flags);
    }

    LWBxDF getBxDF (LWBSDF bsdf, unsigned int index, unsigned int flags)
    {
      return globPtr->getBxDF(bsdf, index, flags);
    }
    /*
    double(*F)(LWBSDF bsdf, const LWDVector wo, const LWDVector wi, unsigned int flags, LWDVector f);
    LWBxDF(*sampleF)(LWBSDF bsdf, const LWShading* sa, const LWDVector sample, LWBxDFSampleOutput* so, unsigned int flags);

    // Only use the below functions for LWBxDFs acquired via getBxDF or returned by sampleF. BxDFs acquired using LWBxDFFuncs will not work.
    double(*BxDF_F)(LWBxDF bxdf, const LWDVector wo, const LWDVector wi, LWDVector f);
    void(*BxDF_SampleF)(LWBxDF bxdf, const LWShading* sa, const LWDVector sample, LWBxDFSampleOutput* so);
    */
    void BxDF_GetWeight(LWBxDF bxdf, LWDVector weight)
    {
      globPtr->BxDF_GetWeight(bxdf, weight);
    }
    void BxDF_SetWeight (LWBxDF bxdf, const LWDVector weight)
    {
      globPtr->BxDF_SetWeight(bxdf, weight);
    }
    /*
    LWAOVID(*BxDF_AOVID)(LWBxDF bxdf, LWBxDFAOVLabelType type);
    */
    unsigned int BxDF_Flags(LWBxDF bxdf)
    {
      return globPtr->BxDF_Flags(bxdf);
    }
    /*
    LWBTDFVolume(*BTDF_GetVolume)(LWBxDF btdf);
    LWBTDFVolumeType(*BTDF_VolumeType)(LWBTDFVolume btdfVolume);
    double(*BTDF_VolumeStepSize)(LWBTDFVolume btdfVolume);
    void(*BTDF_VolumeExtinction)(LWBTDFVolume btdfVolume, const LWVolumeSpot*, LWDVector extinction);
    void(*BTDF_VolumeSample)(LWBTDFVolume btdfVolume, const LWVolumeSpot*, LWDVector emission, LWDVector scattering, LWDVector absorption);
    double(*BTDF_VolumePhase)(LWBTDFVolume btdfVolume, const LWVolumeSpot*, const LWDVector wi, const LWDVector wo);
    double(*BTDF_VolumeSamplePhase)(LWBTDFVolume btdfVolume, const LWVolumeSpot*, const LWDVector sample, LWDVector wo);

    // Emission functions
	*/
    void getEmission (LWBSDF bsdf, LWDVector emission)
	{
		globPtr->getEmission(bsdf, emission);
	}
    void setEmission(LWBSDF bsdf, const LWDVector emission)
	{
		globPtr->setEmission(bsdf, emission);
	}
	/*
    // BSSRDF functions.
    int(*addBSSRDF)(LWBSDF bsdf, LWBSSRDF bssrdf, const LWDVector weight, const LWDVector scattering, double distance);
	*/
	unsigned int numBSSRDF (LWBSDF bsdf)
	{
		return globPtr->numBSSRDF(bsdf);
	}
#if (LWSDKVER_MAJOR < 2020)
    void BSSRDF_GetParameters (LWBSDF bsdf, unsigned int index, LWDVector weight, LWDVector scattering, double* distance)
	{

		globPtr->BSSRDF_GetParameters(bsdf, index, weight, scattering, distance);
	}
    void BSSRDF_SetParameters (LWBSDF bsdf, unsigned int index, const LWDVector weight, const LWDVector scattering, double distance)	
	{
		globPtr->BSSRDF_SetParameters(bsdf, index, weight, scattering, distance);
	}
#else
	void BSSRDF_GetParameters(LWBSDF bsdf, unsigned int index, LWDVector weight, LWDVector scattering, double* distance, double* asymmetry)
	{

		globPtr->BSSRDF_GetParameters(bsdf, index, weight, scattering, distance, asymmetry);
	}
	void BSSRDF_SetParameters(LWBSDF bsdf, unsigned int index, const LWDVector weight, const LWDVector scattering, double distance, double asymmetry)
	{
		globPtr->BSSRDF_SetParameters(bsdf, index, weight, scattering, distance, asymmetry);
	}
#endif

	/*
    LWBSSRDF(*getBSSRDF)(LWBSDF bsdf, unsigned int index);
    LWAOVID(*BSSRDF_AOVID)(LWBSSRDF bssrdf, LWBSSRDFAOVLabelType type);
    */
  };

  class BSDF : private BSDFFuncs
  {
	  LWBSDF mBSDF;
	  bool mDestroy = false;
  public:
	  BSDF(LWBSDF bsdf) : mBSDF(bsdf)
	  {}
	  LWBSDF get() { return mBSDF; }
	  unsigned int numBxDF(unsigned int flags)
	  {
		  return BSDFFuncs::numBxDF(mBSDF, flags);
	  }
	  LWBxDF getBxDF(unsigned int index, unsigned int flags)
	  {
		  return BSDFFuncs::getBxDF(mBSDF, index, flags);
	  }
	  void BxDF_GetWeight(LWBxDF bxdf, LWDVector weight)
	  {
		  BSDFFuncs::BxDF_GetWeight(bxdf, weight);
	  }
	  void BxDF_SetWeight(LWBxDF bxdf, const LWDVector weight)
	  {
		  BSDFFuncs::BxDF_SetWeight(bxdf, weight);
	  }
	  void getEmission(LWDVector emission)
	  {
		  BSDFFuncs::getEmission(mBSDF, emission);
	  }
	  void setEmission(const LWDVector emission)
	  {
		  BSDFFuncs::setEmission(mBSDF, emission);
	  }
	  unsigned int numBSSRDF()
	  {
		  return BSDFFuncs::numBSSRDF(mBSDF);
	  }
#if (LWSDKVER_MAJOR < 2020)
	  void BSSRDF_GetParameters(unsigned int index, LWDVector weight, LWDVector scattering, double* distance)
	  {
		  BSDFFuncs::BSSRDF_GetParameters(mBSDF, index, weight, scattering, distance);
	  }
	  void BSSRDF_SetParameters(unsigned int index, const LWDVector weight, const LWDVector scattering, double distance)
	  {
		  BSDFFuncs::BSSRDF_SetParameters(mBSDF, index, weight, scattering, distance);
	  }
#else
		void BSSRDF_GetParameters(unsigned int index, LWDVector weight, LWDVector scattering, double* distance, double *asymmetry)
		{
			BSDFFuncs::BSSRDF_GetParameters(mBSDF, index, weight, scattering, distance, asymmetry);
		}
		void BSSRDF_SetParameters(unsigned int index, const LWDVector weight, const LWDVector scattering, double distance, double asymmetry)
		{
			BSDFFuncs::BSSRDF_SetParameters(mBSDF, index, weight, scattering, distance, asymmetry);
		}
#endif
  };

}

#endif // LWPP_BXDF_H
