SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\Program Files (x86)\Python36-32\Scripts\;C:\Program Files (x86)\Python36-32\;C:\ProgramData\Oracle\Java\javapath;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\Cadence\SPB_16.5\tools\pcb\bin;C:\Cadence\SPB_16.5\tools\bin;C:\Cadence\SPB_16.5\tools\libutil\bin;C:\Cadence\SPB_16.5\tools\fet\bin;C:\Cadence\SPB_16.5\tools\specctra\bin;C:\Cadence\SPB_16.5\tools\PSpice;C:\Cadence\SPB_16.5\tools\PSpice\Library;C:\Cadence\SPB_16.5\tools\Capture;C:\Cadence\SPB_16.5\OpenAccess\bin\win32\opt;C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Bin\;C:\Program Files\IVI Foundation\VISA\Win64\Bin\;C:\Program Files (x86)\Microsoft SQL Server\80\Tools\Binn\;C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Bin;c:\Program Files (x86)\Microsoft SQL Server\100\Tools\Binn\;c:\Program Files (x86)\Microsoft SQL Server\100\DTS\Binn\;C:\Program Files (x86)\National Instruments\Motion Assistant\bin\mxwplugins;C:\Program Files\MATLAB\R2017a\bin;C:\Program Files\Git\cmd;C:\Program Files\leJOS EV3\bin;C:\Program Files\Doxygen\bin;C:\Program Files (x86)\IVI Foundation\IVI\bin;C:\Program Files\IVI Foundation\IVI\bin;C:\apache-ant-1.10.1\bin;C:\Users\hwiltz\AppData\Local\Microsoft\WindowsApps;
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32F407VGTx
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=G19_LAB2
SET CPU_CLOCK=0x0A037A00
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_it.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_msp.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\fput_debug.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\system_stm32f4xx.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_adc_ex.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_adc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_flash_ex.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_dac_ex.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_pwr.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_flash.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_dma.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_pwr_ex.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_cortex.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_flash_ramfunc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_dma_ex.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_tim_ex.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_dac.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_tim.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_rcc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal_rcc_ex.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "g19_lab2\stm32f4xx_hal.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via "g19_lab2\startup_stm32f407xx._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via "G19_LAB2\G19_LAB2.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" "G19_LAB2\G19_LAB2.axf" --i32combined --output "G19_LAB2\G19_LAB2.hex"
