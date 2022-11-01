from cs50 import get_float

# TODO
# This is basically a do while loop but for python
while True:
    input = get_float("Change owed: ")
    if input >= 0:
        break

# Cast input to cents
cents = (int(input * 100) + 0.5)

total_coins = 0
possible_cents = [25, 10, 5, 1]

# Create a list of all possible types of coins.
for i in possible_cents:
    total_coins += cents // i
    cents %= i

print(total_coins)

