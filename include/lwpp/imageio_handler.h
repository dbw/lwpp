#ifndef LWPP_IMAGEIO_HANDLER
#define LWPP_IMAGEIO_HANDLER

#include <lwimageio.h>
#include <lwtypes.h>
#include <lwpp/lw_server.h>
#include <lwpp/monitor.h>
#include <lwpp/message.h>
#include <sstream>
 
namespace lwpp
{
  //! Pushes an image to LW using the LWImageProtocol. Only useful for image loaders or animation loaders.
  class ImageLoaderProtocol
  {
    LWImageProtocolID prot;
    int rc;
  public:
    ImageLoaderProtocol(LWImageProtocolID _prot)
      : prot(_prot), rc(IPSTAT_OK)
    {}
    ~ImageLoaderProtocol()
    {
      
    }
    LWImageProtocolID getID() const {return prot;}
    bool isValid()
    {
      return prot != 0;
    }
    void Done()
    {
      if (prot) prot->done(prot->priv_data, rc);
    }
    //! Set the pixel dimensions of the image.
    /*!
     * This is called before the first call to sendLine. 
     *
     * @param[in] w, h The width and height are the number of pixels in a scanline and the number of scanlines, respectively.
     */
    void SetSize (int w, int h)
    {
      if (prot) prot->setSize(prot->priv_data, w, h);
    }    

    int SetParam (LWImageParam param, const char *data)
    {
      if (prot) return prot->setParam(prot->priv_data, param, data);
      return 0;
    }

    int SetParam (LWImageParam param, const std::string data)
    {
      return SetParam(param, data.c_str());
    }

    //! Send one scanline from the image. 
    /*!
     * Loaders must call setSize before the first call to sendLine. Scanlines are numbered from the top of the image, starting at 0.
     * Loaders don't have to send scanlines in a particular order, but savers will receive scanlines in top to bottom order (or bottom to top if they specified the IMGF_REVERSE flag in their sendData call). A scanline begins with the leftmost pixel.
     * The structure of the pixel data depends on the pixel type.
     *
     * @param[in] line the current line to write to
     * @param[in] pix the pixel values for the scanline
     * @return IPSTAT_OK or an error code
     */
    int SendLine (int line, const LWPixelID pix)
    {
      if (prot)
        return prot->sendLine(prot->priv_data, line, pix);
      else
        return IPSTAT_FAILED;
    }

    void SetMap (int idx, const unsigned char col[3])
    {
      if (prot) return prot->setMap(prot->priv_data, idx, col);
    }
  };

	struct LWImageProtocol_V2 {
		int		  type;
		void		 *priv_data;
		int		(*done)      (void *, int);
		void		(*setSize)   (void *, int w, int h);
		void		(*setParam)  (void *, LWImageParam, int, float);
		int		(*sendLine)  (void *, int, const LWPixelID);
		void		(*setMap)    (void *, int, const unsigned char[3]);
	};

	struct LWImageProtocol_V3 {
		int		  type;
		void		 *priv_data;
		int		(*done)      (void *, int);
		void		(*setSize)   (void *, int w, int h);
		int		(*setParam)  (void *, LWImageParam, const char*);
		int		(*sendLine)  (void *, int, const LWPixelID);
		void		(*setMap)    (void *, int, const unsigned char[3]);
	};

	//! @ingroup Handler
	class ImageLoaderHandler
	{
	protected:
		union
		{
			LWImageProtocol_V2 *protocol2;
			LWImageProtocol_V3 *protocol3;
			LWImageProtocol *protocol;
		};
		//LWImageProtocol *protocol;
		LWImageLoaderLocal *local = nullptr;
		SimpleMonitor monitor;
		int version = 0;
		void SetRC(int res)	{local->result = res;}

