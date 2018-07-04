#ifndef LWPP_LIGHT_H
#define LWPP_LIGHT_H

#include <lwpp/wrapper.h>
#include <lwpp/item.h>
#include <lwlight.h>
namespace lwpp
{
  
	/*
	typedef struct st_LWLightEvaluationFuncs
	{
	LWLightEvaluatorID  (*create)(LWItemID light);
	void                (*destroy)(LWLightEvaluatorID);
	const char*         (*serverName)(LWLightEvaluatorID);
	unsigned int        (*flags)(LWLightEvaluatorID);
	LWError             (*init)(LWLightEvaluatorID, int mode);
	void                (*cleanUp)(LWLightEvaluatorID);
	LWError             (*newTime)(LWLightEvaluatorID, LWFrame frame, LWTime time);
	LWError             (*newFrame)(LWLightEvaluatorID, const LWFrameInfo* frameinfo, unsigned int* maxIlluminations);

	size_t              (*memorySize)(LWLightEvaluatorID);
	int                 (*localBounds)(LWLightEvaluatorID, LWDVector min, LWDVector max);
	int                 (*worldBounds)(LWLightEvaluatorID, LWDVector min, LWDVector max);
	int                 (*intersect)(LWLightEvaluatorID, const LWRay*, LWShadingGeometry* is);
	int                 (*power)(LWLightEvaluatorID, LWDVector power);

	int                 (*volumeSegment)(LWLightEvaluatorID, const LWMemChunk memory, const LWRay*, unsigned int flags, double tMin, double tMax, double* segmin, double* segmax);
	double              (*volumeSample)(LWLightEvaluatorID, const LWMemChunk memory, const LWRay*, double segmin, double segmax, double ru, double *dist);
	double              (*volumeSamplePdf)(LWLightEvaluatorID, const LWMemChunk memory, const LWRay*, double segmin, double segmax, double dist);
	} LWLightEvaluationFuncs;
	*/


  //! Wrapper for LWLightEvaluationFuncs
  //! @ingroup Globals
  /*!
   * Evaluate a light
   */
  class LightEvaluation  : protected GlobalBase<LWLightEvaluationFuncs>
  {
		bool mDestroy = false;
		LWLightEvaluatorID mID;
	public:
		LightEvaluation(LWLightEvaluatorID id)
			: mID(id)
		{}
		~LightEvaluation() {
			if (mDestroy) globPtr->destroy(mID);
		}
		unsigned int evaluate(LWMemChunk memory, const LWRay* ray, unsigned int flags, const LWDVector p, unsigned int *samples)
		{
			return globPtr->evaluate(mID, memory, ray, flags, p, samples);
		}
		unsigned int sample(const LWMemChunk memory, const LWRay* ray, const LWDVector p,
			unsigned int maxSamples, const LWDVector samples[], LWLightSample lightsamples[])
		{
			return globPtr->sample(mID, memory, ray, p, maxSamples, samples, lightsamples);
		}
		void emission(const LWMemChunk memory, const LWRay*ray, const LWDVector p,
			unsigned int nmaxSamples, const LWBxDFSample bxdfsamples[], LWLightEmission emissions[])
		{
			return globPtr->emission(mID, memory, ray, p, nmaxSamples, bxdfsamples, emissions);
		}
  };
}
#endif //LWPP_LIGHT	/_H