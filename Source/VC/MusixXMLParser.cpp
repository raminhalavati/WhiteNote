#include "StdAfx.h"
#include "MusixXMLParser.h"

typedef	TinyXML2::XMLElement XMLElem ;

CMusixXMLParser::CMusixXMLParser(void)
{
}

CMusixXMLParser::~CMusixXMLParser(void)
{
}

#define ALL_NODES1( pVar , pParent , Text1 )							( XMLElem * pVar = GetXMLNestedElement( pParent , Text1 ) ;							pVar ;	pVar = pVar##->NextSiblingElement( Text1 ) )
#define ALL_NODES2( pVar , pParent , Text1 , Text2 )					( XMLElem * pVar = GetXMLNestedElement( pParent , Text1 , Text2 ) ;					pVar ;	pVar = pVar##->NextSiblingElement( Text2 ) )
#define ALL_NODES3( pVar , pParent , Text1 , Text2 , Text3 )			( XMLElem * pVar = GetXMLNestedElement( pParent , Text1 , Text2 , Text3 ) ;			pVar ;	pVar = pVar##->NextSiblingElement( Text3 ) )
#define ALL_NODES4( pVar , pParent , Text1 , Text2 , Text3 , Text4 )	( XMLElem * pVar = GetXMLNestedElement( pParent , Text1 , Text2 , Text3 , Text4 ) ;	pVar ;	pVar = pVar##->NextSiblingElement( Text4 ) )

int	_safe_atoi( const char * pchText )
{
	if ( pchText && pchText[ 0 ] )
		return Round( atof( pchText ) ) ;
	else
		return -1 ;
}

char _safe_first_upper( const char * pchText )
{
	if ( pchText )
		return toupper( pchText[ 0 ] ) ;
	else
		return 0 ;
}

CStringA	NodeToText( XMLElem * pNode )
{
	CStringA	Text = "[" ;

	Text += pNode->Name() ;

	const TinyXML2::XMLAttribute * pAttr = pNode->FirstAttribute() ;

	if ( pAttr )
	{
		Text += " " ;
		Text += pAttr->Name() ;
		Text += "=" ;
		Text += pAttr->Value() ;
		Text += " " ;
	}

	Text += "]" ;

	return Text ;
}

void	ReportError( XMLElem * pNode , CString Title )
{
	CStringA	Text( "" ) ;

	XMLElem * pChild = pNode->FirstChildElement() ;

	while ( pNode )
	{
	
		if ( Text.GetLength() )
			Text = NodeToText( pNode ) + CStringA( "->\r\n" ) + Text ;
		else
			Text = CStringA( "{" ) + NodeToText( pNode ) + CStringA( "}" ) ;

		if ( pNode->Parent() )
			pNode = pNode->Parent()->ToElement() ;
		else
			break ;
	}

	while( pChild )
	{
		Text = Text + CStringA( "->\r\n" ) + NodeToText( pChild ) ;

		pChild = pChild->FirstChildElement() ;
	}
	
	MessageBox( NULL , CA2W( Text ) , Title , MB_ICONERROR ) ;
}

