#pragma once

#ifdef _AFX
#define ERR_MSG(Message) AfxMessageBox(Message)
#else
#define ERR_MSG(Message) MessageBox(nullptr,Message,L"System_Error",MB_OK)
#endif

// KEY 관련
#define		KEY_LBUTTON		0x00000001
#define		KEY_RBUTTON		0x00000002 
#define		KEY_LEFT		0x00000004
#define		KEY_RIGHT		0x00000008 
#define		KEY_S			0x00000010
#define		KEY_RETURN		0x00000020
#define		KEY_UP			0x00000040
#define		KEY_DOWN		0x00000080
#define		KEY_P			0x00000100
#define		KEY_D			0x00000200
#define		KEY_SPACE		0x00000400
#define		KEY_SHIFT		0x00000800
#define		KEY_E			0x00001000
#define		KEY_F			0x00002000
#define		KEY_G			0x00004000
#define		KEY_A			0x00008000
#define		KEY_W			0x00010000
#define		KEY_TAB			0x00020000
#define		KEY_R			0x00040000
#define		KEY_T			0x00080000
#define		KEY_M			0x00100000
#define		KEY_Y			0x00200000

#define WINCX 800
#define WINCY 600

// 싱글톤 관련
#define GET_SINGLE(_class)				_class::Get_Instance()
#define SAFE_DELETE(_pObj)				if(_pObj) { delete _pObj; _pObj = nullptr;}

#define DECLARE_SINGLETON(ClassName)							\
public:															\
static ClassName* Get_Instance()								\
{																\
	if (nullptr == m_pInstance)									\
		m_pInstance = new ClassName;							\
	return m_pInstance;											\
}																\
static void Destroy_Instance()									\
{																\
	if (m_pInstance)											\
	{															\
		delete m_pInstance;										\
		m_pInstance = nullptr;									\
	}															\
}																\
private:														\
	static ClassName* m_pInstance;				

#define DEFINITION_SINGLETON(ClassName)							\
ClassName* ClassName::m_pInstance = nullptr;


// 오브젝트 유효성 체크
#define IS_VALID_OBJ(_pObj)				((_pObj) && (_pObj->IsValid()))
#define DO_IF_IS_NOT_VALID_OBJ(_pObj)	if(!IS_VALID_OBJ(_pObj))
#define DO_IF_IS_VALID_OBJ(_pObj)		if(IS_VALID_OBJ(_pObj))