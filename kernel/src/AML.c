#include "AML.h"
#include "console.h"
#include "string.h"

enum AML_Operations
{
	ZeroOp = 0,
	OneOp = 1,
	AliasOp = 0x6,
	NameOp = 0x8,
	BytePrefix = 0xA,
	WordPrefix = 0xB,
	DWordPrefix = 0xC,
	StringPrefix = 0xD,
	QWordPrefix = 0xE,
	ScopeOp = 0x10,
	BufferOp = 0x11,
	PackageOp = 0x12,
	VarPackageOp = 0x13,
	MethodOp = 0x14,
	DualNamePrefix = 0x2E,
	MultiNamePrefix = 0x2F,
	DigitCharStart = 0x30,
	DigitCharEnd = 0x39,
	NameCharStart = 0x41,
	NameCharEnd = 0x5A,
	ExtOpPrefix = 0x5B,
	RootChar = 0x5C,
	ParentPrefixChar = 0x5E,
	NameChar = 0x5F,
	Local0Op = 0x60,
	Local1Op = 0x61,
	Local2Op = 0x62,
	Local3Op = 0x63,
	Local4Op = 0x64,
	Local5Op = 0x65,
	Local6Op = 0x66,
	Local7Op = 0x67,
	Arg0Op = 0x68,
	Arg1Op = 0x69,
	Arg2Op = 0x6A,
	Arg3Op = 0x6B,
	Arg4Op = 0x6C,
	Arg5Op = 0x6D,
	Arg6Op = 0x6E,
	StoreOp = 0x70,
	RefOfOp = 0x71,
	AddOp = 0x72,
	ConcatOp = 0x73,
	SubtractOp = 0x74,
	IncrementOp = 0x75,
	DecrementOp = 0x76,
	MultiplyOp = 0x77,
	DivideOp = 0x78,
	ShiftLeftOp = 0x79,
	ShiftRightOp = 0x7A,
	AndOp = 0x7B,
	NandOp = 0x7C,
	OrOp = 0x7D,
	NorOp = 0x7E,
	XorOp = 0x7F,
	NotOp = 0x80,
	FindSetLeftBitOp = 0x81,
	FindSetRightBitOp = 0x82,
	DerefOfOp = 0x83,
	ConcatResOp = 0x84,
	ModOp = 0x85,
	NotifyOp = 0x86,
	SizeOfOp = 0x87,
	IndexOp = 0x88,
	MatchOp = 0x89,
	CreateDWordFieldOp = 0x8A,
	CreateWordFieldOp = 0x8B,
	CreateByteFieldOp = 0x8C,
	CreateBitFieldOp = 0x8D,
	TypeOp = 0x8E,
	CreateQWordFieldOp = 0x8F,
	LandOp = 0x90,
	LorOp = 0x91,
	LnotOp = 0x92,
	LNotEqualOp = 0x93,
	LLessEqualOp = 0x94,
	LGreaterEqualOp = 0x95,
	LEqualOp = 0x93,
	LGreaterOp = 0x94,
	LLessOp = 0x95,
	ToBufferOp = 0x96,
	ToDecimalStringOp = 0x97,
	ToHexStringOp = 0x98,
	ToIntegerOp = 0x99,
	ToStringOp = 0x9C,
	CopyObjectOp = 0x9D,
	MidOp = 0x9E,
	ContinueOp = 0x9F,
	IfOp = 0xA0,
	ElseOp = 0xA1,
	WhileOp = 0xA2,
	NoopOp = 0xA3,
	ReturnOp = 0xA4,
	BreakOp = 0xA5,
	BreakPointOp = 0xCC,
	OnesOp = 0xFF,
};

enum AML_Ext_Op
{
	MutexOp = 0x1,
	EventOp = 0x2,
	CondRefOfOp = 0x12,
	CreateFieldOp = 0x13,
	LoadTableOp = 0x1F,
	LoadOp = 0x20,
	StallOp = 0x21,
	SleepOp = 0x22,
	AcquireOp = 0x23,
	SignalOp = 0x24,
	WaitOp = 0x25,
	ResetOp = 0x26,
	ReleaseOp = 0x27,
	FromBCDOp = 0x28,
	ToBCD = 0x29,
	UnloadOp = 0x2A,
	RevisionOp = 0x30,
	DebugOp = 0x31,
	FatalOp = 0x32,
	TimerOp = 0x33,
	OpRegionOp = 0x80,
	FieldOp = 0x81,
	DeviceOpList = 0x82,
	ProcessorOp = 0x83,
	PowerResOp = 0x84,
	ThermalZoneOpList = 0x85,
	IndexFieldOp = 0x86,
	BankFieldOp = 0x87,
	DataRegionOp = 0x88,
};
bool is_name()
{
	return false;
}

char* AML_parse(struct RSDT* rsdt)
{
	uint32_t length = rsdt->length - sizeof(struct RSDT);
	//uint8_t buffer[5000] = {0};
	uint8_t* bytecode = ((void*) (uint32_t)rsdt + sizeof(struct RSDT));

	for(uint32_t i = 0; i<length; ++i)
	{
		switch(bytecode[i])
		{
			case ExtOpPrefix:
			{
				++i;
				continue;
			} break;
			

			default: break;
			
		}
	}

	return 0;
}
