#ifndef LWPP_IO_H
#define LWPP_IO_H
#include <lwpp/global.h>
#include <lwio.h>

namespace lwpp
{
	class Storeable;
	class LWPP_Storeable;

	//! Wrapper for a LW save state
	class SaveState
	{
	protected:
		void write(const char *data, int len)
		{
			ss->write(ss->writeData, data, len);
		}

	public:
		const LWSaveState *ss;
		SaveState(const LWSaveState *_ss) : ss(_ss)
		{
			;
		}
		const LWSaveState *getState(void) const {return ss;}
		bool isValid() {return ss != 0;}
		void Begin(const LWBlockIdent &id, bool leaf = true) const
		{
			ss->beginBlk(ss->writeData, &id, leaf);
		}
		void End(void) const
		{
			ss->endBlk(ss->writeData);
		}
		void Depth(void) const
		{
			ss->depth(ss->writeData);
		}

		//! Write a single boolean value
		void Write (const bool data) const
		{
			unsigned char c = data;
			ss->writeU1(ss->writeData, &c, 1);
		}

		//! Write a single byte
		void Write (const char data) const
		{
			ss->writeI1(ss->writeData, &data, 1);
		}

		void Write (const char *data, int length) const
		{
			ss->writeI1(ss->writeData, data, length);
		}

		//! Write a single short
		void Write (const short data) const
		{
			ss->writeI2(ss->writeData, &data, 1);
		}

		void Write (const short *data, int length) const
		{
			ss->writeI2(ss->writeData, data, length);
		}

		//! Write a single integer
		void Write (const int data) const
		{
			ss->writeI4(ss->writeData, &data, 1);
		}

		void Write (const int *data, int length) const
		{
			ss->writeI4(ss->writeData, data, length);
		}

		//! Write a single unsigned byte
		void Write (const unsigned char data) const
		{
			ss->writeU1(ss->writeData, &data, 1);
		}

		void Write (const unsigned char *data, int length) const
		{
			ss->writeU1(ss->writeData, data, length);
		}

		void Write (const unsigned short data) const
		{
			ss->writeU2(ss->writeData, &data, 1);
		}

		void Write (const unsigned short *data, int length) const
		{
			ss->writeU2(ss->writeData, data, length);
		}

		void Write (const unsigned int data) const
		{
			ss->writeU4(ss->writeData, &data, 1);
		}

		void Write (const unsigned int *data, int length) const
		{
			ss->writeU4(ss->writeData, data, length);
		}

		void Write (const float data) const
		{
			ss->writeFP(ss->writeData, &data, 1);
		}

		void Write (const float *data, int length) const
		{
			ss->writeFP(ss->writeData, data, length);
		}
		void Write (const double data) const
		{
			ss->writeDP(ss->writeData, &data, 1);
		}

		void Write (const double *data, int length) const
		{
			ss->writeDP(ss->writeData, data, length);
		}
		void Write (const char *data) const
		{
			ss->writeStr(ss->writeData, data);
		}
		void Write (const std::string data) const
		{
			ss->writeStr(ss->writeData, data.c_str());
		}
		//! Save a single value embedded in a block
		template <class T> void WriteBlock (const LWBlockIdent &id, T value, bool leaf = true) const
		{
			Begin(id, leaf);
			Write(value);
			End();
		}
		template <class T> void WriteBlock (const LWBlockIdent &id, T value, int len, bool leaf = true) const
		{
		   Begin(id, leaf);
		   Write(value, len);
		   End();
		}

    //! Write a class that implements lwpp::Storeable
		void WriteItem(const LWBlockIdent &id, Storeable &object, bool leaf = false) const;

		void WriteItem(const LWBlockIdent &id, Storeable *object, bool leaf = false) const;

	};


	//! Wrapper for a LW save state
	class LoadState
	{
	protected:
		const LWLoadState *ls;
		mutable int rc;
		int read(char *data, int len = 1) const
		{
			rc = ls->read(ls->readData, data, len);
			return rc;
		}

	public:
		LoadState(const LWLoadState *load) : ls(load), rc(0)
		{
			;
		}
		int LastRC() {return rc;}
		bool isValid() {return ls != 0;}
		const LWLoadState *getState() const {return ls;}
		const LWLoadState *operator->() const {return ls;}
		LWID Find(const LWBlockIdent *id) const
		{
			return ls->findBlk(ls->readData, id);
		}
		void End(void) const
		{
			ls->endBlk(ls->readData);
		}
		int Depth(void) const
		{
			return ls->depth(ls->readData);
		}
		const bool Eof(void) const {return rc == -1;}
		const int Status(void) const {return rc;}

