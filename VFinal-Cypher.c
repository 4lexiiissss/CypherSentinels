#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Bookshelves to include usleep

#define MAX_LINES 10000
#define MAX_NAME 25
#define MAX_WAIT_TIME 10
#define maxTxt 3
#define maxLang 2

char ChoiceTxt[maxTxt];
char language[maxLang];
char *tab[MAX_LINES];
char *file_language;
int *score;

// Reads the content of a language file and stores it in an array for later use and read the language file based on user's choice.
// file_language (const char *): Path of the language file to read.
// Return: No return (void). Stores the read data into the tab array.
void readLanguageFile(const char *file_language)
{
    int i = 0;
    char line[MAX_LINES];

    FILE *file = fopen(file_language, "r");
    if (file == NULL)
    {
        perror("Error opening language file");
        exit(1);
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (line[strlen(line) - 1] == '\n')
        {
            line[strlen(line) - 1] = '\0';
        }
        tab[i] = strdup(line);
        i++;
    }

    fclose(file);
}

// Displays a confidence bar based on a specified confidence level to show the pourcentage of Trust in the android
// confidenceLevel (int): Confidence level (0-100) to display.
// Return: No return (void). Visualizes the confidence bar.
void displayConfidenceBar(int confidenceLevel)
{
    int barLength = 20; // Confidence bar length
    int filledLength;

    filledLength = (confidenceLevel * barLength) / 100; // Filled bar length

    if (strcmp(file_language, "histoire_fr.txt") == 0)
    {
        printf("Votre confiance actuelle est de: [");
    }
    else if (strcmp(file_language, "histoire_en.txt") == 0)
    {
        printf("Your Trust at the moment: [");
    }
    for (int i = 0; i < barLength; ++i)
    {
        if (i < filledLength)
        {
            printf("#"); // Display filled parts of the bar
        }
        else
        {
            printf("-"); // Display unfilled parts of the bar
        }
    }
    printf("] %d%%\n", confidenceLevel); // Display confidence percentage
}

// Displays the final score based on the chosen language.
// score (int) - The final score to display.
// Return: No return (void). Shows the final score based on the language.
void showFinalScore(int score)
{
    if (strcmp(file_language, "histoire_fr.txt") == 0)
    {
        printf("Votre score final est : %d\n", score); // Print the final score
    }
    else if (strcmp(file_language, "histoire_en.txt") == 0)
    {
        printf("You're final score is : %d\n", score); // Print the final score
    }
}

// Displays choices progressively (character by character) with a slight delay. And show the lines we want in the story.
// start (int): Starting index in the tab array for displaying choices.
// end (int): Ending index in the tab array for displaying choices.
// Return: No return (void). Displays choices from the tab array progressively.
void printLines(int start, int end)
{
    for (int j = start; j < end; j++)
    {
        // Character-by-character progressive display
        int k = 0;
        while (tab[j][k] != '\0')
        {
            putchar(tab[j][k]);
            fflush(stdout); // Force display of current character
            // usleep(10000);  10-millisecond pause
            k++;
        }
        putchar('\n'); // Add a line break at the end of each text
    }
}

// Displays the progress of a quest or task, showcasing the current step and total steps.
// currentStep (int): The current step or stage completed in the quest.
// totalSteps (int): The total number of steps or stages in the quest.
// Return: No return (void). Displays the quest progress bar based on the language selected.
void showProgress(int currentStep, int totalSteps) {
    int progressBarLength;
    int progressPercentage;
    int filledLength;
    int remainingLength;

    // Displaying the quest progression based on the selected language
    if (strcmp(file_language, "histoire_fr.txt") == 0) {
        printf("\n Progression sur la quête: [%d/%d]\n", currentStep, totalSteps);
    } else if (strcmp(file_language, "histoire_en.txt") == 0) {
        printf("\n Progress on the quest: [%d/%d]\n", currentStep, totalSteps);
    }

    // Calculating the progress percentage and visualizing it in a progress bar
    progressPercentage = (currentStep * 100) / totalSteps;
    progressBarLength = 20;
    filledLength = (progressPercentage * progressBarLength) / 100;
    remainingLength = progressBarLength - filledLength;

    printf("[");
    for (int i = 0; i < filledLength; ++i) {
        printf("#");
    }
    for (int i = 0; i < remainingLength; ++i) {
        printf("-");
    }
    printf("] %d%%\n", progressPercentage);
}

// Manages typos on yes/no answers. 
// ChoiceTxt (const char*): The user input string to check.
// choice (int): Represents the selection for "Yes" (1) or "No" (0).
// Return: Returns conditions with choiceTxt equality = yes or no based on user choice either 0 or 1 
int ifChoice(const char *ChoiceTxt, int choice)
{
    if (choice == 0)
    {
        return (strcmp(ChoiceTxt, "Non") == 0 || strcmp(ChoiceTxt, "non") == 0 || strcmp(ChoiceTxt, "NON") == 0 || strcmp(ChoiceTxt, "NO") == 0 || strcmp(ChoiceTxt, "No") == 0 || strcmp(ChoiceTxt, "no") == 0);
    }
    else if (choice == 1)
    {
        return (strcmp(ChoiceTxt, "Oui") == 0 || strcmp(ChoiceTxt, "oui") == 0 || strcmp(ChoiceTxt, "OUI") == 0 || strcmp(ChoiceTxt, "YES") == 0 || strcmp(ChoiceTxt, "Yes") == 0 || strcmp(ChoiceTxt, "yes") == 0);
    }
}

