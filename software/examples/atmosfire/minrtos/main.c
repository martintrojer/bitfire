
#include "minrtos.h"

int main(void)
{
  minrtos_init();
  
  minrtos_setup_clocks(); 

  minrtos_setup_tick_timer(10240);
  
  minrtos_setup_uart(3,38400);
  
  while (1) ;
}
