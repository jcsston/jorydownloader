
#ifndef _FOOBAR2000_IO_CALLBACK_H_
#define _FOOBAR2000_IO_CALLBACK_H_

#include "../SDK/foobar2000.h"
#include "ebml/IOCallback.h"

using namespace LIBEBML_NAMESPACE;

class Foobar2000ReaderIOCallback : public IOCallback {
public:
	Foobar2000ReaderIOCallback(reader *source) {
		m_NoSeekReader = NULL;
		m_Reader = source;
		m_Reader->service_add_ref();
		if (!m_Reader->can_seek()) {
			m_NoSeekReader = source;
			m_Reader = new reader_seekback_wrap(source, 1024*1024);
			m_Reader->service_add_ref();
		}
	};
	virtual ~Foobar2000ReaderIOCallback() {
		close();
	};

	virtual uint32 read(void*Buffer, size_t Size) {
		return m_Reader->read(Buffer, Size);
	};

	virtual void setFilePointer(int64 Offset, seek_mode Mode=seek_beginning) {
		switch (Mode)
		{
			case seek_beginning:
				m_Reader->seek(Offset);
				break;
			case seek_current:
				m_Reader->seek2(Offset, SEEK_CUR);
				break;
			case seek_end:
				m_Reader->seek2(Offset, SEEK_END);
				break;
			default:
				//throw "Invalid Seek Mode!!!";
				;
		};
	}

	virtual size_t write(const void*Buffer, size_t Size) {
		return m_Reader->write(Buffer, Size);
	}

	virtual uint64 getFilePointer() {
		return m_Reader->get_position();
	};

	virtual void close() {
		if (m_Reader != NULL) {
			m_Reader->service_release();
			m_Reader = NULL;
		}
		if (m_NoSeekReader != NULL) {
			m_NoSeekReader->service_release();
			m_NoSeekReader = NULL;
		}
	}

	bool seekable() {
		if (m_NoSeekReader == NULL)
			return true;
		return false;
	};

protected:
	reader *m_Reader;
	reader *m_NoSeekReader;
};

#endif // _FOOBAR2000_IO_CALLBACK_H_
