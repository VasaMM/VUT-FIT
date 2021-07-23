-- Autor: Václav Martinka
-- Login: xmarti76
-- Datum: 31. 03. 2020
-- Předmět: FLP - FIT VUT Brno
-- Zadání: Funkcionální projekt rka-2-dka (Převod rozšířených nedeterministických konečných automatů na deterministické)
-- Popis: Ponocné výpočetní funkce pro determinizaci konečného automatu

module Functions(nka2dka,validate) where

import DataFsm
import Data.List (sort)
import ListFuncs



-- Převede rozšížený konečný automat na deterministický
nka2dka :: FSM -> FSM
nka2dka fsm = renameFsm $ fillStates $ fillFinalStates $ removeEmptyTrans $ nka2dkaCalc fsm



-- Pomocná funkce pro nka2dka
nka2dkaCalc :: FSM -> FSM
nka2dkaCalc fsm = FSM
    []
    (alphabet fsm)
    (newInitState fsm)
    (finalStates fsm)
    (determize fsm [newInitState fsm] [] [])



-- Najde volné jméno
findName :: FSM -> State
findName fsm = findNameCalc (states fsm) (listify [0 ..])



-- Najde první nekolizní jméno stavu ze seznamu
findNameCalc ::
       [State]   -- Seznam existujicích jmen
    -> [State]   -- Seznam nových jmen  
    -> State     -- První volné nekolizní jméno  
findNameCalc existing new =
    if
        head new `elem` existing
    then
        findNameCalc existing (tail new)
    else
        head new



-- Přejmenuje stavy v automatu
renameFsm :: FSM -> FSM
renameFsm fsm = rename fsm (sort $ states fsm)



-- Přejmenuje stavy v automatu
rename :: FSM -> [State] -> FSM
rename fsm left =
    if
        null left
    then
        fsm
    else
        rename (renameCalc fsm (head left)) (tail left)



-- Pomocná funkce pro rename
renameCalc :: FSM -> State -> FSM
renameCalc fsm name = FSM
    (changeName (states fsm) name (findName fsm))
    (alphabet fsm)
    (head (changeName [initialState fsm] name (findName fsm)))
    (changeName (finalStates fsm) name (findName fsm))
    (changeNameInTrans (transitions fsm) name (findName fsm))



changeName ::    -- Změní jméno stavu v seznamu za jiné
       [State]   -- Seznam stavů
      -> State   -- Staré jméno
      -> State   -- Nové jméno
    -> [State]   -- 
changeName [] _ _ = []
changeName (x:xs) old new = (if x == old then new else x) : changeName xs old new



-- Podobně jako changeName ale pro seznam přechodových pravidel
changeNameInTrans :: [Transition] -> State -> State -> [Transition]
changeNameInTrans [] _ _ = []
changeNameInTrans (x:xs) old new =
    Transition
        (head (changeName [fromState x] old new))
        (bySymbol x)
        (head (changeName [toState x] old new))
    :
    changeNameInTrans xs old new



-- Spočte epsilon uzávěr v daném FSM pro daný seznam stavů
eClosure :: FSM -> [State] -> [State]
eClosure fsm = eClosureCalc (transitions fsm) []



-- Pomocná výpočetní funkce pro eClosure
eClosureCalc ::
    [Transition]   -- Seznam přechodových pravidel
      -> [State]   -- Stavy dostupné v přechozím kroku
      -> [State]   -- Stavy dostupné v aktuálním kroku
      -> [State]   -- Výsledek aktuálního kroku
eClosureCalc [] _ _ = []
eClosureCalc _ _ [] = []
eClosureCalc trans q q' =
    if
        size q == size q'
    then
        q
    else
        eClosureCalc trans q' (availStates trans q' Epsilon `addList` q')



availStates ::
    [Transition]   -- Seznam přechodových pravidel
      -> [State]   -- Výchozí stavy
       -> Symbol   -- Symbol
      -> [State]   -- Všechny dostupné stavy z daných stavů pomocí daného symbolu
availStates _ [] _ = []
availStates trans (x:xs) by = availStatesCalc trans x by `addList` availStates trans xs by



availStatesCalc ::
    [Transition]   -- Seznam přechodových pravidel
        -> State   -- Výchozí stav
       -> Symbol   -- Symbol
      -> [State]   -- Všechny dostupné stavy z daného stavu pomocí daného symbolu
