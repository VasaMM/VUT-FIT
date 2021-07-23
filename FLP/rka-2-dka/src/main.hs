-- Autor: Václav Martinka
-- Login: xmarti76
-- Datum: 31. 03. 2020
-- Předmět: FLP - FIT VUT Brno
-- Zadání: Funkcionální projekt rka-2-dka (Převod rozšířených nedeterministických konečných automatů na deterministické)
-- Popis: Main funkce celého projektu

module Main (main)  where

import DataFsm(FSM)
import Parse(parseFsm)
import Functions(validate,nka2dka)
import System.Environment
import System.Exit
import System.IO



main :: IO ()
main = do
    (action, input) <- procArgs =<< getArgs
    action input


-- Zpracování příkazového řádku
procArgs :: [String] -> IO (String -> IO (), String)
procArgs [x] = procArgs [x,""]
procArgs [x,y] = do
    input <- getInput y
    case x of
     "-i"     -> return (putStrLn, show $ parseAndValid input)
     "-t"     -> return (putStrLn, show $ nka2dka $ parseAndValid input)
     "-h"     -> return (putStrLn, help)
     "--help" -> return (putStrLn, help)
     _        -> die' ("Unknown option " ++ x ++ "!\n")
procArgs _ = die' ("Missing option!\n" ++ help)


-- Zpracuje vstup a rovnou ho validuje
parseAndValid :: String -> FSM
parseAndValid input = validate (parseFsm input) ? parseFsm input :? error "Input FSM isn't valid!" 


-- Napoveda
help :: String
help = "Usage: rka-2-dka (-i|-t) <FILE>\n -i   just read and view\n -t   determize fsm"


-- Nacte obsah souboru, pokud neni zadan nazev, cte ze stdin
getInput :: String -> IO String
getInput "" = getContents
getInput file = readFile file



-- Ternární operátor
-- https://wiki.haskell.org/Ternary_operator
data Cond a = a :? a

infixl 0 ?
infixl 1 :?

(?) :: Bool -> Cond a -> a
True  ? (x :? _) = x
False ? (_ :? y) = y



-- | Write given error message to `stderr` and terminate with `exitFailure`.
--
-- @since 4.8.0.0
die' :: String -> IO a
die' err = hPutStrLn stderr err >> exitFailure