int countdown() { // TOUDOUM
    int seconds = 3; // Temps initial en secondes

    while (seconds >= 1) {
        printf("%d ...", seconds);
        sleep(1); // Attente d'une seconde

        // Diminution du temps
        seconds--;
    }

    printf("\n");
    return 1;
}
// Fonction pour sauvegarder l'endroit où l'utilisateur s'est arrêté
void saveProgress(int currentStep) {
    FILE *saveFile = fopen("savefile.txt", "w");
    if (saveFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sauvegarde");
        exit(1);
    }

    fprintf(saveFile, "%d", currentStep);

    fclose(saveFile);
    printf("Progression sauvegardée.\n");
}

// Fonction pour reprendre l'exécution à partir de la sauvegarde
int loadProgress() {
    int savedStep = 0;
    FILE *saveFile = fopen("savefile.txt", "r");
    if (saveFile == NULL) {
        printf("Aucune sauvegarde trouvée.\n");
        return savedStep; // Retourne l'étape par défaut si aucune sauvegarde n'est trouvée
    }

    fscanf(saveFile, "%d", &savedStep);

    fclose(saveFile);
    printf("Reprise de l'exécution à l'étape %d.\n", savedStep);
    return savedStep;
}


// Handles the main storyline flow based on user choices.
// choice (int): Initial choice to start the story (default 0).
// Return: No return (void). Guides the user through the story based on chosen paths.
void playStory(int choice)
{
    int score;
    char name[MAX_NAME];
    int currentStep;
    int totalSteps = 7;

    printLines(161,185);

    // Name choice
    printLines(1, 4);
    scanf("%s", name);

    // Start of choices for the user to start the story.
    do
    {
        // First choices (1) Understand what happened (2) Ignore the android and intervene directly
        printLines(4, 7);
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printLines(10, 24);
            score = 50;
            currentStep=1;
            showProgress(currentStep, totalSteps);
            break;
        case 2:
            score = 0;
            printLines(29, 31);
            showFinalScore(score);
            exit(0);
        default:
            printf("\n%s\n", tab[31]);
            exit(0);
            break;
        }
        choice = 0;

    } while (choice != 0 && choice < 3);
    do
    {
        // Second choices (Yes): Tell the truth (No): Lie
        printLines(25, 29);
        scanf("%s", ChoiceTxt);
        if (ifChoice(ChoiceTxt, 0))
        {
            printLines(32, 36);
            score = score - 10;
        }
        else if (ifChoice(ChoiceTxt, 1))
        {
            printLines(38, 44);
            score = score + 10;
        }
        else
        {
            printf("\n%s\n", tab[31]);
            exit(0);
        }
    } while (choice != 0 && choice < 3);

    currentStep=2;
    showProgress(currentStep, totalSteps);

    int exitOuterLoop = 0;

    while (!exitOuterLoop)
    {
        // Third choices (1) Reassure him (2) Calm him down (3) Compassion
        printLines(45, 50);
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            while (1)
            {
                printLines(50, 55);
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                    printf("\n");
                    displayConfidenceBar(45);
                    break;
                case 2:
                    printLines(55, 59);
                    score = score + 10;
                    exitOuterLoop = 1;
                    break;
                default:
                    printf("\n%s\n", tab[31]);
                    break;
                }

                if (exitOuterLoop || choice == 2)
                {
                    break;
                }
            }
            break;

        case 2:
            while (1)
            {
                printLines(58, 63);
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                    printf("\n");
                    displayConfidenceBar(50);
                    break;
                case 2:
                    printLines(63, 67);
                    score = score + 10;
                    exitOuterLoop = 1;
                    break;
                default:
                    printf("\n%s\n", tab[31]);
                    break;
                }

                if (exitOuterLoop || choice == 2)
                {
                    break;
                }
            }
            break;

        case 3:
            while (1)
            {
                printLines(66, 71);
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                    printf("\n");
                    displayConfidenceBar(45);
                    break;
                case 2:
                    printLines(71, 74);
                    score = score - 5;
                    exitOuterLoop = 1;
                    break;
                default:
                    printf("\n%s\n", tab[31]);
                    break;
                }

                if (exitOuterLoop || choice == 2)
                {
                    break;
                }
            }
            break;

        default:
            printf("\n%s\n", tab[31]);
            break;
        }
    }

    currentStep=3;
    showProgress(currentStep, totalSteps);

    currentStep = loadProgress();
    saveProgress(currentStep);
    exitOuterLoop = 0;
    while (!exitOuterLoop)
    {
        // Fourth choices (1) : Be realistic (2) : Compassion
        printLines(75, 79);
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            while (1)
            {
                printLines(79, 85);
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                    printf("\n");
                    displayConfidenceBar(35);
                    break;
                case 2:
                    printLines(92, 98);
                    score = score + 10;
                    exitOuterLoop = 1;
                    break;
                default:
                    printf("\n%s\n", tab[31]);
                    break;
                }

                if (exitOuterLoop || choice == 2)
                {
                    break;
                }
            }
            break;

        case 2:
            while (1)
            {
                printLines(84, 92);
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                    printf("\n");
                    displayConfidenceBar(55);
                    break;
                case 2:
                    printLines(92, 98);
                    score = score + 5;
                    exitOuterLoop = 1;
                    break;
                default:
                    printf("\n%s\n", tab[31]);
                    break;
                }

                if (exitOuterLoop || choice == 2)
                {
                    break;
                }
            }
            break;

        default:
            printf("\n%s\n", tab[31]);
            break;
        }
    }

    currentStep=4;
    showProgress(currentStep, totalSteps);

    exitOuterLoop = 0;
    while (!exitOuterLoop)
    {
        // Fourth choices Do you want to move the helicopter? (1) Accept (2) Refuse
        printLines(98, 102);
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            while (1)
            {
                printLines(102, 110);
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                    printf("\n");
                    displayConfidenceBar(40);
                    break;
                case 2:
                    score = score + 5;
                    exitOuterLoop = 1;
                    break;
                default:
                    printf("\n%s\n", tab[31]);
                    break;
                }

                if (exitOuterLoop || choice == 2)
                {
                    break;
                }
            }
            break;

        case 2:
            while (1)
            {
                printLines(110, 118);
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                    printf("\n");
                    displayConfidenceBar(30);
                    break;
                case 2:
                    score = score - 10;
                    exitOuterLoop = 1;
                    break;
                default:
                    printf("\n%s\n", tab[31]);
                    break;
                }

                if (exitOuterLoop || choice == 2)
                {
                    break;
                }
            }
            break;

        default:
            printf("\n%s\n", tab[31]);
            break;
        }
    }

    currentStep=5;
    showProgress(currentStep, totalSteps);

    exitOuterLoop = 0;
    while (!exitOuterLoop)
    {
        // Fifth choices (1) Reassure him (2) Threaten him
        printLines(118, 122);
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            while (1)
            {
                printLines(122, 128);
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                    printf("\n");
                    displayConfidenceBar(45);
                    break;
                case 2:
                    score = score + 5;
                    exitOuterLoop = 1;
                    break;
                default:
                    printf("\n%s\n", tab[31]);
                    break;
                }

                if (exitOuterLoop || choice == 2)
                {
                    break;
                }
            }
            break;

        case 2:
            while (1)
            {
                printLines(128, 134);
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                    printf("\n");
                    displayConfidenceBar(25);
                    break;
                case 2:
                    score = score - 20;
                    exitOuterLoop = 1;
                    break;
                default:
                    printf("\n%s\n", tab[31]);
                    break;
                }

                if (exitOuterLoop || choice == 2)
                {
                    break;
                }
            }
            break;

        default:
            printf("\n%s\n", tab[31]);
            break;
        }
    }

    currentStep=6;
    showProgress(currentStep, totalSteps);

    if (ifChoice(ChoiceTxt, 0))
    {
        printLines(134, 141);
    }
    else if (ifChoice(ChoiceTxt, 1))
    {
        printLines(134, 139);
    }
    do
    {
        // sixth choices (1) Find a compromise (2) Refuse (3) Tell the truth (4) Use the gun (Only if u lie about the gun)
        scanf("%d", &choice);
        countdown(); //TOUDOUM

        switch (choice)
        {
        case 1:
            printLines(141, 147);
            score = score + 10;
            showFinalScore(score);
            exit(0);
            break;
        case 2:
            printLines(147, 149);
            score = score - 20;
            showFinalScore(score);
            exit(0);
            break;
        case 3:
            printLines(149, 153);
            score = score - 20;
            showFinalScore(score);
            exit(0);
            break;
        case 4:
            if (ifChoice(ChoiceTxt, 0))
            {
                printLines(153, 159);
                score = score - 25;
                showFinalScore(score);
                exit(0);
            }
            else if (ifChoice(ChoiceTxt, 1))
            {
                printf("\n%s\n", tab[31]);
            }
            break;
        default:
            printf("\n%s\n", tab[31]);
            break;
        }
    } while (choice != 0 && choice < 5);
}

int main()
{

    FILE *file;

    printf("Hello Player 1, please select your language: \n (FR): French \n (EN): English \n");
    scanf("%s", language);

    if (strcmp(language, "FR") == 0 || strcmp(language, "fr") == 0 || strcmp(language, "Fr") == 0)
    {
        file_language = "histoire_fr.txt";
    }
    else if (strcmp(language, "EN") == 0 || strcmp(language, "en") == 0 || strcmp(language, "En") == 0)
    {
        file_language = "histoire_en.txt";
    }
    else
    {
        printf("Invalid language \n");
        return 1;
    }
    readLanguageFile(file_language);
    playStory(0);

    return 0;
}
