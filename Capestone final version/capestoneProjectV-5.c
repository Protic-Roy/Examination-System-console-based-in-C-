#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_USERNAME 50
#define MAX_PASSWORD 50

#define ADMIN_FILE "admin.txt"
#define STUDENT_FILE "student.txt"
#define MASTER_PASSWORD "roy&esha@20696"
#define QUESTION_FILE "questions.txt"
#define EXAM_FILE "exams.txt"
#define RESULT_FILE "results.txt"

typedef struct
{
  int id;
  char username[MAX_USERNAME];
  char password[MAX_PASSWORD];
  char fullname[200];
} user;

typedef struct
{
  int qid;
  char type; // M - MCQ S - Short Answer
  char quesTxt[500];
  char optionA[200];
  char optionB[200];
  char optionC[200];
  char optionD[200];
  char correctOption; // For MCQ
  char correctAnswer[200]; // For Short Answer
  int marks;
} question;

typedef struct
{
  char examid[55];
  char examTitle[200];
  int duration;
  int totalQuestions;
  int totalMarks;
} exam;

void displayHeader(const char *title);
void removeNewline(char *str);
void clear_input_buffer();
int check_username_exists(const char *username);
int check_admin_exists(const char *username);
int isValidName(const char *name);
int check_student_id_exists(int id);
int isValidUsername(const char *username);//>3 or not
void registerUser();
void registerAdmin();
int login_user();
int login_admin();
void adminMenu(char *admin_name);
void studentMenu(char *student_name);
void displayStudent();
void mainMenu();
void createExam();
void createQuestion();
void savequestion(question que, char ExamID[]);
void saveexam(exam ex);
int getExamDuration(char *examid);
void takeExam(char Examid[], char *studentName);
void viewExams();
void viewResults(char *searchName);
void viewAllResults();

void displayHeader(const char *title)
{
  printf("\n-----------------------------------------------------------------\n");
  printf("                 ONLINE EXAMINATION SYSTEM\n");
  printf("-----------------------------------------------------------------\n");
  printf("                     [%s]\n", title);
  printf("-----------------------------------------------------------------\n\n");
}

void removeNewline(char *str)
{
  int len = strlen(str);
  if (len > 0 && str[len - 1] == '\n')
  {
    str[len - 1] = '\0';
  }
}

void clear_input_buffer()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

int check_username_exists(const char *username)
{
  FILE *fp;
  char line[500];
  char stored_username[MAX_USERNAME];
  fp = fopen(STUDENT_FILE, "r");
  if (fp == NULL)
  {
    return 0;
  }

  while (fgets(line, sizeof(line), fp) != NULL)
  {
    if (sscanf(line, "%[^|]", stored_username) == 1)
    {
      if (strcmp(stored_username, username) == 0)
      {
        fclose(fp);
        return 1;
      }
    }
  }
  fclose(fp);
  return 0;
}
int check_admin_exists(const char *username)
{
  FILE *fp;
  char line[500];
  char stored_username[MAX_USERNAME];
  fp = fopen(ADMIN_FILE, "r");
  if (fp == NULL)
  {

    return 0;
  }
  while (fgets(line, sizeof(line), fp) != NULL)
  {
    if (sscanf(line, "%*d|%[^|]", stored_username) == 1)
    {
      if (strcmp(stored_username, username) == 0)
      {
        fclose(fp);
        return 1;
      }
    }
  }
  fclose(fp);
  return 0;
}

int isValidName(const char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ')
            return 0;
    }
    return 1;
}
int check_student_id_exists(int id)
{
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) return 0;

    char line[500];
    int existing_id;

    while (fgets(line, sizeof(line), fp))
    {
        if (sscanf(line, "%d|", &existing_id) == 1)
        {
            if (existing_id == id)
            {
                fclose(fp);
                return 1;  // ID already exists
            }
        }
    }
    fclose(fp);
    return 0;
}
int isValidUsername(const char *username) {
    return strlen(username) >= 3;
}