		bool readBool(void) const
		{
			unsigned char c;
			rc = ls->readU1(ls->readData, &c, 1);
			return (c == 1);
		}
		unsigned char readUChar(void) const
		{
			unsigned char c;
			rc = ls->readU1(ls->readData, &c, 1);
			return c;
		}
		char readChar(void) const
		{
			char c;
			rc = ls->readI1(ls->readData, &c, 1);
			return c;
		}

		unsigned short readUShort(void) const
		{
			unsigned short s;
			rc = ls->readU2(ls->readData, &s, 1);
			return s;
		}
		short readShort(void) const
		{
			short s;
			rc = ls->readI2(ls->readData, &s, 1);
			return s;
		}

		unsigned int readUInt(void) const
		{
			unsigned int i;
			rc = ls->readU4(ls->readData, &i, 1);
			return i;
		}
		int readInt(void) const
		{
			int i;
			rc = ls->readI4(ls->readData, &i, 1);
			return i;
		}

		float readFloat(void) const
		{
			float f;
			rc = ls->readFP(ls->readData, &f, 1);
			return f;
		}
		void read(float *f, int length) const
		{
			rc = ls->readFP(ls->readData, f, length);
		}				
		double readDouble(void) const
		{
			double d;
			rc = ls->readDP(ls->readData, &d, 1);
			return d;
		}

		void read(double *d, int length) const
		{
			rc = ls->readDP(ls->readData, d, length);
		}
		void readString(char *buffer, int max) const
		{
			rc = ls->readStr(ls->readData, buffer, max);
		}

		std::string readString() const
		{
			char buffer[1024];
			rc = ls->readStr(ls->readData, buffer, 1023);
			return std::string(buffer);
		}
		// Catch all read overloaded read function
		LWError read(lwpp::Storeable &object) const;
		void read(bool &b) const
		{
			b = readBool();
		}
		void read(unsigned char &c) const
		{
			rc = ls->readU1(ls->readData, &c, 1);
		}
		void read(char &c) const
		{
			rc = ls->readI1(ls->readData, &c, 1);
		}
		void read(unsigned short &s) const
		{
			rc = ls->readU2(ls->readData, &s, 1);
		}
		void read(short &s) const
		{
			rc = ls->readI2(ls->readData, &s, 1);
		}
		void read(unsigned int &i) const
		{
			rc = ls->readU4(ls->readData, &i, 1);
		}
		void read(int &i) const
		{
			rc = ls->readI4(ls->readData, &i, 1);
		}
		void read(float &f) const
		{
			rc = ls->readFP(ls->readData, &f, 1);
		}
		void read(std::string &str) const
		{
			char buffer[1024];
			rc = ls->readStr(ls->readData, buffer, 1023);
			str.clear();
			str = buffer;
		}
	};

	//! @ingroup Globals
	class File : protected GlobalBase<LWFileIOFuncs>
	{
		public:
			enum ioType {FILE_LOAD, FILE_SAVE};
		private:
			ioType m_type;
			union
			{
				LWSaveState *saveState;
				LWLoadState *loadState;
			};
		public:
			File(std::string name, ioType type, int ioMode = LWIO_ASCII) :
				m_type(type),
				saveState(0)
			{
				switch (m_type)
				{
					case (FILE_LOAD):
						loadState = globPtr->openLoad (name.c_str(), ioMode);
						break;

					case (FILE_SAVE):
						saveState = globPtr->openSave (name.c_str(), ioMode);
						break;

					default:
						break;
				}
			}

			~File()
			{
				switch (m_type)
				{
					case (FILE_LOAD):
						if (loadState) globPtr->closeLoad(loadState);
						break;

					case (FILE_SAVE):
						if (saveState) globPtr->closeSave(saveState);
						break;

					default:
						break;
				}
			}
			bool isValid() {return (saveState != 0);}
			LWSaveState *getLWSaveState() const {return saveState;}
			LWLoadState *getLWLoadState() const {return loadState;}
			SaveState getSaveState() const {return SaveState(saveState);}
			LoadState getLoadState() const {return LoadState(loadState);}
			LWError Save(Storeable &item);
			LWError Load(Storeable &item);
	};
}

#endif // LWPP_IO_H