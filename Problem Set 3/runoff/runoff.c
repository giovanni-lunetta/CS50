#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
//THIS FUNCTION IS VERY SIMILAR TO PLURALITY. IT CHECKS TO SEE IF THE KEY NAME IS THE SAME AS THE CANDIDATES NAME USING STRCOMP.
bool vote(int voter, int rank, string name)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // TODO
    //For this part, I was pretty stuck, I used Anvea on youtube to help with this.
    //Here, we need to add to the voter count for each candidate. This requires a nested for loop because there are two arrays at hand.
    //Essentially, this function checks if the voter has been eliminated or not and if they are not, adds their vote to their respective index.
    //This takes place for each runoff, so if a candidate has been eliminated, we do not count their vote but instead, count the second place vote for the respective candidate.
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (!candidates[preferences[i][j]].eliminated)
            {
                candidates[preferences[i][j]].votes++;
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
//The process of the election works that if a candidate has more than half of the votes during any runoff, they are declared the winner.
//For example, if one candidate has 5 1st place votes and the other has 4, candidate one wins the election.
//But, if a no candidate gets there we have a runoff. After someone is eliminated, they are removed from the indexes and votes are counted again.
bool print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > voter_count / 2)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
//This function is very similar to the winner function in the plurality code. We first set a variable minimum_count to be equal to the voter count.
//Then, we iterate through each candidates vote count total using the for loop. The first candidate's number of counts is set as the lowest. Then we check the next candidate.
//If their vote count is lower than the current vote count we set the new minimum to this smaller number.
//We have an && statement here as well because we only want to look at candidates who have not been eliminated.
int find_min(void)
{
    // TODO
    int minimum_count = voter_count;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes < minimum_count && candidates[i].eliminated == false)
        {
            minimum_count = candidates[i].votes;
        }
    }
    return minimum_count;
}

// Return true if the election is tied between all candidates, false otherwise
//In this function we return true if every candidate thats left in the election has the same number of votes. In order to do this we must count the number of candidates remaining
//and also count the number of candidates whose total vote count is equal to the minimum vote count. If these numbers are the same, there must be a tie.
//Here, we declare two variables. The first is count, which keeps track of the total number of candidates who have the minimum vote count.
//The second is running which keeps track of the total number that are still running. We see the if statement that uses ! to then update running if the candidate has not been eliminated.
bool is_tie(int min)
{
    // TODO
    int running = 0;
    int count = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            running++;

            if (candidates[i].votes == min)
            {
                count++;
            }
        }
    }
    if (running == count)
    {
        return true;
    }
    return false;
}
// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}

//Help from Google
//Help from Anvea on Youtube