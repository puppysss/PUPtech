/*
	plib::Data::HBST
	Hierarchial Binary Search Tree
*/
#include "plibDataHBST.h"

void plibDataHBST_initialize( struct plibDataHBST *Node , struct plibError *Error )
{
	// error
	if( Node == plibCommonNullPointer )
	{
		plibError_report( Error , plibErrorParameter , plibDataHBST_initialize ) ;
		return ;
	}
	
	Node->Key = plibCommonNullPointer ;
	Node->Value = plibCommonNullPointer ;
	
	Node->Top = plibCommonNullPointer ;
	Node->Left = plibCommonNullPointer ;
	Node->Right = plibCommonNullPointer ;
	
	Node->SuperIndex = 0 ;
	Node->Super = plibCommonNullPointer ;
	
	Node->SubLength = 0 ;
	Node->Sub = plibCommonNullPointer ;
}

bool plibDataHBST_push( struct plibDataHBST **EntryNode , struct plibDataHBST *NewNode , plibDataHBSTStatusFxType StatusFx , struct plibError *Error )
{
	// error
	if( EntryNode == plibCommonNullPointer || NewNode == plibCommonNullPointer || StatusFx == plibCommonNullPointer )
	{
		plibError_report( Error , plibErrorParameter , plibDataHBST_push ) ;
		return false ;
	}
	
	struct plibDataHBST *ThisNode = *EntryNode ;
	
	if( ThisNode == plibCommonNullPointer )
		*EntryNode = NewNode ;
	else
	{
		do
		{
			switch( StatusFx( NewNode->Key , ThisNode->Key ) )
			{
				case plibDataHBSTStatusLess : 
					if( ThisNode->Left == plibCommonNullPointer )
					{
						// pointing a new node on Left and this node on Top
						ThisNode->Left = NewNode ;
						NewNode->Top = ThisNode ;
						return true ;
					}
					
					ThisNode = ThisNode->Left ;
				break ;
				case plibDataHBSTStatusGreat : 
					if( ThisNode->Right == plibCommonNullPointer )
					{
						// pointing a new node on Right and this node on Top
						ThisNode->Right = NewNode ;
						NewNode->Top = ThisNode ;
						return true ;
					}
					
					ThisNode = ThisNode->Right ;
				break ;
				
				case plibDataHBSTStatusEqual : 
				default : 
					// error
					plibError_report( Error , plibErrorProcess , plibDataHBST_push ) ;
					return false ;
			}
		}
		while( ThisNode != plibCommonNullPointer ) ;
	}
	
	return true ;
}
struct plibDataHBST* plibDataHBST_pop( struct plibDataHBST **EntryNode , plibCommonAnyType *Key , plibDataHBSTStatusFxType StatusFx , struct plibError *Error )
{
	// error
	if( EntryNode == plibCommonNullPointer || Key == plibCommonNullPointer || StatusFx == plibCommonNullPointer )
	{
		plibError_report( Error , plibErrorParameter , plibDataHBST_pop ) ;
		return plibCommonNullPointer ;
	}
	
	struct plibDataHBST *ThisNode = *EntryNode , *CandidateNode ;
	bool Flag ;
	
