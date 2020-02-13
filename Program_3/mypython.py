#######################################################################
# ## Program:
# ## Author: Lake Peterson
# ## Date:
# ## Description:
# ## Input:
# ## Output:
#######################################################################

import random
import string

#######################################################################
# ## Function: randLetterGenerator
# ## Description:
# ## Parameters:
# ## Pre-Conditions:
# ## Post-Conditions:
#######################################################################

def randLetterGenerator(amount):

    stringOfLetters = ""

    for i in range(amount):
        letterPick = random.choice(string.ascii_lowercase)
        stringOfLetters += letterPick

    stringOfLetters += "\n"

    return stringOfLetters

#######################################################################
# ## Function:
# ## Description:
# ## Parameters:
# ## Pre-Conditions:
# ## Post-Conditions:
#######################################################################

def fileIO():

    fileA = open("fileA.txt", "w+")
    fileB = open("fileB.txt", "w+")
    fileC = open("fileC.txt", "w+")

    fileA.write(randLetterGenerator(10))
    fileB.write(randLetterGenerator(10))
    fileC.write(randLetterGenerator(10))

    # with open(fileA) as f:
    #     content = f.readlines()
    #     print(f)

    # print(tempstr)

    fileA.close
    fileB.close
    fileC.close

#######################################################################
# ## Function: main
#######################################################################

def main():

    fileIO()

if __name__ == "__main__":

    main()
