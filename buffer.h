/* buffer.h */

typedef int buffer_item;
#define BUFFER_SIZE 10

/* I only made the following max value limit to make it cleaner when printing output,
  the numbers for integers get huge, it's easier for me to read when they are
  just 1-2 digits. */
#define RAND_NUM_HIGH_LIMIT 100 /* max size of an integer in the array, non-inclusive */