	while( ThisNode != plibCommonNullPointer )
	{
		switch( StatusFx( Key , ThisNode->Key ) )
		{
			case plibDataHBSTStatusLess :
				ThisNode = ThisNode->Left ;
				Flag = false ;
			break ;
			case plibDataHBSTStatusGreat :
				ThisNode = ThisNode->Right ;
				Flag = true ;
			break ;
			case plibDataHBSTStatusEqual :
				// popping a leaf node
				if( ThisNode->Left == plibCommonNullPointer && ThisNode->Right == plibCommonNullPointer )
				{
					if( ThisNode == *EntryNode )
						// root node
						*EntryNode = plibCommonNullPointer ;
					else
					{
						if( Flag == false )
							ThisNode->Top->Left = plibCommonNullPointer ;
						else
							ThisNode->Top->Right = plibCommonNullPointer ;
					}
					
					return ThisNode ;
				}
				// popping a node having left child node
				else if( ThisNode->Left != plibCommonNullPointer && ThisNode->Right == plibCommonNullPointer )
				{
					if( ThisNode == *EntryNode )
					{
						// root node
						ThisNode->Left->Top = plibCommonNullPointer ;
						*EntryNode = ThisNode->Left ;
					}
					else
					{
						ThisNode->Left->Top = ThisNode->Top ;
						if( Flag == false )
							ThisNode->Top->Left = ThisNode->Left ;
						else
							ThisNode->Top->Right = ThisNode->Left ;
					}
					
					return ThisNode ;
				}
				// popping a node having right child node
				else if( ThisNode->Left == plibCommonNullPointer && ThisNode->Right != plibCommonNullPointer )
				{
					if( ThisNode == *EntryNode )
					{
						// root node
						ThisNode->Right->Top = plibCommonNullPointer ;
						*EntryNode = ThisNode->Right ;
					}
					else
					{
						ThisNode->Right->Top = ThisNode->Top ;
						if( Flag == false )
							ThisNode->Top->Left = ThisNode->Right ;
						else
							ThisNode->Top->Right = ThisNode->Right ;
					}
					
					return ThisNode ;
				}
				// popping a full node
				else
				{
					// getting a node that is most high value at left tree of this node
					for( CandidateNode = ThisNode->Left ; CandidateNode->Right != plibCommonNullPointer ; CandidateNode = CandidateNode->Right ) ;
					if( CandidateNode != ThisNode->Left )
					{
						CandidateNode->Top->Right = CandidateNode->Left ;
						if( CandidateNode->Left != plibCommonNullPointer )
							CandidateNode->Left->Top = CandidateNode->Top ;
					}
					
					if( ThisNode == *EntryNode )
					{
						// root node
						CandidateNode->Top = plibCommonNullPointer ;
						*EntryNode = CandidateNode ;
					}
					else
					{
						CandidateNode->Top = ThisNode->Top ;
						if( Flag == false )
							ThisNode->Top->Left = CandidateNode ;
						else
							ThisNode->Top->Right = CandidateNode ;
					}
					
					return ThisNode ;
				}
				
			default :
				// error
				plibError_report( Error , plibErrorProcess , plibDataHBST_pop ) ;
				return plibCommonNullPointer ;
		}
	}
	
	// error
	plibError_report( Error , plibErrorProcess , plibDataHBST_pop ) ;
	return plibCommonNullPointer ;
}
struct plibDataHBST* plibDataHBST_lookup( struct plibDataHBST *ThisNode , plibCommonAnyType *Key , plibDataHBSTStatusFxType StatusFx , struct plibError *Error )
{
	// error
	if( ThisNode == plibCommonNullPointer || Key == plibCommonNullPointer || StatusFx == plibCommonNullPointer )
	{
		plibError_report( Error , plibErrorParameter , plibDataHBST_lookup ) ;
		return plibCommonNullPointer ;
	}
	
	while( ThisNode != plibCommonNullPointer )
	{
		switch( StatusFx( Key , ThisNode->Key ) )
		{
			case plibDataHBSTStatusLess : 
				ThisNode = ThisNode->Left ;
			break ;
			case plibDataHBSTStatusGreat : 
				ThisNode = ThisNode->Right ;
			break ;
			case plibDataHBSTStatusEqual : 
				return ThisNode ;
			
			default : 
				// error
				plibError_report( Error , plibErrorProcess , plibDataHBST_lookup ) ;
				return plibCommonNullPointer ;
		}
	}
	
	// error
	plibError_report( Error , plibErrorProcess , plibDataHBST_lookup ) ;
	return plibCommonNullPointer ;
}

