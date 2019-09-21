#pragma once
#include "../std.h"
#include "../result.h"
#include "../attributes.h"
#include "partition.h"

enum ATA_TYPE {
	ATA_NONE = 0,
	ATA_UNKNOWN = 1,
    ATA_SATA = 2,
    ATA_PATA = 3,
    ATA_SATAPI = 4,
    ATA_PATAPI = 5,
    ATA_INVALID,
};

struct ATAIdentify {
    struct PACKED {
        uint16_t ATADevice : 1;
        uint16_t useless1 : 12;
        uint16_t IncompleteResponse : 1;
        uint16_t useless2 : 2;
    } GeneralConfiguration;

    uint16_t useless1;
    uint16_t SpecificConfiguration;
    uint16_t useless2[7];

    uint8_t SerialNumber[20];
    
    uint16_t useless3[3];

    uint16_t FirmwareRevision[4];
    uint8_t ModelNumber[40];

    uint16_t MaximumTransferPerDRQBlock;

    uint16_t TrustedComputerFeatureSet;
    
    struct PACKED {
        uint16_t useless1 : 2;
        uint16_t StandbyTimer : 1;
        uint16_t useless2 : 1;
        uint16_t IORDYSupport : 1;
        uint16_t IORDYDisabled : 1;
        uint16_t LBASupport : 1;
        uint16_t DMASupport : 1;
        uint16_t useless3 : 6;
        uint16_t LongPhysicalSectorAlignmentError : 2;

        uint16_t useless4 : 15;
        uint16_t MinimumStandbyValue : 1;
    } Capabilities1;

    uint16_t useless5[2];

    struct PACKED {
        uint16_t useless1 : 14;
        uint16_t UltraDMAValid : 1;
        uint16_t Shorts64to70Valid : 1;
    } Valid1;

    uint16_t useless6[5];

    struct PACKED {
        uint16_t BLOCK_ERASE_EXT : 1;
        uint16_t OVERWRITE_EXT : 1;
        uint16_t CRYPTO_SCRAMBLE_EXT : 1;

        uint16_t SanitizeFeatureSet : 1;
        uint16_t SanitizeStandardized : 1;

        uint16_t SANITIZE_ANTIFREEZE_LOCK_EXT : 1;
        uint16_t useless1 : 1;
        
        uint16_t MultipleLogicalSectorSettingValid : 1;
        uint16_t SectorsPerDRQSetting : 8;
    } Supported1;

    uint32_t SectorCountLow;
    
    uint16_t useless7;

    struct PACKED {
        uint16_t useless1 : 5;
        uint16_t Mode2Selected : 1;
        uint16_t Mode1Selected : 1;
        uint16_t Mode0Selected : 1;
        uint16_t useless2 : 5;
        uint16_t Mode2Supported : 1;
        uint16_t Mode1Supported : 1;
        uint16_t Mode0Supported : 1;
    } MultiwordDMA;

    uint16_t useless8 : 14;
    uint8_t PIO3_4Supported : 2;

    uint16_t MinimumMultiwordDMATransferCycleTimePerWord;
    uint16_t RecommendedMultiwordDMATransferCycleTime;
    uint16_t MinimumPIOTransferCycleTime_NOFLOW;
    uint16_t MinimumPIOTransferCycleTime_IORDY;

    struct PACKED {
        uint16_t reservedForCFA : 1;
        uint16_t DeterministicDataInTrimmedLBA : 1;
        uint16_t LongPhysSectorAlignmentErrorReportControl : 1;
        uint16_t useless2 : 1;
        uint16_t READ_BUFFER_DMA : 1;
        uint16_t WRITE_BUFFER_DMA : 1;
        uint16_t useless3 : 1;
        uint16_t DOWNLOAD_MICROCODE_DMA : 1;
        uint16_t useless4 : 1;
        uint16_t OptionalATADevice28bitCommands : 1;
        uint16_t TrimmedLBARangeReturnZeroedData : 1;
        uint16_t DeviceEncryption : 1;
        uint16_t ExtendedUserAddressableSectors : 1;
        uint16_t WriteCacheNonVolatile : 1;
        uint16_t useless5 : 2;
    } Supported2;

    uint16_t useless9[5];
    
    struct PACKED {
        uint16_t useless : 11;
        uint16_t MaxQueueDepth : 5; // minus 1
    } QueueDepth;

    struct PACKED {
        uint16_t READ_LOG_DMA_EXT : 1; // if is equal to READ_LOG_EXT
        uint16_t DeviceAutomaticPartialToSlumberTransition : 1;
        uint16_t HostAutomaticPartialToSlumberTransition : 1;
        uint16_t NCQPriorityInformation : 1;
        uint16_t UnloadWhileNCQOutsanding : 1;
        uint16_t PhyEventCountersLog : 1;
        uint16_t HostInitiatedPowerManagementRequests : 1;
        uint16_t NCQFeatureSet : 1;
        uint16_t useless1 : 4;
        uint16_t SATAGen3Speed : 1; // 6.0Gb/s
        uint16_t SATAGen2Speed : 1; // 3.0Gb/s
        uint16_t SATAGen1Speed : 1; // 1.5Gb/s
        uint16_t zero : 1;
    } SATA_Capabilities1;

