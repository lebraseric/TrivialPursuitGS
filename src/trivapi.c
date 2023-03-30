/*
 *  trivapi.c -- The Trivia API queries
 */


segment "fujinet";

#include "trivapi.h"
#include "net.h"
#include "sp.h"
#include "trivial.h"
#include <stdint.h>
#include <string.h>
#include <window.h>

static const char category_query[] = "/0/category";
static const char id_query[] = "/0/id";
static const char correctAnswer_query[] = "/0/correctAnswer";
static const char incorrectAnswer0_query[] = "/0/incorrectAnswers/0";
static const char incorrectAnswer1_query[] = "/0/incorrectAnswers/1";
static const char incorrectAnswer2_query[] = "/0/incorrectAnswers/2";
static const char question_query[] = "/0/question";
static const char type_query[] = "/0/type";
static const char difficulty_query[] = "/0/difficulty";


static void make_url(const tQuestion *question, const char *url);
static void json_query(const char *query);

/*
 * Function:  trivapi_GetQuestion
 * ------------------------------
 *  Gets a question and answer from the-trivia-api.com.
 *
 *  question: structure where the question data is put
 *            On entry, question->qSujet must contain the desired category (0
 *            through 5), question->qDiff must contain the desired difficulty
 *            (1 through 4).
 *
 *  returns: zero if OK
 *           returns non zero on error
 */

int trivapi_GetQuestion(tQuestion *question)
{
  const char url[120];
  int result;

  make_url(question, url);

  question->qCarte = 0;

  // Open
  if ((result = sp_open(sp_net)) > 0)
    return result;
  if ((result = net_open_url(sp_net, 0x0C, 0x80, url)) > 0)
    return result;

  // Set channel mode
  sp_payload[0]=0x01; // length of packet.
  sp_payload[1]=0x00;
  sp_payload[2]=0x01; // Set to JSON mode
  sp_control(sp_net,0xFC); // Do it.

  // Parse the JSON
  sp_control(sp_net,'P'); // Do the parse
  if (sp_error > 0)
    AlertWindow(0, NULL, (Ref)"24/JSON parsing error!/^#6");

  json_query(category_query);
  strncpy(question->category, sp_payload, sizeof(question->category)-1);
  json_query(id_query);
  strncpy(question->id, sp_payload, sizeof(question->id)-1);
  json_query(correctAnswer_query);
  strncpy(question->qReponse, sp_payload, sizeof(question->qReponse)-1);
  json_query(incorrectAnswer0_query);
  strncpy(question->incorrectAnswer[0], sp_payload, sizeof(question->incorrectAnswer[0])-1);
  json_query(incorrectAnswer1_query);
  strncpy(question->incorrectAnswer[1], sp_payload, sizeof(question->incorrectAnswer[1])-1);
  json_query(incorrectAnswer2_query);
  strncpy(question->incorrectAnswer[2], sp_payload, sizeof(question->incorrectAnswer[2])-1);
  json_query(question_query);
  strncpy(question->qQuestion, sp_payload, sizeof(question->qQuestion)-1);
  json_query(type_query);
  strncpy(question->type, sp_payload, sizeof(question->type)-1);
  json_query(difficulty_query);
  strncpy(question->difficulty, sp_payload, sizeof(question->difficulty)-1);

  sp_payload[0] = 0;
  sp_payload[1] = 0;
  sp_control(sp_net, 'C');
  
  sp_close(sp_net);

  return 0;
}

static void make_url(const tQuestion *question, const char *url)
{
  static const char category_parm[][20] =
    {"geography","film_and_tv","history","arts_and_literature","science","sport_and_leisure"};
  static const char difficulty_parm[][8] = {"easy","medium","hard","hard"};

  sprintf(url, "N:https://the-trivia-api.com/api/questions?limit=1&categories=%s&difficulty=%s",
    category_parm[question->qSujet-1], difficulty_parm[question->qDiff-1]);
}

static void json_query(const char *query)
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
