#include "StdAfx.h"
#include "MusixXMLParser.h"


CMusicXMLParser::CMusicXMLParser(void)
{
}

CMusicXMLParser::~CMusicXMLParser(void)
{
}

void	ReportError(XMLElem * pNode, CString Title)
{
	CString	Text;

	XMLElem * pChild = pNode->FirstChildElement();

	while (pNode)
	{
		if (Text.GetLength())
			Text = NodeToText(pNode) + CString(L"->\r\n") + Text;
		else
			Text = CString(L"{") + NodeToText(pNode) + CString(L"}");

		if (pNode->Parent())
			pNode = pNode->Parent()->ToElement();
		else
			break;
	}

	while (pChild)
	{
		Text = Text + CString(L"->\r\n") + NodeToText(pChild);

		pChild = pChild->FirstChildElement();
	}

	MessageBox(NULL, Text, Title, MB_ICONERROR);
}

bool ValidDirection(MusicSheet::Measure* cur_measure, MusicSheet::Direction * dir, pair<int,int>& last_voice_note, int staff=-1, int voice=-1) {
  // If not told by direction type, give it to last staff
  if (staff == -1)
    staff = cur_measure->Voices[last_voice_note.first].iStaff;

  if (dir->nType != MusicSheet::DIR_UNKNWON || dir->Text.GetLength()) {
    dir->iStaff = staff;
    dir->iVoice = voice;
    dir->BeforeNote = last_voice_note;

    // Check Staff, if it is different from last staff, find original and keep it.									
    if (cur_measure->Voices[last_voice_note.first].iStaff != staff) {
      for ALL(cur_measure->Voices, pVoice)
        if (pVoice->iStaff == staff) {
          dir->BeforeNote.first = VEC_INDEX(pVoice, cur_measure->Voices);
          dir->BeforeNote.second = (int)pVoice->Notes.size();
          break;
        }
    }
    return true;
  }
  return false;
}