// Parses an xml input.
bool CMusixXMLParser::ParsXML(CString FileName , MusicSheet & Sheet )
{
	try
	{
		TinyXML2::XMLDocument	Doc ;

		if ( Doc.LoadFile( CW2A( FileName ) ) )
			return false ;

		for ALL_NODES2( pCredit , & Doc , "score-partwise" , "credit" )
		{		
			XMLElem *	pWords = GetXMLNestedElement( pCredit , "credit-words" ) ;

			if ( pWords && pWords->GetText() && strlen( pWords->GetText() ) )
				Sheet.Credits.push_back( CStringA( pWords->GetText() ) )  ;
		}

		// Get Parts Count and Names
		for ALL_NODES3( pPart  , & Doc , "score-partwise" , "part-list" , "score-part" )
		{
			XMLElem *	pName = GetXMLNestedElement( pPart , "part-name" ) ;

			if ( pName )
			{
				MusicSheet::Part	NewPart ;

				Sheet.Parts.push_back( NewPart ) ;
				Sheet.Parts.back().Name = pName->GetText() ;
			}
		}

		// Read All Parts
		int	iPartNo = 0 ;

		for ALL_NODES2( pPart , & Doc , "score-partwise" , "part" )
		{
			if ( iPartNo >= ( int ) Sheet.Parts.size() )
			{	
				AfxMessageBox( L"Abnormal parts count." , MB_ICONERROR ) ;
				return false ;
			}

			MusicSheet::Part *	pCurPart = & Sheet.Parts[ iPartNo ] ;

			// Read All Measures
			for ALL_NODES1( pMeasure , pPart , "measure" )
			{
				pCurPart->Measures.push_back( MusicSheet::Measure() ) ;
				
				MusicSheet::Measure	*	pCurMeasure = & pCurPart->Measures.back() ;
				map<pair<int, int>, int>	SV2V;
				pCurMeasure->iNumber = pCurPart->Measures.size() - 1; //--> atoi(pMeasure->Attribute("number"));

				int	iLastX = 0 ;
				pair<int, int> LastNote = make_pair(0, 0);

				bool	bBackedUp = false ; // A flag indicating that after the last seen note, a backup is issued.

				// Iterate All Nodes
				for ( XMLElem *	pCurNode = pMeasure->FirstChildElement() ;
								pCurNode ; pCurNode = pCurNode->NextSiblingElement() )
					try
					{
						const char * pName = pCurNode->Name() ;

						if ( pName && ! strcmp( pName , "barline" ) ) // Barline
						{
							MusicSheet::BarLine	BL ;

							BL.nType = MusicSheet::BarLine::BL_Unknown ;
							BL.iVoltaNumber = -1 ;

							// Forward/Backward
							if ( GetXMLNestedElement( pCurNode , "repeat" ) )
							{
								pName = GetXMLNestedElement( pCurNode , "repeat" )->Attribute( "direction" ) ;

								if ( pName )
								{
									if ( ! strcmp( pName , "forward" ) )
										BL.nType = MusicSheet::BarLine::BL_FORWARD ;
									else
										if ( ! strcmp( pName , "backward" ) )
											BL.nType = MusicSheet::BarLine::BL_BACKWARD ;
									pCurMeasure->BarLines.push_back( BL ) ;
								}
							}
							
							// Volta
							if ( GetXMLNestedElement( pCurNode , "ending" ) )
							{
								if ( ! strcmp( GetXMLNestedElement( pCurNode , "ending" )->Attribute( "type" ) , "start" ) )
									BL.nType = MusicSheet::BarLine::BL_VOLTA_START ;
								else
								if ( ! strcmp( GetXMLNestedElement( pCurNode , "ending" )->Attribute( "type" ) , "stop" ) )
									BL.nType = MusicSheet::BarLine::BL_VOLTA_END ;

								BL.iVoltaNumber = _safe_atoi( GetXMLNestedElement( pCurNode , "ending" )->Attribute( "number" ) ) ;

								pCurMeasure->BarLines.push_back( BL ) ;
							}

							// Fail-Safe
							if ( BL.nType == MusicSheet::BarLine::BL_Unknown )
							{
								pName = pCurNode->Attribute( "location" ) ;

								if ( pName )
								{
									if ( ! strcmp( pName , "left" ) )
										BL.nType = MusicSheet::BarLine::BL_START ;
									else
										if ( ! strcmp( pName , "right" ) )
											BL.nType = MusicSheet::BarLine::BL_END ;
								}
								else
								{
									if ( GetXMLNestedElement( pCurNode , "bar-style" ) )
									{
										pName = GetXMLNestedElement( pCurNode , "bar-style" )->GetText() ;

										if ( pName && ! strcmp( pName , "light-heavy" ) )
											BL.nType = MusicSheet::BarLine::BL_END ;
									}
								}

								pCurMeasure->BarLines.push_back( BL ) ;
							}
						}

						if ( pName && ! strcmp( pName , "attributes" ) )	// Attribtutes
						{
							MusicSheet::Signatures	Sigs ;

							SETZ( Sigs ) ;

							bool	bValid = false ;

							// Key
							{
								XMLElem *	pKey = GetXMLNestedElement( pCurNode , "key" ) ;

								if ( pKey )
								{
									Sigs.Key.iFifths	= _safe_atoi( GetXMLNestedText( pKey , "fifths" ) ) ;

									const char * pchMode = GetXMLNestedText( pKey , "mode" ) ;

									if ( pchMode )
										strncpy_s( 
											Sigs.Key.Mode ,
											 pchMode ,
											sizeof( Sigs.Key.Mode ) ) ;
									else
										SETZ( Sigs.Key.Mode ) ;

									bValid = true ;
								}
							}

							// Time
							{
								XMLElem *	pTime = GetXMLNestedElement( pCurNode , "time" ) ;

								if ( pTime )
								{
									Sigs.Time.iBeats	= _safe_atoi( GetXMLNestedText( pTime , "beats" ) ) ;
									Sigs.Time.iBeatType	= _safe_atoi( GetXMLNestedText( pTime , "beat-type" ) ) ;

									bValid = true ;
								}
							}

							// Clefs
							for ALL_NODES1( pClef , pCurNode , "clef" )
							{
								int	iClef = _safe_atoi( pClef->Attribute( "number" ) ) ;

								if ( iClef == -1 )	// Seems as if when there is one clef only (as in Guitar), it is not stated.
									iClef = 1 ;

								iClef--;
								while (iClef >= (int)Sigs.Clefs.size())
									Sigs.Clefs.push_back(MusicSheet::Signatures::Clef());
								
								Sigs.Clefs[iClef].Sign = _safe_first_upper(GetXMLNestedText(pClef, "sign"));
								Sigs.Clefs[iClef].iLine = _safe_atoi(GetXMLNestedText(pClef, "line"));

								bValid = true ;
							}							
					
							Sigs.BeforeNote = LastNote;

							if ( bValid )
								pCurMeasure->Signatures.push_back( Sigs ) ;
						}
						else if ( pName && ! strcmp( pName , "direction" ) ) // Directions
						{	
							MusicSheet::Direction	Dir ;

							int		iStaff = GetDirectionType( pCurNode , Dir ) ;

							// If not told by direction type, give it to last staff
							if (iStaff == -1)
							{
								iStaff = 0;
								for each (pair<pair<int, int>, int> SV in SV2V)
									iStaff = max(iStaff, SV.first.first);
							}

							if ( Dir.nType != MusicSheet::DIR_UNKNWON || Dir.Text.GetLength() )
							{
								Dir.iStaff = iStaff ;
								Dir.BeforeNote = LastNote;
								// Check Staff, if it is not a new one and it is different with last staff, find original and keep it.
								if (LastNote.first < (int)pCurMeasure->Voices.size())
									if (pCurMeasure->Voices[LastNote.first].iStaff != iStaff)
									{
										for ALL(pCurMeasure->Voices, pVoice)
											if (pVoice->iStaff == iStaff)
											{
												Dir.BeforeNote.first = VEC_INDEX(pVoice, pCurMeasure->Voices);
												Dir.BeforeNote.second = (int)pVoice->Notes.size();
												break;
											}
									}
									
								pCurMeasure->Directions.push_back( Dir ) ;							
							}
						}
						else if ( pName && ! strcmp( pName , "note" ) ) // Note
						{
							MusicSheet::Note	NewNote ;

							int		iStaff, iVoice ;

							// Get Data
							{
								NewNote.iXPos			= _safe_atoi( pCurNode->Attribute( "default-x" ) ) ;
								NewNote.chAccidental	= _safe_first_upper( GetXMLNestedText( pCurNode , "accidental" ) ) ;
								NewNote.iOctave			= _safe_atoi( GetXMLNestedText( pCurNode , "pitch" , "octave" ) ) ;
								NewNote.Type			= StringToNoteType( GetXMLNestedText( pCurNode , "type" ) ) ;						
								NewNote.chStep			= _safe_first_upper( GetXMLNestedText( pCurNode , "pitch" , "step" ) ) ;
								iStaff					= _safe_atoi( GetXMLNestedText( pCurNode , "staff" ) ) - 1 ;
								iVoice					= _safe_atoi(GetXMLNestedText(pCurNode, "voice")) - 1;
								
								if ( iStaff == -2 )	// Looks as if when there is only one staff (as in Guitar), it is not stated.
									iStaff = 0 ;
								if (iVoice == -2)
									iVoice = 0;

								// Double Accidental
								NewNote.bAccdidentalDouble = 
									( NewNote.chAccidental && 
										strchr( GetXMLNestedText( pCurNode , "accidental" ) , '-' ) ) ;
								
								// Rest?
								if ( ! NewNote.chStep && GetXMLNestedElement( pCurNode , "rest" ) )
									NewNote.chStep = 'R' ;

								if ( GetXMLNestedElement( pCurNode , "chord" ) )
									NewNote.Extras.insert( MusicSheet::NE_CHORD ) ;

								// DOT and DOUBLE_DOT
								if ( GetXMLNestedElement( pCurNode , "dot" ) )
								{
									XMLElem *	pDot = GetXMLNestedElement( pCurNode , "dot" ) ;

									if ( pDot->NextSiblingElement( "dot" ) )
										NewNote.Extras.insert( MusicSheet::NE_DOUBLE_DOT ) ;
									else
										NewNote.Extras.insert( MusicSheet::NE_DOT ) ;
								}									

								if ( GetXMLNestedElement( pCurNode , "grace" ) )
									NewNote.Extras.insert( MusicSheet::NE_GRACE ) ;

								// Notations

								// Slur
								{
									XMLElem *	pSlur = GetXMLNestedElement ( pCurNode , "notations" , "slur" ) ;

									if ( pSlur )
									{
										const char *	pchType = pSlur->Attribute( "type" ) ;

										if ( pchType )
											if ( ! strcmp( pchType , "start" ) )	NewNote.Extras.insert( MusicSheet::NE_SLUR_START ) ;
											else
												if ( ! strcmp( pchType , "stop" ) )		NewNote.Extras.insert( MusicSheet::NE_SLUR_END ) ;
									}
								}

								// Tied
								for ALL_NODES2( pTied , pCurNode , "notations" , "tied"  )
								{
									const char *	pchType = pTied->Attribute( "type" ) ;

									if ( pchType )
										if ( ! strcmp( pchType , "start" ) )	NewNote.Extras.insert( MusicSheet::NE_TIED_START ) ;
										else
											if ( ! strcmp( pchType , "stop" ) )		NewNote.Extras.insert( MusicSheet::NE_TIED_END ) ;									
								}

								if ( GetXMLNestedElement( pCurNode , "notations" , "arpeggiate" ) )
									NewNote.Extras.insert( MusicSheet::NE_ARPEGGIATE ) ;

								if ( GetXMLNestedElement( pCurNode , "notations" , "tuplet" ) )
								{
									XMLElem * pTuplet = GetXMLNestedElement( pCurNode , "notations" , "tuplet" ) ;

									if ( ! strcmp( pTuplet->Attribute( "type" ) , "start" ) )
										NewNote.Extras.insert( MusicSheet::NE_TUPLET ) ;
								}

								if ( GetXMLNestedElement( pCurNode , "notations" , "ornaments" , "turn" ) )
									NewNote.Extras.insert( MusicSheet::NE_GRUPPETTO ) ;

								if ( GetXMLNestedElement( pCurNode , "notations" , "articulations" , "staccato" ) )
									NewNote.Extras.insert( MusicSheet::NE_STACCATO ) ;

								if ( GetXMLNestedElement( pCurNode , "notations" , "articulations" , "staccatissimo" ) )
									NewNote.Extras.insert( MusicSheet::NE_STACCATISSIMO ) ;

								if ( GetXMLNestedElement( pCurNode , "notations" , "articulations" , "accent" ) )
									NewNote.Extras.insert( MusicSheet::NE_ACCENT) ;

								if ( GetXMLNestedElement( pCurNode , "notations" , "articulations" , "strong-accent" ) )
									NewNote.Extras.insert( MusicSheet::NE_STRONG_ACCENT) ;


								if ( GetXMLNestedElement( pCurNode , "notations" , "fermata" ) )
									NewNote.Extras.insert( MusicSheet::NE_FERMATA ) ;
							}

							// Check
							if ( NewNote.Type == MusicSheet::DIR_UNKNWON ||
								 NewNote.chStep == NULL ||
								 iStaff < 0 || 
								 ( NewNote.chStep != 'R' && NewNote.iOctave == -1 ) )
							{
								ReportError( pCurNode , L"Wiered Note" ) ;
								return false ;
							}

							// Add It
							pair<int, int> SV = make_pair(iStaff, iVoice);
							if (SV2V.find(SV) == SV2V.end())
							{
								SV2V.insert(make_pair(SV, pCurMeasure->Voices.size()));
								pCurMeasure->Voices.push_back(MusicSheet::Voice());
								pCurMeasure->Voices.back().iStaff = iStaff;
							}
							LastNote.first = SV2V.find(SV)->second;
							pCurMeasure->Voices[LastNote.first].Notes.push_back(NewNote);
							LastNote.second = pCurMeasure->Voices[LastNote.first].Notes.size();
							bBackedUp = false ;							
						}
						else if ( pName && ! strcmp( pName , "backup" ) ) // backup
							bBackedUp = true ;
					}
					catch(...)
					{
						ReportError( pCurNode , L"Unknown Error" ) ;
						return false ;
					}
			}

			iPartNo++ ;
		}
		
		return true ;
	}
	catch(...)
	{
		return false;
	}
}