bool plibDataHBST_pushSub( struct plibDataHBST *SuperNode , plibCommonCountType Index , struct plibDataHBST *NewNode , plibDataHBSTStatusFxType StatusFx , struct plibError *Error )
{
	// error
	if( SuperNode == plibCommonNullPointer )
	{
		plibError_report( Error , plibErrorParameter , plibDataHBST_pushSub ) ;
		return false ;
	}
	else if( Index >= SuperNode->SubLength )
	{
		plibError_report( Error , plibErrorParameter , plibDataHBST_pushSub ) ;
		return false ;
	}
	
	if( plibDataHBST_push( &( SuperNode->Sub[ Index ].RootNode ) , NewNode , StatusFx , Error ) == false )
	{
		plibError_report( Error , plibErrorProcess , plibDataHBST_pushSub ) ;
		return false ;
	}
	
	NewNode->SuperIndex = Index ;
	NewNode->Super = SuperNode ;
	SuperNode->Sub[ Index ].Count ++ ;
	
	return true ;
}
struct plibDataHBST* plibDataHBST_popSub( struct plibDataHBST *SuperNode , plibCommonCountType Index , plibCommonAnyType *Key , plibDataHBSTStatusFxType StatusFx , struct plibError *Error )
{
	// error
	if( SuperNode == plibCommonNullPointer )
	{
		plibError_report( Error , plibErrorParameter , plibDataHBST_popSub ) ;
		return plibCommonNullPointer ;
	}
	else if( Index >= SuperNode->SubLength )
	{
		plibError_report( Error , plibErrorParameter , plibDataHBST_popSub ) ;
		return plibCommonNullPointer ;
	}
	
	struct plibDataHBST *OldNode = plibDataHBST_pop( &( SuperNode->Sub[ Index ].RootNode ) , Key , StatusFx , Error ) ;
	
	// error
	if( OldNode == plibCommonNullPointer )
	{
		plibError_report( Error , plibErrorProcess , plibDataHBST_popSub ) ;
		return plibCommonNullPointer ;
	}
	
	SuperNode->Sub[ Index ].Count -- ;
	return OldNode ;
}
bool plibDataHBST_traverse( struct plibDataHBST *EntryNode , plibDataHBSTTraversalFxType TraversalFx , plibCommonAnyType *Data , struct plibError *Error )
{
	// error
	if( TraversalFx == plibCommonNullPointer )
	{
		plibError_report( Error , plibErrorParameter , plibDataHBST_traverse ) ;
		return false ;
	}
	
	struct plibDataHBST *TempNode = EntryNode ;
	plibCommonCountType Index = 0 ;
	
	if( TempNode != plibCommonNullPointer )
	{
		Visiting :
		TraversalFx( TempNode , TempNode->SuperIndex , Data , Error ) ;
			
		// sub
		CheckingSub :
		if( Index < TempNode->SubLength )
		{
			if( TempNode->Sub[ Index ].RootNode == plibCommonNullPointer )
			{
				Index ++ ;
				goto CheckingSub ;
			}
			
			TempNode = TempNode->Sub[ Index ].RootNode ;
			Index = 0 ;
			goto Visiting ;
		}
		
		// left right top
		if( TempNode->Left != plibCommonNullPointer )
		{
			TempNode = TempNode->Left ;
			Index = 0 ;
			goto Visiting ;
		}
		else if( TempNode->Right != plibCommonNullPointer )
		{
			TempNode = TempNode->Right ;
			Index = 0 ;
			goto Visiting ;
		}
		else
		{
			while( TempNode->Top != plibCommonNullPointer )
			{
				if( TempNode->Top->Right != plibCommonNullPointer && TempNode->Top->Right != TempNode )
				{
					TempNode = TempNode->Top->Right ;
					Index = 0 ;
					goto Visiting ;
				}
				
				TempNode = TempNode->Top ;
			}
		}
		
		// super
		if( TempNode->Super != plibCommonNullPointer )
		{
			Index = TempNode->SuperIndex + 1 ;
			TempNode = TempNode->Super ;
			goto CheckingSub ;
		}
		
		// no nodes to visit
	}
	
	return true ;
}