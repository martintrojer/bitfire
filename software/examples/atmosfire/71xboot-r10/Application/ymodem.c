#include <string.h>
#include "platform.h"
#include "common.h"
#include "ymodem.h"

char file_name[FILE_NAME_LENGTH];

static int receive_byte (char *c, unsigned long timeout)
{
    while (timeout-- > 0)
    {
        if (SerialKeyPressed(c) == 1)
   	    {
            return 0;
        }
   	}
	return -1;
}

static int send_byte (char c)
{
    SerialPutChar(c);
    return 0;
}

/************************************************************
*   Function:       Receive a Ymodem Packet from Sender     *
*   Return Value:                                           *
*       0   --      normally return                         *
*       -1  --      timeout or packet error                 *
*       1   --      abort by user                           *
*   *length:                                                *
*       0   --      end of transmission                     *
*       -1  --      abort by sender                         *
*       >0  --      packet length                           *
************************************************************/
static int receive_packet (char *data, int *length, unsigned long timeout)
{
	int i, packet_size;
	char c;

	*length = 0;

	if (receive_byte(&c, timeout) != 0)
	{
        return -1;
    }

	switch (c)
	{
        case SOH:
            packet_size = PACKET_SIZE;
            break;

        case STX:
            packet_size = PACKET_1K_SIZE;
            break;

        case EOT:
            return 0;

        case CAN:
            if ((receive_byte(&c, timeout) == 0) && (c == CAN))
            {
                *length = -1;
                return 0;
            }
            else
            {
                return -1;
            }

        case ABORT1:
        case ABORT2:
            return 1;

        default:
            return -1;
    }

	*data = c;

    for (i = 1; i < (packet_size + PACKET_OVERHEAD); i ++)
    {
        if (receive_byte(data + i, timeout) != 0)
        {
            return -1;
        }
    }

    if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))
    {
        return -1;
    }

	*length = packet_size;
	return 0;
}

/********************************************************
*   Function:       Receive a File by Ymodem Protocol   *
*   Return Value:                                       *
*       0   --      timeout, error or abort by sender   *
*       -1  --      abort by user                       *
*       >0  --      file length                         *
********************************************************/
int ymodem_receive (char *buf)
{
	char packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD], file_size[FILE_SIZE_LENGTH], *file_ptr, *buf_ptr;
	int i, packet_length, session_done, file_done, packets_received, errors, session_begin;
    unsigned int size = 0;

    for (session_done = 0, errors = 0, session_begin = 0; ;)
    {
        for (packets_received = 0, file_done = 0, buf_ptr = buf; ;)
        {
            switch (receive_packet(packet_data, &packet_length, NAK_TIMEOUT))
            {
                case 0:
                    errors = 0;

                    switch (packet_length)
                    {
                        case -1:    /* abort by sender */
                            send_byte(ACK);
                            return 0;

                        case 0:     /* end of transmission */
                            send_byte(ACK);
                            file_done = 1;
                            break;

                        default:    /* normal packet */
                            if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))
                            {
                                send_byte(NAK);
                            }
                            else
                            {
                                if (packets_received == 0)                  /* filename packet */
                                {
                                    if (packet_data[PACKET_HEADER] != 0)    /* filename packet has valid data */
                                    {
                                        for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
                                        {
                                            file_name[i++] = *file_ptr++;
                                        }
                                        file_name[i++] = '\0';

                                        for (i = 0, file_ptr ++; (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH);)
                                        {
                                            file_size[i++] = *file_ptr++;
                                        }
                                        file_size[i++] = '\0';
                                        Str2Int(file_size, &size);

                                        send_byte(ACK);
                                        send_byte(CRC16);
                                    }
                                    else                                    /* filename packet is empty; end session */
                                    {
                                        send_byte(ACK);
                                        file_done = 1;
                                        session_done = 1;
                                        break;
                                    }
                                }
                                else                                        /* data packet */
                                {
                                    memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);
                                    buf_ptr += packet_length;
                                    send_byte(ACK);
                                }

                                packets_received ++;
                                session_begin = 1;
                            }
                    }
                    break;

                case 1:
                    send_byte(CAN);
                    send_byte(CAN);
                    return -1;

                default:
                    if (session_begin > 0)
                    {
                        errors ++;
                    }
                    if (errors > MAX_ERRORS)
                    {
                        send_byte(CAN);
                        send_byte(CAN);
                        return 0;
                    }

                    send_byte(CRC16);
                    break;
            }

            if (file_done != 0)
            {
                break;
            }
        }

        if (session_done != 0)
        {
            break;
        }
    }

    return (int)size;
}
