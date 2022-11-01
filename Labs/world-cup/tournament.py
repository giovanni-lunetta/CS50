# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = []
    # TODO: Read teams into memory from file
    # We use a function called DictReader to get info easily from the CSV file and then read them. Then, we specify that the ratings are integers, and then append them to our empty list teams.
    fname = sys.argv[1]
    with open(fname) as f:
        r = csv.DictReader(f)
        for team in r:
            team['rating'] = int(team['rating'])
            teams.append(team)

    counts = {}
    # TODO: Simulate N tournaments and keep track of win counts
    # We start with a dictionary counts. We run the simulation 1000 times as specified by N above. Each time we iterate through we get 1 winner.
    # If that winner has already been appended to the dictionary, we incrament their winnings by 1, otherwise, a winner who is not in the dictionary gets added and gives them a 1 for their first win.
    for i in range(N):
        w = simulate_tournament(teams)
        if w in counts:
            counts[w] += 1
        else:
            counts[w] = 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    # TODO
    # This function will work by running a while loop as long as then length of teams is greater than one. AKA as long as there are still teams that have to play, we keep running the while loop.
    # Once this while loop is over or no longer entered to run, we print the winning team.
    while len(teams) > 1:
        teams = simulate_round(teams)
    return teams[0]['team']


if __name__ == "__main__":
    main()
