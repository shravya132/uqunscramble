// Imports
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <csse2310a1.h>
#include <ctype.h>

// Definitions
#define USAGE_ERROR                                                            \
    "Usage: uqunscramble [--lett letters] [--min-length len]"                  \
    " [--dictionary dictfile]\n"
#define INVALID_LETTERS_ERROR "uqunscramble: invalid letters given\n"
#define INSUFFICIENT_LETTERS                                                   \
    "uqunscramble: insufficient letters for the "                              \
    "given minimum length (4)\n"
#define EXCESSIVE_LETTERS                                                      \
    "uqunscramble: too many letters - no more than 12 "                        \
    "expected\n"
#define LENGTH_CONSTRAINT                                                      \
    "uqunscramble: min length value must be between "                          \
    "3 and 6\n"
#define INVALID_DICTIONARY                                                     \
    "uqunscramble: dictionary file with name "                                 \
    "\"%s\" cannot be opened\n"
#define WRONG_CHARACTERS                                                       \
    "Your word must contain only uppercase "                                   \
    "and/or lowercase letters\n"
#define NO_VALID_WORDS "No valid words guessed!\n"

#define WELCOME "Welcome to UQunscramble!\n"
#define INSTRUCTIONS                                                           \
    "Enter words of length %d to %d made from the letters "                    \
    "\"%s\"\n"
#define SCORE "Good! Your score so far is %d\n"
#define WORD_SHORT "Too short! At least %d characters expected\n"
#define WORD_LONG "Word must be no more than %d characters long\n"
#define DEFAULT_DICTIONARY_PATH "/local/courses/csse2310/etc/words"
#define BUFFER_SIZE 256
#define LETTERS "--lett"
#define DICTIONARY "--dictionary"
#define MIN_LENGTH "--min-length"
#define DEFAULT_MIN_LENGTH 4
#define DEFAULT_LETTERS_LENGTH 9
#define ALPHABET 26
#define BONUS_POINTS 10
#define VALID_ARGUMENTS 3
#define MAX_ARGUMENTS 7
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 12
#define MIN_LENGTH_VALUE 3
#define MAX_LENGTH 6

/* Struct that stores variables used in this program */
typedef struct {
    int minLength;
    const char* letters;
    const char* dictionary;
    bool dictionaryFilename;

} Variable;

/* init_variable()
 * −−−−−−−−−−−−−−−
 * Initialises the Variable struct with default values.
 * If user gives command line directions, it will be overwritten.
 *
 * Returns: The initialised Variable struct with values set to its default or
 *     false.
 */

Variable init_variable(void)
{
    Variable variable;

    variable.minLength = DEFAULT_MIN_LENGTH;
    variable.letters = get_random_letters(DEFAULT_LETTERS_LENGTH);
    variable.dictionary = DEFAULT_DICTIONARY_PATH;
    variable.dictionaryFilename = false;

    return variable;
}

/* is_integer()
 * −−−−−−−−−−−−−−−
 * Determines whether string input is an integer.
 * Used when integers are inputted into command line.
 *
 * arg1: str pointer value which is checked if integer.
 *
 * Returns: returns 1 if string is an integer, 0 otherwise.
 */

