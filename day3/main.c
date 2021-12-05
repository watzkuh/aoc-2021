#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVALID 'X'
#define END_OF_ARRAY LONG_MAX

long getFileSize(FILE *fp) {
  long size = 0;
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return size;
}

long getLineLength(char *content) {
  for (long i = 0; i < strlen(content); i++) {
    if (content[i] == '\n') {
      return i + 1;
    }
  }
  printf("error: expected at least one line break\n");
  exit(1);
}

struct Report {
  char mostCommonBit;
  long noOfZeros;
  long noOfOnes;
  long *zeroLines;
  long *oneLines;
};

struct Report getColumnReport(char *content, long fileSize, int column,
                              long lineLength) {
  struct Report report;
  long maxElements = fileSize / lineLength * sizeof(long);
  report.zeroLines = malloc(maxElements);
  report.oneLines = malloc(maxElements);
  long noOfZeros = 0;
  long noOfOnes = 0;
  for (long i = column, j = 0; i < fileSize; i += lineLength, j++) {
    if (content[i] == '0') {
      report.zeroLines[noOfZeros++] = j;
    } else if (content[i] == '1') {
      report.oneLines[noOfOnes++] = j;
    }
  }
  report.zeroLines[noOfZeros] = END_OF_ARRAY;
  report.oneLines[noOfOnes] = END_OF_ARRAY;
  // task does not specify what to do in case of equal amounts
  report.mostCommonBit = (noOfZeros > noOfOnes) ? '0' : '1';
  report.noOfZeros = noOfZeros;
  report.noOfOnes = noOfOnes;
  return report;
}

long toDecimal(char bits[], long arrayLength) {
  long result = 0;
  for (long i = 0; i < arrayLength; i++) {
    if (bits[arrayLength - 1 - i] == '1')
      result += (1 << i);
  }
  return result;
}

long lineToDecimal(char *content, long lineNumber, long lineLength) {
  char bits[lineLength];
  memcpy(&bits[0], &content[lineNumber * lineLength], lineLength);
  return toDecimal(bits, lineLength - 1);
}

void clearLines(char *content, long lines[], long lineLength) {
  long i = 0;
  while (lines[i] != END_OF_ARRAY) {
    memset(&content[lines[i] * lineLength], INVALID, lineLength);
    i++;
  }
}

long getLifeLine(bool invert, char *content, long fileSize, long lineLength) {
  for (int i = 0; i < lineLength; i++) {
    struct Report rep = getColumnReport(content, fileSize, i, lineLength);
    bool sensorPredicate = rep.noOfOnes >= rep.noOfZeros;
    if (invert)
      sensorPredicate = !sensorPredicate;
    if (sensorPredicate) {
      clearLines(content, rep.zeroLines, lineLength);
      if (rep.noOfOnes == 1) {
        return rep.oneLines[0];
      }
    } else {
      clearLines(content, rep.oneLines, lineLength);
      if (rep.noOfZeros == 1) {
        return rep.zeroLines[0];
      }
    }
  }
  return END_OF_ARRAY;
}

long getOxygenLine(char *content, long fileSize, long lineLength) {
  return getLifeLine(false, content, fileSize, lineLength);
}

long getCo2Line(char *content, long fileSize, long lineLength) {
  return getLifeLine(true, content, fileSize, lineLength);
}
int main(int argc, char **argv) {
  long fileSize;
  char *content;
  if (argc < 2) {
    printf("error: no input file\n");
    return 1;
  }
  char *file = argv[1];
  FILE *fp = fopen(file, "r");
  if (fp != NULL) {
    fileSize = getFileSize(fp);
    content = malloc(fileSize + 1);
    fread(content, 1, fileSize, fp);
    fclose(fp);
    content[fileSize] = '\0';

    long lineLength = getLineLength(content);
    long noOfBits = lineLength - 1; // Ignore \n

    // Part One
    char gammaBits[noOfBits];
    for (long i = 0; i < noOfBits; i++) {
      gammaBits[i] =
          getColumnReport(content, fileSize, i, lineLength).mostCommonBit;
    }
    long gamma = toDecimal(gammaBits, noOfBits);
    long epsilon = (1 << noOfBits) - 1 - gamma;
    printf("Power consumption is %ld\n", epsilon * gamma);

    // Part Two
    char oxygenCopy[fileSize + 1];
    char co2Copy[fileSize + 1];
    memcpy(oxygenCopy, content, fileSize);
    memcpy(co2Copy, content, fileSize);

    long oxygenValue = lineToDecimal(
        content, getOxygenLine(oxygenCopy, fileSize, lineLength), lineLength);
    long co2Value = lineToDecimal(
        content, getCo2Line(co2Copy, fileSize, lineLength), lineLength);
    printf("Life support rating is %ld\n", oxygenValue * co2Value);

    return 0;
  }
}