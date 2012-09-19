/*!
 * @file
 * @brief
 * @version $Id: particle_services.h 93 2011-01-05 17:27:10Z mwolf $
 */

#ifndef PARTICLE_SERVICES_H
#define PARTICLE_SERVICES_H

#include <lwpp/global.h>

#include <lwpp/point3d.h>
#include <lwpp/vector3d.h>

#include <lwprtcl.h>

namespace lwpp
{
	//! @ingroup Globals
	/*!
	*	Allow you to create particle systems and to read and write particle data. 
	*	Particles are typically used by volumetric renderers to define the extent and local density of a volume.
	*	
	*	The host side of the particles global manages a database of particle systems. 
	*	The global supplies methods for adding, deleting and reading particle data in the database. 
	*	Having such a database allows one plug-in to create particle systems that others can later use.
	*	
	*	You can allocate, initialize and read particle data either individually (one particle at a time) or all at once. 
	*	Which approach you take will depend primarily on where the data comes from and how you use it.
	*/
	class ParticleServices : protected GlobalBase<LWPSysFuncs>
	{
	
	private:
		/*! ID of the particle service	*/
		LWPSysID prtsv_id;
		
		/*! Position Buffer ID */
		LWPSBufID posbf_id;
		/*! Size Buffer ID */
		LWPSBufID sizbf_id;
		/*! Scale Buffer ID */
		LWPSBufID sclbf_id;
		/*! Rotation Buffer ID */
		LWPSBufID rotbf_id;
		/*! Velocity Buffer ID */
		LWPSBufID velbf_id;
		/*! Age Buffer ID */
		LWPSBufID agebf_id;
		/*! Force Buffer ID */
		LWPSBufID frcbf_id;
		/*! Pressure Buffer ID */
		LWPSBufID prsbf_id;
		/*! Temperature Buffer ID */
		LWPSBufID tmpbf_id;
		/*! Mass Buffer ID */
		LWPSBufID masbf_id;
		/*! Linked Particle Buffer ID */
		LWPSBufID lnkbf_id;
		/*! Particle ID Buffer ID */
		LWPSBufID pidbf_id;
		/*! Enable Buffer ID */
		LWPSBufID enbbf_id;
		/*! RGBA Buffer ID */
		LWPSBufID rgbabf_id;
		/*! Collision Age Buffer ID */
		LWPSBufID cagebf_id;
		/*! Relative Age Buffer ID */
		LWPSBufID ragebf_id;
		/*! Start Position Buffer ID */
		LWPSBufID sposbf_id;
		/*! Start Scale Buffer ID */
		LWPSBufID ssclbf_id;
		/*! Start Rotation Buffer ID */
		LWPSBufID srotbf_id;	

	public:
		//! Constructor
		ParticleServices( LWPSysID _prtsv_id = 0) : prtsv_id(_prtsv_id) {;}

		//! Returns the ID of the particle service
		LWPSysID getID() const { return prtsv_id; }

