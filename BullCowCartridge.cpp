// Fill out your copyright notice in the Description page of ProIndex2ect Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    ValidWords = GetValidWord(WordList);
    SetupGame(); 
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
        return;
    }    
    ProcessGuess(Input);
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = ValidWords[FMath::RandRange(0,ValidWords.Num()-1)];
    Lives = HiddenWord.Len();
    bGameOver = false;
    PrintLine(TEXT("The Word Is %i Letter Long"),HiddenWord.Len());
    PrintLine(TEXT("The Word Is %s"),*HiddenWord);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true; 
    PrintLine(TEXT("The Hidden Word Is: %s"),*HiddenWord);           
    PrintLine(TEXT("Press Enter To Play Again"));           
    SetupGame();
}

void UBullCowCartridge::ProcessGuess(const FString& PlayerGuess)
{
    if (PlayerGuess == HiddenWord)
    {
        PrintLine(TEXT("You Have Won."));
        EndGame(); 
        return;               
    }
    
    if (HiddenWord.Len() != PlayerGuess.Len())
    {
        PrintLine(TEXT("Wrong Length"));
        return;
    } 

    if (IsIsogram(PlayerGuess))
    {
        PrintLine(TEXT("You Entered An Isogram, Try Again"));
        return;
    }

    PrintLine(TEXT("You Have %i Lives Left"),Lives);
    PrintLine(TEXT("Guess Again"));
    --Lives;

    if (Lives < 1)
    {           
        PrintLine(TEXT("orz=3 You're Dead"));
        EndGame();
        return;
    } 

    FBullCowCount BullsCowsCounter = GetBullCows(PlayerGuess); 
    PrintLine(TEXT("Bulls = %i, Cows = %i"),BullsCowsCounter.Bulls,BullsCowsCounter.Cows);           
}

bool UBullCowCartridge::IsIsogram(const FString& PlayerGuess) const
{
    bool Isogram = false;

    for (int32 Index1 = 0; Index1 < PlayerGuess.Len(); ++Index1)
    {
        for (int32 Index2 = Index1+1; Index2 < PlayerGuess.Len(); ++Index2)
        {
            if (PlayerGuess[Index2] == PlayerGuess[Index1])
            {
            Isogram = true;
            return Isogram;
            } 
        }
    }
    return Isogram;
}

TArray<FString> UBullCowCartridge::GetValidWord(const TArray<FString>& WordList) const
{
     TArray<FString> Selection;
    
    for (FString Candidate: WordList)
    {
        if (Candidate.Len() > 4 && Candidate.Len() < 8 && !IsIsogram(Candidate) )
        {
            Selection.Emplace(Candidate);
        }
    }
    return Selection; 
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    for (int32 GuessIndex=0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            ++Count.Bulls;
            continue;
        }
        for (int32 CompareIndex=0; CompareIndex < HiddenWord.Len(); CompareIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[CompareIndex])
            {
                ++Count.Cows;
                break;
            }
        }
    }
    return Count;
}