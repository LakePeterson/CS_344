#######################################################################
# ## Program: Python Exploration
# ## Author: Lake Peterson
# ## Date: Fubruary 19, 2020
# ## Description: Creates and fills files with random letters, as well as generates two random numbers to be multiplied
# ## Input: No input or command line arguments are necissary
# ## Output: Creates files, two random numbers, and the product of those numbers
#######################################################################

#######################################################################
# ## Imported Libraries
#######################################################################

import random
import string

#######################################################################
# ## Function: randNumberGeneratorandMultiplier
# ## Description: Generates two random numbers and multies them
# ## Parameters: No parameters are necissary for this function
# ## Pre-Conditions: The random library must be imported
# ## Post-Conditions: Prints out two random number and their product
#######################################################################

def randNumberGeneratorandMultiplier():

    randomNumberOne = random.randint(1, 42)                                     #Generates a random integer between 1 - 42 and stores the value
    randomNumberTwo = random.randint(1, 42)                                     #Generates a random integer between 1 - 42 and stores the value
    multipliedValue = (randomNumberOne * randomNumberTwo)                       #Multiplies the two rondomly generated values

    print(randomNumberOne)                                                      #print statement of the first random value that was generated
    print(randomNumberTwo)                                                      #print statement of the second random value that was generated
    print(multipliedValue)                                                      #print statement of the product of the two randomly generated values

#######################################################################
# ## Function: randLetterGenerator
# ## Description: Generates ten random letter and appends them into one string
# ## Parameters: int amount (length of the string)
# ## Pre-Conditions: The string library must be imported
# ## Post-Conditions: Returns the string of randomly generated letters
#######################################################################

def randLetterGenerator(amount):

    stringOfLetters = ""                                                        #Initilizes an empty string of letters

    for i in range(amount):                                                     #Randomly assign ten letters to a string
        letterPick = random.choice(string.ascii_lowercase)
        stringOfLetters += letterPick

    stringOfLetters += "\n"                                                     #Assigns an endline character to the end of the string

    return stringOfLetters                                                      #Returns the created string

#######################################################################
# ## Function: fileIOandPrint
# ## Description: Creates three files and prints out letter string and numbers
# ## Parameters: No parameters are necissary for this function
# ## Pre-Conditions: Their are no preconditions for this function
# ## Post-Conditions: Creates files, two random numbers, and the product of those numbers
#######################################################################

def fileIOandPrint():

    fileA = open("fileA.txt", "w+")                                             #Opening three different files and setting their permissions
    fileB = open("fileB.txt", "w+")
    fileC = open("fileC.txt", "w+")

    stringA = randLetterGenerator(10)                                           #Assigns three random letter strings to a variable
    stringB = randLetterGenerator(10)
    stringC = randLetterGenerator(10)

    fileA.write(stringA)                                                        #Writes the randomly generated letter strings into the created files
    fileB.write(stringB)
    fileC.write(stringC)

    print(stringA, end = "")                                                    #Prints out the three randomly generated letter strings
    print(stringB, end = "")
    print(stringC, end = "")
    randNumberGeneratorandMultiplier()                                          #Prints out the random numbers and the product of them

    fileA.close                                                                 #Closes the files, due to them no longer being needed
    fileB.close
    fileC.close

#######################################################################
# ## Function: main
#######################################################################

def main():

    fileIOandPrint()

if __name__ == "__main__":

    main()
