#pragma once
#include "MusicSheet.h"	
#include "Common.h"

#define CHKSTR( T , N ) if ( Name == T ) return N ;

class CMusixXMLParser
{
public:
	CMusixXMLParser(void);
	~CMusixXMLParser(void);

	MusicSheet::NoteTypes	StringToNoteType( const char * pchName )
	{
		if ( pchName )
		{
			CStringA	Name( pchName ) ;
			Name.MakeUpper() ;

			CHKSTR( "WHOLE" ,	MusicSheet::TYPE_WHOLE) ;		// گرد
			CHKSTR( "HALF" ,	MusicSheet::TYPE_HALF ) ;		// سفید
			CHKSTR( "QUARTER" ,	MusicSheet::TYPE_QUARTER ) ;	// سیاه
			CHKSTR( "EIGHTH" ,	MusicSheet::TYPE_EIGHTH ) ;		// چنگ
			CHKSTR( "16TH" ,	MusicSheet::TYPE_SIXTEENTH ) ;	// 
			CHKSTR( "32ND" ,	MusicSheet::TYPE_32ND ) ;	// 

		}
		else
			return MusicSheet::TYPE_UNTOLD ;

		return MusicSheet::TYPE_UNKNOWN ;
	}

	MusicSheet::DirectionTypes StringToDirectionsType( const char * pchName )
	{
		if ( pchName )
		{
			CStringA	Name( pchName ) ;
			Name.MakeUpper() ;

			//CHKSTR( "ROMANZE" ,		MusicSheet::DIR_ROMANZE ) ;
			CHKSTR( "DOLCE" ,		MusicSheet::DIR_DOLCE ) ;
			CHKSTR( "MESTO" ,		MusicSheet::DIR_MESTO ) ;
			CHKSTR( "A TEMPO" ,		MusicSheet::DIR_A_TEMPO ) ;
			CHKSTR( "POCO RIT." ,	MusicSheet::DIR_POCO_RIT ) ;
			CHKSTR( "POCO RALL" ,	MusicSheet::DIR_POCO_RALL ) ;
			CHKSTR( "SEMPRE" ,		MusicSheet::DIR_SEMPRE ) ;
			CHKSTR( "ACCEL" ,		MusicSheet::DIR_ACCEL ) ;
			CHKSTR( "TRILL" ,		MusicSheet::DIR_TRILL ) ;
			CHKSTR( "GRUPPETTO" ,	MusicSheet::DIR_GRUPPETTO ) ;
			CHKSTR( "MORDENT" ,		MusicSheet::DIR_MORDENT ) ;
			
			CHKSTR( "ALLEGRO" ,		MusicSheet::DIR_ALLEGRO) ;
			CHKSTR( "ALLEGRATO" ,	MusicSheet::DIR_ALLEGRATO) ;
			CHKSTR( "GRAVE" ,		MusicSheet::DIR_GRAVE) ;
			CHKSTR( "LARGO" ,		MusicSheet::DIR_LARGO) ;
			CHKSTR( "MODERATO" ,	MusicSheet::DIR_MODERATO ) ;
						
			CHKSTR( "ACCEL." ,		MusicSheet::DIR_DYNAMIC_ACCELERANDO  ) ;
			CHKSTR( "P" ,			MusicSheet::DIR_DYNAMIC_P ) ;
			CHKSTR( "PP" ,			MusicSheet::DIR_DYNAMIC_PP) ;
			CHKSTR( "PPP" ,			MusicSheet::DIR_DYNAMIC_PPP) ;
			CHKSTR( "MP" ,			MusicSheet::DIR_DYNAMIC_MP) ;
			CHKSTR( "MF" ,			MusicSheet::DIR_DYNAMIC_MF) ;
			CHKSTR( "F" ,			MusicSheet::DIR_DYNAMIC_F) ;
			CHKSTR( "FF" ,			MusicSheet::DIR_DYNAMIC_FF ) ;
			CHKSTR( "FZ" ,			MusicSheet::DIR_DYNAMIC_SFZ) ;
			CHKSTR( "SF",			MusicSheet::DIR_DYNAMIC_SFZ);
			CHKSTR( "SFZ" ,			MusicSheet::DIR_DYNAMIC_SFZ) ;
			CHKSTR( "SFFZ" ,		MusicSheet::DIR_DYNAMIC_SFZ) ;
			CHKSTR( "DIM" ,			MusicSheet::DIR_DYNAMIC_DIM) ;
			CHKSTR( "DECRESC" ,		MusicSheet::DIR_DYNAMIC_DECRESC) ;
			CHKSTR( "CRESC" ,		MusicSheet::DIR_DYNAMIC_CRESC) ;
			CHKSTR( "STACCATO" ,	MusicSheet::DIR_DYNAMIC_STACCATO) ;
			CHKSTR( "LEGATO" ,		MusicSheet::DIR_DYNAMIC_LEGATO) ;
							
			CHKSTR( "DIMINUENDO" ,	MusicSheet::DIR_WEDGE_DIMINUENDO ) ;
			CHKSTR( "CRESCENDO" ,	MusicSheet::DIR_WEDGE_CRESCENDO ) ;
			CHKSTR( "STOP" ,		MusicSheet::DIR_WEDGE_STOP) ;

			CHKSTR("0", MusicSheet::DIR_FINGER_0);
			CHKSTR("1", MusicSheet::DIR_FINGER_1);
			CHKSTR("2", MusicSheet::DIR_FINGER_2);
			CHKSTR("3", MusicSheet::DIR_FINGER_3);
			CHKSTR("4", MusicSheet::DIR_FINGER_4);
			CHKSTR("5", MusicSheet::DIR_FINGER_5);

			CHKSTR("I", MusicSheet::DIR_FINGER_I);
			CHKSTR("M", MusicSheet::DIR_FINGER_M);
			CHKSTR("A", MusicSheet::DIR_FINGER_A);
			CHKSTR("P", MusicSheet::DIR_FINGER_P);
		}

		return MusicSheet::DIR_UNKNWON ;
	}

	// Parses an xml input.
	bool	ParsXML(CString FileName , MusicSheet & Sheet );

	// Returns the direction type of a node and the staffs the will receive it.
	int		CMusixXMLParser::GetDirectionType(TinyXML2::XMLElement * pNode , MusicSheet::Direction & Dir ) ;
};

