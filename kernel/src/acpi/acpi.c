#include "acpi.h"
#include "rsdp.h"

bool AcpiInit()
{
    return RsdpInit();
}