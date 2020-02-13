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

    letterArray = []

    for i in range(amount):
        letterPick = random.choice(string.ascii_lowercase)
        letterArray.append(letterPick)

    #letterArray.append("\n")

    print(letterArray)

#######################################################################
# ## Function:
# ## Description:
# ## Parameters:
# ## Pre-Conditions:
# ## Post-Conditions:
#######################################################################

def fileIO():

    file openFile

    openFile = "fileOne.txt, w+"

#######################################################################
# ## Function: main
#######################################################################

def main():

    fileIO

    randLetterGenerator(10)
    randLetterGenerator(10)
if __name__ == "__main__":

    main()
