// BaseClassesNET.h

#pragma once

using namespace System;

namespace BaseClassesNET
{
	public __gc class MediaType
	{
	public:    
		MediaType() {
			m_CMediaType = new CMediaType();
		};
    MediaType(REFGUID majortype) {
			m_CMediaType = new CMediaType(&majortype);
		};
    MediaType(const MediaType &type) {
			m_CMediaType = new CMediaType(*type.m_CMediaType);
		};
    MediaType(MediaType __gc *type) {
			m_CMediaType = new CMediaType(*type->m_CMediaType);
		};
		~MediaType() {
			delete m_CMediaType;
		};
		
		virtual bool Equals(Object __gc *obj) {
			if (!obj->GetType()->Equals(this->GetType())) {
				return false;
			}
			
			return Equals(__try_cast<MediaType __gc *>(obj));
		};
		virtual bool Equals(MediaType __gc *type) {
			return ((*m_CMediaType == *type->m_CMediaType) == TRUE);
		};

    HRESULT Set(const MediaType& rt) {
			return m_CMediaType->Set(*rt.m_CMediaType);
		};
    HRESULT Set(MediaType __gc *rt) {
			return m_CMediaType->Set(*rt->m_CMediaType);
		};
		bool IsValid() {
			return (m_CMediaType->IsValid() == TRUE);
		};

    REFGUID Type() { return m_CMediaType->majortype;} ;
    void SetType(REFGUID guid) {
			m_CMediaType->SetType(&guid);
		};
    REFGUID Subtype() { return m_CMediaType->subtype;} ;
		void SetSubtype(REFGUID guid) {
			m_CMediaType->SetSubtype(&guid);
		};

    bool IsFixedSize() { return (m_CMediaType->bFixedSizeSamples == TRUE); };
    bool IsTemporalCompressed() { return (m_CMediaType->bTemporalCompression == TRUE); };
		long GetSampleSize() { return (long)m_CMediaType->GetSampleSize(); };

    void SetSampleSize(long sz) {
			m_CMediaType->SetSampleSize(sz);
		};
    void SetVariableSize() {
			m_CMediaType->SetVariableSize();
		};
    void SetTemporalCompression(bool bCompressed) {
			m_CMediaType->SetTemporalCompression(bCompressed);
		};

    // read/write pointer to format - can't change length without
    // calling SetFormat, AllocFormatBuffer or ReallocFormatBuffer
    Byte Format() [] { 
			Byte format[] = new Byte[m_CMediaType->cbFormat];
			
			for (int i = 0; i < (int)m_CMediaType->cbFormat; i++) {
				format[i] = m_CMediaType->pbFormat[i];
			}

			return format; 
		};
    long FormatLength() { return (long)m_CMediaType->cbFormat; };
    void SetFormat(Byte format[]) { 
			BYTE *pFormatBuffer = m_CMediaType->pbFormat;
			
			if (format->Count == 0) {
				m_CMediaType->ResetFormatBuffer();
				return;
			}
			
			if (pFormatBuffer == NULL || format->Count != m_CMediaType->cbFormat) {
				pFormatBuffer = m_CMediaType->AllocFormatBuffer((ULONG)format->Count);
			}

			for (int i = 0; i < (int)format->Count; i++) {
				pFormatBuffer[i] = format[i];
			}
		};

    void SetFormatType(REFGUID guid) {
			m_CMediaType->SetFormatType(&guid);
		};
    REFGUID FormatType() { return m_CMediaType->formattype; };
    //BOOL SetFormat(BYTE *pFormat, ULONG length);
    void ResetFormatBuffer() {
			m_CMediaType->ResetFormatBuffer();
		};
    //BYTE* AllocFormatBuffer(long length);
    //BYTE* ReallocFormatBuffer(long length);
    
    bool MatchesPartial(const MediaType &partial) {
			return (m_CMediaType->MatchesPartial(partial.m_CMediaType) == TRUE);
		};
    bool MatchesPartial(MediaType __gc *partial) {
			return (m_CMediaType->MatchesPartial(partial->m_CMediaType) == TRUE);
		};
    bool IsPartiallySpecified() {
			return (m_CMediaType->IsPartiallySpecified() == TRUE);
		};

	private:
//		friend MediaType;
		CMediaType *m_CMediaType;
	};
}
