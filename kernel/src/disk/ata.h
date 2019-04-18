#pragma once
#include "disk/partition.h"
#include <stdbool.h>

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
    struct GeneralConfiguration
    {
        unsigned char ATADevice : 1;
        unsigned short useless1 : 12;
        unsigned char IncompleteResponse : 1;
        unsigned char useless2 : 2;
    };

    unsigned short useless1;
    unsigned short SpecificConfiguration;
    unsigned short useless2[7];

    unsigned char SerialNumber[20];
    
    unsigned short useless3[2];

    unsigned long long FirmwareRevision[8];
    unsigned char ModelNumber[40];

    unsigned short TrustedComputerFeatureSet;
    
    struct
    {
        unsigned char useless1 : 2;
        unsigned char StandbyTimer : 1;
        unsigned char useless2 : 1;
        unsigned char IORDYSupport : 1;
        unsigned char IORDYDisabled : 1;
        unsigned char LBASupport : 1;
        unsigned char DMASupport : 1;
        unsigned char useless3 : 6;
        unsigned char LongPhysicalSectorAlignmentError :1;

        unsigned short useless4 : 15;
        unsigned char MinimumStandbyValue : 1;
    } Capabilities1;

    unsigned short useless5;

    struct
    {
        unsigned short useless1 : 14;
        unsigned char UltraDMAValid : 1;
        unsigned char Shorts64to70Valid : 1;
    } Valid1;

    unsigned short useless5[5];

    struct
    {
        unsigned char BLOCK_ERASE_EXT : 1;
        unsigned char OVERWRITE_EXT : 1;
        unsigned char CRYPTO_SCRAMBLE_EXT : 1;

        unsigned char SanitizeFeatureSet : 1;
        unsigned char SanitizeStandardized : 1;

        unsigned char SANITIZE_ANTIFREEZE_LOCK_EXT : 1;
        unsigned char useless1 : 1;
        
        unsigned char MultipleLogicalSectorSettingValid : 1;
        unsigned char SectorsPerDRQSetting;
    } Supported1;

    unsigned int AdressableSectors28;
    
    unsigned short useless6;

    struct
    {
        unsigned char useless1 : 5;
        unsigned char Mode2Selected : 1;
        unsigned char Mode1Selected : 1;
        unsigned char Mode0Selected : 1;
        unsigned char useless2 : 5;
        unsigned char Mode2Supported : 1;
        unsigned char Mode1Supported : 1;
        unsigned char Mode0Supported : 1;
    } MultiwordDMA;

    unsigned short useless7 : 14;
    unsigned char PIO3_4Supported : 2;

    unsigned short MinimumMultiwordDMATransferCycleTimePerWord;
    unsigned short RecommendedMultiwordDMATransferCycleTime;
    unsigned short MinimumPIOTransferCycleTime_NOFLOW;
    unsigned short MinimumPIOTransferCycleTime_IORDY;

    struct
    {
        unsigned char useless1 : 1;
        unsigned char DeterministicDataInTrimmedLBA : 1;
        unsigned char LongPhysSectorAlignmentErrorReportControl : 1;
        unsigned char useless2 : 1;
        unsigned char READ_BUFFER_DMA : 1;
        unsigned char WRITE_BUFFER_DMA : 1;
        unsigned char useless3 : 1;
        unsigned char DOWNLOAD_MICROCODE_DMA : 1;
        unsigned char useless4 : 1;
        unsigned char OptionalATADevice28bitCommands :1;
        unsigned char TrimmedLBARangeReturnZeroedData : 1;
        unsigned char DeviceEncryption : 1;
        unsigned char ExtendedUserAddressableSectors;
        unsigned char WriteCacheNonVolatile;
        unsigned char useless5 : 2;
    } Supported2;

    unsigned short useless8[2];
    
    struct
    {
        unsigned short useless : 11;
        unsigned char MaxQueueDepth : 4; // minus 1
    } QueueDepth;

    struct 
    {
        unsigned char READ_LOG_DMA_EXT : 1; // if is equal to READ_LOG_EXT
        unsigned char DeviceAutomaticPartialToSlumberTransition : 1;
        unsigned char HostAutomaticPartialToSlumberTransition : 1;
        unsigned char NCQPriorityInformation : 1;
        unsigned char UnloadWhileNCQOutsanding : 1;
        unsigned char PhyEventCountersLog : 1;
        unsigned char HostInitiatedPowerManagementRequests : 1;
        unsigned char NCQFeatureSet : 1;
        unsigned char useless1 : 4;
        unsigned char SATAGen3Speed : 1; // 6.0Gb/s
        unsigned char SATAGen2Speed : 1; // 3.0Gb/s
        unsigned char SATAGen1Speed : 1; // 1.5Gb/s
        unsigned char zero : 1;
    } SATA_Capabilities1;

    struct 
    {
        unsigned short useless1 : 9;
        unsigned char RECEIVE_SEND_FPDMA_QUEUED : 1;
        unsigned char NCQueueManagementCommand : 1;
        unsigned char NCQStreaming : 1;
        unsigned char CurrentNegotiatedSATASpeed : 3;
        unsigned char zero : 1;
    } SATA_Capabilities2;

    struct
    {
        unsigned char useless1;
        unsigned char NCQAutosense : 1;
        unsigned char SoftwareSettingsPreservation : 1;
        unsigned char HardwareFeatureControl :1;
        unsigned char InOrderDataDelivery : 1;
        unsigned char InitiatingPowerManagement : 1;
        unsigned char DMASetupAutoActivation : 1;
        unsigned char NonZeroBufferOffsets : 1;
        unsigned char zero;
    } SATA_Features;

    struct 
    {
        unsigned char useless1;
        unsigned char AutomaticPartialToSlumberTransition : 1;
        unsigned char SoftwareSettingsPreservation : 1;
        unsigned char HardwareFeatureControl : 1;
        unsigned char InOrderDataDelivery : 1;
        unsigned char InitiatedPowerManagement : 1;
        unsigned char DMASetupAutoActivation : 1;
        unsigned char NonZeroBufferOffsets : 1;
        unsigned char zero : 1;
    } SATA_Features_Enabled;

    unsigned short MajorVersionNumber;
    unsigned short MinorVersionNumber;

    struct
    {
        unsigned char useless1 : 1;
        unsigned char NOP : 1;
        unsigned char READ_BUFFER : 1;
        unsigned char WRITE_BUFFER : 1;
        unsigned char useless2 : 2;
        unsigned char zero1 : 1;
        unsigned char useless3 : 2;
        unsigned char ReadLookAhead : 1;
        unsigned char VolatileWriteCache : 1;
        unsigned char zero2 : 1;
        unsigned char one : 1;
        unsigned char useless4 : 1;
        unsigned char SecurityFeatureSet : 1;
        unsigned char SMARTFeatureSet : 1;
    } Supported3;

    struct 
    {
        unsigned char zero : 1;
        unsigned char one : 1;
        unsigned char FLUSH_CACHE_EXT : 1;
        unsigned char FLUSH_CACHE : 1;
        unsigned char useless1 : 1;
        unsigned char AdressFeatureSet48 : 1;
        unsigned char useless2 : 3;
        unsigned char SET_FEATURES_required : 1;
        unsigned char PUIS : 1;
        unsigned char useless3 : 1;
        unsigned char APMFeatureSet : 1;
        unsigned char useless4 : 2;
        unsigned char DOWNLOAD_MICROCODE : 1;
    } Supported4;

    struct 
    {
        unsigned char zero : 1;
        unsigned char one : 1;
        unsigned IDLE_IMMIEDIATE_UNLOAD : 1;
        unsigned char useless1 : 4;
        unsigned char WorldWideName : 1;
        unsigned char useless2 : 1;
        unsigned char WRITE_DMA_FUA_EXT : 1; // WRITE_MULTIPLE_FUA_EXT
        unsigned char GPL_FeatureSet : 1;
        unsigned char StreamingFeatureSet : 1;
        unsigned char useless3 : 2;
        unsigned char SMARTSelfTest : 1;
        unsigned char SMARTErrorLogging : 1;
    } Supported5;

} __attribute__((packed));

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
	enum ATA_TYPE type;
    bool master;

    const struct ATA_Registers registers;
    unsigned long long sector_count;
    struct ATA_Identify identify;
};

void ata_init();
struct Drive* grab_drive(unsigned char index);

unsigned char ata_read(struct Drive* drive, unsigned long long lba, unsigned short sector_count, unsigned char* buffer);
unsigned char ata_write(struct Drive* drive, unsigned long long lba, unsigned short sector_count, unsigned char* buffer);
