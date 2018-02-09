/*
 * errors.h
 *
 *  Created on: 3 févr. 2018
 *      Author: jerome
 */

#ifndef ERRORS_H_
#define ERRORS_H_

#include "stdio.h"
#include <stdlib.h>

#define ERROR(file,line,info)	({char s[100]; sprintf(s,"ERROR %s:%d - %s\n",file, line, info); printf(s);;exit(1);})
#define ASSERT(cond,file,line)	({if ( (cond) == false){char s[100]; sprintf(s,"ERROR %s:%d\n",file, line); printf(s);exit(1) ;}})


#endif /* ERRORS_H_ */
