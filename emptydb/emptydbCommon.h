/*
	emptydb::Common
*/
#pragma once
#include "../plib/plibStdDataBST.h"
#include "../plib/plibStdMemoryPool.h"

typedef uint32_t emptydbCommonKeyType ;

struct emptydbCommonObjectValueType
{
	struct plibStdDataBST *MemberObjectRootNode , *MemberKeyValueRootNode ;
} ;
