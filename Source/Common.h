#include <algorithm>
#include <fstream>
#include <vector>
#include <math.h>
#include <map>
#include <set>
#include <queue>
#include "tinyxml2.h"

#pragma once

#ifdef _DEBUG
#define _CHECK_ALL_
#else
//#define _CHECK_ALL_		//<--------------------- IS IT ACTIVE IN RELEASE?
#endif

#define _SINGLE_SOURCE_ASSUMPTION_	1
using namespace std ;

int		Round( double d ) ;

#define IsInRange( X , Mn , Mx ) ( ( X ) >= ( Mn ) && ( X ) <= ( Mx ) )
#define SetInRange( x , Min , Max ) { if ( (x) < (Min) ) (x) = (Min) ; else if ( (x) > (Max) ) (x) = (Max) ; }

#define DECLEQ( Var , Val ) decltype( Val ) Var = Val

#define	binc( V , M ) ( ( V + 1 ) % M )
#define	bdec( V , M ) ( ( V + M - 1 ) % M )
#define	SETZ( X ) memset( & X , 0 , sizeof( X ) )
#define RANGE( I , EN ) ( int I = 0 ; I < (EN) ; I++ )
#define DRANGE( I , ST , EN ) ( int I = ST ; I < (EN) ; I++ )
#define REVRANGE( I , EN ) ( int I = (EN) - 1 ; I >= 0 ; I-- )
#define DREVRANGE( I , EN , ST ) ( int I = (EN) ; I >= ST ; I-- )
#define ALL( VEC , pVAR ) ( decltype( ( VEC ).begin() ) pVAR = ( VEC ).begin() ; pVAR != ( VEC ).end() ; pVAR++ )
#define ALL_EXCEPT_FIRST( VEC , pVAR ) ( decltype( ( VEC ).begin() ) pVAR = ( VEC ).begin() + 1 ; pVAR != ( VEC ).end() ; pVAR++ )
#define ALL_REV( VEC , pVAR ) ( decltype( ( VEC ).rbegin() ) pVAR = ( VEC ).rbegin() ; pVAR != ( VEC ).rend() ; pVAR++ )
#define ALL_INDICES( VEC , iVAR ) ( int iVAR = 0 ; iVAR < ( int ) ( VEC ).size() ; iVAR++ )
#define ALL_INDICES_REV( VEC , iVAR ) ( int iVAR = ( ( int ) ( VEC ).size() ) - 1 ; iVAR >= 0 ; iVAR-- )
#define ALL_INDICES_REV_EXCEPT_LAST( VEC , iVAR ) ( int iVAR = ( ( int ) ( VEC ).size() ) - 2 ; iVAR >= 0 ; iVAR-- )
#define ALL_INDICES_EXCEPT_LAST( VEC , iVAR ) ( int iVAR = 0 ; iVAR < ( ( int ) ( VEC ).size() ) - 1 ; iVAR++ )
#define ALL_INDICES_EXCEPT_FIRST( VEC , iVAR ) ( int iVAR = 1 ; iVAR < ( ( int ) ( VEC ).size() ) ; iVAR++ )
#define COUNT( VEC , i ) ( int i = ( int ) ( VEC ).size() ; i ; i-- )
#define FOR_ALL_SHUFF( VEC , pVAR , SHUFF , iTemp ) decltype( ( VEC ).begin() ) pVAR = VEC.begin() + SHUFF[ 0 ] ; for ( int iTemp = 0 ; iTemp < ( int ) SHUFF.size() ; iTemp++ , pVAR = VEC.begin() + SHUFF[ min( iTemp , ( int ) SHUFF.size() - 1 ) ] )

#define IF_FOUND( VEC , VAL , VAR ) decltype( ( VEC ).begin() ) VAR = find( ( VEC ).begin() , ( VEC ).end() , VAL ) ; if ( VAR != ( VEC ).end() )
#define IF_NOT_FOUND( VEC , VAL , VAR ) decltype( ( VEC ).begin() ) VAR = find( ( VEC ).begin() , ( VEC ).end() , VAL ) ; if ( VAR == ( VEC ).end() )

#define IF_FOUND_IN_MAP( MAP , VAL , VAR ) decltype( ( MAP ).begin() ) VAR = MAP.find( VAL ) ; if ( VAR != ( MAP ).end() )
#define IF_NOT_FOUND_IN_MAP( MAP , VAL , VAR ) decltype( ( MAP ).begin() ) VAR = MAP.find( VAL ) ; if ( VAR == ( MAP ).end() )

#define FOUND_IN_SET( SET , VAL ) ( ( SET ).find( VAL ) != ( SET ).end() )