// Parses an xml input.
bool CMusicXMLParser::ParsXML(CString FileName, MusicSheet & Sheet)
{
	try
	{
		Sheet.Reset();
		tinyxml2::XMLDocument *	pDoc = new tinyxml2::XMLDocument();

		if (pDoc->LoadFile(CW2A(FileName)))
		{
			delete pDoc;
			if (!(pDoc = FixAndLoadXMLFile(FileName)))
				return false;
		}

		// Read Credits
		for ALL_NODES2(pCredit, pDoc, "score-partwise", "credit")
		{
			XMLElem *	pWords = GetXMLNestedElement(pCredit, "credit-words");

			if (pWords && pWords->GetText() && strlen(pWords->GetText()))
				Sheet.Credits.push_back(CString(pWords->GetText()));
		}

		// Get Movements Count and Names
		for ALL_NODES3(pMovement, pDoc, "score-partwise", "part-list", "score-part")
		{
			XMLElem *	pName = GetXMLNestedElement(pMovement, "part-name");

			if (pName)
			{
				MusicSheet::Movement	NewMovement;

				Sheet.Movements.push_back(NewMovement);
				Sheet.Movements.back().Name = pName->GetText();
			}
		}

		// Read All Movements
		int	iMovementNo = 0;

		for ALL_NODES2(pMovement, pDoc, "score-partwise", "part")
		{
			if (iMovementNo >= (int)Sheet.Movements.size())
			{
				AfxMessageBox(L"Abnormal parts count.", MB_ICONERROR);
				delete pDoc;
				return false;
			}

			MusicSheet::Movement *	pCurMovement = &Sheet.Movements[iMovementNo];

			// Iteration 1, find voice to staff assignments
			map<int, pair<int, int>>	V2VS;	// Original Voice ID to Voice Index and Staff Index
			{
				vector<map<int, int>>	Counts;
				map<int, int>			EmptyMap;
				for ALL_NODES1(pMeasure, pMovement, "measure")
					for (XMLElem *	pCurNode = pMeasure->FirstChildElement();
						pCurNode; pCurNode = pCurNode->NextSiblingElement())
						if (CString(pCurNode->Name()) == "note")
						{
							int iStaff = max(0, _safe_atoi(GetXMLNestedText(pCurNode, "staff")) - 1);
							int iVoice = _safe_atoi(GetXMLNestedText(pCurNode, "voice"));

							while ((int)Counts.size() <= iVoice)
								Counts.push_back(EmptyMap);
							if (Counts[iVoice].find(iStaff) == Counts[iVoice].end())
								Counts[iVoice].insert(make_pair(iStaff, 1));
							else
								Counts[iVoice].find(iStaff)->second += 1;
						}
				vector<pair<int, int>> Originals;
				for ALL_INDICES(Counts, v)
					if (Counts[v].size())
					{
						int	iStaff = -1;
						int iNotesCount = 0;
						for ALL(Counts[v], pStaff)
							if (pStaff->second > iNotesCount)
							{
								iNotesCount = pStaff->second;
								iStaff = pStaff->first;
							}
						Originals.push_back(make_pair(v, iStaff));
					}
				// SORT
				for ALL_INDICES(Originals, i)
					for ALL_INDICES_EXCEPT_LAST(Originals, j)
						if (Originals[j].second > Originals[j + 1].second)
							swap(Originals[j], Originals[j + 1]);
				for ALL_INDICES(Originals, i)
					V2VS.insert(make_pair(Originals[i].first, make_pair(i, Originals[i].second)));
			}

			// Read All Measures
			for ALL_NODES1(pMeasure, pMovement, "measure")
			{
				pCurMovement->Measures.push_back(MusicSheet::Measure());
				MusicSheet::Measure	*	pCurMeasure = &pCurMovement->Measures.back();
				pCurMeasure->iNumber = pCurMovement->Measures.size() - 1;

				// Create all voices and set their staff numbers.
				for ALL(V2VS, pVoices)
					pCurMeasure->Voices.push_back(MusicSheet::Voice());
				for ALL(V2VS, pVoices)
					pCurMeasure->Voices[pVoices->second.first].iStaff = pVoices->second.second;

				int	iLastX = 0;
				pair<int, int> LastVoiceNote = make_pair(V2VS.begin()->second.first, 0);

				// Iterate All Nodes
				for (XMLElem *	pCurNode = pMeasure->FirstChildElement();
					pCurNode; pCurNode = pCurNode->NextSiblingElement())
					try
				{
					const char * pName = pCurNode->Name();

					// Barline
					if (pName && !strcmp(pName, "barline"))
					{
						MusicSheet::BarLine	BL;

						BL.nType = MusicSheet::BarLine::BL_Unknown;
						BL.iVoltaNumber = -1;

						// Forward/Backward
						if (GetXMLNestedElement(pCurNode, "repeat"))
						{
							pName = GetXMLNestedElement(pCurNode, "repeat")->Attribute("direction");

							if (pName)
							{
								if (!strcmp(pName, "forward"))
									BL.nType = MusicSheet::BarLine::BL_FORWARD;
								else
									if (!strcmp(pName, "backward"))
										BL.nType = MusicSheet::BarLine::BL_BACKWARD;
								pCurMeasure->BarLines.push_back(BL);
							}
						}

						// Volta
						if (GetXMLNestedElement(pCurNode, "ending"))
						{
							if (!strcmp(GetXMLNestedElement(pCurNode, "ending")->Attribute("type"), "start"))
								BL.nType = MusicSheet::BarLine::BL_VOLTA_START;
							else
								if (!strcmp(GetXMLNestedElement(pCurNode, "ending")->Attribute("type"), "stop"))
									BL.nType = MusicSheet::BarLine::BL_VOLTA_END;

							BL.iVoltaNumber = _safe_atoi(GetXMLNestedElement(pCurNode, "ending")->Attribute("number"));

							pCurMeasure->BarLines.push_back(BL);
						}

						// Fail-Safe
						if (BL.nType == MusicSheet::BarLine::BL_Unknown)
						{
							pName = pCurNode->Attribute("location");

							if (pName)
							{
								if (!strcmp(pName, "left"))
									BL.nType = MusicSheet::BarLine::BL_START;
								else
									if (!strcmp(pName, "right"))
										BL.nType = MusicSheet::BarLine::BL_END;
							}
							else
							{
								if (GetXMLNestedElement(pCurNode, "bar-style"))
								{
									pName = GetXMLNestedElement(pCurNode, "bar-style")->GetText();

									if (pName && !strcmp(pName, "light-heavy"))
										BL.nType = MusicSheet::BarLine::BL_END;
								}
							}
							pCurMeasure->BarLines.push_back(BL);
						}
					}

					// Attribtutes
					if (pName && !strcmp(pName, "attributes"))
					{
						MusicSheet::Signatures	Sigs;

						SETZ(Sigs);

						bool	bValid = false;

						// Key
						{
							XMLElem *	pKey = GetXMLNestedElement(pCurNode, "key");

							if (pKey)
							{
								Sigs.Key.iFifths = _safe_atoi(GetXMLNestedText(pKey, "fifths"));

								const char * pchMode = GetXMLNestedText(pKey, "mode");

								if (pchMode && !strcmp(pchMode, "major"))
									Sigs.Key.bMajor = true;
								else if (pchMode && !strcmp(pchMode, "minor"))
									Sigs.Key.bMajor = false;
								else
								{
									_RPTF1(_CRT_WARN, "Unexpected Key Mode: ", pchMode);
									Sigs.Key.bMajor = true;	// <----- ASSUMED DEFAULT
								}

								bValid = true;
							}
							else
								Sigs.Key.iFifths = -1;
						}

						// Time
						{
							XMLElem *	pTime = GetXMLNestedElement(pCurNode, "time");

							if (pTime)
							{
								Sigs.Time.iBeats = _safe_atoi(GetXMLNestedText(pTime, "beats"));
								Sigs.Time.iBeatType = _safe_atoi(GetXMLNestedText(pTime, "beat-type"));

								bValid = true;
							}
						}

						// Clefs
						for ALL_NODES1(pClef, pCurNode, "clef")
						{
							int	iClef = _safe_atoi(pClef->Attribute("number"));

							if (iClef == -1)	// Seems as if when there is one clef only (as in Guitar), it is not stated.
								iClef = 1;

							iClef--;
							while (iClef >= (int)Sigs.Clefs.size())
								Sigs.Clefs.push_back(MusicSheet::Signatures::Clef());

							Sigs.Clefs[iClef].Sign = GetXMLNestedText(pClef, "sign");
							Sigs.Clefs[iClef].iLine = _safe_atoi(GetXMLNestedText(pClef, "line"));

							bValid = true;
						}

						Sigs.BeforeNote = LastVoiceNote;

						if (bValid)
							pCurMeasure->Signatures.push_back(Sigs);
					}
					else if (pName && !strcmp(pName, "direction")) // Directions
					{
						vector<MusicSheet::Direction>	Dirs;
						pair<int, int> SV = GetDirectionTypes(pCurNode, Dirs);
					
						for ALL(Dirs, pDir)
              if (ValidDirection(pCurMeasure, &*pDir, LastVoiceNote, SV.first, SV.second))
                pCurMeasure->Directions.push_back(*pDir);
          }
					// Note
					else if (pName && !strcmp(pName, "note"))
					{
						MusicSheet::Note	NewNote;

						int		iVoice = LastVoiceNote.first;

						// Get Data
						{
#ifdef _DEBUG
							char Temp[100];
							sprintf_s(Temp, "%i,%i,%s,%s\n", iMovementNo, pCurMeasure->iNumber,
								pCurNode->Attribute("default-x"),
								pCurNode->Attribute("default-y"));
							afxDump << Temp;
#endif
							NewNote.iXPos = _safe_atoi(pCurNode->Attribute("default-x"));
							NewNote.iOctave = _safe_atoi(GetXMLNestedText(pCurNode, "pitch", "octave"));
							NewNote.Type = StringToNoteType(GetXMLNestedText(pCurNode, "type"));
							NewNote.chStep = _safe_first_upper(GetXMLNestedText(pCurNode, "pitch", "step"));
							iVoice = V2VS.find(_safe_atoi(GetXMLNestedText(pCurNode, "voice")))->second.first;
							NewNote.bUnpitched = false;

							// Unpitched?
							if (!NewNote.chStep)
							{
								NewNote.iOctave = _safe_atoi(GetXMLNestedText(pCurNode, "unpitched", "display-octave"));
								NewNote.chStep = _safe_first_upper(GetXMLNestedText(pCurNode, "unpitched", "display-step"));
								NewNote.bUnpitched = (NewNote.chStep != NULL);
							}
							// Accidental
							{
								CStringA Accidental(GetXMLNestedText(pCurNode, "accidental"));
								Accidental.MakeLower();
								if (Accidental.GetLength() == 0)
									NewNote.chAccidental = 0;
								else if (Accidental.Find("sharp") != -1)
									NewNote.chAccidental = (Accidental.Find("-") != -1 || Accidental.Find("double") != -1) ? 'S' : 's';
								else if (Accidental.Find("flat") != -1)
									NewNote.chAccidental = (Accidental.Find("-") != -1 || Accidental.Find("double") != -1) ? 'F' : 'f';
								else if (Accidental.Find("natural") != -1)
									NewNote.chAccidental = 'n'; 
								else
									_RPT1(_CRT_ERROR, "Unexpected Accidental: %s", Accidental);

                if (!NewNote.chAccidental) {
                  Accidental = GetXMLNestedText(pCurNode, "pitch", "alter");
                  if (Accidental.GetLength())
                    if (Accidental == "1")
                      NewNote.chAccidental = 's';
                    else if (Accidental == "-1")
                      NewNote.chAccidental = 'f';
                    else if (Accidental == "2")
                      NewNote.chAccidental = 'S';
                    else if (Accidental == "-2")
                      NewNote.chAccidental = 'F';
                    else
                      _RPT1(_CRT_ERROR, "Unexpected Alter Accidental: %s", Accidental);
                }
							}

							// Rest?
							if (!NewNote.chStep && GetXMLNestedElement(pCurNode, "rest"))
								NewNote.chStep = 'R';

							if (GetXMLNestedElement(pCurNode, "chord"))
								NewNote.Extras.insert(MusicSheet::NE_CHORD);

							// DOT and DOUBLE_DOT
							if (GetXMLNestedElement(pCurNode, "dot"))
							{
								XMLElem *	pDot = GetXMLNestedElement(pCurNode, "dot");

								if (pDot->NextSiblingElement("dot"))
									NewNote.Extras.insert(MusicSheet::NE_DOUBLE_DOT);
								else
									NewNote.Extras.insert(MusicSheet::NE_DOT);
							}

							if (GetXMLNestedElement(pCurNode, "grace"))
								NewNote.Extras.insert(MusicSheet::NE_GRACE);

							// Notations
							if (GetXMLNestedElement(pCurNode, "notations"))
								for (XMLElem * pNotation = GetXMLNestedElement(pCurNode, "notations")->FirstChildElement();
									pNotation; pNotation = pNotation->NextSiblingElement())
							{
								CString Name = CA2W(pNotation->Name());

								if (Name == "slur")
								{
									const char *	pchType = pNotation->Attribute("type");

									if (pchType)
										if (!strcmp(pchType, "start"))
											NewNote.Extras.insert(MusicSheet::NE_SLUR_START);
										else if (!strcmp(pchType, "stop"))
											NewNote.Extras.insert(MusicSheet::NE_SLUR_END);
								}
								else if (Name == "tied")
								{
									const char *	pchType = pNotation->Attribute("type");

									if (pchType)
										if (!strcmp(pchType, "start"))
											NewNote.Extras.insert(MusicSheet::NE_TIED_START);
										else if (!strcmp(pchType, "stop"))
											NewNote.Extras.insert(MusicSheet::NE_TIED_END);
								}
								else if (Name == "arpeggiate")
									NewNote.Extras.insert(MusicSheet::NE_ARPEGGIATE);
								else if (Name == "tuplet")
								{
									if (!strcmp(pNotation->Attribute("type"), "start"))
										NewNote.Extras.insert(MusicSheet::NE_TUPLET_START);
									else if (!strcmp(pNotation->Attribute("type"), "stop"))
										NewNote.Extras.insert(MusicSheet::NE_TUPLET_STOP);
								}
								else if (Name == "ornaments")
								{
									if (GetXMLNestedElement(pNotation, "turn"))
										NewNote.Extras.insert(MusicSheet::NE_GRUPPETTO);
								}
								else if (Name == "articulations")
								{
									if (GetXMLNestedElement(pNotation, "staccato"))
										NewNote.Extras.insert(MusicSheet::NE_STACCATO);
									else if (GetXMLNestedElement(pNotation, "staccatissimo"))
										NewNote.Extras.insert(MusicSheet::NE_STACCATISSIMO);
									else if (GetXMLNestedElement(pNotation, "accent"))
										NewNote.Extras.insert(MusicSheet::NE_ACCENT);
									else if (GetXMLNestedElement(pNotation, "strong-accent"))
										NewNote.Extras.insert(MusicSheet::NE_STRONG_ACCENT);
								}
								else if (Name == "fermata")
									NewNote.Extras.insert(MusicSheet::NE_FERMATA);
                else if (Name == "technical") {
                  for (XMLElem * pChild = pNotation->FirstChildElement();
                       pChild; pChild = pChild->NextSiblingElement()) {
                    Name = CA2W(pChild->Name());
                    if (Name == "fingering") {
                      MusicSheet::DirectionTypes finger = StringToDirectionsType(pChild->GetText());
                      if (finger != MusicSheet::DIR_UNKNWON)
                        NewNote.Fingers.push_back(finger);
                      else
                        _RPTF1(_CRT_ERROR, "Unexpected technical-fingering tag %s", Name);
                    }
                    else {
                      _RPTF1(_CRT_ERROR, "Unexpected technical tag %s", Name);
                    }
                  }
                }
                else {
                  _RPTF1(_CRT_ERROR, "Unexpected notation tag %s", Name);
                }
							}
						}

						// Check
						if (NewNote.Type == MusicSheet::DIR_UNKNWON ||
							NewNote.chStep == NULL ||
							(NewNote.chStep != 'R' && NewNote.iOctave == -1))
						{
							ReportError(pCurNode, L"Wiered Note");
							delete pDoc;
							return false;
						}

						// Add It
						pCurMeasure->Voices[iVoice].Notes.push_back(NewNote);

            LastVoiceNote.first = iVoice;
						LastVoiceNote.second = pCurMeasure->Voices[iVoice].Notes.size();
					}
					else if (pName && !strcmp(pName, "backup"))
					{
					} // backup
				}
				catch (...)
				{
					ReportError(pCurNode, L"Unknown Error");
					delete pDoc;
					return false;
				}
			}

			iMovementNo++;
		}

		delete pDoc;
		return true;
	}
	catch (...)
	{
		return false;
	}
}

