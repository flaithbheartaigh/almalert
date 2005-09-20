#include "hwtricks.hpp"
#include <hal.h>

EXPORT_C void HWLcd::SetContrastL(TInt aContrast)
{
  User::LeaveIfError(HAL::Set(HALData::EDisplayContrast,aContrast));
}

EXPORT_C void HWLcd::ContrastL(TInt& aContrast)
{
  User::LeaveIfError(HAL::Get(HALData::EDisplayContrast,aContrast));
}

EXPORT_C void HWLcd::MaxContrastL(TInt& aMaxContrast)
{
  User::LeaveIfError(HAL::Get(HALData::EDisplayContrastMax,aMaxContrast));
}
