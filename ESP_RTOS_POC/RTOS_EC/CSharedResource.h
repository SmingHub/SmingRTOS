/**
 * @file  CSharedResource.h
 * @class CSharedResource
 * @ingroup FreeRTOS_Managed
 *
 * This class implement a simple and generic paradigm to access a given resource T in
 * a controlled and task safe way.
 *
 * @date   Aug 23, 2010
 * @author Stefano Oliveri
 *         E-mail: software@stf12.net
 */

#ifndef CSharedResource_H_
#define CSharedResource_H_

#include <assert.h>
#include "FreeRTOS.h"

// forward declaration
template <class T, class L, int nTimeout> class CDefaultResourceEditor;

template<class T, class L, int nTimeout=(20/portTICK_RATE_MS)>
class CSharedResource {
public:
	class IResourceEditor {
	public:
		virtual T GetResource(T &resource, void *pParams=NULL) =0;
		virtual void SetResource(T &resource, const T &value, void *pParams=NULL) =0;

		virtual void OnDetach(CSharedResource<T, L, nTimeout> &formResource) {};
		virtual void OnTimeOut(CSharedResource<T, L, nTimeout> &formResource) {};
	};

private:
	static CDefaultResourceEditor<T, L, nTimeout> s_defaultResourceEditor;

	L m_look;
	T m_resouce;

	IResourceEditor *m_pEditor;

public:

	CSharedResource() {
		m_pEditor = NULL;
		m_look.Create();
		SetEditor(&s_defaultResourceEditor);
	}

	virtual ~CSharedResource() {
		if (m_pEditor != NULL)
			m_pEditor->OnDetach(*this);
	};

	void SetEditor(IResourceEditor *pEditor);

	T GetResource(void *pParams=NULL);
	void SetResource(const T &value, void *pParams=NULL);

	inline CSharedResource<T,L,nTimeout> &operator=(const T &value) {
		SetResource(value);
		return *this;
	}

	inline operator T() { return GetResource(); }
};


/**
 * \class CDefaultResourceEditor
 */
template<class T, class L, int nTimeout>
class CDefaultResourceEditor: public CSharedResource<T, L, nTimeout>::IResourceEditor {
public:
	inline T GetResource(T &resource, void *pParams=NULL) { return resource; }
	inline void SetResource(T &resource, const T &value, void *pParams=NULL) { resource = value; }
};

template<class T, class L, int nTimeout>
CDefaultResourceEditor<T, L, nTimeout> CSharedResource<T, L, nTimeout>::s_defaultResourceEditor;

// Class CSharedResource<T, L, nTimeout> implementation
// ----------------------------------------------------

template<class T, class L, int nTimeout>
void CSharedResource<T, L, nTimeout>::SetEditor(IResourceEditor *pEditor) {
	// Detach previous delegate if one;
	if (m_pEditor != NULL)
		m_pEditor->OnDetach(*this);

	m_pEditor = pEditor;
}

template<class T, class L, int nTimeout>
T CSharedResource<T, L, nTimeout>::GetResource(void *pParams) {
	assert(m_pEditor != NULL);

	T value;
	if (m_look.Take(nTimeout) == pdTRUE) {
		value = m_pEditor->GetResource(m_resouce, pParams);
		m_look.Give();
	}
	else {
		m_pEditor->OnTimeOut(*this);
	}

	return value;
}

template<class T, class L, int nTimeout>
void CSharedResource<T, L, nTimeout>::SetResource(const T &value, void *pParams) {
	assert(m_pEditor != NULL);

	if (m_look.Take(nTimeout) == pdTRUE) {
		m_pEditor->SetResource(m_resouce, value, pParams);
		m_look.Give();
	}
	else {
		m_pEditor->OnTimeOut(*this);
	}
}

#endif /* CSharedResource_H_ */
