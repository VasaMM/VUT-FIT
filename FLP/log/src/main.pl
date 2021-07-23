%% Autor: Vaclav Martinka
%% Login: xmarti76
%% Datum: 19. 04. 2020
%% Předmět: FLP - FIT VUT Brno
%% Zadání: Logicky projekt - Kostra grafu
%% Popis: Ziska data od uzivatele
%%        prevede je na reprezentaci grafu
%%        najde vsechny kostry tohoto grafu


% Main funkce
main :-
	prompt(_, '') ->
	read_lines(LL) ->
	split_lines(LL, S) ->
	removeBadLines(S, Res) ->
	prepareEdges(Res, EdgesUnsort) ->
	sort(EdgesUnsort, Edges) ->
	allNodes(Res, Nodes) ->
	setof(TreeEdges, spanningTree(Nodes, Edges, TreeEdges), Trees) ->
	printResult(Trees) ->
	halt.


% Ignoruji neplatne radky
removeBadLines(Input, Output) :-
	removeEmptyLines(Input, Result1),
	removeOnlyOneNode(Result1, Result2),
	removeThreeAndMoreNode(Result2, Result3),
	removeInvalidEdges(Result3, Output).

% Preskocim prazdne radky
removeEmptyLines(Input, Output) :- delete(Input, [[]], Output).


% Preskocim radky pouze s jednim uzlem
removeOnlyOneNode(Input, Output) :- delete(Input, [[_]], Output).


% Preskocim radky s vice jak dvema uzly
%% [[D],[C],[X]]
removeThreeAndMoreNode(Input, Output) :- delete(Input, [[_],[_],[_]|_], Output).


% Preskocim radky ktere neobsahuji hrany v pozadovanem tvaru
removeInvalidEdges(Input, Output) :- include(validEdge, Input, Output).


% Validni hrana je tvorena dvojici validnich jmen
validEdge([[X],[Y]]) :- validName(X), validName(Y).


% Validni jmeno je v rozsahu A - Z
validName(ID) :- atom_string(ID, X), X >= "A", X =< "Z".


% Prevedu seznam uzlu na struktury
prepareEdges(Input, Output) :- maplist(makeEdge, Input, Output).


% Vytvori strukturu edge
% Uzly seradim podle abecedy abych zabranil duplicitam
makeEdge([[X],[Y]], edge(X, Y)) :- atom_string(X, Xs), atom_string(Y, Ys), Xs < Ys.
makeEdge([[X],[Y]], edge(Y, X)) :- atom_string(X, Xs), atom_string(Y, Ys), Xs >= Ys.





% Z grafu vytvori jeho kostru
% Vynecham prvni uzel -> bude slouzit jako zaklad pro tvorbu kostry
spanningTree([_|Nodes], GraphEdges, TreeEdges) :-
	% Vytvori mnozinu hran reprezentujici kostru grafu
	makeSpanningTree(Nodes, GraphEdges, TreeEdgesUnsorted),

	% Seradim, jinak by vznikali duplicitny kostry
	sort(TreeEdgesUnsorted, TreeEdges).


% Vytvori kostru grafu (mnozinu hran), na zaklade seznamu uzlu Nodes
% a seznamu hran GraphEdges
makeSpanningTree([], _, []).
makeSpanningTree(Nodes, GraphEdges, [GraphEdge|TreeEdges]) :- 
	% Vyberu hranu GraphEdge ze seznamu hran GraphEdges
	select(GraphEdge, GraphEdges, NewGraphEdges),
	
	% Ziskam uzly X a Y z hrany GraphEdge
	getNodes(GraphEdge, X, Y),
	
	% Zkontroluji, zda muzu pouzit uzly X a Y (uz jsem jeden z nich pouzil)
	canUseNodesXY(X, Y, Nodes),

	% Vymazu uzly X a Y ze seznamu uzlu
	% Stacilo bY mazat jen ten nove pridany do kostry,
	% ale nevim, ktery to je
	delete(Nodes, X, TempNodes),
	delete(TempNodes, Y, NewNodes),
	
	% Pokracuji v "cyklu"
	makeSpanningTree(NewNodes, NewGraphEdges, TreeEdges).


% True, pokud je hrana e(X, Y) tvorena uzly X a Y
getNodes(edge(X, Y), X, Y).
getNodes(edge(X, Y), Y, X).


% V jednom kroku mohu pripojit pouze jeden uzel,
% proto musim vybrat takovou hranu, ktera alespon
% jednim uzlem navazuje na stavajici kostru
canUseNodesXY(X, Y, Nodes) :- memberchk(X, Nodes), not(memberchk(Y, Nodes)), !.
canUseNodesXY(X, Y, Nodes) :- memberchk(Y, Nodes), not(memberchk(X, Nodes)).


% Ziska vychozi uzel
getFromNode([[From],[_]], From).


% Ziska cilovy uzel
getToNode([[_],[To]], To).


% Ziska vsechny uzly v grafu
allNodes(Input, Output) :-
	maplist(getFromNode, Input, Nodes1),
	maplist(getToNode, Input, Nodes2),

	append(Nodes1, Nodes2, Nodes),   % Spojim seznamy v jeden
	sort(Nodes, Output).   % Seradim - tim se zaroven odstrani duplicity


% Zobrazi vysledky ve spravnem formatu
printResult([]).
printResult([Tree|Trees]) :- printTree(Tree), nl, printResult(Trees).


% Zobrazi jeden strom (kostru)
printTree([]).
printTree([Edge|[]]) :- printEdge(Edge), !.   % zabranim tisku mezery na konci radku
printTree([Edge|Edges]) :- printEdge(Edge), write(' '), printTree(Edges).


% Vytiskne jednu hranu
printEdge(edge(X, Y)) :- format('~w-~w', [X, Y]).










%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FLP 2020
% Toto je ukazkovy soubor zpracovani vstupu v prologu.
% Tento soubor muzete v projektu libovolne pouzit.
% 
% autor: Martin Hyrs, ihyrs@fit.vutbr.cz
% preklad: swipl -q -g start -o flp19-log -c input2.pl


% cte radky ze standardniho vstupu, konci na LF nebo EOF
read_line(L,C) :-
	get_char(C),
	(isEOFEOL(C), L = [], !;
		read_line(LL,_),
		[C|LL] = L).


% testuje znak na EOF nebo LF
isEOFEOL(C) :-
	C == end_of_file;
	(char_code(C,Code), Code==10).


read_lines(Ls) :-
	read_line(L,C),
	( C == end_of_file, Ls = [] ;
	  read_lines(LLs), Ls = [L|LLs]
	),!.


% rozdeli radek na podseznamy
split_line([],[[]]) :- !.
split_line([' '|T], [[]|S1]) :- !, split_line(T,S1).
split_line([H|T], [[H|G]|S1]) :- split_line(T,[G|S1]). % G je prvni seznam ze seznamu seznamu G|S1


% vstupem je seznam radku (kazdy radek je seznam znaku)
split_lines([],[]).
split_lines([L|Ls],[H|T]) :- split_lines(Ls,T), split_line(L,H).