		/*! Create a particle system. 
		*	The flags indicate which buffers should be allocated for the particles and can be any of the following, combined using bitwise-or.
		*	LWPSB_POS		position 							float[3]
		*	LWPSB_SIZ 		size 								float
		*	LWPSB_SCL 		scale 								float[3]
		*	LWPSB_ROT 		rotation 							float[3]
		*	LWPSB_VEL 		velocity 							float[3]
		*	LWPSB_AGE 		age 								float
		*	LWPSB_FCE 		force 								float
		*	LWPSB_PRS 		pressure 							float
		*	LWPSB_TMP 		temperature 						float
		*	LWPSB_MAS 		mass 								float
		*	LWPSB_LNK 		link to particle (for trails) 		int
		*	LWPSB_ID 		ID (unique index for the particle) 	int
		*	LWPSB_ENB 		enable state (dead/alive/limbo) 	char
		*	LWPSB_RGBA 		display color and alpha 			char[4]
		*	LWPSB_CAGE 		time since last collision 			float	
		*
		*	The particle type can be either LWPST_PRTCL (single points) or LWPST_TRAIL (line segments). 
		*	LWPST_TRAIL particle systems has the LWPSB_LNK buffer for the second point in each trail created automatically. 
		*/
		LWPSysID create (int flags, int type = LWPST_PRTCL)
		{
			prtsv_id = globPtr->create(flags, type);
			
			// Get and store all the buffers created
			// to avoid later calls to getBufferID.
			// Position
			if (flags & LWPSB_POS)  { posbf_id  = getBufferID(LWPSB_POS); }
			// Size
			if (flags & LWPSB_SIZ)  { sizbf_id  = getBufferID(LWPSB_SIZ); }
			// Scale
			if (flags & LWPSB_SCL)  { sclbf_id  = getBufferID(LWPSB_SCL); }
			// Rotation
			if (flags & LWPSB_ROT)  { rotbf_id  = getBufferID(LWPSB_ROT); }
			// Velocity
			if (flags & LWPSB_VEL)  { velbf_id  = getBufferID(LWPSB_VEL); }
			// Age
			if (flags & LWPSB_AGE)  { agebf_id  = getBufferID(LWPSB_AGE); }
			// Force
			if (flags & LWPSB_FCE)  { frcbf_id  = getBufferID(LWPSB_FCE); }
			// Pressure
			if (flags & LWPSB_PRS)  { prsbf_id  = getBufferID(LWPSB_PRS); }
			// Temperature
			if (flags & LWPSB_TMP)  { tmpbf_id  = getBufferID(LWPSB_TMP); }
			// Mass
			if (flags & LWPSB_MAS)  { masbf_id  = getBufferID(LWPSB_MAS); }
			// Linked Particle
			// This buffer is created as well if the type is a trail particle system
			if (flags & LWPSB_TMP || type == LWPST_TRAIL ) { lnkbf_id  = getBufferID(LWPSB_TMP); }
			// Particle ID 
			if (flags & LWPSB_ID) { pidbf_id  = getBufferID(LWPSB_ID); }
			// Enable
			if (flags & LWPSB_ENB) { enbbf_id  = getBufferID(LWPSB_ENB); }
			// RGBA
			if (flags & LWPSB_RGBA) { rgbabf_id = getBufferID(LWPSB_RGBA); }
			// Colision Age
			if (flags & LWPSB_CAGE) { cagebf_id = getBufferID(LWPSB_CAGE); }
			// Relative Age
			if (flags & LWPSB_RAGE) { ragebf_id = getBufferID(LWPSB_RAGE); }
			// Start Position
			if (flags & LWPSB_POS0) { sposbf_id = getBufferID(LWPSB_POS0); }
			// Start Scale
			if (flags & LWPSB_SCL0) { ssclbf_id = getBufferID(LWPSB_SCL0); }
			// Start Rotation
			if (flags & LWPSB_ROT0) { ssclbf_id = srotbf_id = getBufferID(LWPSB_ROT0); }

			return prtsv_id;
		}
		
		/*! Free the particle system. */
		int	destroy ()  { return globPtr->destroy(prtsv_id); }

		/*! Allocate memory for the particles. This is equivalent to calling the addParticle function nparticles times. */
		int init (int num_particles) { return globPtr->init(prtsv_id, num_particles); }

		/*! Frees the memory allocated by init and addParticle. */
		void cleanup () { globPtr->cleanup(prtsv_id); }
		
		/*! Read the particle system data from a file. This will typically be used by handler load callbacks. */
		void load ( LWLoadState *loadstate ) { globPtr->load(prtsv_id, loadstate); }

		/*! Write the particle system data to a file. This will typically be used by handler save callbacks. */
		void save ( LWSaveState *savestate ) { globPtr->save(prtsv_id, savestate); }
		
		/*! Returns the number of particles. */
		int getParticleCount () { return globPtr->getPCount(prtsv_id); }
		
		/*! Associate a particle system with an item in the scene, usually an object. 
		*	More than one particle system can be attached to an item, 
		*	and more than one item can share the same particle system. 
		*	Attaching a particle system to an item makes it possible for others, 
		*	Hypervoxels in particular, to use the getPSys function to find it.
		*/
		void attach (LWItemID lwitid) { globPtr->attach(prtsv_id, lwitid); }

		/*! Remove the association between a particle system and an item. */
		void detach (LWItemID lwitid) { globPtr->detach(prtsv_id, lwitid); }

		/*! Returns a NULL-terminated array of particle system IDs 
		*	that have been associated with the item by the attach function.
		*/
		LWPSysID * getParticleSystems (LWItemID lwitid) { return globPtr->getPSys(lwitid); }
		
		/*! Add a custom per-particle buffer. 
		*	Call this before any calls to init or addParticle. 
		*	(The predefined buffer types should be added when create is called.) 
		*	The structure used to define the buffer is described below. 
		*	The buffer ID returned by this function can be used with the functions that get and set buffer data.
		*
		*	The function uses a buffer descriptor to define the buffer to be added.
		*	typedef struct st_LWPSBufDesc 
		*	{ 
		*		const char *name;
		*		int         dataType;
		*		int         dataSize;
		*	} LWPSBufDesc;
		*
		*		name		A string that names the buffer. In the future, this may allow users or plug-ins to refer to the buffer by name.
		*		dataType	The data type of the data in the buffer.
		*						LWPSBT_FLOAT	0
		*						LWPSBT_INT		1
		*						LWPSBT_CHAR		2
		*		dataSize	The number of values per particle in the buffer (and not the number of bytes). 
		*/
		LWPSBufID addBuffer (LWPSBufDesc lwbufdesc) { return globPtr->addBuf(prtsv_id, lwbufdesc); }
		