// Returns the direction type of a node and the staffs the will receive it.
int		CMusixXMLParser::GetDirectionType( TinyXML2::XMLElement * pNode , MusicSheet::Direction & Dir )
{
	int		iStaff ;

	// Staff
	{
		iStaff = _safe_atoi( GetXMLNestedText( pNode , "staff" ) ) ;

		if ( iStaff != -1 )
			iStaff-- ;
	}

	// Code
	{
		Dir.nType = MusicSheet::DIR_UNKNWON ;
		Dir.Text = "" ;

		// Has words?
		{
			const char * pchWords = GetXMLNestedText( pNode , "direction-type" , "words" ) ;

			if ( ! pchWords && GetXMLNestedText( pNode , "direction-type" , "other-direction" ) )
				pchWords = GetXMLNestedElement( pNode , "direction-type" , "other-direction" )->GetText() ;

			if ( pchWords && pchWords[ 0 ] )
				Dir.nType = StringToDirectionsType( pchWords ) ;

			Dir.Text = pchWords ;
		}

		if ( Dir.nType == MusicSheet::DIR_UNKNWON )
		{
			XMLElem * pType = GetXMLNestedElement( pNode , "direction-type" ) ;
			XMLElem * pChild = pType ? pType->FirstChildElement() : NULL ;

			if ( pType &&  pChild )
			{
				const char * pTypeName = pChild->Name() ;

				// Dynamics
				if ( ! strcmp( pTypeName , "dynamics" ) )
				{
					if ( pChild->FirstChildElement() )
						Dir.nType = StringToDirectionsType( pChild->FirstChildElement()->Name() ) ;
				}
				else if ( ! strcmp( pTypeName , "wedge" ) )
					// Wedges
					Dir.nType = StringToDirectionsType( pChild->Attribute( "type" ) ) ;
			}
		}

		if (Dir.nType == MusicSheet::DIR_UNKNWON)
		{
			// Tempo?
			{
				XMLElem * pChild = GetXMLNestedElement(pNode, "sound");
				if (pChild)
				{
					CStringA Temp = pChild->Attribute("tempo");

					if (Temp != "")
					{
						Dir.nType = MusicSheet::DIR_TEMPO_SPEED;
						Dir.Text = Temp;
					}
				}
			}
		}
	}

	// Placement
	{
		const char * pchPlacement = pNode->Attribute( "placement" ) ;

		Dir.bAbove = false ;

		if ( pchPlacement )
			if ( ! strcmp( pchPlacement , "above" ) )
				Dir.bAbove = true ;
			else
				if ( ! strcmp( pchPlacement , "below" ) )
					Dir.bAbove = false ;
	}

	return iStaff ;
}