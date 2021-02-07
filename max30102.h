
#ifndef MAX30102_H_
#define MAX30102_H_

#include <stdint.h>

void maxim_max30102_reset (void);
void maxim_max30102_init (void);


void maxim_max30102_read_fifo (uint32_t *, uint32_t *);



#endif /* MAX30102_H_ */