#define CATCH_AND_REPORT( DEFAULT_TXT ) catch ( TCHAR * pText ) { MyMessageBox( pText , MB_ICONERROR ) ; } catch ( CException * pExp ) { pExp->ReportError( MB_ICONERROR ) ; }	catch (...)	{ MyMessageBox( DEFAULT_TXT , MB_ICONERROR ) ; }
#define CATCH_AND_REPORT_AND_RETURN( DEFAULT_TXT , VAL ) catch ( TCHAR * pText ) { MyMessageBox( pText , MB_ICONERROR ) ; return VAL ; } catch ( CException * pExp ) { pExp->ReportError( MB_ICONERROR ) ; return VAL ; }	catch (...)	{ MyMessageBox( DEFAULT_TXT , MB_ICONERROR ) ; return VAL ; }
#define CATCH_AND_REPORT_AND_DO( DEFAULT_TXT , ACTION ) catch ( TCHAR * pText ) { MyMessageBox( pText , MB_ICONERROR ) ; ACTION ; } catch ( CException * pExp ) { pExp->ReportError( MB_ICONERROR ) ; ACTION ; }	catch (...)	{ MyMessageBox( DEFAULT_TXT , MB_ICONERROR ) ; ACTION ; }

#define	CREATE_SHUFFLE_LIST( VEC , SIZE , OFFS ) vector<int> VEC( SIZE ) ; { for( int i = 0 ; i < SIZE ; i++ ) VEC[ i ] = i + OFFS ; } random_shuffle( VEC.begin() , VEC.end() ) ;

#define	VEC_INDEX( VAR , VEC ) ( ( VAR ) - ( VEC ).begin() )
#define VEC_INSERT( VEC , POS , ITEM ) ( VEC ).insert( ( VEC ).begin() + ( POS ) , ITEM )
#define VEC_ERASE( VEC , POS ) ( VEC ).erase( ( VEC ).begin() + ( POS ) )
#define VEC_INDEX_VALID( I , VEC ) ( I < ( int ) ( VEC ).size() )

#define IsInRange( x , Mn , Mx ) ( ( x ) >= ( Mn ) && ( x ) <= ( Mx ) )

#define DELFNZ( X ) if ( X ) { delete X ; X = 0 ; }
#define DELFNZV( X ) if ( X ) { delete[] X ; X = 0 ; }

// Check Failure Return
#ifdef _DEBUG
#define _CFR_( X , Y )	if ( ! ( X ) ) { _RPTF3( _CRT_ERROR ,"Unexpected '%s'\r\n at file %s, Line %i." , #X , __FILE__ , __LINE__ ) ; return Y ; }
#define _IF_NOT_FAILED_( X ) if ( ! ( X ) ) { _RPTF3( _CRT_ERROR ,"Unexpected '%s'\r\n at file %s, Line %i." , #X , __FILE__ , __LINE__ ) ; } else
#define _IF_FAILED_RETURN_( X , Y ) if ( ! ( X ) ) { _RPTF3( _CRT_ERROR ,"Unexpected '%s'\r\n at file %s, Line %i." , #X , __FILE__ , __LINE__ ) ; return Y ; }
#elif defined( _CHECK_ALL_ )
#define _CFR_( X , Y )	if ( ! ( X ) ) { CString Error ; Error.Format( L"Unexpected '%S'\r\n at file %S, Line %i." , #X , __FILE__ , __LINE__ ) ; AfxMessageBox( Error , MB_ICONERROR ) ; return Y ; }
#define _IF_NOT_FAILED_( X ) if ( ! ( X ) ) { CString Error ; Error.Format( L"Unexpected '%S'\r\n at file %S, Line %i." , #X , __FILE__ , __LINE__ ) ; AfxMessageBox( Error , MB_ICONERROR ) ; } else
#define _IF_FAILED_RETURN_( X , Y ) if ( ! ( X ) ) { CString Error ; Error.Format( L"Unexpected '%S'\r\n at file %S, Line %i." , #X , __FILE__ , __LINE__ ) ; AfxMessageBox( Error , MB_ICONERROR ) ; return Y ; }
#else
#define _CFR_( X , Y )
#define _IF_NOT_FAILED_( X ) if ( X )
#define _IF_FAILED_RETURN_( X , Y ) if ( ! ( X ) ) return Y ;
#endif

#define _CFRF_( X ) _CFR_( X , false ) ;


////////////////
//
// SERIALIZATION
//
//

