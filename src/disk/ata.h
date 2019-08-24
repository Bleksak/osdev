#pragma once
#include "partition.h"
#include <stdbool.h>

#include "../result.h"

#include "../attributes.h"

struct ata_registers
{
    unsigned short device_control;
    unsigned short alternate_status;
    unsigned short data;
    unsigned short drive_address;
    unsigned short error;
    unsigned short features;
    unsigned short sector_count;
    unsigned short lba_low;
    unsigned short lba_mid;
    unsigned short lba_high;
    unsigned short drive;
    unsigned short status;
    unsigned short command;
};

enum ATA_TYPE
{
	ATA_NONE = 0,
	ATA_UNKNOWN = 1,
    ATA_SATA = 2,
    ATA_PATA = 3,
    ATA_SATAPI = 4,
    ATA_PATAPI = 5,
    ATA_INVALID,
};

struct ATA_Identify
{
    struct PACKED
    {
        unsigned short ATADevice : 1;
        unsigned short useless1 : 12;
        unsigned short IncompleteResponse : 1;
        unsigned short useless2 : 2;
    } GeneralConfiguration;

    unsigned short useless1;
    unsigned short SpecificConfiguration;
    unsigned short useless2[7];

    unsigned char SerialNumber[20];
    
    unsigned short useless3[3];

    unsigned short FirmwareRevision[4];
    unsigned char ModelNumber[40];

    unsigned short MaximumTransferPerDRQBlock;

    unsigned short TrustedComputerFeatureSet;
    
    struct
    {
        unsigned short useless1 : 2;
        unsigned short StandbyTimer : 1;
        unsigned short useless2 : 1;
        unsigned short IORDYSupport : 1;
        unsigned short IORDYDisabled : 1;
        unsigned short LBASupport : 1;
        unsigned short DMASupport : 1;
        unsigned short useless3 : 6;
        unsigned short LongPhysicalSectorAlignmentError : 2;

        unsigned short useless4 : 15;
        unsigned short MinimumStandbyValue : 1;
    } PACKED Capabilities1;

    unsigned short useless5[2];

    struct
    {
        unsigned short useless1 : 14;
        unsigned short UltraDMAValid : 1;
        unsigned short Shorts64to70Valid : 1;
    } PACKED Valid1;

    unsigned short useless6[5];

    struct
    {
        unsigned short BLOCK_ERASE_EXT : 1;
        unsigned short OVERWRITE_EXT : 1;
        unsigned short CRYPTO_SCRAMBLE_EXT : 1;

        unsigned short SanitizeFeatureSet : 1;
        unsigned short SanitizeStandardized : 1;

        unsigned short SANITIZE_ANTIFREEZE_LOCK_EXT : 1;
        unsigned short useless1 : 1;
        
        unsigned short MultipleLogicalSectorSettingValid : 1;
        unsigned short SectorsPerDRQSetting : 8;
    } PACKED Supported1;

    unsigned int SectorCountLow;
    
    unsigned short useless7;

    struct
    {
        unsigned short useless1 : 5;
        unsigned short Mode2Selected : 1;
        unsigned short Mode1Selected : 1;
        unsigned short Mode0Selected : 1;
        unsigned short useless2 : 5;
        unsigned short Mode2Supported : 1;
        unsigned short Mode1Supported : 1;
        unsigned short Mode0Supported : 1;
    } PACKED MultiwordDMA;

    unsigned short useless8 : 14;
    unsigned char PIO3_4Supported : 2;

    unsigned short MinimumMultiwordDMATransferCycleTimePerWord;
    unsigned short RecommendedMultiwordDMATransferCycleTime;
    unsigned short MinimumPIOTransferCycleTime_NOFLOW;
    unsigned short MinimumPIOTransferCycleTime_IORDY;

    struct
    {
        unsigned short reservedForCFA : 1;
        unsigned short DeterministicDataInTrimmedLBA : 1;
        unsigned short LongPhysSectorAlignmentErrorReportControl : 1;
        unsigned short useless2 : 1;
        unsigned short READ_BUFFER_DMA : 1;
        unsigned short WRITE_BUFFER_DMA : 1;
        unsigned short useless3 : 1;
        unsigned short DOWNLOAD_MICROCODE_DMA : 1;
        unsigned short useless4 : 1;
        unsigned short OptionalATADevice28bitCommands : 1;
        unsigned short TrimmedLBARangeReturnZeroedData : 1;
        unsigned short DeviceEncryption : 1;
        unsigned short ExtendedUserAddressableSectors : 1;
        unsigned short WriteCacheNonVolatile : 1;
        unsigned short useless5 : 2;
    } PACKED Supported2;

    unsigned short useless9[5];
    
    struct
    {
        unsigned short useless : 11;
        unsigned short MaxQueueDepth : 5; // minus 1
    } PACKED QueueDepth;

