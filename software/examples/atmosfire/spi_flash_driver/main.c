#include "serialize.h"      /*Header file for SPI master abstract prototypes */
#include "c2082.h"          /* Header file for Flash memory instructions */

int main(void) 
{
  ParameterType fp;         /* Contains all Flash memory Parameters */
  ReturnType rRetVal;       /* Return Type Enum */
  ST_uint8 buf[16];
  ST_uint8 wbuf[4] = { 0x0, 0x1, 0x2, 0x3};
  int i;
  
  
  InitSPIMaster();          /* initialize the driver */

  Flash(ReadManufacturerIdentification, &fp);

//  printf("Manufacturer Identification: %02Xh\r\n", fp.ReadManufacturerIdentification.ucManufacturerIdentification);

  Flash(ReadDeviceIdentification, &fp);

//  printf("Device Identification: %04Xh\r\n", fp.ReadDeviceIdentification.ucDeviceIdentification);

//	fp.SectorErase.ustSectorNr = 0; /* sector number 2 will be erased*/
//	rRetVal = Flash(SectorErase, &fp); /* function execution */
  
	fp.SubSectorErase.ustSectorNr = 0;
	rRetVal = Flash(SubSectorErase, &fp); 

  rRetVal = Flash(BulkErase, &fp); /* function execution */

//  Flash(ReadStatusRegister,&fp);
  

#if 1
	for (i=0;i<16;i++) {
		fp.Program.udAddr = i*0x1000;
		fp.Program.pArray = wbuf;
  		fp.Program.udNrOfElementsInArray = 4;
		rRetVal = Flash(Program,&fp);
	}
#endif	
    
	for (i=0;i<16;i++) {
		memset(buf,0,16);
		fp.Read.udAddr = i*0x1000;
		fp.Read.pArray = buf;
		fp.Read.udNrOfElementsToRead = 16;
		rRetVal = Flash(Read,&fp);
	}
	
}/* End of Function Main */
