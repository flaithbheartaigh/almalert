#include <a32ngage.hpp>

GLDEF_C TInt E32Dll(TDllReason/* aReason*/)
{
  return KErrNone;
}

EXPORT_C TInt locltest_1(void)
{
  return 0;
}

EXPORT_C TInt locltest_2(void)
{
  return 0;
}

EXPORT_C TInt locltest_3(void)
{
  return 0;
}

EXPORT_C TInt locltest_4(void)
{
  return 0;
}

EXPORT_C TInt locltest_5(void)
{
  return 0;
}

EXPORT_C TInt locltest_6(void)
{
  return 0;
}

EXPORT_C TInt locltest_7(void)
{
  return 0;
}

EXPORT_C TInt locltest_8(void)
{
  return 0;
}

EXPORT_C TInt locltest_9(void)
{
  return 0;
}

EXPORT_C TInt locltest_10(void)
{
  return 0;
}

EXPORT_C TInt locltest_11(void)
{
  return 0;
}

EXPORT_C TInt locltest_12(void)
{
  return 0;
}

EXPORT_C TInt locltest_13(void)
{
  return 0;
}

EXPORT_C TInt locltest_14(void)
{
  return 0;
}

EXPORT_C TInt locltest_15(void)
{
  return 0;
}

EXPORT_C TInt locltest_16(void)
{
  if(TNgage::Register16(0x15020a)&0x2000)
  {
    TNgage::SetRegister16(0x2290,0x149000);
    TNgage::SetRegister32(0x000f8000,0x120018);
    TNgage::SetRegister32(0x00020000,0x120014);
  }
  return 0;
}

EXPORT_C TInt locltest_17(void)
{
  return 0;
}

EXPORT_C TInt locltest_18(void)
{
  return 0;
}

EXPORT_C TInt locltest_19(void)
{
  return 0;
}

EXPORT_C TInt locltest_20(void)
{
  return 0;
}

EXPORT_C TInt locltest_21(void)
{
  return 0;
}
