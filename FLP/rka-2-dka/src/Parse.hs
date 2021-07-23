-- Autor: Václav Martinka
-- Login: xmarti76
-- Datum: 01. 04. 2020
-- Předmět: FLP - FIT VUT Brno
-- Zadání: Funkcionální projekt rka-2-dka (Převod rozšířených nedeterministických konečných automatů na deterministické)
-- Popis: Parsovací funkce

module Parse(parseFsm) where

import DataFsm
import ListFuncs
import Text.Read(readMaybe)
import Data.Maybe



-- Načtě konečný automat ze stringu
parseFsm :: String -> FSM
parseFsm input =
    let i = lines input in
    if
        size i < 5
    then
        error "Missing some data!"
    else
        FSM
            (parseStates      (i `at` 0))
            (parseAlphabet    (i `at` 1))
            (parseState       (i `at` 2))
            (parseStates      (i `at` 3))
            (parseTransitions  i `fromPos` 4)



-- Převede string na stav
-- Očekává nezáporné celé číslo na vstupu
parseState :: String -> State
parseState input =
    if
        isNothing (readMaybe input :: Maybe StateBase) || (read input :: StateBase) < 0
    then
        error ("State name must be nonnegative number! Input: '" ++ input ++ "'")
    else
        [read input]



-- Převede množinu stavů na seznam
-- Očekává stavy oddělené čárkou
parseStates :: String -> [State]
parseStates input = let i = split input ',' in map parseState i



-- Převede string na symbol
-- Očekává prázdná string pro epsilon
-- nebo string obsahující právě jeden symbol z rozsahu a - z
parseSymbol :: String -> Symbol
parseSymbol "" = Epsilon
parseSymbol [x] =
    if
        x `elem` ['a' .. 'z']
    then
        Name x
    else
        error ("The alphabet can only contain characters in the range from 'a' to 'z'! Input: '" ++ [x] ++ "'")
parseSymbol x = error ("Transition can contain only one character! Input: '" ++ x ++ "'")



-- Načte abecedu a doplní ji o epsilon
parseAlphabet :: String -> [Symbol]
parseAlphabet []     = error "Empty alphabet!"
parseAlphabet [x]    = [parseSymbol [x]]
parseAlphabet (x:xs) = parseSymbol [x] : parseAlphabet xs



-- Převede string na přechod
-- Očekává vstup ve tvaru from, by, to
parseTransition :: String -> Transition
parseTransition input = 
    let i = split input ',' in
    if
        size i /= 3
    then
        error ("Incomplete transition! Input: '" ++ input ++ "'")
    else
        Transition (parseState (i `at` 0))  (parseSymbol (i `at` 1))  (parseState (i `at` 2))



-- Převede množinu strngů na množinu přechodů
parseTransitions :: [String] -> [Transition]
parseTransitions = map parseTransition



-- Rozdělí string podle symbolu
-- https://jmmv.dev/2006/08/split-function-in-haskell.html
split :: String -> Char -> [String]
split [] _ = [""]
split (c:cs) delim
    | c == delim = "" : rest
    | otherwise = (c : head rest) : tail rest
    where
        rest = split cs delim