availStatesCalc [] _ _ = []
availStatesCalc (x:xs) from by =
    if
        fromState x == from && bySymbol x == by
    then
        toState x : availStatesCalc xs from by
    else
        availStatesCalc xs from by



-- Odstraní pravidla, kterou vedou z nebo do prázdného stavu
-- Fakticky se jedná o SINK stav, který je v tomot případě nežádoucí
removeEmptyTrans :: FSM -> FSM
removeEmptyTrans (FSM stats alpha initial final trans) =
    FSM
        stats
        alpha
        initial
        final
        (removeEmptyTransCalc trans)



-- Pomocná funkce pro removeEmptyTrans
removeEmptyTransCalc :: [Transition] -> [Transition]
removeEmptyTransCalc [] = []
removeEmptyTransCalc trans = [x | x <- trans, notNull (fromState x) && notNull (toState x)]



-- Získá nový výchozí stav pomocí epsilon uzávěru
newInitState :: FSM -> State
newInitState fsm =  sort $ group $ eClosure fsm [initialState fsm]



makeTrans ::          -- Na základě starého fsm vytvoří nová pravidla
                FSM   -- 
           -> State   -- Nový stav (sdružené jméno)
        -> [Symbol]   -- Seznam symbolů (abeceda)
    -> [Transition]   -- Nová přechodová pravidla
makeTrans _ _ [] = []
makeTrans fsm from (x:xs) = makeTransCalc fsm from x : makeTrans fsm from xs



makeTransCalc ::    -- Vytvoří nové pravidlo na základě sdruženého jména nového stavu a symbolu
              FSM   -- 
         -> State   -- Nový stav
        -> Symbol   -- Symbol
    -> Transition   -- Nové pravidlo
makeTransCalc fsm from by =
    Transition
        from
        by
        (sort $ group (eClosure fsm (availStates (transitions fsm) (listify from) by)))



determize ::          -- Determizace přechodových pravidel
                FSM   -- 
         -> [State]   -- Seznam nově vzniklych stavů
         -> [State]   -- Seznam všech nových stavů
    -> [Transition]   -- Nová pravidla
    -> [Transition]   -- Nová pravidla
determize fsm x new result =
    if
        null x
    then
        result
    else
        determize
            fsm
            (getDests $ deterStep fsm x new)
            (x `addList` new)
            (deterStep fsm x new `addList` result)



-- Jeden determizační krok (pomocná funkce pro determize)
deterStep ::
                FSM   -- 
         -> [State]   -- Seznam nově vzniklych stavů
         -> [State]   -- Seznam všech nových stavů
    -> [Transition]   -- Nová pravidla
deterStep _ [] _ = []
deterStep fsm [x]    new = if x `elem` new then [] else makeTrans fsm x (alphabet fsm)
deterStep fsm (x:xs) new = deterStep fsm [x] new `addList` deterStep fsm xs new



-- Získá seznam všech cílových stavů pro daná pravidla
getDests :: [Transition] -> [State]
getDests [] = []
getDests (x:xs) = getDests xs `add` toState x



-- Vytvoří nový seznam stavů na základě seznamu přechodých pravidel 
fillStates :: FSM -> FSM
fillStates (FSM _ alpha initial final trans) =
    FSM
        (getAllStates trans `addList` final `add` initial)
        alpha
        initial
        final
        trans



-- Doplní seznam koncových stavů
fillFinalStates :: FSM -> FSM
fillFinalStates (FSM stats alpha initial final trans) =
    FSM
        stats
        alpha
        initial
        (fillFinalCalc (getDests trans) final)
        trans



-- Doplní seznam koncových stavů - pomocná funkce
fillFinalCalc :: [State] -> [State] -> [State]
fillFinalCalc [] _ = [] 
fillFinalCalc (x:xs) final = [x | listify x `common` final] ++ fillFinalCalc xs final



-- Získá seznam stavů vyskytujících se v pravidlech
getStates :: Transition -> [State]
getStates (Transition from _ to) = [from] `add` to



-- Získá seznam stavů vyskytujících se v pravidlech
getAllStates :: [Transition] -> [State]
getAllStates trans = group (map getStates trans)



-- Validuje zadany FSM
-- Kontroluje, zda pravidla obsahuji stavy a symboly z mnoziny stavu a abecedy
validate:: FSM -> Bool
validate (FSM stats alpha initial final trans) =
    initial `elem` stats &&
    stats `contains` final &&
    stats `contains` getAllStates trans &&
    (Epsilon : alpha) `contains` map bySymbol trans