#include <coap.h>
#include <string.h>
#include <periph/gpio.h>

#define MAX_RESPONSE_LEN 500
static uint8_t response[MAX_RESPONSE_LEN] = { 0 };

static int handle_get_well_known_core( coap_rw_buffer_t *scratch,
                                       const coap_packet_t *inpkt,
                                       coap_packet_t *outpkt,
                                       uint8_t id_hi, uint8_t id_lo );

static int handle_get_servos( coap_rw_buffer_t *scratch,
                              const coap_packet_t *inpkt,
                              coap_packet_t *outpkt,
                              uint8_t id_hi, uint8_t id_lo );

static int handle_put_testled ( coap_rw_buffer_t *scratch,
                              const coap_packet_t *inpkt,
                              coap_packet_t *outpkt,
                              uint8_t id_hi, uint8_t id_lo );

static const coap_endpoint_path_t path_well_known_core =
    { 2, { ".wellknown", "core" } };

static const coap_endpoint_path_t path_servos =
    { 2, { "periph", "servos" } };

static const coap_endpoint_path_t path_testled =
    { 2, { "periph", "testled" } };

// https://tools.ietf.org/html/rfc7252 12.3.
// 0  = plain/text
// 40 = link-format
// 50 = json
const coap_endpoint_t endpoints[] = {
    { COAP_METHOD_GET, handle_get_well_known_core,
      &path_well_known_core, "ct=40" },
    { COAP_METHOD_GET, handle_get_servos,
      &path_servos, "ct=50" },
    { COAP_METHOD_PUT, handle_put_testled,
      &path_testled, "ct=0" },
    { (coap_method_t)0, NULL, NULL, NULL }
};

// Taken from RIOT example
static int handle_get_well_known_core(coap_rw_buffer_t *scratch,
        const coap_packet_t *inpkt, coap_packet_t *outpkt,
        uint8_t id_hi, uint8_t id_lo)
{
    char *rsp = (char *)response;
    /* resetting the content of response message */
    memset(response, 0, sizeof(response));
    int len = sizeof(response);
    const coap_endpoint_t *ep = endpoints;
    int i;

    len--; // Null-terminated string

    while (NULL != ep->handler) {
        if (NULL == ep->core_attr) {
            ep++;
            continue;
        }

        if (0 < strlen(rsp)) {
            strncat(rsp, ",", len);
            len--;
        }

        strncat(rsp, "<", len);
        len--;

        for (i = 0; i < ep->path->count; i++) {
            strncat(rsp, "/", len);
            len--;

            strncat(rsp, ep->path->elems[i], len);
            len -= strlen(ep->path->elems[i]);
        }

        strncat(rsp, ">;", len);
        len -= 2;

        strncat(rsp, ep->core_attr, len);
        len -= strlen(ep->core_attr);

        ep++;
    }

    return coap_make_response(scratch, outpkt, (const uint8_t *)rsp,
                              strlen(rsp), id_hi, id_lo, &inpkt->tok,
                              COAP_RSPCODE_CONTENT,
                              COAP_CONTENTTYPE_APPLICATION_LINKFORMAT);
}

static int handle_get_servos( coap_rw_buffer_t *scratch,
                              const coap_packet_t *inpkt,
                              coap_packet_t *outpkt,
                              uint8_t id_hi, uint8_t id_lo ) {
      const char *testresponse = "[ {\"name\":\"servoH\", \"unis\":2130}, {\"name\":\"servoL\", \"units\":70} ]";
      int len = strlen(testresponse);

      memcpy(response, testresponse, len);

      return coap_make_response(scratch, outpkt, (const uint8_t *)response,
                         len, id_hi, id_lo, &inpkt->tok,
                         COAP_RSPCODE_CONTENT,
                         (coap_content_type_t)50);
}

static int handle_put_testled ( coap_rw_buffer_t *scratch,
                              const coap_packet_t *inpkt,
                              coap_packet_t *outpkt,
                              uint8_t id_hi, uint8_t id_lo ) {
    uint8_t value = inpkt->payload.p[0];

    if(inpkt->payload.len != 1 || value > 1) {
        puts("Error, inpkt->len != 1");
    }
    else {
        if(value == 1)
            //gpio_set(GPIO_PIN(0, 06));
            puts("Set LED");
        else if(value == 0)
            //gpio_clear(GPIO_PIN(0, 06));
            puts("Reset LED");
        else
            puts("False value for LED");

        char valuehandle[2];
        snprintf(valuehandle, 2, "%u", value);

        char testresponse[20];
        strcpy(testresponse, "{ \"ledstatus\":");
        strncat(testresponse, valuehandle, strlen(testresponse));
        strncat(testresponse, " }", strlen(testresponse));

        int len = strlen(testresponse);

        memcpy(response, testresponse, len);

        return coap_make_response(scratch, outpkt, (const uint8_t *)response,
                            len, id_hi, id_lo, &inpkt->tok,
                            COAP_RSPCODE_CONTENT,
                            (coap_content_type_t)50);
    }

    return -1;
}
