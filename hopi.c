#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <modbus/modbus.h>

/* The Hopi defaults to device # 1, can be changed, but not implemented here */
#define SLAVE_ID 1
/* The serial port it appears as, ch341 uart */
#define SERIALPORT "/dev/ttyUSB0"
/* The Hopi communicates via serial ModBus at 9600,8N1 */
#define SERIALBAUD 9600
/* TRUE causes libmodbus library to emit debug information */
#define DEBUGFLAG FALSE
/* Pull 20 words (16 bits) starting at holding register 0 (40001-40020) */
#define START_REG 0
#define NUM_WORDS 20

int main()
{
    modbus_t *ctx;
    uint16_t dat[NUM_WORDS];
    int rc;

    ctx=modbus_new_rtu(SERIALPORT,SERIALBAUD,'N',8,1);
    if (ctx==NULL) {
        fprintf(stderr,"Unable to create libmodbus context\n");
        return EXIT_FAILURE;
    }

    modbus_set_slave(ctx,SLAVE_ID);
    /* response timeout set to 100ms */
    modbus_set_response_timeout(ctx,0,100000);

    if (modbus_connect(ctx)==-1) {
        fprintf(stderr,"Connection Failed: %s\n",modbus_strerror(errno));
        modbus_free(ctx);
        return EXIT_FAILURE;
    }

    modbus_set_debug(ctx,DEBUGFLAG);

    rc=modbus_read_registers(ctx,START_REG,NUM_WORDS,dat);
    if (rc<0) {
        fprintf(stderr,"%s\n",modbus_strerror(errno));
        modbus_close(ctx);
        modbus_free(ctx);
        return EXIT_FAILURE;
    }

#ifdef TESTING
    uint8_t req[]={0x01,0x2b,0x0e,0x01,0x00};
    uint8_t resp[MODBUS_MAX_ADU_LENGTH];

    rc=modbus_send_raw_request(ctx,req,sizeof(req));
    printf("send_raw_request=%d\n",rc);
    if (rc<0) {
        fprintf(stderr,"%s\n",modbus_strerror(errno));
        modbus_close(ctx);
        modbus_free(ctx);
        return EXIT_FAILURE;
    }

    rc=modbus_receive_confirmation(ctx,resp);
    printf("receive_confirmation=%d\n",rc);
    if (rc<0) {
        fprintf(stderr,"%s\n",modbus_strerror(errno));
    } else {
        for (int i=0; i<rc; ++i) {
            printf("%02x ",resp[i]);
        }
        printf("\n");
    }
#endif

    printf("%10.5f W   Active Power\n",modbus_get_float_dcba(dat));
    printf("%10.5f A   RMS Current\n",modbus_get_float_dcba(dat+2));
    printf("%10.5f V   Voltage\n",modbus_get_float_dcba(dat+4));
    printf("%10.5f Hz  Frequency\n",modbus_get_float_dcba(dat+6));
    printf("%10.5f pf  Power Factor\n",modbus_get_float_dcba(dat+8));
    printf("%10.5f KWH Annual Power Consumption\n",modbus_get_float_dcba(dat+10));
    printf("%10.5f KWH Active Consumption\n",modbus_get_float_dcba(dat+12));
    printf("%10.5f KWH Reactive Consumption\n",modbus_get_float_dcba(dat+14));
    printf("%10.5f Hrs Load Time\n",modbus_get_float_dcba(dat+16)/100.0f);
    printf("%10d Hrs Work Hours per Day\n",dat[18]);
    printf("%10d     Device Address\n",dat[19]);

    modbus_close(ctx);
    modbus_free(ctx);

    return EXIT_SUCCESS;
}