#define WRITE( X ) { if ( fwrite( & ( X ) , sizeof( X ) , 1 , hFile ) != 1 ) throw 0 ; }
#define WRITES( X , S ) { if ( fwrite( ( X ) , 1 , S , hFile ) != S ) throw 0 ; }
#define WRITE_STR( X ) { int iLen = X ? wcslen( X ) : 0 ; WRITE( iLen ) ; if ( iLen ) if ( fwrite( X , sizeof( TCHAR ) , iLen , hFile ) != iLen ) throw 0 ; }
#define WRITE_STRA( X ) { int iLen = X.GetLength() ; WRITE( iLen ) ; if ( iLen ) if ( fwrite( X.GetBuffer() , sizeof( char ) , iLen , hFile ) != iLen ) throw 0 ; }

#define READ( X ) { if ( fread( & X , sizeof( X ) , 1 , hFile ) != 1 ) throw 0 ; }
#define READS( X , S ) { if ( fread( X , 1 , S , hFile ) != S ) throw 0 ; }
#define READ_STR( X ) { int iLen ; READ( iLen ) ; if ( iLen ) { X = new TCHAR[ iLen + 1 ] ; X[ iLen ] = 0 ; if ( fread_s( X , iLen * sizeof( TCHAR ) , sizeof( TCHAR ) , iLen , hFile ) != iLen ) { delete X ; throw 0 ; } } else X = NULL ; }
#define READ_STRA( X ) { int iLen ; READ( iLen ) ; if ( iLen ) { if ( fread_s( X.GetBufferSetLength( iLen + 1 ) , iLen * sizeof( char ) , sizeof( char ) , iLen , hFile ) != iLen ) { throw 0 ; } else X.SetAt( iLen , 0 ) ; } else X = "" ; }

#define SERIALIZE( X ) { if ( bLoad ) READ( X ) else WRITE( X ) ; }
#define SERIALIZES( X , S ) { if ( bLoad ) READS( X , S ) else WRITES( X , S ) ; }
#define SERIALIZE_STR( X ) { if ( bLoad ) READ_STR( X ) else WRITE_STR( X ) ; }
#define SERIALIZE_STRA( X ) { if ( bLoad ) READ_STRA( X ) else WRITE_STRA( X ) ; }
#define SERIALIZE_VECTOR( V ) { unsigned uCount = V.size() ; SERIALIZE( uCount ) ; if ( bLoad ) { V.clear() ; V.resize( uCount ) ; } \
								for ( unsigned i = 0 ; i < uCount ; i++ ) SERIALIZE( V[ i ] ) } ;


bool	SelectFolder( HWND hWnd , CString Title , CString & theFolder ) ;
void	GetFiles( vector<CString> & FileNames , CString Filter , CString Title , CString InitDir ) ;
////////////////////////////
//
// POINT TYPES AND FUNCTIONS
//
//


template<class T>
tinyxml2::XMLElement * GetXMLNestedElement( T * pDoc , char * pNode0 , char * pNode1 = NULL , char * pNode2 = NULL , char * pNode3 = NULL , char * pNode4 = NULL , char * pNode5 = NULL ) ;

template<class T>
const char * GetXMLNestedText( T * pDoc , char * pNode0 , char * pNode1 = NULL , char * pNode2 = NULL , char * pNode3 = NULL , char * pNode4 = NULL , char * pNode5 = NULL ) ;

// Unzip
void UnZipFile(CString strSrc, CString strDest) ;

#define CHKSTR( T , N ) if ( Name == T ) return N ;
typedef	tinyxml2::XMLElement XMLElem;
#define ALL_NODES1( pVar , pParent , Text1 )							( XMLElem * pVar = GetXMLNestedElement( pParent , Text1 ) ;							pVar ;	pVar = pVar##->NextSiblingElement( Text1 ) )
#define ALL_NODES2( pVar , pParent , Text1 , Text2 )					( XMLElem * pVar = GetXMLNestedElement( pParent , Text1 , Text2 ) ;					pVar ;	pVar = pVar##->NextSiblingElement( Text2 ) )
#define ALL_NODES3( pVar , pParent , Text1 , Text2 , Text3 )			( XMLElem * pVar = GetXMLNestedElement( pParent , Text1 , Text2 , Text3 ) ;			pVar ;	pVar = pVar##->NextSiblingElement( Text3 ) )
#define ALL_NODES4( pVar , pParent , Text1 , Text2 , Text3 , Text4 )	( XMLElem * pVar = GetXMLNestedElement( pParent , Text1 , Text2 , Text3 , Text4 ) ;	pVar ;	pVar = pVar##->NextSiblingElement( Text4 ) )

int	_safe_atoi(const char * pchText);
char _safe_first_upper(const char * pchText);
CString	NodeToText(XMLElem * pNode);
