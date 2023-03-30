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

static void make_url(const tQuestion *question, char *url);
static void copy_json_data(uint8_t sp_net, tQuestion *question);
static void json_query(uint8_t sp_net, const char *query, char *str,
            size_t size);

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

  make_url(question, url);  // Build URL for trivial API

  if ((result = sp_open(sp_net)) > 0) // Open the network device
    return result;
  if ((result = net_open_url(sp_net, 0x0C, 0x80, url)) > 0) // Open URL
    return result;
  if ((result = net_set_json(sp_net)) > 0)
    return result;
  if ((result = sp_control(sp_net,'P')) > 0) // Parse the JSON
    return result;

  copy_json_data(sp_net, question); // Copy JSON data to question

  if ((result = net_close_url(sp_net)) > 0)  // Close URL
    return result;
  return sp_close(sp_net); // Close the network device and return
}

static void make_url(const tQuestion *question, char *url)
{
  static const char category_parm[][20] =
    {"geography","film_and_tv","history","arts_and_literature","science",
    "sport_and_leisure"};
  static const char difficulty_parm[][8] = {"easy","medium","hard","hard"};

  sprintf(url, "N:https://the-trivia-api.com/api/questions?limit=1&categories=%s&difficulty=%s",
    category_parm[question->qSujet-1], difficulty_parm[question->qDiff-1]);
}

static void copy_json_data(uint8_t sp_net, tQuestion *question)
{
  static const char category_query[] = "/0/category";
  static const char id_query[] = "/0/id";
  static const char correctAnswer_query[] = "/0/correctAnswer";
  static const char incorrectAnswer0_query[] = "/0/incorrectAnswers/0";
  static const char incorrectAnswer1_query[] = "/0/incorrectAnswers/1";
  static const char incorrectAnswer2_query[] = "/0/incorrectAnswers/2";
  static const char question_query[] = "/0/question";
  static const char type_query[] = "/0/type";
  static const char difficulty_query[] = "/0/difficulty";

  question->qCarte = 0;
  json_query(sp_net, category_query, question->category,
             sizeof(question->category)-1);
  json_query(sp_net, id_query, question->id, sizeof(question->id)-1);
  json_query(sp_net, correctAnswer_query, question->qReponse,
             sizeof(question->qReponse)-1);
  json_query(sp_net, incorrectAnswer0_query, question->incorrectAnswer[0],
             sizeof(question->incorrectAnswer[0])-1);
  json_query(sp_net, incorrectAnswer1_query, question->incorrectAnswer[1],
             sizeof(question->incorrectAnswer[1])-1);
  json_query(sp_net, incorrectAnswer2_query, question->incorrectAnswer[2],
             sizeof(question->incorrectAnswer[2])-1);
  json_query(sp_net, question_query, question->qQuestion,
             sizeof(question->qQuestion)-1);
  json_query(sp_net, type_query, question->type, sizeof(question->type)-1);
  json_query(sp_net, difficulty_query, question->difficulty,
             sizeof(question->difficulty)-1);
}


static void json_query(uint8_t sp_net, const char *query, char *str,
            size_t size)
{
  unsigned idx = 0;
  unsigned short len;

  memset(sp_payload, 0, sizeof(sp_payload));
  sp_payload[idx++] = strlen(query);
  sp_payload[idx++] = 0;
  strcpy(&sp_payload[idx++], query);
  sp_control(sp_net, 'Q'); // Query
  sp_status(sp_net, 'S'); // Get Status
  len=(unsigned short)sp_payload[0];
  memset(sp_payload, 0, sizeof(sp_payload));
  sp_read(sp_net, len); // Get Result
  sp_payload[len] = '\0';
  strncpy(str, sp_payload, size); // Copy to dest
}
