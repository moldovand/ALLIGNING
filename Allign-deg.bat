@ECHO OFF

setlocal enabledelayedexpansion

@SET allign="C:\Alligning\Register\Debug\Register.exe"

::number of CDM files
set /a Nr_cdm=35

set /a Nr_match=Nr_cdm-1

set /a Nr_allign=Nr_cdm+1

::teddy bear settings
set x_crop=149
set y_crop=148

pause

ECHO Do Registration...
%allign% %x_crop% %y_crop% %Nr_allign%


