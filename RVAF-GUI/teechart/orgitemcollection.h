// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class COrgItem;

/////////////////////////////////////////////////////////////////////////////
// COrgItemCollection wrapper class

class COrgItemCollection : public COleDispatchDriver
{
public:
	COrgItemCollection() {}		// Calls COleDispatchDriver default constructor
	COrgItemCollection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	COrgItemCollection(const COrgItemCollection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	COrgItem GetItems(long Index);
};