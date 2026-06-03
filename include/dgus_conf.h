#ifndef DGUS_CONF_H
#define DGUS_CONF_H

/*
 * DGUS UART2 uses a one-byte length field, so the body after 5A A5 LEN is at
 * most 255 bytes. Override these macros from the compiler command line if a
 * smaller target needs stricter buffers.
 */
#ifndef DGUS_RX_BUFFER_SIZE
#define DGUS_RX_BUFFER_SIZE 255u
#endif

#ifndef DGUS_TX_BUFFER_SIZE
#define DGUS_TX_BUFFER_SIZE 260u
#endif

#ifndef DGUS_MAX_READ_WORDS
#define DGUS_MAX_READ_WORDS 0x7Cu
#endif

#if DGUS_RX_BUFFER_SIZE < 8u
#error "DGUS_RX_BUFFER_SIZE is too small"
#endif

#if DGUS_TX_BUFFER_SIZE < 16u
#error "DGUS_TX_BUFFER_SIZE is too small"
#endif

#endif /* DGUS_CONF_H */