    struct 
    {
        unsigned short READ_LOG_DMA_EXT : 1; // if is equal to READ_LOG_EXT
        unsigned short DeviceAutomaticPartialToSlumberTransition : 1;
        unsigned short HostAutomaticPartialToSlumberTransition : 1;
        unsigned short NCQPriorityInformation : 1;
        unsigned short UnloadWhileNCQOutsanding : 1;
        unsigned short PhyEventCountersLog : 1;
        unsigned short HostInitiatedPowerManagementRequests : 1;
        unsigned short NCQFeatureSet : 1;
        unsigned short useless1 : 4;
        unsigned short SATAGen3Speed : 1; // 6.0Gb/s
        unsigned short SATAGen2Speed : 1; // 3.0Gb/s
        unsigned short SATAGen1Speed : 1; // 1.5Gb/s
        unsigned short zero : 1;
    } PACKED SATA_Capabilities1;

    struct 
    {
        unsigned short useless1 : 9;
        unsigned short RECEIVE_SEND_FPDMA_QUEUED : 1;
        unsigned short NCQueueManagementCommand : 1;
        unsigned short NCQStreaming : 1;
        unsigned short CurrentNegotiatedSATASpeed : 3;
        unsigned short zero : 1;
    } PACKED SATA_Capabilities2;

    struct
    {
        unsigned short useless1 : 8;
        unsigned short NCQAutosense : 1;
        unsigned short SoftwareSettingsPreservation : 1;
        unsigned short HardwareFeatureControl :1;
        unsigned short InOrderDataDelivery : 1;
        unsigned short InitiatingPowerManagement : 1;
        unsigned short DMASetupAutoActivation : 1;
        unsigned short NonZeroBufferOffsets : 1;
        unsigned short zero : 1;
    } PACKED SATA_Features;

    struct 
    {
        unsigned short useless : 8;
        unsigned short AutomaticPartialToSlumberTransition : 1;
        unsigned short SoftwareSettingsPreservation : 1;
        unsigned short HardwareFeatureControl : 1;
        unsigned short InOrderDataDelivery : 1;
        unsigned short InitiatedPowerManagement : 1;
        unsigned short DMASetupAutoActivation : 1;
        unsigned short NonZeroBufferOffsets : 1;
        unsigned short zero : 1;
    } PACKED SATA_Features_Enabled;

    unsigned short MajorVersionNumber;
    unsigned short MinorVersionNumber;

    struct
    {
        unsigned short useless1 : 1;
        unsigned short NOP : 1;
        unsigned short READ_BUFFER : 1;
        unsigned short WRITE_BUFFER : 1;
        unsigned short useless2 : 2;
        unsigned short zero1 : 1;
        unsigned short useless3 : 2;
        unsigned short ReadLookAhead : 1;
        unsigned short VolatileWriteCache : 1;
        unsigned short zero2 : 1;
        unsigned short one : 1;
        unsigned short useless4 : 1;
        unsigned short SecurityFeatureSet : 1;
        unsigned short SMARTFeatureSet : 1;
    } PACKED Supported3;

    struct 
    {
        unsigned short zero : 1;
        unsigned short one : 1;
        unsigned short FLUSH_CACHE_EXT : 1;
        unsigned short FLUSH_CACHE : 1;
        unsigned short useless1 : 1;
        unsigned short AdressFeatureSet48 : 1;
        unsigned short useless2 : 3;
        unsigned short SET_FEATURES_required : 1;
        unsigned short PUIS : 1;
        unsigned short useless3 : 1;
        unsigned short APMFeatureSet : 1;
        unsigned short useless4 : 2;
        unsigned short DOWNLOAD_MICROCODE : 1;
    } PACKED Supported4;

    struct 
    {
        unsigned short zero : 1;
        unsigned short one : 1;
        unsigned short IDLE_IMMIEDIATE_UNLOAD : 1;
        unsigned short useless1 : 4;
        unsigned short WorldWideName : 1;
        unsigned short useless2 : 1;
        unsigned short WRITE_DMA_FUA_EXT : 1; // WRITE_MULTIPLE_FUA_EXT
        unsigned short GPL_FeatureSet : 1;
        unsigned short StreamingFeatureSet : 1;
        unsigned short useless3 : 2;
        unsigned short SMARTSelfTest : 1;
        unsigned short SMARTErrorLogging : 1;
    } PACKED Supported5;

    struct 
    {
        unsigned short useless : 1;
        unsigned short nop : 1;
        unsigned short READ_BUFFER : 1;
        unsigned short WRITE_BUFFER : 1;
        unsigned short useless2 : 2;
        unsigned short zero : 1;
        unsigned short useless3 : 2;
        unsigned short read_lookahead : 1;
        unsigned short volatile_write_cache : 1;
        unsigned short zero2 : 1;
        unsigned short one : 1;
        unsigned short useless4 : 1;
        unsigned short security : 1;
        unsigned short smart : 1;
    } PACKED Supported6;

   struct
   {
        unsigned short Words119_120Valid : 1;
        unsigned short useless : 1;
        unsigned short FLUSH_CACHE_EXT : 1;
        unsigned short FLUSH_CACHE : 1;
        unsigned short useless2 : 1;
        unsigned short Features48bit : 1;
        unsigned short useless3 : 3;
        unsigned short SET_FEATURES_REQUIRED : 1;
        unsigned short PUIS : 1;
        unsigned short useless4 : 1;
        unsigned short APM : 1;
        unsigned short CFA_reserved : 1;
        unsigned short useless5 : 1;
        unsigned short DOWNLOAD_MICROCODE : 1;
   } PACKED Supported7;

