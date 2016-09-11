#pragma once
#include <vector>
#include <set>
using namespace std;

typedef struct _MusicSheet
{
	typedef enum { TYPE_WHOLE, TYPE_HALF, TYPE_QUARTER, TYPE_EIGHTH, TYPE_SIXTEENTH, 
		TYPE_32ND, TYPE_64TH, TYPE_128TH, TYPE_256TH, TYPE_UNTOLD, TYPE_UNKNOWN
	}	NoteTypes;

	typedef enum {
		NE_CHORD,
		NE_TUPLET_START,
		//NE_TUPLET_MIDDLE,
		NE_TUPLET_STOP,
		NE_ARPEGGIATE,
		NE_ACCENT,
		NE_STRONG_ACCENT,
		NE_GRUPPETTO,
		NE_DOT,
		NE_DOUBLE_DOT,
		NE_STACCATO,
		NE_STACCATISSIMO,
		NE_GRACE,
		NE_FERMATA,
		NE_SLUR_START,
		NE_SLUR_END,
		NE_TIED_START,
		NE_TIED_END,
		NE_UNKNOWN
	} NoteExtras;

	typedef enum {
		DIR_first_Feeling,
		//DIR_ROMANZE , 
		DIR_DOLCE,
		DIR_MESTO,
		DIR_A_TEMPO,
		DIR_POCO_RIT,
		DIR_POCO_RALL,
		DIR_SEMPRE,
		DIR_ACCEL,
		DIR_TRILL,
		DIR_GRUPPETTO,
		DIR_MORDENT,
		DIR_last_Feeling,

		// Temp Texts are told at the beginning of the measure
		DIR_first_TempoText,
		DIR_ALLEGRO,
		DIR_ALLEGRATO,
		DIR_GRAVE,
		DIR_LARGO,
		DIR_MODERATO,
		DIR_last_TempoText,

		// Dynamics are told before the closeset note in for all staffs
		DIR_first_Dynamic,
		DIR_DYNAMIC_ACCELERANDO,
		DIR_DYNAMIC_P,
		DIR_DYNAMIC_PP,
		DIR_DYNAMIC_PPP,
		DIR_DYNAMIC_MF,
		DIR_DYNAMIC_MP,
		DIR_DYNAMIC_F,
		DIR_DYNAMIC_FF,
		DIR_DYNAMIC_FFF,
		DIR_DYNAMIC_FZ,
		DIR_DYNAMIC_FP,
		DIR_DYNAMIC_SFZ,
		DIR_DYNAMIC_DIM,
		DIR_DYNAMIC_DECRESC,
		DIR_DYNAMIC_CRESC,
		DIR_DYNAMIC_STACCATO,
		DIR_DYNAMIC_LEGATO,
		DIR_last_Dynamic,

		// Wedges are told before the closest note of current staff
		DIR_first_Wedge,
		DIR_WEDGE_DIMINUENDO,
		DIR_WEDGE_STOP,
		DIR_WEDGE_CRESCENDO,
		DIR_last_Wedge,

		DIR_first_Finger,
		DIR_FINGER_0,
		DIR_FINGER_1,
		DIR_FINGER_2,
		DIR_FINGER_3,
		DIR_FINGER_4,
		DIR_FINGER_5,
		DIR_last_Finger,

		DIR_first_guitar_Finger,
		DIR_FINGER_I,
		DIR_FINGER_A,
		DIR_FINGER_M,
		DIR_FINGER_P,
		DIR_last_guitar_Finger,

		DIR_OCTAVE_SHIFT_UP,
		DIR_OCTAVE_SHIFT_DOWN,
		DIR_OCTAVE_SHIFT_STOP,

		DIR_METRONOME,
		DIR_SOUND,
		DIR_UNKNWON
	} DirectionTypes;

	typedef struct _Note
	{
		char		chStep;	// C,D,E,F,G,A,B, R become Do,Re,Me,Fa,Sol,La,Si, Rest
		// Bemole: Flat , Diez: Sharp, Bekar: Natural
		char		chAccidental;			// 0 , Sharp, Flat, Natural
		bool		bAccdidentalDouble;	// For Double Codes
		int			iOctave;
		NoteTypes	Type;
		int			iXPos;		// XPosition in the sheet, used for direction positioning.
    bool    bUnpitched;
		set<NoteExtras> Extras;
	} Note;

	typedef struct _Direction
	{
		int				iStaff;
		int				iVoice; // Optional
		pair<int, int>	BeforeNote;	// Before which notes of which voice it is told? (voice, note)
		bool			bAbove;
		DirectionTypes	nType;
		CStringA		Text;
	} Direction;

	typedef struct _Voice
	{
		int					iStaff;
		vector<Note>		Notes;
	} Voice;

	typedef struct _Signatures
	{
		struct _Key
		{
			int			iFifths;
			bool		bMajor;
		} Key;

		struct _Time
		{
			int		iBeatType, iBeats;	// First tell type, then beats. Choices: 
			// 2-4, 3-4 , 4-4 , 3-8, 6-8
		} Time;

		struct Clef
		{
			CStringA	Sign;
			int			iLine;
		};
		vector<_Signatures::Clef>	Clefs;

		pair<int,int>	BeforeNote;	// Before which notes of which voice it is told? (voice, note)
		bool	bChangedInThisMeasure;
	} Signatures;

	typedef struct _BarLines
	{
		enum	{ BL_FORWARD, BL_START, BL_VOLTA_START, BL_LEFT_HAND_BARS, BL_VOLTA_END, BL_BACKWARD, BL_END, BL_Unknown }		nType;
		int		iVoltaNumber;
	} BarLine;

	typedef struct _Measure
	{
		int					iNumber;
		vector<BarLine>		BarLines;
		vector<Signatures>	Signatures;
		vector<Direction>	Directions;
		vector<Voice>		Voices;

	} Measure;

	typedef struct _Movement
	{
		CStringA		Name;

		//vector<DirectionTypes>	Directions ;

		vector<Measure>	Measures;
	} Movement;

	vector<CStringA>	Credits;
	vector<Movement>		Movements;

	void	Reset()
	{
		Credits.clear();
		Movements.clear();
	}
} MusicSheet;


typedef struct _NarratedMusicSheet
{
	struct Voice
	{
		vector<CStringA>	Text;
		int					iStaff;
		int					iVoice;
		CStringA			Lily; // Text for LilyPond
		CStringA			PendingLilyText;
	};

	struct MeasureText
	{
		vector<Voice>	Voices;	// Strings
	}	;

	struct MovementText
	{
		CString	MovementName;
		vector<MeasureText> Measures;
	};

	CString		Credits;	
	vector<MovementText> Movements;

	void Clear()
	{
		Credits = L"";
		Movements.clear();
	};
} NarratedMusicSheet;