		bool Begin (LWImageType type)
		{
			if (version == 2)	protocol2 = (LWImageProtocol_V2 *)local->begin(local->priv_data, type);
			else if (version == 3)	protocol3 = (LWImageProtocol_V3 *)local->begin(local->priv_data, type);
			else if (version == LWIMAGELOADER_VERSION)	protocol = (LWImageProtocol *)local->begin(local->priv_data, type);
			return (protocol2 != 0);
		}
		int Done (int rc = IPSTAT_OK)
		{
			int ret = 0;
			if (protocol2) ret = protocol2->done ((LWImageProtocolID)protocol2->priv_data, rc);
			if (protocol2) local->done(local->priv_data, (LWImageProtocolID)protocol2);
			protocol2 = 0;
			return ret;
		}
		void SetSize (int w, int h)
		{
			if (protocol2) protocol2->setSize(protocol2->priv_data,w, h);
		}
		void SetParam (LWImageParam p, int i, float f)
		{
			if ((version == 2) && (protocol2))
			{
				protocol2->setParam(protocol2->priv_data,p,i,f);
			}
		}
		int SendLine (int y, const LWPixelID buffer)
		{
			if (protocol2)	return protocol2->sendLine(protocol2->priv_data,y,buffer);
			return 0;
		}
		void SetMap (int index, const unsigned char colour[3] )
		{
			if (protocol2) protocol2->setMap(protocol2->priv_data,index,colour);
		}
	public:
		int SetParam (LWImageParam p, const char *c)
		{
			if ((version == 3) && (protocol3))
			{
				return protocol3->setParam(protocol2->priv_data,p,c);
			}
			return 0;
		}
		int SetParam (LWImageParam p, const std::string str)
		{
			if ((version == 3) && (protocol3))
			{
				return protocol3->setParam(protocol2->priv_data,p,str.c_str());
			}
			return 0;
		}
		int SetParam (LWImageParam p, int i)
		{
			if (((version == 3) && (protocol3)) ||
                ((version == LWIMAGELOADER_VERSION) && (protocol)))
			{
				std::ostringstream ss;
				ss << i;
				return SetParam(p, ss.str());
			}
			return 0;
		}
		int SetParam (LWImageParam p, double d)
		{
			if (((version == 3) && (protocol3)) ||
                ((version == LWIMAGELOADER_VERSION) && (protocol)))
			{
				std::ostringstream ss;
				ss << d;
				return SetParam(p, ss.str());
			}
			return 0;
		}
		ImageLoaderHandler()	: protocol(0) {}
		void InitHandler(LWImageLoaderLocal *l, int ver)
		{
			version = ver;
			local = l;
			monitor.setMonitor(local->monitor);
		}
		virtual ~ImageLoaderHandler() {Done();}

		virtual int LoadImage(const char *filename) = 0;
	};

	//! @ingroup Adaptor
	template <class T>
	class ImageLoaderAdaptor
	{
		public:
			ImageLoaderAdaptor(const char *name, ServerTagInfo *tags)
			{
				LWServer::AddPlugin(LWIMAGELOADER_CLASS, name, Activate, tags);
			}
			//! Set static callbacks for the LightWave instance handler
			static int Activate( int version, GlobalFunc *global, void *l,  void *serverData )
			{
				if (version > LWIMAGELOADER_VERSION) return AFUNC_BADVERSION;
				try
				{
					lwpp::SetSuperGlobal(global);
					LWImageLoaderLocal *local = static_cast<LWImageLoaderLocal *>(l);
					T loader;
					loader.InitHandler(local, version);
					return loader.LoadImage(local->filename);
				}
				catch (std::exception &e)
				{
					lwpp::LWMessage::Error("An exception occured when attempting to load an image:", e.what());
					return AFUNC_BADAPP_SILENT;
				}
			}
	};
	
	/*
	 * Image Saver
	 */

	//! @ingroup Handler
	class ImageSaverHandler
	{
	protected:
		union
		{
			LWImageProtocol_V2 protocol;
			LWImageProtocol_V3 protocol3;
		};
		int version;
		LWImageSaverLocal *local;
		SimpleMonitor monitor;
		int width, height;
		const char *fileName;
		void SetRC(int res)	{local->result = res;}

		static int CBdone(LWInstance inst, int rc)
		{
			try
			{
				ImageSaverHandler *saver = static_cast<ImageSaverHandler *>(inst);
				saver->monitor.Done();
				return saver->Done(rc);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in ImageSaver::Done():", e.what());
				return IPSTAT_FAILED;
			}
		}
		static void CBsetSize(LWInstance inst, int w, int h)
		{
			try
			{
				ImageSaverHandler *saver = static_cast<ImageSaverHandler *>(inst);
				saver->monitor.Init(h);
				saver->SetSize(w, h);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in ImageSaver::SetSize():", e.what());
				return;
			}
		}
		static void CBsetParam(LWInstance inst, LWImageParam p, int i, float f)
		{
			try
			{
				ImageSaverHandler *saver = static_cast<ImageSaverHandler *>(inst);
				saver->SetParam(p, i, f);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in ImageSaver::SetParam():", e.what());
				return ;
			}
		}
		static int CBsetParam3(LWInstance inst, LWImageParam p, const char *c)
		{
			try
			{
				ImageSaverHandler *saver = static_cast<ImageSaverHandler *>(inst);
				return saver->SetParam(p, c);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in ImageSaver::SetParam():", e.what());
				return 0;
			}
		}
		static int CBsendLine(LWInstance inst, int y, const LWPixelID pixel)
		{
			try
			{
				ImageSaverHandler *saver = static_cast<ImageSaverHandler *>(inst);
				saver->monitor.Step();
				return saver->SendLine(y, pixel);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in ImageSaver::SendLine():", e.what());
				return IPSTAT_FAILED;
			}
		}
		static void CBsetMap(LWInstance inst, int index, const unsigned char rgb[3])
		{
			try
			{
				ImageSaverHandler *saver = static_cast<ImageSaverHandler *>(inst);
				saver->SetMap(index, rgb);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in ImageSaver::SetMap():", e.what());
				return;
			}
		}