   struct
   {
       unsigned short zero : 1;
       unsigned short one : 1;
       unsigned short IDLE_IMMEDIATE : 1;
       unsigned short useless : 4;
       unsigned short one1 : 1;
       unsigned short useless2 : 1;
       unsigned short WRITE_DMA_FUA_EXT : 1;
       unsigned short GPL_feature_set : 1;
       unsigned short useless3 : 2;
       unsigned short MediaSerialNumberValid : 1;
       unsigned short SMART_SelfTest : 1;
       unsigned short SMART_ErrorLog : 1;
   } PACKED Supported8;

    struct 
    {
        unsigned short reserved : 1;
        unsigned short mode6_selected : 1;
        unsigned short mode5_selected : 1;
        unsigned short mode4_selected : 1;
        unsigned short mode3_selected : 1;
        unsigned short mode2_selected : 1;
        unsigned short mode1_selected : 1;
        unsigned short mode0_selected : 1;
        unsigned short _reserved : 1;
        unsigned short mode6_supported : 1;
        unsigned short mode5_supported : 1;
        unsigned short mode4_supported : 1;
        unsigned short mode3_supported : 1;
        unsigned short mode2_supported : 1;
        unsigned short mode1_supported : 1;
        unsigned short mode0_supported : 1;
    } PACKED UltraDMAModes;

    unsigned int ExtendedTime;
    unsigned char reserved2115;
    unsigned char CurrentAPMValue;
    unsigned short MasterPasswordIdentifier;
    unsigned short HardwareResetResults;
    unsigned short useless3489573495;
    unsigned short StreamMinimumRequestSize;
    unsigned short DMAStreamingTransferTime;
    unsigned short StreamingAccessLatency;
    unsigned int StreamingPerformanceGranularity;
    unsigned long long SectorCount;
    unsigned short PIOStreamingTransferTime;
    unsigned short DataSetManagementBlocks;
    struct
    {
        unsigned short zero : 1;
        unsigned short one : 1;
        unsigned short MultipleLogicalPerPhysical : 1;
        unsigned short LogicalSectorLongerThan256Words : 1;
        unsigned short reserved : 8;
        unsigned short LogicalSectorsPerPhysical : 4;
    } PACKED PhysicalLogicalSectorSize;

    unsigned short InterSeekDelay;
    unsigned char WorldWideName[8];
    unsigned long long reserved23875;
    unsigned short obsolete2435;
    unsigned int LogicalSectorSize;

    unsigned int supported_some;
    unsigned char reserved98345[12];
    unsigned short obsolete436387;
    unsigned short SecurityStatus;
    unsigned short VendorSpecific[31];
    unsigned short CFAReserved[8];
    unsigned short DeviceNominalNormFactor;
    unsigned short DataSetManagementSupport;
    unsigned long long AdditionalProductIdentifier;
    unsigned int reserved98435049;
    unsigned short CurrentMediaSerialNumber[30];
    unsigned short SCTCommandTransport;
    unsigned int reserved92835;
    
    struct 
    {
        unsigned short zero : 1;
        unsigned short one : 1;
        unsigned short offset : 14;
    } PACKED LogicalSectorAlignment;

    unsigned int WriteReadVerifySectorMode3Count;
    unsigned int WriteReadVerifySectorMode2Count;
    unsigned short obsolete9348[3];
    unsigned short NominalMediaRotationRate;
    unsigned short reserved87346[2];
    unsigned short WriteReadVerifyFeatureSetCurrentMode;
    unsigned short reserved983467;
    unsigned short TransportMajorVersionNumber;
    unsigned short TransportMinorVersionNumber;
    unsigned short reserved9347851[6];
    unsigned long long SectorCountExt;
    unsigned short DownloadMicrocodeMinimumBlocks;
    unsigned short DownloadMicrocodeMaximumBlocks;
    unsigned short reserved8347638457[19];
    unsigned short IntegrityWord;
} PACKED;

struct ATA_Registers
{
    unsigned short data,
                   error,
                   features,
                   sector_count,
                   lba_low,
                   lba_mid,
                   lba_high,
                   drive,
                   status,
                   command,
                   alternate_status,
                   device_control,
                   drive_address;
};

struct Drive
{
    unsigned char id;
	enum ATA_TYPE type;
    const bool slave;
    
    const struct ATA_Registers registers;
    struct ATA_Identify identify;
    struct
    {
        unsigned short LogicalSectorMultiplier;
        unsigned short LogicalSectorAlignment;
        unsigned int PhysicalSectorSize;
        unsigned int LowAddressableSectors;
        unsigned long long HighAddressableSectors;
        unsigned long long ExtSectors;
    } data;
};

void ata_init(void);
struct Drive* grab_drive(unsigned char index);

Result ata_read(const struct Drive* drive, unsigned long long lba, unsigned short count);
Result ata_write(const struct Drive* drive, unsigned long long lba, unsigned short count, const unsigned short* buffer);
