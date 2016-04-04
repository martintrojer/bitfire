This is the str7xx linux bootloader (Flash, ymodem).
GNU-ified by Martin Trojer (mtrojer@arrownordic.com)
-------------

======================Environment=========================================
	Arm Development Suit 1.2

===========================Basic Guidelines===============================
* When in development stage, please set SW14,SW13,SW15 on the STR710-EVAL board
to RAM boot mode(see Page 13 of STR710-EVAL datasheet). Then Use ICE to open *.axf.
	
* Normally as a released version, Set switchs to External Flash boot mode.

===========================Changlog=======================================

---- 13/Aug/04	 version 0.1
--Xinghua Li
* Initial Version.
* Can boot from RAM.	
* User interface (menu,command mode ) is ready.
* Time Function WaitMs() is finished.

---- 20/Aug/04	 version 0.2
--by Xinghua Li, Ryan Sheng
* Set SYS mode (71x_init.c) 
* Add flash program routines 
* Add user command/menu for progextflash, loadlinux operations
* Fix bugs for user command input (GetInputString())
* Add "getversion" user command 
- Known Bugs
? 4MB external flash device appears mapping as 1MB address space.

--21/Aug/04 Xinghua Li 
* Bug is fixed. Set GPIO2.4/2.5 as alternative function A20/A21 (main.c).
* Now can boot system from external flash.

--27/Aug/04 Xinghua LI version 0.3
* Modify ProgramExternalFlash() according to design spec v0.3 (ask user to input offset instead absolute address)
* Rewrite Loadlinux() function according to design spec v0.4 (not necessary to input imagesize)
* Add jumto user command for Lab developement purpose.

--01/Sep/04 Ryan Sheng version 0.4
* Add serial download routines by Ymodem protocol.

--08/Sep/04 Xinghua Li version 0.5
* Support linux setup parameters in Loadlinux()
* Modify SerialDownload(), setting 0x62000000 as default download address
* Modify main.c. Add bootloader/uclinux selection at the bootup stage. 

--10/Sep/04 Ryan Sheng
* Bug Fixed: Calculate correct file size in serial download.

--17/Sep/04 Xinghua Li
* Bug Fixed : Modify GetInputString() in platform.c,platform.h, fixing characters like 'A' 'D'..... cannot be input. 

--21/Sep/04 Xinghua Li Version 1.0
* Release V1.0

 

====================================Release History===================================

=Version=	=FileName=	=Checksum(32bit)=	=ReleaseDate=	 =Released By=	=Major/Minor Change=
-----------------------------------------------------------------------------------------------------------
0.1		---		---			13/Aug/04	Xinghua LI		Major
0.2		---		---			20/Aug/04	Xinghua,Ryan		Minor
0.3		---		---			27/Aug/04	Xinghua LI		Minor
0.4		---		---			01/Sep/04	Ryan Sheng		Minor
0.5		---		---			08/Sep/04	Xinghua LI		Major
1.0		71xboot-R10.bin		00284BFA	21/Sep/04	Xinghua LI		Minor