    struct PACKED {
        uint16_t useless1 : 9;
        uint16_t RECEIVE_SEND_FPDMA_QUEUED : 1;
        uint16_t NCQueueManagementCommand : 1;
        uint16_t NCQStreaming : 1;
        uint16_t CurrentNegotiatedSATASpeed : 3;
        uint16_t zero : 1;
    } SATA_Capabilities2;

    struct PACKED {
        uint16_t useless1 : 8;
        uint16_t NCQAutosense : 1;
        uint16_t SoftwareSettingsPreservation : 1;
        uint16_t HardwareFeatureControl :1;
        uint16_t InOrderDataDelivery : 1;
        uint16_t InitiatingPowerManagement : 1;
        uint16_t DMASetupAutoActivation : 1;
        uint16_t NonZeroBufferOffsets : 1;
        uint16_t zero : 1;
    } SATA_Features;

    struct PACKED {
        uint16_t useless : 8;
        uint16_t AutomaticPartialToSlumberTransition : 1;
        uint16_t SoftwareSettingsPreservation : 1;
        uint16_t HardwareFeatureControl : 1;
        uint16_t InOrderDataDelivery : 1;
        uint16_t InitiatedPowerManagement : 1;
        uint16_t DMASetupAutoActivation : 1;
        uint16_t NonZeroBufferOffsets : 1;
        uint16_t zero : 1;
    } SATA_Features_Enabled;

    uint16_t MajorVersionNumber;
    uint16_t MinorVersionNumber;

    struct PACKED {
        uint16_t useless1 : 1;
        uint16_t NOP : 1;
        uint16_t READ_BUFFER : 1;
        uint16_t WRITE_BUFFER : 1;
        uint16_t useless2 : 2;
        uint16_t zero1 : 1;
        uint16_t useless3 : 2;
        uint16_t ReadLookAhead : 1;
        uint16_t VolatileWriteCache : 1;
        uint16_t zero2 : 1;
        uint16_t one : 1;
        uint16_t useless4 : 1;
        uint16_t SecurityFeatureSet : 1;
        uint16_t SMARTFeatureSet : 1;
    } Supported3;

    struct PACKED {
        uint16_t zero : 1;
        uint16_t one : 1;
        uint16_t FLUSH_CACHE_EXT : 1;
        uint16_t FLUSH_CACHE : 1;
        uint16_t useless1 : 1;
        uint16_t AdressFeatureSet48 : 1;
        uint16_t useless2 : 3;
        uint16_t SET_FEATURES_required : 1;
        uint16_t PUIS : 1;
        uint16_t useless3 : 1;
        uint16_t APMFeatureSet : 1;
        uint16_t useless4 : 2;
        uint16_t DOWNLOAD_MICROCODE : 1;
    } Supported4;

    struct PACKED {
        uint16_t zero : 1;
        uint16_t one : 1;
        uint16_t IDLE_IMMIEDIATE_UNLOAD : 1;
        uint16_t useless1 : 4;
        uint16_t WorldWideName : 1;
        uint16_t useless2 : 1;
        uint16_t WRITE_DMA_FUA_EXT : 1; // WRITE_MULTIPLE_FUA_EXT
        uint16_t GPL_FeatureSet : 1;
        uint16_t StreamingFeatureSet : 1;
        uint16_t useless3 : 2;
        uint16_t SMARTSelfTest : 1;
        uint16_t SMARTErrorLogging : 1;
    } Supported5;

    struct PACKED {
        uint16_t useless : 1;
        uint16_t nop : 1;
        uint16_t READ_BUFFER : 1;
        uint16_t WRITE_BUFFER : 1;
        uint16_t useless2 : 2;
        uint16_t zero : 1;
        uint16_t useless3 : 2;
        uint16_t read_lookahead : 1;
        uint16_t volatile_write_cache : 1;
        uint16_t zero2 : 1;
        uint16_t one : 1;
        uint16_t useless4 : 1;
        uint16_t security : 1;
        uint16_t smart : 1;
    } Supported6;

   struct PACKED {
        uint16_t Words119_120Valid : 1;
        uint16_t useless : 1;
        uint16_t FLUSH_CACHE_EXT : 1;
        uint16_t FLUSH_CACHE : 1;
        uint16_t useless2 : 1;
        uint16_t Features48bit : 1;
        uint16_t useless3 : 3;
        uint16_t SET_FEATURES_REQUIRED : 1;
        uint16_t PUIS : 1;
        uint16_t useless4 : 1;
        uint16_t APM : 1;
        uint16_t CFA_reserved : 1;
        uint16_t useless5 : 1;
        uint16_t DOWNLOAD_MICROCODE : 1;
   } Supported7;

