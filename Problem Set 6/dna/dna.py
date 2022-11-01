import csv
import sys


def main():

    # TODO: Check for command-line usage
    # If they post any less than 3 command line arguements we return this usage error.
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit()

    # TODO: Read database file into a variable
    # Here we are opening the file given by the command line argument given using the code given to us in the shorts.
    with open(sys.argv[1]) as e:
        r = csv.DictReader(e)
        db = list(r)

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2]) as f:
        seq = f.read()

    # TODO: Find longest match of each STR in DNA sequence
    # Here we define an empty dictionary called match
    match = {}

    # Then, using the code given to us in the shorts we iterate through the keys to acess the value that we need.
    for i in db[0]:
        match[i] = (longest_match(seq, i))

    # TODO: Check database for matching profiles
    # Declare two variables one for no match if there is no match, the other is a counter, starting at one.
    counter = 1
    suspect = 'No match'

    # Because we need to get and store DNA sequences in a Dict we need a key value pair.
    # The DNA sequence will be the key
    # The return value will be the value pair
    # As we see in the for loop, if the DNA sequence matches what is assigned by the person, we increase their counter by 1.
    # If their counter is the same size as the longest match function, we print that suspect.
    for i in range(len(db)):
        for j in match:
            # Here, because match is as int value, and we need to comare strings, we cast them as ints using the str() function to then compare.
            if str(match[j]) == db[i][j]:
                # Incriment counter by one if there is a match
                counter += 1
        # If the size of counter is the same as the length of match that we defined as the longest match function we know that the person is right.
        if counter == len(match):
            suspect = db[i]['name']
            break
        else:
            counter = 1

    print(suspect)
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()

# Help from TheDigitalBison on Reddit.