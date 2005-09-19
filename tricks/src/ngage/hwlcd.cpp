#include "hwtricks.hpp"
#include <e32svr.h>

EXPORT_C void HWLcd::SetContrastL(TInt aContrast)
{
  User::LeaveIfError(UserSvr::HalSet(0x1b,&aContrast));
}

EXPORT_C void HWLcd::ContrastL(TInt& aContrast)
{
  User::LeaveIfError(UserSvr::HalGet(0x1c,&aContrast));
}

EXPORT_C void HWLcd::MaxContrastL(TInt& aMaxContrast)
{
  User::LeaveIfError(UserSvr::HalGet(0x1a,&aMaxContrast));
}
