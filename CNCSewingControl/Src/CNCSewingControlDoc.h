//===================================================================================================
// Summary:
//		主文档类
// Date:
//		2018-07-07
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#pragma once


class CCNCSewingControlDoc : public CDocument
{
protected: // 仅从序列化创建
	CCNCSewingControlDoc();
	DECLARE_DYNCREATE(CCNCSewingControlDoc)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CCNCSewingControlDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};


