/*
 * MCP7940N.h
 *
 * Created: 2015-10-08 17:51:26
 *  Author: Rodos
 */ 


#ifndef MCP7940N_H_
#define MCP7940N_H_

void MCP7940N_ReadTime (TWI_t * twiname, DateTime * dateTime);
void MCP7940N_WriteTime (TWI_t * twiname, DateTime * dateTime);

#endif /* MCP7940N_H_ */