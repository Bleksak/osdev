#pragma once

#include "attributes.h"

struct BiosDataArea
{
    unsigned char padding[0x400];

    unsigned short COMptr[4];
    unsigned short LPTptr[3];
    unsigned short ExtendedBDAPtr;

    struct PACKED
    {
        /*
            maybe reverse the order, not sure
        */

        unsigned char ParallelPorts : 2;
        /*
            00b => 1 parallel port
            01 => 2 parallel ports,
            11 => 3 parallel ports,
        */
        unsigned char _reserved1 : 2;

        unsigned char SerialPorts : 3;
        /*
            000 => none
            001 => 1
            002 => 2
            003 => 3
            004 => 4
        */

        unsigned char _reserved2 : 1;

        unsigned char FloppyDrives : 2;
        /*
            00 => 1
            01 => 2
        */

       unsigned char VideoMode : 2;
       /*
            0 => EGA or later
            01 => color 40x25
            10 => color 80x25
            11 => monochrome 80x25
       */
       unsigned char reserved3 : 1;
       unsigned char PS2Mouse : 1; // 1 => installed, 0 => none
       unsigned char MathCoprocessor : 1;
       unsigned char BootFloppy : 1;
    } Equipment;

    unsigned char InterruptFlag;
    unsigned short MemorySize; // in Kb
    unsigned short unused1;

    struct PACKED
    {
        unsigned char Insert : 1;
        unsigned char CapsLock : 1;
        unsigned char NumLock : 1;
        unsigned char ScrollLock : 1;
        unsigned char AltKey : 1; // 0 => up, 1 => down
        unsigned char Control : 1;
        unsigned char LeftShift : 1;
        unsigned char RightShift : 1;
    } KeyboardShiftFlags;

    struct PACKED
    {
        unsigned char Insert : 1; // up or down
        unsigned char CapsLock : 1;
        unsigned char NumLock : 1;
        unsigned char ScrollLock : 1;
        unsigned char PauseKey : 1; // active or inactive
        unsigned char SysRegKey : 1;
        unsigned char LeftAlt : 1;
        unsigned char RightAlt : 1;
    } KeyboardShiftFlags2;

    unsigned char AltNumpadWorkArea;
    
    struct PACKED
    {
        unsigned short NextCharPtr;
        unsigned short LastCharPtr;
        unsigned char Buffer[32];
    } KeyboardBuffer;

    unsigned char unused2[11]; //floppy stuff

    struct PACKED
    {
        unsigned char ActiveModeSetting;
        unsigned short TextColumnsPerRow;
        unsigned short ActiveVideoSize; // in page bytes (not sure what it means)
        unsigned short VideoPageOffset;
        unsigned short CursorPositions[8];
        unsigned short CursorShape;
        unsigned char ActivePage;
        unsigned short DisplayAdapterIOPort;
        
        struct PACKED
        {
            unsigned char unused1 : 2;
            unsigned char Controls : 1; // 0 background intensity, 1 blinking
            unsigned char GraphicsOperation : 1;
            unsigned char VideoSignal : 1; // enabled/disabled
            unsigned char ColorOperation : 1; // 0 => color, 1 => monochrome
            unsigned char unused2 : 2;
        } InternalModeRegister;
        
        unsigned char ColorPalette;
        unsigned short AdapterROMOffset;
        unsigned short AdapterROMSegment;
    } VideoMode;

    unsigned char LastInterrupt;
    unsigned int UselessCounter;
    unsigned char Timer24HourFlag;
    unsigned char KeyboardCtrlBreakFlag;
    unsigned short SoftResetFlag;
    
    struct PACKED
    {
        unsigned char LastOperationStatus;
        unsigned char HardDriveCount;
        unsigned char DriveControlByte;
        unsigned char DriveIOPortOffset;
    } Drives;

    unsigned char unused3[12];

    unsigned char VideoRowsCount; // minus 1
    unsigned char unused4[19];

    struct PACKED
    {
        unsigned int FlagPtr; //Segment:Offset
        unsigned int Count;
        unsigned char Flag;
    } UserWait;

    unsigned char LANBytes[7];
    unsigned int VideoParameterControlBlockPtr; //Segment:Offset
    unsigned char _reserved3[68];
    unsigned char CommunicationsArea[16];
} PACKED;

const struct BiosDataArea* bda_get(void);

void bda_init(void);
