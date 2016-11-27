#pragma once
#include "MusicSheet.h"	
#include "Common.h"

class CMusicXMLParser
{
public:
	CMusicXMLParser(void);
	~CMusicXMLParser(void);

	MusicSheet::NoteTypes	StringToNoteType( const char * pchName )
	{
		if ( pchName )
		{
			CString	Name( pchName ) ;
			Name.MakeUpper() ;

			CHKSTR(L"WHOLE" ,	MusicSheet::TYPE_WHOLE) ;		// گرد
			CHKSTR(L"HALF" ,	MusicSheet::TYPE_HALF ) ;		// سفید
			CHKSTR(L"QUARTER" ,	MusicSheet::TYPE_QUARTER ) ;	// سیاه
			CHKSTR(L"EIGHTH" ,	MusicSheet::TYPE_EIGHTH ) ;		// چنگ
			CHKSTR(L"16TH" ,	MusicSheet::TYPE_SIXTEENTH ) ;	// 
			CHKSTR(L"32ND" ,	MusicSheet::TYPE_32ND ) ;	// 
			CHKSTR(L"64TH",		MusicSheet::TYPE_64TH);	// 
			CHKSTR(L"128TH",	MusicSheet::TYPE_128TH);	// 
			CHKSTR(L"256TH",	MusicSheet::TYPE_256TH);	// 

		}
		else
			return MusicSheet::TYPE_UNTOLD ;

		return MusicSheet::TYPE_UNKNOWN ;
	}

	MusicSheet::DirectionTypes StringToDirectionsType( const char * pchName )
	{
		if ( pchName )
		{
			CString	Name( pchName ) ;
			Name.MakeUpper() ;

			//CHKSTR(L"ROMANZE" ,		MusicSheet::DIR_ROMANZE ) ;
			CHKSTR(L"DOLCE" ,		MusicSheet::DIR_DOLCE ) ;
			CHKSTR(L"MESTO" ,		MusicSheet::DIR_MESTO ) ;
			CHKSTR(L"A TEMPO" ,		MusicSheet::DIR_A_TEMPO ) ;
			CHKSTR(L"POCO RIT." ,	MusicSheet::DIR_POCO_RIT ) ;
			CHKSTR(L"POCO RALL" ,	MusicSheet::DIR_POCO_RALL ) ;
			CHKSTR(L"SEMPRE" ,		MusicSheet::DIR_SEMPRE ) ;
			CHKSTR(L"ACCEL" ,		MusicSheet::DIR_ACCEL ) ;
			CHKSTR(L"TRILL" ,		MusicSheet::DIR_TRILL ) ;
			CHKSTR(L"GRUPPETTO" ,	MusicSheet::DIR_GRUPPETTO ) ;
			CHKSTR(L"MORDENT" ,		MusicSheet::DIR_MORDENT ) ;
			
			CHKSTR(L"ALLEGRO" ,		MusicSheet::DIR_ALLEGRO) ;
			CHKSTR(L"ALLEGRATO" ,	MusicSheet::DIR_ALLEGRATO) ;
			CHKSTR(L"GRAVE" ,		MusicSheet::DIR_GRAVE) ;
			CHKSTR(L"LARGO" ,		MusicSheet::DIR_LARGO) ;
			CHKSTR(L"MODERATO" ,	MusicSheet::DIR_MODERATO ) ;
						
			CHKSTR(L"ACCEL." ,		MusicSheet::DIR_DYNAMIC_ACCELERANDO  ) ;
			CHKSTR(L"P" ,			MusicSheet::DIR_DYNAMIC_P ) ;
			CHKSTR(L"PP" ,			MusicSheet::DIR_DYNAMIC_PP) ;
			CHKSTR(L"PPP" ,			MusicSheet::DIR_DYNAMIC_PPP) ;
			CHKSTR(L"MP" ,			MusicSheet::DIR_DYNAMIC_MP) ;
			CHKSTR(L"MF" ,			MusicSheet::DIR_DYNAMIC_MF) ;
			CHKSTR(L"F" ,			MusicSheet::DIR_DYNAMIC_F) ;
			CHKSTR(L"FF" ,			MusicSheet::DIR_DYNAMIC_FF ) ;
			CHKSTR(L"FFF",			MusicSheet::DIR_DYNAMIC_FFF);
			CHKSTR(L"FZ" ,			MusicSheet::DIR_DYNAMIC_FZ) ;
			CHKSTR(L"FP",			MusicSheet::DIR_DYNAMIC_FP);
			CHKSTR(L"SF",			MusicSheet::DIR_DYNAMIC_SFZ);
			CHKSTR(L"SFZ" ,			MusicSheet::DIR_DYNAMIC_SFZ) ;
			CHKSTR(L"SFFZ" ,		MusicSheet::DIR_DYNAMIC_SFZ) ;
			CHKSTR(L"DIM" ,			MusicSheet::DIR_DYNAMIC_DIM) ;
			CHKSTR(L"DECRESC" ,		MusicSheet::DIR_DYNAMIC_DECRESC) ;
			CHKSTR(L"CRESC" ,		MusicSheet::DIR_DYNAMIC_CRESC) ;
			CHKSTR(L"STACCATO" ,	MusicSheet::DIR_DYNAMIC_STACCATO) ;
			CHKSTR(L"LEGATO" ,		MusicSheet::DIR_DYNAMIC_LEGATO) ;
							
			CHKSTR(L"DIMINUENDO" ,	MusicSheet::DIR_WEDGE_DIMINUENDO ) ;
			CHKSTR(L"CRESCENDO" ,	MusicSheet::DIR_WEDGE_CRESCENDO ) ;
			CHKSTR(L"STOP" ,		MusicSheet::DIR_WEDGE_STOP) ;

			CHKSTR(L"0", MusicSheet::DIR_FINGER_0);
			CHKSTR(L"1", MusicSheet::DIR_FINGER_1);
			CHKSTR(L"2", MusicSheet::DIR_FINGER_2);
			CHKSTR(L"3", MusicSheet::DIR_FINGER_3);
			CHKSTR(L"4", MusicSheet::DIR_FINGER_4);
			CHKSTR(L"5", MusicSheet::DIR_FINGER_5);

			CHKSTR(L"I", MusicSheet::DIR_FINGER_I);
			CHKSTR(L"M", MusicSheet::DIR_FINGER_M);
			CHKSTR(L"A", MusicSheet::DIR_FINGER_A);
			CHKSTR(L"P", MusicSheet::DIR_FINGER_P);
		}

		return MusicSheet::DIR_UNKNWON ;
	}

	// Parses an xml input.
	bool	ParsXML(CString FileName , MusicSheet & Sheet );

	// Returns the direction type of a node and its (staff,voice)
	pair<int,int>		CMusicXMLParser::GetDirectionTypes(tinyxml2::XMLElement * pNode , vector<MusicSheet::Direction> & Directions ) ;
  // Tries to fix and load XML File
	tinyxml2::XMLDocument * FixAndLoadXMLFile(CString FileName);
};

