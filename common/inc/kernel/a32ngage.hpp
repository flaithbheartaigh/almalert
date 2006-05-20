/*
    a32ngage.hpp
    Copyright (C) 2006 zg

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef __A32NGAGE_HPP__
#define __A32NGAGE_HPP__

#include <m32std61.hpp>

_LIT8(KFiqTimer0,"FiqTimer0");
_LIT8(KFiqTimer1,"FiqTimer1");
_LIT8(KFiqTimer2,"FiqTimer2");
_LIT8(KFiqTimer3,"FiqTimer3");
_LIT8(KFiqTimer4,"FiqTimer4");
_LIT8(KFiqTimer5,"FiqTimer5");
_LIT8(KFiqDmaChannel0,"FiqDmaChannel0");
_LIT8(KFiqDmaChannel1,"FiqDmaChannel1");
_LIT8(KFiqDmaChannel2,"FiqDmaChannel2");
_LIT8(KFiqDmaChannel3,"FiqDmaChannel3");
_LIT8(KFiqDmaChannel4,"FiqDmaChannel4");
_LIT8(KFiqDmaChannel5,"FiqDmaChannel5");
_LIT8(KFiqDmaChannel6,"FiqDmaChannel6");
_LIT8(KFiqDmaChannel7,"FiqDmaChannel7");
_LIT8(KFiqFlashCopyRdy,"FiqFlashCopyRdy");
_LIT8(KFiqLCDBuffer,"FiqLCDBuffer");
_LIT8(KFiqIr4MTxData,"FiqIr4MTxData");
_LIT8(KFiqIr4MTxIdle,"FiqIr4MTxIdle");
_LIT8(KFiqIr4MRxData,"FiqIr4MRxData");
_LIT8(KFiqIr4MRxIdle,"FiqIr4MRxIdle");
_LIT8(KFiqIr4MRxBusy,"FiqIr4MRxBusy");
_LIT8(KFiqMMCDataIdle,"FiqMMCDataIdle");
_LIT8(KFiqMMCDataRdy,"FiqMMCDataRdy");
_LIT8(KFiqMMCCmdIdle,"FiqMMCCmdIdle");
_LIT8(KFiqMMCCmdRdy,"FiqMMCCmdRdy");
_LIT8(KFiqCTSI_IRQ,"FiqCTSI_IRQ");
_LIT8(KFiqCTSI_FIQ,"FiqCTSI_FIQ");
_LIT8(KFiqAPIIOAbort,"FiqAPIIOAbort");
_LIT8(KFiqAPIMissAlign,"FiqAPIMissAlign");
_LIT8(KFiqGPIO0Low,"FiqGPIO0Low");
_LIT8(KFiqGPIO0High,"FiqGPIO0High");
_LIT8(KFiqGPIO1Low,"FiqGPIO1Low");
_LIT8(KFiqGPIO1High,"FiqGPIO1High");
_LIT8(KFiqGPIO2Low,"FiqGPIO2Low");
_LIT8(KFiqGPIO2High,"FiqGPIO2High");
_LIT8(KFiqGPIO3Low,"FiqGPIO3Low");
_LIT8(KFiqGPIO3High,"FiqGPIO3High");
_LIT8(KFiqGPIO4Low,"FiqGPIO4Low");
_LIT8(KFiqGPIO4High,"FiqGPIO4High");
_LIT8(KFiqGPIO5Low,"FiqGPIO5Low");
_LIT8(KFiqGPIO5High,"FiqGPIO5High");
_LIT8(KFiqGPIO6Low,"FiqGPIO6Low");
_LIT8(KFiqGPIO6High,"FiqGPIO6High");
_LIT8(KFiqGPIO7Low,"FiqGPIO7Low");
_LIT8(KFiqGPIO7High,"FiqGPIO7High");
_LIT8(KFiqGPIO8Low,"FiqGPIO8Low");
_LIT8(KFiqGPIO8High,"FiqGPIO8High");
_LIT8(KFiqGPIO9Low,"FiqGPIO9Low");
_LIT8(KFiqGPIO9High,"FiqGPIO9High");
_LIT8(KFiqGPIO10Low,"FiqGPIO10Low");
_LIT8(KFiqGPIO10High,"FiqGPIO10High");
_LIT8(KFiqGPIO11Low,"FiqGPIO11Low");
_LIT8(KFiqGPIO11High,"FiqGPIO11High");
_LIT8(KFiqGPIO12Low,"FiqGPIO12Low");
_LIT8(KFiqGPIO12High,"FiqGPIO12High");
_LIT8(KFiqGPIO13Low,"FiqGPIO13Low");
_LIT8(KFiqGPIO13High,"FiqGPIO13High");
_LIT8(KFiqGPIO14Low,"FiqGPIO14Low");
_LIT8(KFiqGPIO14High,"FiqGPIO14High");
_LIT8(KFiqGPIO15Low,"FiqGPIO15Low");
_LIT8(KFiqGPIO15High,"FiqGPIO15High");
_LIT8(KFiqGPIO16Low,"FiqGPIO16Low");
_LIT8(KFiqGPIO16High,"FiqGPIO16High");
_LIT8(KFiqGPIO17Low,"FiqGPIO17Low");
_LIT8(KFiqGPIO17High,"FiqGPIO17High");
_LIT8(KFiqGPIO18Low,"FiqGPIO18Low");
_LIT8(KFiqGPIO18High,"FiqGPIO18High");
_LIT8(KFiqGPIO19Low,"FiqGPIO19Low");
_LIT8(KFiqGPIO19High,"FiqGPIO19High");
_LIT8(KFiqGPIO20Low,"FiqGPIO20Low");
_LIT8(KFiqGPIO20High,"FiqGPIO20High");
_LIT8(KFiqGPIO21Low,"FiqGPIO21Low");
_LIT8(KFiqGPIO21High,"FiqGPIO21High");
_LIT8(KFiqGPIO22Low,"FiqGPIO22Low");
_LIT8(KFiqGPIO22High,"FiqGPIO22High");
_LIT8(KFiqGPIO23Low,"FiqGPIO23Low");
_LIT8(KFiqGPIO23High,"FiqGPIO23High");
_LIT8(KFiqGPIO24Low,"FiqGPIO24Low");
_LIT8(KFiqGPIO24High,"FiqGPIO24High");
_LIT8(KFiqGPIO25Low,"FiqGPIO25Low");
_LIT8(KFiqGPIO25High,"FiqGPIO25High");
_LIT8(KFiqGPIO26Low,"FiqGPIO26Low");
_LIT8(KFiqGPIO26High,"FiqGPIO26High");
_LIT8(KFiqSTIAbort,"FiqSTIAbort");
_LIT8(KFiqCCPRdFour,"FiqCCPRdFour");
_LIT8(KFiqCCPRdOne,"FiqCCPRdOne");
_LIT8(KFiqCCPStatus,"FiqCCPStatus");
_LIT8(KFiqDIFFIFO,"FiqDIFFIFO");
_LIT8(KFiqPIPORdFour,"FiqPIPORdFour");
_LIT8(KFiqPIPORdOne,"FiqPIPORdOne");
_LIT8(KFiqPIPOStatus,"FiqPIPOStatus");
_LIT8(KFiqWBCDMAStatus,"FiqWBCDMAStatus");
_LIT8(KFiqRTCWBStatus,"FiqRTCWBStatus");
_LIT8(KIrqTimer0,"IrqTimer0");
_LIT8(KIrqTimer1,"IrqTimer1");
_LIT8(KIrqTimer2,"IrqTimer2");
_LIT8(KIrqTimer3,"IrqTimer3");
_LIT8(KIrqTimer4,"IrqTimer4");
_LIT8(KIrqTimer5,"IrqTimer5");
_LIT8(KIrqDmaChannel0,"IrqDmaChannel0");
_LIT8(KIrqDmaChannel1,"IrqDmaChannel1");
_LIT8(KIrqDmaChannel2,"IrqDmaChannel2");
_LIT8(KIrqDmaChannel3,"IrqDmaChannel3");
_LIT8(KIrqDmaChannel4,"IrqDmaChannel4");
_LIT8(KIrqDmaChannel5,"IrqDmaChannel5");
_LIT8(KIrqDmaChannel6,"IrqDmaChannel6");
_LIT8(KIrqDmaChannel7,"IrqDmaChannel7");
_LIT8(KIrqFlashCopyRdy,"IrqFlashCopyRdy");
_LIT8(KIrqLCDBuffer,"IrqLCDBuffer");
_LIT8(KIrqIr4MTxData,"IrqIr4MTxData");
_LIT8(KIrqIr4MTxIdle,"IrqIr4MTxIdle");
_LIT8(KIrqIr4MRxData,"IrqIr4MRxData");
_LIT8(KIrqIr4MRxIdle,"IrqIr4MRxIdle");
_LIT8(KIrqIr4MRxBusy,"IrqIr4MRxBusy");
_LIT8(KIrqMMCDataIdle,"IrqMMCDataIdle");
_LIT8(KIrqMMCDataRdy,"IrqMMCDataRdy");
_LIT8(KIrqMMCCmdIdle,"IrqMMCCmdIdle");
_LIT8(KIrqMMCCmdRdy,"IrqMMCCmdRdy");
_LIT8(KIrqCTSI_IRQ,"IrqCTSI_IRQ");
_LIT8(KIrqCTSI_FIQ,"IrqCTSI_FIQ");
_LIT8(KIrqAPIIOAbort,"IrqAPIIOAbort");
_LIT8(KIrqAPIMissAlign,"IrqAPIMissAlign");
_LIT8(KIrqGPIO0Low,"IrqGPIO0Low");
_LIT8(KIrqGPIO0High,"IrqGPIO0High");
_LIT8(KIrqGPIO1Low,"IrqGPIO1Low");
_LIT8(KIrqGPIO1High,"IrqGPIO1High");
_LIT8(KIrqGPIO2Low,"IrqGPIO2Low");
_LIT8(KIrqGPIO2High,"IrqGPIO2High");
_LIT8(KIrqGPIO3Low,"IrqGPIO3Low");
_LIT8(KIrqGPIO3High,"IrqGPIO3High");
_LIT8(KIrqGPIO4Low,"IrqGPIO4Low");
_LIT8(KIrqGPIO4High,"IrqGPIO4High");
_LIT8(KIrqGPIO5Low,"IrqGPIO5Low");
_LIT8(KIrqGPIO5High,"IrqGPIO5High");
_LIT8(KIrqGPIO6Low,"IrqGPIO6Low");
_LIT8(KIrqGPIO6High,"IrqGPIO6High");
_LIT8(KIrqGPIO7Low,"IrqGPIO7Low");
_LIT8(KIrqGPIO7High,"IrqGPIO7High");
_LIT8(KIrqGPIO8Low,"IrqGPIO8Low");
_LIT8(KIrqGPIO8High,"IrqGPIO8High");
_LIT8(KIrqGPIO9Low,"IrqGPIO9Low");
_LIT8(KIrqGPIO9High,"IrqGPIO9High");
_LIT8(KIrqGPIO10Low,"IrqGPIO10Low");
_LIT8(KIrqGPIO10High,"IrqGPIO10High");
_LIT8(KIrqGPIO11Low,"IrqGPIO11Low");
_LIT8(KIrqGPIO11High,"IrqGPIO11High");
_LIT8(KIrqGPIO12Low,"IrqGPIO12Low");
_LIT8(KIrqGPIO12High,"IrqGPIO12High");
_LIT8(KIrqGPIO13Low,"IrqGPIO13Low");
_LIT8(KIrqGPIO13High,"IrqGPIO13High");
_LIT8(KIrqGPIO14Low,"IrqGPIO14Low");
_LIT8(KIrqGPIO14High,"IrqGPIO14High");
_LIT8(KIrqGPIO15Low,"IrqGPIO15Low");
_LIT8(KIrqGPIO15High,"IrqGPIO15High");
_LIT8(KIrqGPIO16Low,"IrqGPIO16Low");
_LIT8(KIrqGPIO16High,"IrqGPIO16High");
_LIT8(KIrqGPIO17Low,"IrqGPIO17Low");
_LIT8(KIrqGPIO17High,"IrqGPIO17High");
_LIT8(KIrqGPIO18Low,"IrqGPIO18Low");
_LIT8(KIrqGPIO18High,"IrqGPIO18High");
_LIT8(KIrqGPIO19Low,"IrqGPIO19Low");
_LIT8(KIrqGPIO19High,"IrqGPIO19High");
_LIT8(KIrqGPIO20Low,"IrqGPIO20Low");
_LIT8(KIrqGPIO20High,"IrqGPIO20High");
_LIT8(KIrqGPIO21Low,"IrqGPIO21Low");
_LIT8(KIrqGPIO21High,"IrqGPIO21High");
_LIT8(KIrqGPIO22Low,"IrqGPIO22Low");
_LIT8(KIrqGPIO22High,"IrqGPIO22High");
_LIT8(KIrqGPIO23Low,"IrqGPIO23Low");
_LIT8(KIrqGPIO23High,"IrqGPIO23High");
_LIT8(KIrqGPIO24Low,"IrqGPIO24Low");
_LIT8(KIrqGPIO24High,"IrqGPIO24High");
_LIT8(KIrqGPIO25Low,"IrqGPIO25Low");
_LIT8(KIrqGPIO25High,"IrqGPIO25High");
_LIT8(KIrqGPIO26Low,"IrqGPIO26Low");
_LIT8(KIrqGPIO26High,"IrqGPIO26High");
_LIT8(KIrqSTIAbort,"IrqSTIAbort");
_LIT8(KIrqCCPRdFour,"IrqCCPRdFour");
_LIT8(KIrqCCPRdOne,"IrqCCPRdOne");
_LIT8(KIrqCCPStatus,"IrqCCPStatus");
_LIT8(KIrqDIFFIFO,"IrqDIFFIFO");
_LIT8(KIrqPIPORdFour,"IrqPIPORdFour");
_LIT8(KIrqPIPORdOne,"IrqPIPORdOne");
_LIT8(KIrqPIPOStatus,"IrqPIPOStatus");
_LIT8(KIrqWBCDMAStatus,"IrqWBCDMAStatus");
_LIT8(KIrqRTCWBStatus,"IrqRTCWBStatus");

class TNgage
{
  public:
    //common
    IMPORT_C static TUint32 Register32(TUint anAddr);
    IMPORT_C static void SetRegister32(TUint32 aValue,TUint anAddr);
    IMPORT_C static void ModifyRegister32(TUint anAddr,TUint32 aSetMask,TUint32 aClearMask);
    IMPORT_C static TUint16 Register16(TUint anAddr);
    IMPORT_C static void SetRegister16(TUint16 aValue,TUint anAddr);
    IMPORT_C static void ModifyRegister16(TUint anAddr,TUint16 aSetMask,TUint16 aClearMask);
    //interrupts
    IMPORT_C static void EnableFiq(TInt anId);
    IMPORT_C static void DisableFiq(TInt anId);
    IMPORT_C static void EnableIrq(TInt anId);
    IMPORT_C static void DisableIrq(TInt anId);
};

#endif
