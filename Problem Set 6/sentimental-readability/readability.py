from cs50 import get_string

# TODO
# Get user input
input = get_string('Text: ')

# Declare variables and set them to 0
words, letters, sentences = 0, 0, 0

# Run a for loop with nested if statements each incramenting their respective variables if the input meets the condition
for i in input:
    if i.isalpha():
        letters += 1
    if i.isspace():
        words += 1
    if i in ['.', '?', '!']:
        sentences += 1

# Add one more to words because we will always have one less than needed
words += 1

# Calculate L and S for the formula
L = (letters * 100.0) / words
S = (sentences * 100.0) / words
result = round(0.0588 * L - 0.296 * S - 15.8)

if result < 1:
    print('Before Grade 1')
elif result > 16:
    print('Grade 16+')
else:
    print("Grade", result)