		/*! Returns a buffer ID for one of the predefined buffers. 
		*	This is used with the functions that get and set buffer data. 
		*	The second argument is one of the buffer flags passed to create.
		*/
		LWPSBufID getBufferID ( int bufferFlag) { return globPtr->getBufID( prtsv_id, bufferFlag ); }
		
		/*! Set the buffer values for all particles. 
		*	The data is an array of the appropriate type for the buffer, 
		*	with a number of entries equal to the number of particles. 
		*	Use setParticle  to set the buffer data for one particle at a time.
		*/
		template <typename T>
		void setBufferData (LWPSBufID buffer_id, T data) { globPtr->setBufData( buffer_id, &data ); }
		
		/*!
		*	Set the buffer values for all particles. 
		*	The data is an array of the appropriate type for the buffer, 
		*	with a number of entries equal to the number of particles. 
		*	Use setParticle  to set the buffer data for one particle at a time.
		*/
		template <typename T>
		void getBufferData (LWPSBufID buffer_id, T &data) { globPtr->getBufData( buffer_id, &data ); }
		
		/*! Add a particle. */
		int addParticle () { return globPtr->addParticle(prtsv_id); }
		
		/*! Set the buffer value for a particle. 
		*	Particles are numbered from 0 to getPCount-1 in the order in which they're added.
		*/
		void setParticle (LWPSBufID buffer_id, int index, void *data) { globPtr->setParticle(buffer_id, index, data); }
		
		/*! Get the buffer value for a particle. 
		*	Particles are numbered from 0 to getPCount-1 in the order in which they're added.
		*/
		void getParticle (LWPSBufID buffer_id, int index, void *data) { globPtr->getParticle(buffer_id, index, data); }
		
		/*! Remove a particle.
		*/
		void removeParticle (int index) { globPtr->remParticle(prtsv_id, index); }

		/*	Auxiliary Functions from the wrapper to help to the writing of plugins
		*	This ARE NOT part originally of the LWSDK but make use of it
		*/

		/*! Returns the position of a particle
		*	@param index Index of the particle
		*	@param position Pointer to a float[3] array were store the position of the particle
		*/
		void getPosition(int index, float *position) { getParticle(posbf_id, index, position); }
		
		/*! Returns the position of a particle
		*	@param index Index of the particle
		*/
		Point3f getPosition(int index)
		{
			float position[3] = { 0, 0, 0 };
			getPosition(index, position);
			return Point3f(position);
		}

		/*! Changes the position of a particle
		*	@param index Index of the particle
		*	@param position Pointer to a float[3] array were is the new position of the particle
		*/
		void setPosition(int index, float *position) { setParticle(posbf_id, index, position); }

		/*! Changes the position of a particle
		*	@param index Index of the particle
		*	@param position Pointer to a Point3D were is the new position of the particle
		*/
		void setPosition(int index, Point3d &position)
		{
			float fposition[3] = { (float) position.x, (float) position.y, (float) position.z };
			setParticle(posbf_id, index, fposition);
		}

		/*! Returns the Size of a particle
		*	@param[in] index Index of the particle
    * @param[out] size A float storing containing the size
		*/
		void getSize(int index, float *size) { getParticle(sizbf_id, index, size); }

		/*! Changes the Size of a particle
		*	@param[in] index Index of the particle
		*	@param[in] size a float with the new Size of the particle
		*/
		void setSize(int index, float size) { setParticle(sizbf_id, index, &size); }

		/*! Returns the scale of a particle
		*	@param index Index of the particle
		*	@param scale Pointer to a float[3] array were store the scale of the particle
		*/
		void getScale(int index, float *scale) { getParticle(sclbf_id, index, scale); }

		/*! Changes the scale of a particle
		*	@param index Index of the particle
		*	@param scale Pointer to a float[3] array were is the new scale of the particle
		*/
		void setScale(int index, float scale) { setParticle(sclbf_id, index, &scale); }

		/*! Returns the rotation of a particle
		*	@param index Index of the particle
		*	@param rotation Pointer to a float[3] array were store the rotation of the particle
		*/
		void getRotation(int index, float *rotation) { getParticle(rotbf_id, index, rotation); }