		virtual int	SendData (int flags = 0)
		{
			// init Protocol
			if (version == 2)
			{
				protocol.type = local->type;
				protocol.priv_data = this;
				protocol.done = CBdone;
				protocol.setSize = CBsetSize;
				protocol.setParam = CBsetParam;
				protocol.sendLine = CBsendLine;
				protocol.setMap = CBsetMap;
				return local->sendData (local->priv_data, reinterpret_cast<LWImageProtocolID>(&protocol), flags);
			}
			else if (version == 3)
			{
				protocol3.type = local->type;
				protocol3.priv_data = this;
				protocol3.done = CBdone;
				protocol3.setSize = CBsetSize;
				protocol3.setParam = CBsetParam3;
				protocol3.sendLine = CBsendLine;
				protocol3.setMap = CBsetMap;
				return local->sendData (local->priv_data, reinterpret_cast<LWImageProtocolID>(&protocol3), flags);
			}
			else return IPSTAT_FAILED;
		}
	public:
		ImageSaverHandler(int v, LWImageSaverLocal *l)
			: version (v), local(0), monitor(0), width(0), height(0), fileName(0)
		{
			local = l;
			monitor.setMonitor(local->monitor);
		}
		virtual ~ImageSaverHandler() {}
		virtual int SaveImage(const char *filename)
		{
			try
			{
				fileName = filename;
				SetRC(IPSTAT_OK);
				int rc = SendData(0);
				SetRC(rc);
				//if (rc == IPSTAT_OK)
				return AFUNC_OK;
				//else return 		
			}
			catch (std::exception &e)
			{
				SetRC(IPSTAT_FAILED);
				throw;
			}
		}

		virtual void SetSize (int w, int h) 
		{
			width = w;
			height = h;
		}
		virtual int Done(int rc)
		{
			return rc;
		}
		virtual void SetParam (LWImageParam p, int i, float f) {}
		virtual int SetParam (LWImageParam p, const char *c) {return 0;}

		virtual int SendLine (int y, const LWPixelRGB24 *buffer){return IPSTAT_OK;}
		virtual int SendLine (int y, const LWPixelRGBFP *buffer) {return IPSTAT_OK;}
		virtual int SendLine (int y, const LWPixelRGBA32 *buffer) {return IPSTAT_OK;}
		virtual int SendLine (int y, const LWPixelRGBAFP *buffer) {return IPSTAT_OK;}
		virtual int SendLine (int y, const float *buffer) {return IPSTAT_OK;}
		virtual int SendLine (int y, const unsigned char *buffer) {return IPSTAT_OK;}
		virtual int PreSendLine (int y) {return IPSTAT_OK;}
		virtual int PostSendLine (int y) {return IPSTAT_OK;}
		virtual int SendLine (int y, const LWPixelID buffer)
		{
			int rc = PreSendLine(y);
			if (rc != IPSTAT_OK) return rc;
			switch (protocol.type)
			{
				case LWIMTYP_GREY8:
				case LWIMTYP_INDEX8:
					rc = SendLine(y, static_cast<const unsigned char*>(buffer));
					break;

				case LWIMTYP_RGB24:
					rc = SendLine(y, static_cast<const LWPixelRGB24 *>(buffer));
					break;

				case LWIMTYP_RGBA32:
					rc = SendLine(y, static_cast<const LWPixelRGBA32 *>(buffer));
					break;

				case LWIMTYP_GREYFP:
					rc = SendLine(y, static_cast<const float *>(buffer));
					break;

				case LWIMTYP_RGBFP:
					rc = SendLine(y, static_cast<const LWPixelRGBFP *>(buffer));
					break;

				case LWIMTYP_RGBAFP:
					rc = SendLine(y, static_cast<const LWPixelRGBAFP *>(buffer));
					break;

				default:
					return IPSTAT_FAILED;
					break;
			}
			if (rc != IPSTAT_OK) return rc;
			return PostSendLine(y);
		}
		virtual void SetMap (int index, const unsigned char colour[3] ) {}

	};

	//! @ingroup Adaptor
	template <class T>
	class ImageSaverAdaptor
	{
		public:
			ImageSaverAdaptor(const char *name, ServerTagInfo *tags)
			{
				LWServer::AddPlugin(LWIMAGESAVER_CLASS, name, Activate, tags);
			}
			//! Set static callbacks for the LightWave instance handler
			static int Activate( int version, GlobalFunc *global, void *l,  void *serverData )
			{
				if (version != LWIMAGESAVER_VERSION) return AFUNC_BADVERSION;
				try
				{
					lwpp::SetSuperGlobal(global);
					LWImageSaverLocal *local = static_cast<LWImageSaverLocal *>(l);
					T saver(version, local);
					return saver.SaveImage(local->filename);
				}
				catch (std::exception &e)
				{
					lwpp::LWMessage::Error("An exception occured when attempting to save an image:", e.what());
					return AFUNC_BADAPP_SILENT;
				}
			}
	};

}

#endif // LWPP_IMAGEIO_HANDLER