// Returns the direction type of a node and its (staff,voice)
pair<int, int>	CMusicXMLParser::GetDirectionTypes(tinyxml2::XMLElement * pNode, vector<MusicSheet::Direction> & Directions)
{
	int		iStaff = -1;
	int		iVoice = -1;
	bool	bPlacementAbove = false;


	// Placement
	{
		const char * pchPlacement = pNode->Attribute("placement");

		if (pchPlacement)
			if (!strcmp(pchPlacement, "above"))
				bPlacementAbove = true;
			else if (!strcmp(pchPlacement, "below"))
				bPlacementAbove = false;
	}

	// Iterate all directions
	for (XMLElem * pCurNode = pNode->FirstChildElement(); pCurNode; pCurNode = pCurNode->NextSiblingElement())
	{
		MusicSheet::Direction Dir;
		Dir.nType = MusicSheet::DIR_UNKNWON;
		Dir.Text = pCurNode->Value();

		// Staff
		if (Dir.Text == "staff")
		{
			iStaff = _safe_atoi(pCurNode->GetText());

			if (iStaff != -1)
				iStaff--;
			continue;
		}
		else if (Dir.Text == "voice")
		{
			iVoice = _safe_atoi(pCurNode->GetText());
			continue;
		}
		else if (Dir.Text == "direction-type")
		{
			// Has words?
			{
				const char * pchWords = GetXMLNestedText(pCurNode, "words");

				if (!pchWords && GetXMLNestedText(pCurNode, "direction-type", "other-direction"))
					pchWords = GetXMLNestedElement(pCurNode, "direction-type", "other-direction")->GetText();

				if (pchWords && pchWords[0])
					Dir.nType = StringToDirectionsType(pchWords);

				Dir.Text = pchWords;
			}
			// Metronome?
			if (Dir.nType == MusicSheet::DIR_UNKNWON && GetXMLNestedElement(pCurNode, "metronome"))
			{
				Dir.nType = MusicSheet::DIR_METRONOME;
				Dir.Text.Format(L"%S|%S",
					GetXMLNestedText(pCurNode, "metronome", "beat-unit"),
					GetXMLNestedText(pCurNode, "metronome", "per-minute"));
			}
			if (Dir.nType == MusicSheet::DIR_UNKNWON)
			{
				XMLElem * pChild = pCurNode->FirstChildElement();

				if (pChild)
				{
					const char * pTypeName = pChild->Name();

					// Dynamics
					if (!strcmp(pTypeName, "dynamics"))
					{
						if (pChild->FirstChildElement())
							Dir.nType = StringToDirectionsType(pChild->FirstChildElement()->Name());
					}
					else if (!strcmp(pTypeName, "wedge"))
						// Wedges
						Dir.nType = StringToDirectionsType(pChild->Attribute("type"));
					else if (!strcmp(pTypeName, "octave-shift"))
					{
						const char * pTypeName = pChild->Attribute("type");
						if (!strcmp(pTypeName, "stop"))
							Dir.nType = MusicSheet::DIR_OCTAVE_SHIFT_STOP;
						else if (!strcmp(pTypeName, "down"))
							Dir.nType = MusicSheet::DIR_OCTAVE_SHIFT_DOWN;
						else if (!strcmp(pTypeName, "up"))
							Dir.nType = MusicSheet::DIR_OCTAVE_SHIFT_UP;
						else
						{
							_RPTF1(_CRT_ERROR, "Unexpected direction type: %s", Dir.Text);
							continue;
						}
					}
				}
			}
		}
		else if (Dir.Text == "sound")
		{
			Dir.nType = MusicSheet::DIR_SOUND;
			pCurNode->Attribute("tempo");

			Dir.Text.Format(L"%S|%S", pCurNode->Attribute("tempo"), pCurNode->Attribute("dynamics"));
		}
		else
			if (Dir.Text == "offset")
				continue;
			else
			{
				_RPTF1(_CRT_ERROR, "Unexpected direction type: %s", Dir.Text);
				continue;
			}

		Directions.push_back(Dir);
	}

	return make_pair(iStaff, iVoice);
}