void registerUser()
{
    FILE *fp;
    user newuser;

    displayHeader("Student Registration");
    clear_input_buffer();

    printf("Enter Student ID: ");
    if (scanf("%d", &newuser.id) != 1) {
        printf("Invalid ID! Must be numbers only.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (check_student_id_exists(newuser.id))
{
    printf("Error: Student ID %d is already registered!\n", newuser.id);
    return;
}
    printf("Enter full name: ");
    fgets(newuser.fullname, sizeof(newuser.fullname), stdin);
    removeNewline(newuser.fullname);

    if (!isValidName(newuser.fullname)) {
        printf("Invalid name! Only letters and spaces allowed.\n");
        return;
    }

    printf("Enter username: ");
    fgets(newuser.username, sizeof(newuser.username), stdin);
    removeNewline(newuser.username);

    if (!isValidUsername(newuser.username)) {
        printf("Username must be at least 3 characters long!\n");
        return;
    }

    if (check_username_exists(newuser.username)) {
        printf("Username '%s' already exists! Choose another.\n", newuser.username);
        return;
    }

    printf("Enter password: ");
    fgets(newuser.password, sizeof(newuser.password), stdin);
    removeNewline(newuser.password);

    if (strlen(newuser.password) == 0) {
        printf("Password cannot be empty!\n");
        return;
    }

    fp = fopen(STUDENT_FILE, "a");
    if (fp == NULL) {
        printf("Error: Unable to open Student File\n");
        return;
    }

    fprintf(fp, "%d|%s|%s|%s\n", newuser.id, newuser.username, newuser.password, newuser.fullname);
    fclose(fp);

    printf("Registration successful!\n");
}

void registerAdmin()
{
    FILE *fp;
    user newadmin;
    char master_password[MAX_PASSWORD];

    displayHeader("ADMIN REGISTRATION");
    clear_input_buffer();

    printf("Enter Master Password: ");
    fgets(master_password, sizeof(master_password), stdin);
    removeNewline(master_password);

    if (strcmp(master_password, MASTER_PASSWORD) != 0) {
        printf("Access Denied! Invalid master password.\n");
        return;
    }

    printf("\nMaster password verified!\n");

    printf("Enter Admin Full Name: ");
    fgets(newadmin.fullname, sizeof(newadmin.fullname), stdin);
    removeNewline(newadmin.fullname);

    if (!isValidName(newadmin.fullname)) {
        printf("Invalid name! Only letters and spaces allowed.\n");
        return;
    }

    printf("Enter Admin Username: ");
    fgets(newadmin.username, sizeof(newadmin.username), stdin);
    removeNewline(newadmin.username);

    if (!isValidUsername(newadmin.username)) {
        printf("Username must be at least 4 characters long!\n");
        return;
    }

    if (check_admin_exists(newadmin.username)) {
        printf("Admin username '%s' already exists!\n", newadmin.username);
        return;
    }

    printf("Enter Admin Password: ");
    fgets(newadmin.password, sizeof(newadmin.password), stdin);
    removeNewline(newadmin.password);

    if (strlen(newadmin.password) == 0) {
        printf("Password cannot be empty!\n");
        return;
    }

    fp = fopen(ADMIN_FILE, "a");
    if (fp == NULL) {
        printf("Error: Unable to open admin.txt\n");
        return;
    }

    fprintf(fp, "%s|%s|%s\n", newadmin.username, newadmin.password, newadmin.fullname);
    fclose(fp);

    printf("\nAdmin registration successful!\n");
}

int login_user()
{
  FILE *fp;
  user input_user, stored_user;
  char line[500];
  displayHeader("Student Login");
  clear_input_buffer();

  printf("Enter username:");
  if (fgets(input_user.username, sizeof(input_user.username), stdin) == NULL)
  {
    return 0;
  }
  removeNewline(input_user.username);

  printf("Enter password:");
  if (fgets(input_user.password, sizeof(input_user.password), stdin) == NULL)
  {
    return 0;
  }
  removeNewline(input_user.password);

  fp = fopen(STUDENT_FILE, "r");
  if (fp == NULL)
  {
    printf("Error!! Login Failed\n");
    return 0;
  }

  while (fgets(line, sizeof(line), fp) != NULL)
  {
    if (sscanf(line, "%d|%[^|]|%[^|]|%[^\n]", &stored_user.id, stored_user.username, stored_user.password, stored_user.fullname) == 4)
    {
      if (strcmp(stored_user.username, input_user.username) == 0 && strcmp(stored_user.password, input_user.password) == 0)
      {
        fclose(fp);
        printf("Login Successful! Welcome %s\n", stored_user.fullname);
        studentMenu(stored_user.fullname);
        return 1;
      }
    }
  }
  fclose(fp);
  printf("Login Failed! Invalid user name or password.\n");
  return 0;
}

// login for admin
int login_admin()
{
  FILE *fp;
  user input_user, stored_user;
  char line[500];

  displayHeader("Admin Login");
  clear_input_buffer();

  printf("Enter admin username: ");
  if (fgets(input_user.username, sizeof(input_user.username), stdin) == NULL)
  {
    return 0;
  }
  removeNewline(input_user.username);

  printf("Enter admin password: ");
  if (fgets(input_user.password, sizeof(input_user.password), stdin) == NULL)
  {
    return 0;
  }
  removeNewline(input_user.password);

  fp = fopen(ADMIN_FILE, "r");
  if (fp == NULL)
  {
    printf("Error!! No admin accounts found.\n");
    return 0;
  }
  while (fgets(line, sizeof(line), fp) != NULL)
  {
    if (sscanf(line, "%[^|]|%[^|]|%[^\n]", stored_user.username, stored_user.password, stored_user.fullname) == 3)
    {
      if (strcmp(stored_user.username, input_user.username) == 0 && strcmp(stored_user.password, input_user.password) == 0)
      {
        fclose(fp);
        printf("Login Successful! Welcome Admin %s\n", stored_user.fullname);
        adminMenu(stored_user.fullname);
        return 1;
      }
    }
  }
  fclose(fp);
  printf("Login Failed! Invalid admin username or password.\n");
  return 0;
}
void displayStudent()
{
  FILE *fp = fopen(STUDENT_FILE, "r");
  char line[500];
  user student;
  int count = 0;
  displayHeader("Registered Students");
  if (fp == NULL)
  {
    printf("No students registered yet.\n");
    return;
  }
  printf("--------------------------------------------------------\n");
  printf("%-6s %-20s %-30s\n", "ID", "Username", "Full Name");
  printf("--------------------------------------------------------\n");
  while (fgets(line, sizeof(line), fp) != NULL)
  {
    if (sscanf(line, "%d|%[^|]|%*[^|]|%[^\n]", &student.id, student.username, student.fullname) == 3)
    {
      printf("%-6d %-20s %-30s\n", student.id, student.username, student.fullname);
      count++;
    }
  }
  fclose(fp);
  printf("\nTotal students: %d\n", count);
}

void createQuestion()
{
  displayHeader("Create New Question");
  viewExams();
  char exid[55];
  printf("\nEnter Exam ID to add the question to: ");
  clear_input_buffer();
  if (fgets(exid, sizeof(exid), stdin) == NULL)
  {
    return;
  }
  removeNewline(exid);
  question newquestion;
  clear_input_buffer();

  printf("Enter Question ID: ");
  scanf("%d", &newquestion.qid);
  clear_input_buffer();

  printf("Enter Question Type (M for MCQ, S for Short Answer): ");
  scanf(" %c", &newquestion.type);
  newquestion.type = toupper(newquestion.type);
  clear_input_buffer();

  printf("Enter marks for this question: ");
  scanf("%d", &newquestion.marks);
  if (newquestion.marks < 1) newquestion.marks = 1;
  clear_input_buffer();

  printf("Enter Question Text: ");
  fgets(newquestion.quesTxt, sizeof(newquestion.quesTxt), stdin);
  removeNewline(newquestion.quesTxt);

  if (newquestion.type == 'M')
  {
    printf("Enter Option A: ");
    fgets(newquestion.optionA, sizeof(newquestion.optionA), stdin);
    removeNewline(newquestion.optionA);

    printf("Enter Option B: ");
    fgets(newquestion.optionB, sizeof(newquestion.optionB), stdin);
    removeNewline(newquestion.optionB);

    printf("Enter Option C: ");
    fgets(newquestion.optionC, sizeof(newquestion.optionC), stdin);
    removeNewline(newquestion.optionC);

    printf("Enter Option D: ");
    fgets(newquestion.optionD, sizeof(newquestion.optionD), stdin);
    removeNewline(newquestion.optionD);

    printf("Enter Correct Option (A/B/C/D): ");
    scanf(" %c", &newquestion.correctOption);
    newquestion.correctOption = toupper(newquestion.correctOption);
    clear_input_buffer();
    strcpy(newquestion.correctAnswer, ""); // Empty for MCQ
  }
  else if (newquestion.type == 'S')
  {
    printf("Enter Correct Short Answer: ");
    fgets(newquestion.correctAnswer, sizeof(newquestion.correctAnswer), stdin);
    removeNewline(newquestion.correctAnswer);
    strcpy(newquestion.optionA, "");
    strcpy(newquestion.optionB, "");
    strcpy(newquestion.optionC, "");
    strcpy(newquestion.optionD, "");
    newquestion.correctOption = '\0';
  }
  else
  {
    printf("Invalid type! Question not saved.\n");
    return;
  }

  savequestion(newquestion, exid);
  printf("Question created and saved successfully!\n");
}
void savequestion(question que, char ExamID[])
{
  FILE *fp = fopen(QUESTION_FILE, "a");
  if (fp == NULL)
  {
    printf("Error: Unable to open QUESTION FILE!!\n");
    return;
  }
  fprintf(fp, "ExamID: %s\n", ExamID);
  fprintf(fp, "Q.ID: %d\n", que.qid);
  fprintf(fp, "Type: %c\n", que.type);
  fprintf(fp, "Marks: %d\n", que.marks);
  fprintf(fp, "Question: %s\n", que.quesTxt);
  if (que.type == 'M')
  {
    fprintf(fp, "A: %s\n", que.optionA);
    fprintf(fp, "B: %s\n", que.optionB);
    fprintf(fp, "C: %s\n", que.optionC);
    fprintf(fp, "D: %s\n", que.optionD);
    fprintf(fp, "CorrectOption: %c\n", que.correctOption);
  }
  else if (que.type == 'S')
  {
    fprintf(fp, "CorrectAnswer: %s\n", que.correctAnswer);
  }
  fprintf(fp, "---\n");
  fclose(fp);
}

void createExam()
{
  displayHeader("Create New Examination");
  exam newexam;
  clear_input_buffer();

  printf("Enter Exam ID: ");
  fgets(newexam.examid, sizeof(newexam.examid), stdin);
  removeNewline(newexam.examid);

  printf("Enter Exam Title: ");
  fgets(newexam.examTitle, sizeof(newexam.examTitle), stdin);
  removeNewline(newexam.examTitle);

  printf("Enter Duration (in minutes): ");
  scanf("%d", &newexam.duration);
  if (newexam.duration < 1) newexam.duration = 1;

  printf("Enter Total Questions: ");
  scanf("%d", &newexam.totalQuestions);
  if (newexam.totalQuestions < 1) newexam.totalQuestions = 1;

  printf("Enter Total Marks: ");
  scanf("%d", &newexam.totalMarks);
  if (newexam.totalMarks < 0) newexam.totalMarks = 0;

  clear_input_buffer();
  saveexam(newexam);
}

void saveexam(exam ex)
{
  FILE *fp = fopen(EXAM_FILE, "a");
  if (fp == NULL)
  {
    printf("Error: Unable to open EXAM FILE!!\n");
    return;
  }
  fprintf(fp, "ExamID: %s\n", ex.examid);
  fprintf(fp, "Title: %s\n", ex.examTitle);
  fprintf(fp, "Duration: %d minutes\n", ex.duration);
  fprintf(fp, "TotalQuestions: %d\n", ex.totalQuestions);
  fprintf(fp, "TotalMarks: %d\n", ex.totalMarks);
  fprintf(fp, "---\n");
  fclose(fp);
  printf("Examination created and saved successfully!\n");
}
void viewExams()
{
  FILE *fp = fopen("exams.txt", "r");
  if (fp == NULL)
  {
    printf("No exams available.\n");
    return;
  }

  char line[200];
  printf("\n===== Available Exams =====\n");
  while (fgets(line, sizeof(line), fp))
  {
    if (strncmp(line, "ExamID:", 7) == 0)
    {
      char examid[20], title[100];
      int duration, totalQ, totalM;

      sscanf(line, "ExamID: %[^\n]", examid);
      fgets(line, sizeof(line), fp);
      sscanf(line, "Title: %[^\n]", title);
      fgets(line, sizeof(line), fp);
      sscanf(line, "Duration: %d", &duration);
      fgets(line, sizeof(line), fp);
      sscanf(line, "TotalQuestions: %d", &totalQ);
      fgets(line, sizeof(line), fp);
      sscanf(line, "TotalMarks: %d", &totalM);
      fgets(line, sizeof(line), fp);

      printf("\nExam ID: %s\n", examid);
      printf("Title: %s\n", title);
      printf("Duration: %d minutes\n", duration);
      printf("Questions: %d\n", totalQ);
      printf("Marks: %d\n", totalM);
    }
  }
  fclose(fp);
}

// timer function
int getExamDuration(char *examid)
{
  FILE *fp = fopen(EXAM_FILE, "r");
  if (fp == NULL)
    return 30;

  char line[200];
  while (fgets(line, sizeof(line), fp))
  {
    if (strncmp(line, "ExamID:", 7) == 0)
    {
      char storedID[55];
      sscanf(line, "ExamID: %[^\n]", storedID);

      if (strcmp(storedID, examid) == 0)
      {
        fgets(line, sizeof(line), fp); // Skip title
        fgets(line, sizeof(line), fp); // Duration line

        int duration;
        sscanf(line, "Duration: %d", &duration);
        fclose(fp);
        return duration;
      }
    }
  }
  fclose(fp);
  return 30;
}

// Count questions for an exam
int countQuestionsForExam(char *examid)
{
  FILE *fp = fopen(QUESTION_FILE, "r");
  if (fp == NULL)
    return 0;
  
  char line[500];
  char currentExamID[55];
  int count = 0;
  
  while (fgets(line, sizeof(line), fp) != NULL)
  {
    if (strncmp(line, "ExamID:", 7) == 0)
    {
      sscanf(line, "ExamID: %[^\n]", currentExamID);
      if (strcmp(currentExamID, examid) == 0)
      {
        while (fgets(line, sizeof(line), fp) && strncmp(line, "---", 3) != 0)
        {
          if (strncmp(line, "Q.ID:", 5) == 0)
            count++;
        }
      }
    }
  }
  fclose(fp);
  return count;
}
void shuffleArray(int arr[], int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void takeExam(char Examid[], char *studentName)
{
  displayHeader("Take Examination");

  time_t startTime = time(NULL);
  int examDuration = getExamDuration(Examid);
  int timeLimit = examDuration * 60;

  printf("\nExam Duration: %d minutes\n", examDuration);
  printf("Press Enter to start...");
  clear_input_buffer();
  getchar();
  startTime = time(NULL);

  // Collect all question IDs first
  FILE *fp = fopen(QUESTION_FILE, "r");
  if (!fp) {
      printf("Error opening questions file!\n");
      return;
  }

  int qids[100], qcount = 0;
  char line[500], currentExamID[55];

  while (fgets(line, sizeof(line), fp))
  {
      if (strncmp(line, "ExamID:", 7) == 0)
      {
          sscanf(line, "ExamID: %[^\n]", currentExamID);
          if (strcmp(currentExamID, Examid) == 0)
          {
              while (fgets(line, sizeof(line), fp) && strncmp(line, "---", 3) != 0)
              {
                  if (strncmp(line, "Q.ID:", 5) == 0)
                  {
                      int id;
                      sscanf(line, "Q.ID: %d", &id);
                      qids[qcount++] = id;
                  }
              }
          }
      }
  }
  fclose(fp);

  if (qcount == 0) {
      printf("No questions found!\n");
      return;
  }

  // Shuffle questions
  shuffleArray(qids, qcount);

  printf("\n*** EXAM STARTED - %d minutes ***\n\n", examDuration);

  int score = 0, totalQ = qcount, totalMarks = 0, obtainedMarks = 0;
  int timefinish = 0;

  for (int i = 0; i < qcount && !timefinish; i++)
  {
      fp = fopen(QUESTION_FILE, "r");
      int target_id = qids[i];
      int found = 0;

      while (fgets(line, sizeof(line), fp) && !found)
      {
          if (strncmp(line, "Q.ID:", 5) == 0)
          {
              int thisid;
              sscanf(line, "Q.ID: %d", &thisid);
              if (thisid == target_id)
              {
                  found = 1;

                  // Read question details
                  int qmarks = 1;
                  char type, questxt[500], optionA[200]="", optionB[200]="", optionC[200]="", optionD[200]="";
                  char correctOption = 0;
                  char correctAnswer[200];

                  fgets(line, sizeof(line), fp); sscanf(line, "Type: %c", &type);
                  fgets(line, sizeof(line), fp); sscanf(line, "Marks: %d", &qmarks);
                  fgets(line, sizeof(line), fp); sscanf(line, "Question: %[^\n]", questxt);

                  if (type == 'M')
                  {
                      fgets(line, sizeof(line), fp); sscanf(line, "A: %[^\n]", optionA);
                      fgets(line, sizeof(line), fp); sscanf(line, "B: %[^\n]", optionB);
                      fgets(line, sizeof(line), fp); sscanf(line, "C: %[^\n]", optionC);
                      fgets(line, sizeof(line), fp); sscanf(line, "D: %[^\n]", optionD);
                      fgets(line, sizeof(line), fp); sscanf(line, "CorrectOption: %c", &correctOption);
                  }
                  else if (type == 'S')
                  {
                      fgets(line, sizeof(line), fp); sscanf(line, "CorrectAnswer: %[^\n]", correctAnswer);
                  }

                  totalMarks += qmarks;

                  // Time check
                  time_t now = time(NULL);
                  int elapsed = (int)difftime(now, startTime);
                  if (elapsed >= timeLimit) {
                      printf("\n*** TIME'S UP! ***\n");
                      timefinish = 1;
                      break;
                  }
                  printf("[Time Left: %d min]\n", (timeLimit - elapsed)/60);

                  printf("\nQ%d (%d marks): %s\n", thisid, qmarks, questxt);

                  if (type == 'M')
                  {
                      printf("A. %s\nB. %s\nC. %s\nD. %s\n", optionA, optionB, optionC, optionD);
                      printf("Your answer: ");
                      char ans;
                      scanf(" %c", &ans);
                      ans = toupper(ans);
                      clear_input_buffer();
                      if (ans == correctOption) {
                          printf("Correct! +%d\n", qmarks);
                          obtainedMarks += qmarks;
                          score++;
                      } else {
                          printf("Wrong! Correct: %c\n", correctOption);
                      }
                  }
                  else if (type == 'S')
                  {
                      char userAns[200];
                      printf("Your answer: ");
                      fgets(userAns, sizeof(userAns), stdin);
                      removeNewline(userAns);
                      if (strcasecmp(userAns, correctAnswer) == 0) {
                          printf("Correct! +%d\n", qmarks);
                          obtainedMarks += qmarks;
                          score++;
                      } else {
                          printf("Wrong! Answer: %s\n", correctAnswer);
                      }
                  }
              }
          }
      }
      fclose(fp);
  }

  float percentage;

if (totalMarks > 0) {
    percentage = ((float)obtainedMarks / totalMarks) * 100;
} else {
    percentage = 0;
}

  printf("\n========== EXAM COMPLETED ==========\n");
  printf("Student     : %s\n", studentName);
  printf("Exam ID     : %s\n", Examid);
  printf("Correct     : %d/%d questions\n", score, totalQ);
  printf("Marks       : %d / %d\n", obtainedMarks, totalMarks);
  printf("Percentage  : %.2f%%\n", percentage);
  printf("====================================\n");

  int studentID = -1;
  FILE *sp = fopen(STUDENT_FILE, "r");
  if (sp) {
      char line[500];
      int id;
      char fullname[200];
      while (fgets(line, sizeof(line), sp)) {
          if (sscanf(line, "%d|%*[^|]|%*[^|]|%199[^\n]", &id, fullname) == 2) {
              removeNewline(fullname);
              if (strcmp(fullname, studentName) == 0) {
                  studentID = id;
                  break;
              }
          }
      }
      fclose(sp);
  }

  // Save result WITH Student ID
  FILE *rfp = fopen(RESULT_FILE, "a");
  if (rfp)
  {
      fprintf(rfp, "StudentID: %d\n", studentID);
      fprintf(rfp, "Name: %s\n", studentName);
      fprintf(rfp, "ExamID: %s\n", Examid);
      fprintf(rfp, "Score: %d/%d\n", score, totalQ);
      fprintf(rfp, "Marks: %d/%d\n", obtainedMarks, totalMarks);
      fprintf(rfp, "Percentage: %.2f%%\n", percentage);
      fprintf(rfp, "---\n");
      fclose(rfp);
  }
}

void viewResults(char *searchName)
{
    FILE *fp = fopen(RESULT_FILE, "r");
    if (fp == NULL)
    {
        printf("No results found.\n");
        return;
    }

    char line[300];
    char tempName[100];
    int found = 0;

    displayHeader("MY RESULTS");

    while (fgets(line, sizeof(line), fp))
    {
        if (sscanf(line, "Name: %99[^\n]", tempName) == 1)
        {
            if (strcmp(tempName, searchName) == 0)
            {
                found = 1;
                printf("\n=== Your Result ===\n");
                printf("%s", line);                                   // Name:
                fgets(line, sizeof(line), fp); printf("%s", line);    // ExamID:
                fgets(line, sizeof(line), fp); printf("%s", line);    // Score:
                fgets(line, sizeof(line), fp); printf("%s", line);    // Marks:
                fgets(line, sizeof(line), fp); printf("%s", line);    // Percentage:
                fgets(line, sizeof(line), fp);                        // skip ---
                printf("====================\n\n");
            }
            else
            {
                // Skip this student's 5 lines
                for (int i = 0; i < 5; i++)
                    fgets(line, sizeof(line), fp);
            }
        }
    }
    fclose(fp);

    if (!found)
        printf("No result found for '%s'.\n", searchName);
}
//  all student results
void viewAllResults()
{
    FILE *fp = fopen(RESULT_FILE, "r");
    if (fp == NULL)
    {
        printf("No results found yet.\n");
        return;
    }

    displayHeader("ALL STUDENT RESULTS");

    char line[300];
    int studentID = 0;
    char name[100], examid[60], scoreStr[30], marksStr[30], percentStr[30];
    int count = 0;

    printf("%-8s %-20s %-15s %-12s %-12s %s\n", 
           "ID", "Student Name", "Exam ID", "Score", "Marks", "Percentage");
    printf("--------------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), fp))
    {
        if (sscanf(line, "StudentID: %d", &studentID) == 1)
        {
            fgets(line, sizeof(line), fp); sscanf(line, "Name: %99[^\n]", name);
            fgets(line, sizeof(line), fp); sscanf(line, "ExamID: %59[^\n]", examid);
            fgets(line, sizeof(line), fp); sscanf(line, "Score: %29[^\n]", scoreStr);
            fgets(line, sizeof(line), fp); sscanf(line, "Marks: %29[^\n]", marksStr);
            fgets(line, sizeof(line), fp); sscanf(line, "Percentage: %29[^\n]", percentStr);
            fgets(line, sizeof(line), fp); // skip ---

            printf("%-8d %-20s %-15s %-12s %-12s %s\n", 
                   studentID, name, examid, scoreStr, marksStr, percentStr);
            count++;
        }
    }

    fclose(fp);
    printf("--------------------------------------------------------------------------------\n");
    printf("Total Results: %d\n\n", count);
}
void adminMenu(char *admin_name)
{
  int choice;
  int choice_result;

  do
  {
    displayHeader("ADMIN DASHBOARD");
    printf("Welcome, %s\n\n", admin_name);
    printf("\n");
    printf("   [1]  View All Registered Students\n");
    printf("   [2]  Create New Examination\n");
    printf("   [3]  Create New Question\n");
    printf("   [4]  View Student Results\n");
    printf("   [5]  Logout\n");
    printf("\n");
    printf("\nEnter choice (1-5): ");
    choice_result = scanf("%d", &choice);

    if (choice_result != 1)
    {
      choice = 0;
      clear_input_buffer();
    }
    printf("\n");

    switch (choice)
    {
    case 1:
    {
      displayStudent();
      break;
    }
    case 2:
      createExam();
      break;
    case 3:
      createQuestion();
      break;
    case 4:
      viewAllResults();
      break;
    case 5:
      printf("Logging out...\n");
      break;
    default:
      printf("Invalid choice.Please use valid option (1-5).\n");
    }
  } while (choice != 5);
}

void studentMenu(char *student_name)
{
  int choice;
  int choice_result;

  do
  {
    displayHeader("STUDENT DASHBOARD");
    printf("Welcome, %s\n\n", student_name);
    printf("   [1]  Take Examination\n");
    printf("   [2]  View My Results\n");
    printf("   [3]  Logout\n");
    printf("\nEnter your choice (1-3): ");

    choice_result = scanf("%d", &choice);
    if (choice_result != 1)
    {
      choice = 0;
      clear_input_buffer();
    }
    printf("\n");

    switch (choice)
    {
    case 1:
      viewExams();
      char exid[55];
      printf("Enter Exam ID to take the exam: ");
      clear_input_buffer();
      fgets(exid, sizeof(exid), stdin);
      removeNewline(exid);
      takeExam(exid, student_name);
      break;
    case 2:
      viewResults(student_name);
      break;
    case 3:
      printf("Logging out...\n");
      break;
    default:
      printf("Invalid choice. Please use valid option (1-3).\n");
    }
  } while (choice != 3);
}
// main menu function
void mainMenu()
{

  int choice;
  int scan_result;
  do
  {
    displayHeader("MAIN MENU");
    printf("   STUDENT PORTAL\n");
    printf("     [1]  Register New Student Account\n");
    printf("     [2]  Student Login\n");
    printf("\n");
    printf("   ADMINISTRATOR PORTAL\n");
    printf("     [3]  Administrator Login\n");
    printf("     [4]  Register New Administrator\n");
    printf("\n");
    printf("   SYSTEM\n");
    printf("     [5]  Exit System\n");
    printf("\nEnter your choice (1-5): ");

    scan_result = scanf("%d", &choice);
    if (scan_result != 1)
    {
      choice = 0;
      clear_input_buffer();
    }
    printf("\n");
    switch (choice)
    {
    case 1:
      registerUser();
      break;
    case 2:
      login_user();
      break;
    case 3:
      login_admin();
      break;
    case 4:
      registerAdmin();
      break;
    case 5:
      printf("Exiting the program. TATA!!\n");
      break;
    default:
      printf("Invalid choice.Please use valid option (1-3).\n");
    }

  }

  while (choice != 5);
}

int main()
{
  srand(time(NULL));
  mainMenu();

  return 0;
}