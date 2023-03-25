/*
 *  io.c -- Network I/O routines
 */


/*  Header application  */

segment "fujinet";

#include "io.h"
#include "sp.h"
#include "trivial.h"
#include "jsmn.h"
#include <stdint.h>
#include <string.h>
#include <window.h>

#define MAXBUFF 1000

static char *url = "N:https://the-trivia-api.com/api/questions?limit=1";
static char JSON_STRING[MAXBUFF];

static void io_open_url(uint8_t mode, uint8_t trans, char *url);
static void io_close_url(void);
static size_t io_get_response(void);
static int jsoneq(const char *json, jsmntok_t *tok, const char *s);

int io_GetQuestion(tQuestion *Question)
{
  size_t len;
  int i;
  int r;
  int l;
  jsmn_parser p;
  jsmntok_t t[300]; /* We expect no more than 128 tokens */
  char message[100];

  Question->qCarte = 1;
  Question->qSujet = 1;
  Question->qDiff = 1;

  sp_open(sp_dest);

  io_open_url(4, 0, url);

  len = io_get_response();

  io_close_url();
  sp_close(sp_dest);

  jsmn_init(&p);
  r = jsmn_parse(&p, JSON_STRING, len, t,
                 sizeof(t) / sizeof(t[0]));
  if (r < 0) {
    sprintf(message, "70/Failed to parse JSON: %d/^#6", r);
    AlertWindow(0, NULL, (Ref)message);
    return 1;
  }

  /* Assume the top-level element is an object */
  if (t[0].type != JSMN_ARRAY) {
    sprintf(message, "70/Not an array/^#6");
    AlertWindow(0, NULL, (Ref)message);
    return 1;
  }

  for (i = 1; i < r; i++) {
    if (t[i].type == JSMN_OBJECT)
      i++;
    if (jsoneq(JSON_STRING, &t[i], "question") == 0) {
      l = t[i + 1].end - t[i + 1].start;
      if (l > 255)
        l = 255;
      memcpy(Question->qQuestion, JSON_STRING + t[i + 1].start, l);
      Question->qQuestion[l] = '\0';
      i++;
    } else if (jsoneq(JSON_STRING, &t[i], "correctAnswer") == 0) {
      l = t[i + 1].end - t[i + 1].start;
      if (l > 255)
        l = 255;
      memcpy(Question->qReponse, JSON_STRING + t[i + 1].start, l);
      Question->qReponse[l] = '\0';
      i++;
    }
  }


  return 0;
}

static void io_open_url(uint8_t mode, uint8_t trans, char *url)
{
  unsigned char idx = 0;
  uint16_t s;

  // to do - copy strings into payload and figure out length
  s = 1 + 1 + strlen(url);
  sp_payload[idx++] = (uint8_t)(s & 0xFF);
  sp_payload[idx++] = (uint8_t)(s >> 8);
  sp_payload[idx++] = mode;
  sp_payload[idx++] = trans;

  strcpy((char *)&sp_payload[idx++], url);

  sp_error = sp_control(sp_dest, 'O');
}

static void io_close_url(void)
{
  sp_payload[0] = 0;
  sp_payload[1] = 0;

  sp_error = sp_control(sp_dest, 'C');
}

static size_t io_get_response(void)
{
  unsigned short bw; // Bytes waiting
  unsigned short l = MAXBUFF;
  uint16_t pt = 0;

  do {
    sp_status(sp_dest, 'S');
    bw = sp_payload[0];
    bw |= (sp_payload[1]) << 8;
    if (bw == 0)
      break;
    if (bw > l) {
      AlertWindow(0, NULL, (Ref)"24/JSON buffer too small!/^#6");
      break;
    }
    memset(sp_payload, 0, sizeof(sp_payload));
    sp_read(sp_dest, bw);
    memcpy(&JSON_STRING[pt], sp_payload, bw);
    pt += bw;
    JSON_STRING[pt] = '\0';
    l -= bw;
  } while (true);
  return pt;
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}