// Tries to fix and load XML File
tinyxml2::XMLDocument* CMusicXMLParser::FixAndLoadXMLFile(CString FileName)
{
	try
	{
		tinyxml2::XMLDocument * pDoc = NULL;

		bool bParsed = false;
		// STAGE 1: If it's UTF-16, convert it to UTF-8
		char * pchBuff = NULL;
		{
			FILE * hFile;
			if (_wfopen_s(&hFile, FileName, L"rt, ccs=UTF-16LE"))
				return NULL;

			// Get File Size
			fseek(hFile, 0, SEEK_END);
			int iFileSize = ftell(hFile);
			fseek(hFile, 0, SEEK_SET);

			// Load file as UTF-16
			TCHAR * ptchBuff = new TCHAR[iFileSize / 2 + 2];
			int  iBufferSize = fread(ptchBuff, 2, iFileSize / 2 + 1, hFile);
			ptchBuff[iBufferSize] = 0;
			fclose(hFile);

			pchBuff = new char[iFileSize + 1];
			// Is it UTF-16?
			if (wcsstr(ptchBuff, L"UTF-16"))
			{
				// Get meomry and convert to UTF8
				strcpy_s(pchBuff, iFileSize, CW2A(ptchBuff, CP_UTF8));
				char * pchPos = strstr(pchBuff, "UTF-16");
				memcpy_s(pchPos, 10, "UTF-8  ", 7);
				pchPos[5] = pchPos[-1];
			}
			else
			{
				// Reload as UTF-8
				_wfopen_s(&hFile, FileName, L"rt");
				iBufferSize = fread(pchBuff, 1, iFileSize + 1, hFile);
				fclose(hFile);
			}

			delete ptchBuff;
		}

		// If can't be parsed, search for extra declerations.
		pDoc = new tinyxml2::XMLDocument;
		if (pDoc->Parse(pchBuff))
		{
			delete pDoc;
			char * pchPos = pchBuff + 10;
			while (true)
			{
				pchPos = strstr(pchPos, "<?");
				if (!pchPos)
					break;
				char *pchEndPos = strstr(pchPos, "?>");
				if (!pchEndPos)
					break;
				memset(pchPos, 32, pchEndPos - pchPos + 2);
				pchPos = pchEndPos;
			}

			pDoc = new tinyxml2::XMLDocument;
			if (pDoc->Parse(pchBuff))
			{
				delete pDoc;
				pDoc = NULL;
			}
		}

		if (pchBuff)
			delete pchBuff;
		return pDoc;
	}
	catch (...)
	{
		return NULL;
	}
}