   struct PACKED {
       uint16_t zero : 1;
       uint16_t one : 1;
       uint16_t IDLE_IMMEDIATE : 1;
       uint16_t useless : 4;
       uint16_t one1 : 1;
       uint16_t useless2 : 1;
       uint16_t WRITE_DMA_FUA_EXT : 1;
       uint16_t GPL_feature_set : 1;
       uint16_t useless3 : 2;
       uint16_t MediaSerialNumberValid : 1;
       uint16_t SMART_SelfTest : 1;
       uint16_t SMART_ErrorLog : 1;
   } Supported8;

    struct PACKED {
        uint16_t reserved : 1;
        uint16_t mode6_selected : 1;
        uint16_t mode5_selected : 1;
        uint16_t mode4_selected : 1;
        uint16_t mode3_selected : 1;
        uint16_t mode2_selected : 1;
        uint16_t mode1_selected : 1;
        uint16_t mode0_selected : 1;
        uint16_t _reserved : 1;
        uint16_t mode6_supported : 1;
        uint16_t mode5_supported : 1;
        uint16_t mode4_supported : 1;
        uint16_t mode3_supported : 1;
        uint16_t mode2_supported : 1;
        uint16_t mode1_supported : 1;
        uint16_t mode0_supported : 1;
    } UltraDMAModes;

    uint32_t ExtendedTime;
    uint8_t reserved2115;
    uint8_t CurrentAPMValue;
    uint16_t MasterPasswordIdentifier;
    uint16_t HardwareResetResults;
    uint16_t useless3489573495;
    uint16_t StreamMinimumRequestSize;
    uint16_t DMAStreamingTransferTime;
    uint16_t StreamingAccessLatency;
    uint32_t StreamingPerformanceGranularity;
    uint64_t SectorCount;
    uint16_t PIOStreamingTransferTime;
    uint16_t DataSetManagementBlocks;

    struct PACKED {
        uint16_t zero : 1;
        uint16_t one : 1;
        uint16_t MultipleLogicalPerPhysical : 1;
        uint16_t LogicalSectorLongerThan256Words : 1;
        uint16_t reserved : 8;
        uint16_t LogicalSectorsPerPhysical : 4;
    } PhysicalLogicalSectorSize;

    uint16_t InterSeekDelay;
    uint8_t WorldWideName[8];
    uint64_t reserved23875;
    uint16_t obsolete2435;
    uint32_t LogicalSectorSize;

    uint32_t supported_some;
    uint8_t reserved98345[12];
    uint16_t obsolete436387;
    uint16_t SecurityStatus;
    uint16_t VendorSpecific[31];
    uint16_t CFAReserved[8];
    uint16_t DeviceNominalNormFactor;
    uint16_t DataSetManagementSupport;
    uint64_t AdditionalProductIdentifier;
    uint32_t reserved98435049;
    uint16_t CurrentMediaSerialNumber[30];
    uint16_t SCTCommandTransport;
    uint32_t reserved92835;
    
    struct PACKED {
        uint16_t zero : 1;
        uint16_t one : 1;
        uint16_t offset : 14;
    } LogicalSectorAlignment;

    uint32_t WriteReadVerifySectorMode3Count;
    uint32_t WriteReadVerifySectorMode2Count;
    uint16_t obsolete9348[3];
    uint16_t NominalMediaRotationRate;
    uint16_t reserved87346[2];
    uint16_t WriteReadVerifyFeatureSetCurrentMode;
    uint16_t reserved983467;
    uint16_t TransportMajorVersionNumber;
    uint16_t TransportMinorVersionNumber;
    uint16_t reserved9347851[6];
    uint64_t SectorCountExt;
    uint16_t DownloadMicrocodeMinimumBlocks;
    uint16_t DownloadMicrocodeMaximumBlocks;
    uint16_t reserved8347638457[19];
    uint16_t IntegrityWord;
} PACKED;

struct ATARegisters {
    uint16_t data;
    uint16_t error;
    uint16_t features;
    uint16_t sector_count;
    uint16_t lba_low;
    uint16_t lba_mid;
    uint16_t lba_high;
    uint16_t drive;
    uint16_t status;
    uint16_t command;
    uint16_t alternate_status;
    uint16_t device_control;
    uint16_t drive_address;
};

struct Drive {
    uint8_t id;
	enum ATA_TYPE type;
    const bool slave;

    Partition* partitions;
    
    const struct ATARegisters* registers;
    struct ATAIdentify identify;
    struct {
        uint16_t LogicalSectorMultiplier;
        uint16_t LogicalSectorAlignment;
        uint32_t PhysicalSectorSize;
        uint32_t LowAddressableSectors;
        uint64_t HighAddressableSectors;
        uint64_t ExtSectors;
    } data;
};

typedef struct Drive Drive;

size_t ata_init(void);

Result ata_read(const Drive* drive, uint64_t lba, uint16_t count);
Result ata_write(const Drive* drive, uint64_t lba, uint16_t count, const uint16_t* buffer);
