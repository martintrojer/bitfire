
#include <stdio.h>
#include <malloc.h>

void init(void);

int main(void)
{
  int i=0;
  char c;
  void *mem;
  
  mem = malloc(1024);
  free(mem);
  init();

  printf("\r\nHello World! %f>\r\n",1.1315);
  //  scanf("%c",&c);
  //  printf("   you typed '%c'\r\n",c);
  
  while(1)
    i++;
}

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
