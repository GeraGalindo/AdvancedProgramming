//
// Created by gurugulundu on 25/03/19.
//

#ifndef LAB3_2_LOGGER_H
#define LAB3_2_LOGGER_H

int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);
int initLogger(char *logType);

#endif //LAB3_2_LOGGER_H
