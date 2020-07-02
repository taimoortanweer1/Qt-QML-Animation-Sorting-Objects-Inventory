
#ifndef CSERIAL_H_INCLUDED
#define CSERIAL_H_INCLUDED

#define COM1 1
#define COM2 2
#define COM3 3

#define COM1_ADX 0x3F8
#define COM2_ADX 0x2F8
#define COM3_ADX 0x3E8

#define SERIAL_BUF_SIZE 4096

#define MSR_CTS_MSK     0x10
#define MSR_DSR_MSK     0x20
#define MSR_RI_MSK      0x40
#define MSR_CD_MSK      0x80

#define MCR_DTR_MSK     0x01
#define MCR_RTS_MSK     0x02

#define MCR_OFFSET      4
#define MSR_OFFSET      6

bool serialCharAvailable(int com_port);
bool serialGetChar(int com_port, char* in);
bool serialSendChar(int com_port, char out);
bool serialSendString(int com_port, const char* out, int size);
void serialDTR(int com, int newState);

#endif // CSERIAL_H_INCLUDED