int is_integer(const char* str)
{
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

/* is_string()
 * −−−−−−−−−−−−−−−
 * Determines whether string input is an string.
 * Used to determine if other types other than string is present in the string..
 *
 * arg1: str Pointer to the string to be checked.
 *
 * Returns: 1 if string is a full string, 0 otherwise.
 */
int is_string(const char* str)
{
    while (*str) {
        if (!isalpha(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

/* Program exit codes  */
enum ExitCodes {
    EXIT_USAGE_ERROR = 2,
    EXIT_INVALID_DICTIONARY = 5,
    EXIT_EXCESSIVE_LETTERS = 7,
    EXIT_INSUFFICIENT_LETTERS = 8,
    EXIT_NO_VALID_WORDS = 15,
    EXIT_LENGTH_CONSTRAINT = 19,
    EXIT_INVALID_LETTERS = 20
};

/* throw_usage_error()
 * −−−−−
 * Prints usage error and exits the game.
 *
 * Returns: None.
 */
void throw_usage_error()
{
    fprintf(stderr, USAGE_ERROR);
    exit(EXIT_USAGE_ERROR);
}

/* throw_invalid_dictionary()
 * −−−−−
 * Prints invalid dictionary error  and exits the game.
 *
 * arg1: Struct containing all variables.
 *
 * Returns: None.
 */

void throw_invalid_dictionary(Variable variable)
{
    fprintf(stderr, INVALID_DICTIONARY, variable.dictionary);
    exit(EXIT_INVALID_DICTIONARY);
}

/* throw_excessive_letters()
 * −−−−−
 * Prints excessive letters error and exits the game.
 *
 * Returns: None.
 */

void throw_excessive_letters()
{
    fprintf(stderr, EXCESSIVE_LETTERS);
    exit(EXIT_EXCESSIVE_LETTERS);
}

/* throw_insufficient_letters()
 * −−−−−
 * Prints insufficient letters error and exits the game.
 *
 * Returns: None.
 */

void throw_insufficient_letters()
{
    fprintf(stderr, INSUFFICIENT_LETTERS);
    exit(EXIT_INSUFFICIENT_LETTERS);
}

/* throw_length_constraint()
 * −−−−−
 * Prints length constraint error and exits the game.
 *
 * Returns: None.
 */

void throw_length_constraint()
{
    fprintf(stderr, LENGTH_CONSTRAINT);
    exit(EXIT_LENGTH_CONSTRAINT);
}

/* throw_invalid_letters_error()
 * −−−−−
 * Prints invalid letters error and exits the game.
 *
 * Returns: None.
 */

void throw_invalid_letters_error()
{
    fprintf(stderr, INVALID_LETTERS_ERROR);
    exit(EXIT_INVALID_LETTERS);
}

/* form_word()
 * −−−−−
 * Determines whether word can be formed from the given letters
 *
 * arg1: Word is the pointer to the word to be checked.
 * arg2: Pointer to a set of letters.
 *
 * Returns: Boolean type, true if all letters of word is in letters,
 *     false otherise
 */

bool form_word(const char* word, const char* letters)
{
    int lettersCount[ALPHABET] = {0};
    while (*letters) {
        lettersCount[toupper(*letters) - 'A']++;
        letters++;
    }

    while (*word) {
        int index = toupper(*word) - 'A';

        if (lettersCount[index] <= 0) {
            return false;
        }
        lettersCount[index]--;
        word++;
    }
    return true;
}

/* read_dictionary()
 * −−−−−
 * Reads word from the given dictionary file, sorts through them based on given
 *     letters and returns a dynamically allocated array of words.
 *
 * arg1: Struct containing all variables.
 * arg2: Pointer to an integer to store the number of words in file.
 *
 * Returns: A dynamically allocated array of words that can be made with
 *     letters.
 */

char** read_dictionary(Variable variable, int* numWords)
{
    FILE* dictionary = fopen(variable.dictionary, "r");
    char** filteredWords = malloc(BUFFER_SIZE * sizeof(char*));
    char buffer[BUFFER_SIZE];
    int count = 0;

    while (fgets(buffer, BUFFER_SIZE, dictionary) != NULL) {
        // Checks if file contains only whitespace
        int whitespace = 1;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (!isspace(buffer[i])) {
                whitespace = 0;
                break;
            }
        }

        if (whitespace) {
            continue;
        }
        // Removes newline character if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        if (form_word(buffer, variable.letters)) {
            if ((len) > (size_t)variable.minLength
                    && strchr(buffer, '\'') == NULL) {
                char* word = strdup(buffer);

                if (count >= BUFFER_SIZE) {
                    filteredWords = realloc(
                            filteredWords, sizeof(char*) * (count + 1));
                }

                for (size_t i = 0; i < len; i++) {
                    word[i] = toupper(word[i]);
                }

                filteredWords[count] = word;
                count++;
            }
        }
    }

    fclose(dictionary);
    *numWords = count;
    return filteredWords;
}

/* word_validity()
 * −−−−−
 * Checks the validity of the word given by the user.
 *
 * arg1: Struct containing variables.
 * arg2: Pointer to line which is the word to be checked.
 * arg3: The maximum length allowed for the word.
 *
 * Returns: 1 if the word is valid, 0 otherwise.
 */
int word_validity(Variable variable, char* line, int maxLength)
{
    // Checks if given word is too short
    int wordLength = strlen(line);
    if (wordLength < variable.minLength) {
        printf(WORD_SHORT, variable.minLength);
        return 0;
    }
    // Checks if given word is too long
    if (wordLength > maxLength) {
        printf(WORD_LONG, maxLength);
        return 0;
    }
    // Checks if another type is present in string, ie, int or whitespace
    if (!is_string(line)) {
        printf(WRONG_CHARACTERS);
        return 0;
    }
    return 1;
}

/* process_input()
 * −−−−−
 * Function processes user input, validates the word given by user and checks
 *    if able to be made with available letters. Also updates score and checks
 *    if word has been entered previously.
 *
 * arg1: Struct containing variables.
 * arg2: An array containing all possible words made from letters.
 * arg3: The number of filtered words in array.
 *
 * Errors: Catches if entered word can be formed with available letters, if
 *      word has been guessed earlier and if word is not in dictionary.
 *
 * Returns: int which is the value of the user's score.
 */

int process_input(Variable variable, char** filteredWords, int numWords)
{
    int maxLength = strlen(variable.letters);
    int score = 0;
    char line[BUFFER_SIZE];
    bool enteredWords[numWords];
    memset(enteredWords, false, sizeof(enteredWords));
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        if (!word_validity(variable, line, maxLength)) {
            continue;
        }
        for (int i = 0; line[i]; i++) {
            line[i] = toupper(line[i]);
        }
        if (!form_word(line, variable.letters)) {
            printf("Word cannot be formed with available letters\n");
            continue;
        }
        bool found = false;
        for (int i = 0; i < numWords; i++) {
            if (enteredWords[i] && strcmp(line, filteredWords[i]) == 0) {
                printf("Word has been guessed earlier\n");
                found = true;
                break;
            }
            if (!enteredWords[i] && strcmp(line, filteredWords[i]) == 0) {
                enteredWords[i] = true;
                found = true;
                score += strlen(filteredWords[i]);
                if (strlen(filteredWords[i]) == (size_t)maxLength) {
                    score += BONUS_POINTS;
                }
                printf(SCORE, score);
                break;
            }
        }
        if (!found) {
            printf("Word not found in dictionary\n");
        }
    }
    return score;
}

/* play()
 * −−−−−
 * Prints welcome messages to user and processes user input and returns score.
 *
 * arg1: Struct containing variables.
 * arg2: An array containing all words that can be made with letters.
 * arg3: The number of words that can be made.
 *
 * Returns: The user's score..
 */
int play(Variable variable, char** filteredWords, int numWords)
{
    printf(WELCOME);
    printf(INSTRUCTIONS, variable.minLength, (int)strlen(variable.letters),
            variable.letters);
    int score = process_input(variable, filteredWords, numWords);
    return score;
}

/* argument_validity()
 * −−−−−
 * Checks for valid command line arguments inserted by user. Checks each
 *    argument is valid, spelt correctly and does not appear twice. If so,
 *    throws usage error.
 *
 * arg1: The number of command line arguments.
 * arg2: Array of strings containing command line arguments.,
 *
 * Error: Throws usage error if number of arguments is odd, goes beyond max
 *     arguments, invalid or occurs more than once.
 *
 * Return: None.
 */
void argument_validity(int argc, char** argv)
{
    const char* validArguments[VALID_ARGUMENTS]
            = {LETTERS, DICTIONARY, MIN_LENGTH};
    bool argEncountered[VALID_ARGUMENTS] = {false};
    if ((argc - 1) % 2 == 1 || (argc - 1) > MAX_ARGUMENTS) {
        throw_usage_error();
    }

    for (int i = 1; i < argc; i += 2) {
        bool validArg = false;
        for (int j = 0; j < VALID_ARGUMENTS; j++) {
            if (strcmp(validArguments[j], argv[i]) == 0) {
                if (argEncountered[j]) {
                    throw_usage_error();
                }
                argEncountered[j] = true;
                validArg = true;
                break;
            }
        }
        if (!validArg) {
            throw_usage_error();
        }
    }

    for (int i = 1; i < argc; i += 2) {
        bool validArg = false;
        for (int j = 0; j < VALID_ARGUMENTS; j++) {
            if (strcmp(validArguments[j], argv[i]) == 0) {
                validArg = true;
                break;
            }
        }
        if (!validArg) {
            throw_usage_error();
        }
    }
}

int main(int argc, char** argv)
{
    Variable variable = init_variable();
    int numWords;

    argument_validity(argc, argv);

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], LETTERS)) {
            if (!is_string(argv[i + 1])) {
                throw_invalid_letters_error();
            } else if (strlen(argv[i + 1]) < MIN_WORD_LENGTH) {
                throw_insufficient_letters();
            } else if (strlen(argv[i + 1]) > MAX_WORD_LENGTH) {
                throw_excessive_letters();
            } else {
                variable.letters = argv[i + 1];
            }
        } else if (!strcmp(argv[i], DICTIONARY)) {
            FILE* dictionaryFilename = fopen(argv[i + 1], "r");
            if (dictionaryFilename == NULL) {
                variable.dictionary = argv[i + 1];
                throw_invalid_dictionary(variable);
            } else {
                fclose(dictionaryFilename);
                variable.dictionary = argv[i + 1];
            }
        } else if (!strcmp(argv[i], MIN_LENGTH)) {
            if (!is_integer(argv[i + 1])) {
                throw_usage_error();
            } else if ((atoi(argv[i + 1])) < MIN_LENGTH_VALUE
                    || (atoi(argv[i + 1]) > MAX_LENGTH)) {
                throw_length_constraint();
            } else {
                variable.minLength = atoi(argv[i + 1]);
            }
        }
    }

    char** filteredWords = read_dictionary(variable, &numWords);

    int score = play(variable, filteredWords, numWords);

    for (int i = 0; i < numWords; i++) {
        free(filteredWords[i]);
    }
    if (score == 0) {
        printf(NO_VALID_WORDS);
        exit(EXIT_NO_VALID_WORDS);
    } else {
        printf("Your final score is %d\n", score);
    }

    free(filteredWords);
    return 0;
}