		/*! Changes the rotation of a particle
		*	@param index Index of the particle
		*	@param rotation Pointer to a float[3] array were is the new rotation of the particle
		*/
		void setRotation(int index, float rotation) { setParticle(rotbf_id, index, &rotation); }

		/*! Returns the velocity of a particle
		*	@param index Index of the particle
		*	@param velocity Pointer to a float[3] array were store the velocity of the particle
		*/
		void getVelocity(int index, float *velocity) { getParticle(velbf_id, index, velocity); }

		/*! Changes the velocity of a particle
		*	@param index Index of the particle
		*	@param velocity Pointer to a float[3] array were is the new velocity of the particle
		*/
		void setVelocity(int index, float velocity) { setParticle(velbf_id, index, &velocity); }

		/*! Returns the Age of a particle
		*	@param index Index of the particle
		*/
		void * getAge(int index)
		{
			void *age;
			getParticle(agebf_id, index, age);
			return age;
		}

		/*! Changes the Age of a particle
		*	@param index Index of the particle
		*	@param age Pointer to a float with the new Age of the particle
		*/
		void setAge(int index, float *age) { setParticle(agebf_id, index, age); }

		/*! Returns the Force of a particle
		*	@param index Index of the particle
		*/
		void * getForce(int index)
		{
			void *force;
			getParticle(frcbf_id, index, force);
			return force;
		}

		/*! Changes the Force of a particle
		*	@param index Index of the particle
		*	@param force Pointer to a float with the new Force of the particle
		*/
		void setForce(int index, float force) { setParticle(frcbf_id, index, &force); }

		/*! Returns the Pressure of a particle
		*	@param index Index of the particle
		*/
		void * getPressure(int index)
		{
			void *pressure;
			getParticle(prsbf_id, index, pressure);
			return pressure;
		}

		/*! Changes the Pressure of a particle
		*	@param index Index of the particle
		*	@param pressure Pointer to a float with the new Pressure of the particle
		*/
		void setPressure(int index, float pressure) { setParticle(prsbf_id, index, &pressure); }
		
		/*! Returns the Temperature of a particle
		*	@param index Index of the particle
		*/
		void * getTemperature(int index)
		{
			void *temperature;
			getParticle(tmpbf_id, index, temperature);
			return temperature;
		}

		/*! Changes the Temperature of a particle
		*	@param index Index of the particle
		*	@param temperature Pointer to a float with the new Temperature of the particle
		*/
		void setTemperature(int index, float temperature) { setParticle(tmpbf_id, index, &temperature); }

		/*! Returns the Mass of a particle
		*	@param index Index of the particle
		*/
		void * getMass(int index)
		{
			void *mass;
			getParticle(masbf_id, index, mass);
			return mass;
		}

		/*! Changes the Mass of a particle
		*	@param index Index of the particle
		*	@param mass Pointer to a float with the new Mass of the particle
		*/
		void setMass(int index, float mass) { setParticle(masbf_id, index, &mass); }

		/*! Returns the particle Link of a particle
		*	@param index Index of the particle
		*/
		void * getLink(int index)
		{
			void *link;
			getParticle(lnkbf_id, index, link);
			return link;
		}

		/*! Changes the particle Link of a particle
		*	@param index Index of the particle
		*	@param link Pointer to a float with the new Mass of the particle
		*/
		void setLink(int index, int link) { setParticle(lnkbf_id, index, &link); }

		/*! Returns the particle ID of a particle
		*	@param index Index of the particle
		*/
		void * getParticleID(int index)
		{
			void *PID;
			getParticle(pidbf_id, index, PID);
			return PID;
		}

		/*! Changes the particle ID of a particle
		*	@param index Index of the particle
		*	@param PID Pointer to a int PID the new particle ID of the particle
		*/
		void setParticleID(int index, int PID) { setParticle(pidbf_id, index, &PID); }

		/*! Returns the particle state of a particle
		*	@param index Index of the particle
		*	Particle state values
		*		#define	LWPST_DEAD		0
		*		#define	LWPST_ALIVE		1
		*		#define	LWPST_LIMBO		2
		*/
		char getParticleState(int index)
		{
			char state = '0';
			getParticle(enbbf_id, index, &state);
			return state;
		}

		/*! Changes the state of a particle
		*	@param index Index of the particle
		*	@param state Pointer to a char with the new state of the particle
		*/
		void setParticleState(int index, char state) { setParticle(enbbf_id, index, &state); }

