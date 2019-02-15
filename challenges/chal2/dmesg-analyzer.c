#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TYPE_GENERAL 0
#define TYPE_GREP 1
#define TYPE_TABBED 2
#define OFFSET 15
#define REPORT_FILE "report.txt"
#define GEN_MSG_FILE "gen-tmp.txt"
#ifndef BUF_SIZE        /* Allow "cc -D" to override definition */
#define BUF_SIZE 1020
#endif

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {

    if (argc < 2) {
    	printf("Usage:./dmesg-analizer.o logfile.txt\n");
    	return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

int getMessageType(char buffer[], int start, int end){
  int end_idx = 0;

  if (buffer[start] == ' ' && buffer[start+1] == ' '){
    return TYPE_TABBED;
  }

  for (int i = start; i <= end; ++i){
    if (buffer[i] == ':'){
      end_idx = i;
      if (i > start+40){
        return TYPE_GENERAL;
      } else
        return TYPE_GREP;
    }
    if (buffer[i] == '\n' && end_idx > start+40){
      return TYPE_GENERAL;
    }
  }

  return TYPE_GENERAL;
}

void processGeneralType(char buffer[], int start, int end){
  int generalFd, openFlags;
  mode_t filePerms;
  ssize_t numRead;
  char line[end-start+1];
  int idx = 0;

  // init file permissions
  openFlags = O_CREAT | O_WRONLY | O_APPEND;
  filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

  generalFd = open(GEN_MSG_FILE, openFlags, filePerms);
  if (generalFd == -1)
    printf("Error: opening file %s", GEN_MSG_FILE);

  for (int i = start; i <= end; ++i){
    line[idx] = buffer[i];
    idx++;
  }
  line[idx+1] = '\n';
  write(generalFd, line, end-start+1);

  if (close(generalFd) == -1)
    printf("Error while closing gen-tmp.txt");

}

int isInDictionary(char *str2){
  // int inputFd;
  // char readChar;
  // char *str1 = "grep -i \"";
  // char *str3 = "\" dictionary.txt | wc -l > found.txt";
  // int cmd_idx = 0;
  // int cmd_len = strlen(str1) + strlen(str2) + strlen(str3);
  //
  // char *command = (char *)calloc(cmd_len, sizeof(char));
  //
  // for(int i = 0; i < strlen(str1); ++i){
  //   command[cmd_idx] = str1[i];
  //   cmd_idx++;
  // }
  // for(int i = 0; i < strlen(str2); ++i){
  //   command[cmd_idx] = str2[i];
  //   cmd_idx++;
  // }
  // for(int i = 0; i < strlen(str3); ++i){
  //   command[cmd_idx] = str3[i];
  //   cmd_idx++;
  // }
  // //printf("command: %s\n", command);
  // system(command);
  // free(command);
  // inputFd = open("found.txt", O_RDONLY);
  // if (inputFd == -1){
  //   printf("Error while opening found.txt\n");
  // }
  //
  // read(inputFd, readChar, 1);
  //
  // // if(readChar != '0'){
  // //   return 1;
  // // }

  return 0; // Not in dictionary
}

void saveToDictionary(char *str){
  // char *msg = (char *)calloc(strlen(str)+1, sizeof(char));
  // int outputFd, openFlags;
  // mode_t filePerms;
  //
  // for(int i = 0; i < strlen(str); ++i){
  //   msg[i] = str[i];
  // }
  //
  // msg[strlen(str)] = '\n';
  //
  // openFlags = O_CREAT | O_WRONLY | O_APPEND;
  // filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  //
  // outputFd = open("dictionary.txt", openFlags, filePerms);
  // if (outputFd == -1)
  //   printf("Error: opening file dictionary.txt");
  //
  // write(outputFd, msg, strlen(msg));
  //
  // if (close(outputFd) == -1)
  //     printf("Error while closing dictionary.txt");
  //
  // free(msg);
}

void writeReportGrep(char *str){
  char *msg = (char *)calloc(strlen(str)+1, sizeof(char));
  int outputFd, openFlags;
  mode_t filePerms;

  for(int i = 0; i < strlen(str); ++i){
    msg[i] = str[i];
  }

  msg[strlen(str)] = '\n';

  openFlags = O_CREAT | O_WRONLY | O_APPEND;
  filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

  outputFd = open("report.txt", openFlags, filePerms);
  if (outputFd == -1)
    printf("Error: opening file report.txt");

  write(outputFd, msg, strlen(msg));

  if (close(outputFd) == -1)
      printf("Error while closing report.txt");

  free(msg);
}

void processGrepType(char buffer[], int start, int end){
  char *str1 = "grep -i ";
  char *str3 = " dmesg.txt >> report.txt";
  char *command = 0;
  char *type = 0;
  int type_idx = 0;
  int tmp_itr = 0;
  int cmd_len = 0;

  // get type
  for (int i = start; i <= end; ++i){
    if(buffer[i] == ':'){
      type_idx = i;
      break;
    }
  }

  type = (char *)calloc(type_idx-start, sizeof(char));
  for (int i = start; i < type_idx; ++i){
    type[tmp_itr] = buffer[i];
    tmp_itr++;
  }

  //printf("type: %s\n", type);

  if (!isInDictionary(type)){
    saveToDictionary(type);
    cmd_len = strlen(str1)+strlen(type)+strlen(str3)+2;
    command = (char *)calloc(cmd_len, sizeof(char));

    // first iteration to fill up the command string
    cmd_len = strlen(str1);
    for (int i = 0; i < cmd_len; ++i){
      command[i] = str1[i];
    }

    tmp_itr = cmd_len;
    command[tmp_itr] = '\"';
    tmp_itr++;

    // second iteration to fill up the command string
    cmd_len = strlen(type);
    for (int i = 0; i < cmd_len; ++i){
      command[tmp_itr] = type[i];
      tmp_itr++;
    }

    command[tmp_itr] = '\"';
    tmp_itr++;

    // third iteration to fill up the command string
    cmd_len = strlen(str3);
    for (int i = 0; i < cmd_len; ++i){
      command[tmp_itr] = str3[i];
      tmp_itr++;
    }

    if (command[tmp_itr] == 'Q' || command[tmp_itr] == '1'){
      command[tmp_itr] = ' ';
    }

    //printf("%s\n", command);
    writeReportGrep(type);
    system(command);

    free(type);
    free(command);
  }

}

void processTabbedType(char buffer[], int start, int end){
  char *msg = (char *)calloc(end-start+1, sizeof(char));
  int outputFd, openFlags;
  mode_t filePerms;

  for(int i = 0; i < end-start; ++i){
    msg[i] = buffer[start+i];
  }
  msg[end-start] = '\n';

  openFlags = O_CREAT | O_WRONLY | O_APPEND;
  filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

  outputFd = open("report.txt", openFlags, filePerms);
  if (outputFd == -1)
    printf("Error: opening file report.txt");

  write(outputFd, msg, strlen(msg));

  if (close(outputFd) == -1)
      printf("Error while closing report.txt");

  free(msg);
}

void analizeLine(char buffer[], int start, int end){
  int type = getMessageType(buffer, start+OFFSET, end);

  switch (type) {
    case TYPE_GENERAL:
      processGeneralType(buffer, start, end);
      break;
    case TYPE_GREP:
      processGrepType(buffer, start+OFFSET, end);
      break;
    case TYPE_TABBED:
      processTabbedType(buffer, start, end);
      break;
    default:
      processGeneralType(buffer, start, end);
      break;
  }
}

void analizeBuf(char buffer[], int len){
  int line_start = 0;
  int line_end = 0;
  for (int i = 0; i < len; ++i){
    if (buffer[i] == '\n'){
      line_end = i;
      analizeLine(buffer, line_start, line_end);
      line_start = i+1;
    }
  }
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    system("rm -fr report.txt");

    // Implement your solution here.
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    // Open input file
    inputFd = open(logFile, O_RDONLY);
    if (inputFd == -1)
      printf("Error: opening file %s", logFile);

    // init file permissions
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    // Open output file
    outputFd = open(report, openFlags, filePerms);
    if (outputFd == -1)
      printf("Error: opening file %s", report);

    while((numRead = read(inputFd, buf, BUF_SIZE)) > 0){
      analizeBuf(buf, numRead);
    }
    if (numRead == -1)
      printf("Error while reading\n");

    if (close(inputFd) == -1)
      printf("Error while closing dmesg.txt");
    if (close(outputFd) == -1)
      printf("Error while closing report.txt");

    printf("Report is generated at: [%s]\n", report);
    system("rm -fr found.txt gen-tmp.txt");
}


/*
  1. open the dmesg.txt file with file-open
  2. read each line and find the string between "] <log-type>:"
    2.1 if the end of the line is reached and there was no ":" after "]"
      2.1.1 then the message is general and we need to write to gen-tmp.txt
    2.2 else, we trim the log-type
      2.2.1 we check if the log-type was already "grepped"
        2.2.1.1 if yes, then we ignore it and continue
        2.2.1.2 if not, then we save the log-type in a dictionary
          2.2.1.2.1 grep the log-type to a typed-tmp.txt
          2.2.1.2.2 then we open report.txt
                    write to report.txt the <log-type> ... plus a new line
                    append typed-tmp.txt to report.txt ... plus a new line

*/
