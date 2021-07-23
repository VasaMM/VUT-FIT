-- Autor: Václav Martinka
-- Login: xmarti76
-- Datum: 31. 03. 2020
-- Předmět: FLP - FIT VUT Brno
-- Zadání: Funkcionální projekt rka-2-dka (Převod rozšířených nedeterministických konečných automatů na deterministické)
-- Popis: Vlastní datové typy

module DataFsm  where

import Data.List (sort)



-- Stav automatu
-- Jednoznačně identifikován seznamem čísel
-- Seznam čísel je zde kvůli možnosti vytvářet sloučené stavy: q0 + q1 = q0_q1 
type StateBase = Integer
type State = [StateBase]

showState :: State -> String
showState [] = ""
showState [q] = show q
showState (q:qs) = show q ++ "_" ++ showState qs


showStates :: [State] -> String
showStates [] = ""
showStates [q] = showState q
showStates (q:qs) = showState q ++ "," ++ showStates qs




data Symbol = Epsilon | Name Char deriving (Eq, Ord)
instance Show Symbol where
    -- show Epsilon = "ε"
    show Epsilon = ""
    show (Name c) = [c]

showAlphabet :: [Symbol] -> String
showAlphabet = concatMap show



-- Pravidlo přechodové funkce
data Transition = Transition
    { fromState  :: State
    , bySymbol :: Symbol
    , toState :: State
    } deriving (Eq)

instance Show Transition where
    show (Transition from by to) = showState from ++ "," ++ show by ++ "," ++ showState to ++ "\n"

instance Ord Transition where
    compare (Transition from1 by1 to1) (Transition from2 by2 to2) =
        if
            from1 == from2
        then
            if
                by1 == by2
            then
                to1 `compare` to2
            else
                by1 `compare` by2
        else
            from1 `compare` from2


showTransitions :: [Transition] -> String
showTransitions = concatMap show



-- Celý konečný automat
data FSM = FSM
    { states :: [State]
    , alphabet :: [Symbol]
    , initialState :: State
    , finalStates :: [State]
    , transitions :: [Transition]
    } deriving (Eq)


instance Show FSM where
    show (FSM stats alpha initial final trans) =
        showStates (sort stats) ++ "\n" ++
        showAlphabet (sort alpha) ++ "\n" ++
        showState initial ++ "\n" ++
        showStates (sort final) ++ "\n" ++
        showTransitions (sort trans)
