-- Autor: Václav Martinka
-- Login: xmarti76
-- Datum: 31. 03. 2020
-- Předmět: FLP - FIT VUT Brno
-- Zadání: Funkcionální projekt rka-2-dka (Převod rozšířených nedeterministických konečných automatů na deterministické)
-- Popis: Poocné funkce rpo práci se seznamy

module ListFuncs where


-- Přidá prvek do seznamu, nevytváří duplicty
add :: (Eq a) =>
       [a]   -- Původní seznam   
      -> a   -- Nový prvek
    -> [a]   -- Původní seznam doplněný o nový prvek pokud ho předtím neobsahoval
add [] x   = [x] 
add list x = if x `elem` list then list else x : list



-- Spojí dva seznamy, nevytváří duplicitní prvky
addList :: (Eq a) =>
       [a]   -- První seznam    
    -> [a]   -- Druhý seznam
    -> [a]   -- První seznam doplněný o prvky z druhého seznamu, které předtím neobsahoval
addList list []     = list
addList list (x:xs) = (list `add` x) `addList` xs



-- Ziska počet prvků v seznamu
-- https://bigonotation.altervista.org/2017/04/24/how-to-find-length-list-haskell/
size :: [a] -> Integer
size [] = 0
size (_:xs) = 1 + size xs



-- Zjistí, zda mají seznamy alespoň jeden společný prvek
common :: (Eq a) => [a] -> [a] -> Bool
common [] _ = False
common (x:xs) final = x `elem` final || common xs final



-- Zjistí, zda seznam obsahuje prvky druhého seznamu
contains :: (Eq a) => [a] -> [a] -> Bool
contains [] (_:_) = False
contains _ [] = True
contains a (x:xs) = x `elem` a && a `contains` xs




-- Převede seznam seznamů na seznam
-- Při převodu nevznikají duplicty
group :: (Eq a) => [[a]] -> [a]
group [] = []
group [x] = x
group (x:xs) = x `addList` group xs



-- Převede seznam na seznam seznamů
listify :: [a] -> [[a]]
listify []     = [[]]
listify [x]    = [[x]]
listify (x:xs) = [x] : listify xs



-- Vrací objekt na pozici
at :: [a] -> Int -> a
at x pos = x !! pos



-- Vrací objekty od pozice
fromPos :: [a] -> Int -> [a]
fromPos x pos = snd $ splitAt pos x



-- Opak null
notNull :: [a] -> Bool
notNull x = not $ null x