		/*! Changes the state of a particle to Dead
		*	@param index Index of the particle
		*/
		void setParticleDead(int index)
		{
			char state = (char) LWPST_DEAD;
			setParticleState(index, state);
		}

		/*! Changes the state of a particle to Alive
		*	@param index Index of the particle
		*/
		void setParticleAlive(int index)
		{
			char state = (char) LWPST_ALIVE; 
			setParticleState(index, state);
		}

		/*! Changes the state of a particle to Limbo
		*	@param index Index of the particle
		*/
		void setParticleLimbo(int index)
		{
			char state = (char) LWPST_LIMBO; 
			setParticleState(index, state);
		}

		/*! Returns true if the particle is Dead
		*	@param index Index of the particle
		*/
		bool isDead(int index)
		{
			char isdead = getParticleState(index);
			return ( isdead == LWPST_DEAD) ? true : false ;
		}

		/*! Returns true if the particle is Alive
		*	@param index Index of the particle
		*/
		bool isAlive(int index)
		{
			int isalive = getParticleState(index);
			return ( isalive == LWPST_ALIVE) ? true : false ;
		}

		/*! Returns true if the particle is in Limbo
		*	@param index Index of the particle
		*/
		bool isLimbo(int index)
		{
			int islimbo = getParticleState(index);
			return ( islimbo == LWPST_LIMBO) ? true : false ;
		}

		/*! Returns the RGBA of a particle
		*	@param index Index of the particle
		*	@param RGBA Pointer to a char[4] array were store the RGBA of the particle
		*/
		void getRGBA(int index, unsigned char *RGBA)
		{
			getParticle(rgbabf_id, index, RGBA);
		}

		/*! Changes the the RGBA of a particle
		*	@param index Index of the particle
		*	@param RGBA Pointer Pointer to a char[4] array were is the new RGBA of the particle
		*/
		void setRGBA(int index, unsigned char *RGBA)
		{
			setParticle(rgbabf_id, index, RGBA);
		}

		/*! Returns the Collision Age (time since last collision) of a particle
		*	@param index Index of the particle
		*/
		void * getCollisionAge(int index)
		{
			void *cage;
			getParticle(cagebf_id, index, cage);
			return cage;
		}

		/*! Changes the Collision Age (time since last collision) of a particle
		*	@param index Index of the particle
		*	@param cage Pointer to a float with the new Collision Age of the particle
		*/
		void setCollisionAge(int index, float cage) { setParticle(cagebf_id, index, &cage); }

		/*! Returns the Relative Age (provide information about the life expectancy) of a particle
		*	@param index Index of the particle
		*/
		void * getRelativeAge(int index) 
		{
			void *rage;
			getParticle(ragebf_id, index, rage);
			return rage;
		}

		/*! Changes the Relative Age (provide information about the life expectancy) of a particle
		*	@param index Index of the particle
		*	@param rage Pointer to a float with the new Relative Age of the particle
		*/
		void setRelativeAge(int index, float rage) { setParticle(ragebf_id, index, &rage); }

		/*! Returns the start position of a particle
		*	@param index Index of the particle
		*	@param sposition Pointer to a float[3] array were store the start position of the particle
		*/
		void * getStartPosition(int index, float *sposition) { getParticle(sposbf_id, index, sposition); }

		/*! Changes the start position of a particle
		*	@param index Index of the particle
		*	@param sposition Pointer to a float[3] array were is the new start position of the particle
		*/
		void setStartPosition(int index, float sposition) { setParticle(sposbf_id, index, &sposition); }

		/*! Returns the start scale of a particle
		*	@param index Index of the particle
		*	@param sscale Pointer to a float[3] array were store the start scale of the particle
		*/
		void getStartScale(int index, float *sscale) { getParticle(ssclbf_id, index, sscale); }

		/*! Changes the start scale of a particle
		*	@param index Index of the particle
		*	@param sscale Pointer to a float[3] array were is the new start scale of the particle
		*/
		void setStartScale(int index, float sscale) { setParticle(ssclbf_id, index, &sscale); }

		/*! Returns the start rotation of a particle
		*	@param index Index of the particle
		*	@param srotation Pointer to a float[3] array were store the rotation of the particle
		*/
		void getStartRotation(int index, float *srotation) { getParticle(srotbf_id, index, srotation); }

		/*! Changes the start rotation of a particle
		*	@param index Index of the particle
		*	@param srotation Pointer to a float[3] array were is the new start rotation of the particle
		*/
		void setStartRotation(int index, float srotation) { setParticle(srotbf_id, index, &srotation); }
		
	};
} // namespace lwpp
#endif // PARTICLE_SERVICES_H
