// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
        
    Isograms = GetValidWords(Words);    

    //grab the text file and convert to array
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString& Word){ return Word.Len() >=4 && IsIsogram(Word);});    

    SetupGame();
   
    // PrintLine(TEXT("The Number of possible words is %i"), Words.Num() -1); //Debug Line
    // PrintLine(TEXT("The HiddenWord is: %s.\nIt is %i characters long!"), *HiddenWord, HiddenWord.Len()); //Debug Line      
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{ 
    if (bGameOver == true)
    {
        ClearScreen();
        SetupGame();
    }

    else
    {
        ClearScreen();
        ProcessGuess(PlayerInput);                
    }     
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Welcome to the game Bulls and Cows!"));

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() -1)];
    Lives = HiddenWord.Len() * 2;
    bGameOver = false;
        
    PrintLine(TEXT("Guess the isogram (no repeating letters)"));
    PrintLine(TEXT("The isogram is %i letters long"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives"), HiddenWord.Len());
    PrintLine(TEXT("Type in your guess and \npress ENTER to continue..."));

    IsIsogram(HiddenWord);   
}
void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPlease press ENTER to play again."));
      
}
void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("Congratulations, you WIN!!!"));
        EndGame();
        return;
    }

    if (HiddenWord.Len() != Guess.Len())    
    {
        PrintLine(TEXT("Please input an isogram that is %i letters long.\n"), HiddenWord.Len());                        
        return;                          
    }

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("Please input a guess\nwith no repeating letters."));
        return; 
    }                 

    --Lives;
    PrintLine(TEXT("You have lost a life!"));
    PrintLine(TEXT("Guess again! Lives remaining: %i"), Lives);
        
    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have run out of lives!\nGAME OVER!"));
        PrintLine(TEXT("The isogram was: %s."), *HiddenWord);
        EndGame();
        return;
    }

    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
}
bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
           if (Word[Index] == Word[Comparison])
           {
               return false;
           }             
        }        
    }
    return true;
}
TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;   
    

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;      
}
FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}