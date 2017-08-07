SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\Program Files (x86)\STMicroelectronics\st_toolset\asm;C:\ProgramData\Oracle\Java\javapath;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\Program Files\doxygen\bin;C:\Program Files\Calibre2\;C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Bin;C:\Program Files (x86)\IDM Computer Solutions\UltraEdit;C:\Program Files (x86)\SSH Communications Security\SSH Secure Shell;C:\Users\Administrator\AppData\Local\Programs\MiKTeX 2.9\miktex\bin\;%USERPROFILE%\AppData\Local\Microsoft\WindowsApps;
SET CPU_TYPE=STM32F429VI
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=flash-text
SET CPU_CLOCK=0x007A1200
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via "..\obj\startup_stm32f429_439xx._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f429i_discovery.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\misc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_exti.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_rcc.__i"
