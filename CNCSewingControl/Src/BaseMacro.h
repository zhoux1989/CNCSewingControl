//===================================================================================================
// Summary:
//		SmartConfig内部使用宏的定义
// Date:
//		2011-08-09
// Author:
//		Liuguoqing(gliu@bplead.com)
//===================================================================================================

#ifndef	__BaseMacro_h__
#define __BaseMacro_h__

//===================================================================================================
// 以下为用户注册表项目


//===================================================================================================

#define WM_FILEDW_PROGRESS			WM_USER + 1500
#define WM_FILEDW_ONEDONE			WM_USER + 1501
#define WM_FILEDW_ALLDONE			WM_USER + 1502
#define WM_FILEDW_ERRREPORT			WM_USER + 1503

#define WM_NETCOMM_DONE				WM_USER + 1600
#define WM_NETCOMM_ERRREPORT		WM_USER + 1601
#define WM_NETCOMM_REQUEST			WM_USER + 1602

// 删除指针
#define SAFE_DELETE_POINTER(p) \
{ \
	if (NULL != p) \
	delete p; \
	p = NULL; \
}

// 删除数组
#define SAFE_DELETE_ARRAY(p) \
{ \
	if (NULL != p) \
	delete[] p; \
	p = NULL; \
}

// 删除资源句柄
#define SAFE_CLOSE_HANDLE(handle)	\
{ \
	if (handle)	\
	::CloseHandle(handle); \
	handle = NULL; \
}

// 删除选择对象
#define SAFE_DELETE_SELECTION(p) \
{ \
	if (NULL != p) \
	ProSelectionFree(&p); \
	p = NULL; \
}

// 删除选择对象
#define SAFE_DELETE_PROARRAY(p) \
{ \
	if (NULL != p) \
	ProArrayFree((ProArray*)&p); \
	p = NULL; \
}

#define SAFE_DESTROY_DIALOG(dlg) \
{ \
	if (dlg != NULL) \
	{ \
		if (dlg->GetSafeHwnd() != NULL) \
			dlg->DestroyWindow(); \
		delete dlg; \
		dlg = NULL; \
	} \
}

#define WM_USER_CREATE_OPERATOR			WM_USER + 0x0001
#define WM_APPLY_ADD_CHECK_RULE			WM_USER + 0x0002
#define WM_MODIFY_CHECK_RULE			WM_USER + 0x0003

// 是否tcp连接
#define WM_TCP_CON_OK					WM_USER + 0x0005

#define DFE_DELTA 0.001
#define DFE_ZERO 0.000001
#define DEQUAL(x, y) ((x)-(y)<DFE_ZERO && (x)-(y)>-DFE_ZERO)
#define ISNEAR(x, y) ((x)-(y)<DFE_DELTA && (x)-(y)>-DFE_DELTA)

// 圆周率
#define DOCTOR_PI 3.14159265359
// 角度转弧度
#define ANGLE_TO_RADIAN(angle) (angle) * (0.017453292519943295769236907684883)
#define RADIAN_TO_ANGLE(rad) (rad) * (57.295779513082320876798154814105)

// 脉冲增量单位0.05mm
#define PULSE_UNIT 0.05

//===================================================================================================

#endif