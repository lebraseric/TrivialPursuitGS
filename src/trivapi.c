/*
 *  trivapi.c -- Network I/O routines
 */


/*  Header application  */

segment "fujinet";

#include "trivapi.h"
#include "sp.h"
#include "trivial.h"
#include <stdint.h>
#include <string.h>
#include <window.h>

static char *url = "N:https://the-trivia-api.com/api/questions?limit=1";
// static char message[100];

static void trivapi_json_query(const char *query);

int trivapi_GetQuestion(tQuestion *Question)
{
  const char category_query[]="/0/category";
  const char id_query[]="/0/id";
  const char correctAnswer_query[]="/0/correctAnswer";
  const char question_query[]="/0/question";
  const char type_query[]="/0/type";
  const char difficulty_query[]="/0/difficulty";

  Question->qCarte = 1;
  Question->qSujet = 1;
  Question->qDiff = 1;

  // Open
  sp_open(sp_net);
  if (sp_error > 0)
    AlertWindow(0, NULL, (Ref)"24/Failed to open network device!/^#6");
  sp_payload[0]=(strlen(url) & 0xFF) + 2;
  sp_payload[1]=(strlen(url) >> 8);
  sp_payload[2]=0x0C; // GET
  sp_payload[3]=0x80; // No translation
  memcpy(&sp_payload[4],url,strlen(url));
  sp_control(sp_net,'O'); // Do open.
  if (sp_error > 0)
    AlertWindow(0, NULL, (Ref)"24/Failed to open URL!/^#6");

  // Set channel mode
  sp_payload[0]=0x01; // length of packet.
  sp_payload[1]=0x00;
  sp_payload[2]=0x01; // Set to JSON mode
  sp_control(sp_net,0xFC); // Do it.

  // Parse the JSON
  sp_control(sp_net,'P'); // Do the parse
  if (sp_error > 0)
    AlertWindow(0, NULL, (Ref)"24/JSON parsing error!/^#6");

  trivapi_json_query(category_query);
  strncpy(Question->category, sp_payload, sizeof(Question->category)-1);
  trivapi_json_query(id_query);
  strncpy(Question->id, sp_payload, sizeof(Question->id)-1);
  trivapi_json_query(correctAnswer_query);
  strncpy(Question->qReponse, sp_payload, sizeof(Question->qReponse)-1);
  trivapi_json_query(question_query);
  strncpy(Question->qQuestion, sp_payload, sizeof(Question->qQuestion)-1);
  trivapi_json_query(type_query);
  strncpy(Question->type, sp_payload, sizeof(Question->type)-1);
  trivapi_json_query(difficulty_query);
  strncpy(Question->difficulty, sp_payload, sizeof(Question->difficulty)-1);

  sp_payload[0] = 0;
  sp_payload[1] = 0;
  sp_control(sp_net, 'C');
  
  sp_close(sp_net);
  // sprintf(message, "90/Question=%.8lX/^#6", Question);
  // AlertWindow(0, NULL, (Ref)message);

  return 0;
}

static void trivapi_json_query(const char *query)
{
  unsigned short len;

  memset(sp_payload, 0, sizeof(sp_payload));
  sp_payload[0] = strlen(query);
  sp_payload[1] = 0;
  strcpy(&sp_payload[2], query);
  sp_control(sp_net, 'Q'); // Query
  sp_status(sp_net, 'S'); // Get Status
  len=(unsigned short)sp_payload[0];
  memset(sp_payload, 0, sizeof(sp_payload));
  sp_read(sp_net, len); // Get Result
  sp_payload[len] = '\0';
}
