; Login studenta: xmarti76


[BITS 32]

	GLOBAL DllMain
	EXPORT DllMain

	GLOBAL ipa_algorithm
	EXPORT ipa_algorithm

section .data

	colors     db 1, 2, 3, 0,   4, 5, 6, 0,   7, 8, 9, 0,   10, 11, 12, 0
	new_colors db 13, 14, 15, 16,   17, 18, 19, 0,   20, 21, 22, 0,   23, 24, 25, 0
	
	dword_mask    dw 0xFFFF, 0xFFFF, 0xFFFF, 0,    0xFFFF, 0xFFFF, 0xFFFF, 0
	byte_mask     db 0xFF,   0xFF,   0xFF,   0,    0xFF,   0xFF,   0xFF,   0,    0xFF, 0xFF, 0xFF, 0,    0xFF, 0xFF, 0xFF, 0
	one_byte_mask db 0xFF,   0xFF,   0xFF,   0,    0,      0,      0,      0,    0,    0,    0,    0,    0,    0,    0,    0

	index dd 0, 4, 8, 12

	one dd 1.0, 1.0, 1.0, 1.0



section .text


DllMain:
	push ebp
	mov ebp,esp
	
	mov esp,ebp
	pop ebp
	mov eax,dword 1
	ret 12
		

ipa_algorithm:
	push ebp
	mov ebp, esp

	; f(image.data, output.data,image.cols, image.rows, argc, argv);
	%define input  ebp +  8		; Vstupni obrazek
	%define output ebp + 12		; Vystupni obrazek
	%define width  ebp + 16		; Sirka obrazku
	%define height ebp + 20		; Vyska obrazku
	%define argc   ebp + 24		; Pocet argumentu
	%define argv   ebp + 28		; Argumenty
	; Data ve tvaru: [sirka][vyska] u_char B, u_char G, u_char R	

	sub esp, 36
	%define difference  ebp -  4	; Odchylka nove barvy od stare
	%define pictureSize ebp -  8 	; Pocet pixelu v obrazku
	%define userColor   ebp - 12 	; Uzivatelem zadana barva

	%define d ebp - 16
	%define c ebp - 20
	%define b ebp - 24
	%define a ebp - 28

	%define counter ebp - 32
	

	; Otestuji podporu AVX2
	call checkAVX2
	test eax, eax
	jz noAVX2

	; Zjistim, zda uzivatel zadal barvu, kterou chce vykreslit
	call getUserColor

	; Nahodne vygeneruju protvni barvy
	call generateRandomColors

	; Vytvorim shluky
	call clustering

	; Prevedu na sedou
	call toGray

	; Vse se povedlo, muzu vratit true
	mov eax, [ counter ]
	
	noAVX2:
	add esp, 36

	mov esp, ebp
	pop ebp
	ret 0


; Postupne projdu vsechny pixely
; Vyberu, ktere ze tri barev se blizi
; Nakonec prepocitam barvy a spustim algoritmus znovu
; Pokud nedojde ke zmene -> konec
clustering:
	; Nactu sirku a delku a vynasobim
	mov eax, [ width ]
	mov ebx, [ height ]
	mul bx   ; v dx,ax mam vysledek
	shl edx, 16
	add eax, edx  ; Ted mam vysledek nasobeni v eax	
	mov [ pictureSize ], eax

	
	; Vynuluji counter
	mov dword [ counter ], 0

	; Do ymm7 si nachystam 0
	vxorpd ymm7, ymm7, ymm7

	clustering_while:
		mov dword [ a ], 0
		mov dword [ b ], 0
		mov dword [ c ], 0
		mov dword [ d ], 0

		; Nactu velikost obrazku do eax
		mov eax, [ pictureSize ]
	
		; Nactu ukazatel na vstupni a vystupni data
		mov ecx, [ input ]    ; Vstup
		mov edx, [ output ]   ; Vystup

		; Zvysim counter o 1
		mov ebx, [ counter ]
		inc ebx
		mov [ counter ], ebx

		clustering_for:
			call distance

			; Vysledne vzdalenosti mam v ymm0

			; Algoritmus pro maximum:

			;   D        C        B        A
			;   B        A        D        C
			vpshufd ymm1, ymm0, 0b01001110
			; ----------------------------------
			; max(BD)  max(AC)  max(BD)  max(AC)
			vpminud ymm1, ymm0, ymm1

			; max(BD)  max(AC)  max(BD)  max(AC)
			; max(AC)  max(BD)  max(AC)  max(BD)
			vpshufd ymm2, ymm1, 0b10110001
			; ----------------------------------
			;          4 x max(ABCD)
			vpminud ymm1, ymm1, ymm2

			; Vytvorim masku
			vpcmpeqd ymm1, ymm0, ymm1
			; Z masky ziskam id
			vmovups ymm0, [ index ]
			vpand ymm0, ymm0, ymm1
			; a = 0  b = 4  c = 8  d = 12
			; Podle id nastavim ebx
			pextrd ebx, xmm0, 3
			test ebx, ebx
			jnz clustering_continue

			pextrd ebx, xmm0, 2
			test ebx, ebx
			jnz clustering_continue

			pextrd ebx, xmm0, 1
			
			; ted je to bud 4 nebo 0
			; test ebx, ebx
			; jnz clustering_continue

			; pextrd ebx, xmm0, 0

			clustering_continue:
			; Poznacim si ID shluku a pak vypoctu nove stredy
			mov byte [ edx ], bl
				
			; new_color_1[ 0, 1, 2 ] = ( ( (float)new_color_1[ 0, 1, 2 ] * a ) + (float)image.data[ i ] ) / ( a + 1 )

			; Nactu new_colors do ymm0
			vmovups ymm0, [ new_colors + ebx ]

			; Nactu image.data do ymm1
			vmovups ymm1, [ ecx ]
			vpand ymm1, ymm1, [ byte_mask ]
			
			; Nactu konstantu do ymm2
			vpbroadcastd ymm2, [ a + ebx ]

			; Prevedu z byte na dword
			vpunpcklbw ymm0, ymm0, ymm7
			vpunpcklbw ymm0, ymm0, ymm7

			vpunpcklbw ymm1, ymm1, ymm7
			vpunpcklwd ymm1, ymm1, ymm7

			; Prevedu na floaty
			vcvtdq2ps ymm0, ymm0
			vcvtdq2ps ymm1, ymm1

			; Vynasobi konstantou a
			vmulps ymm0, ymm2

			; Prictu aktualni barvu z obrazku
			vaddps ymm0, ymm1

			; konst++
			vaddps ymm2, [ one ]
			movss [ a + ebx ], xmm2

			; Vydelim a
			vdivps ymm0, ymm2

			; Prevedu zpet na byte
			vcvtps2dq ymm0, ymm0
			vpackusdw ymm0, ymm0, ymm7
			vpackuswb ymm0, ymm0, ymm7

			movss [ new_colors + ebx ], xmm0

	
			; Zjistim, jestli jsem uz na konci
			dec eax
			jz clustering_for_end

			add ecx, 3
			add edx, 3
			jmp clustering_for

		clustering_for_end:
				; Pri 50 pruchodu ukoncim cyklus
				mov ebx, [counter]
				cmp ebx, 50
				jg clustering_end

				; Porovnam stare a nove barvy
				; Nesmi se moc lisit
				vmovups ymm0, [ new_colors ]
				vmovups ymm1, [ colors ]
				
				; Ziskam absolutni rozdil vsech 4 barev
				vpsadbw ymm1, ymm0, ymm1
				vpshufd ymm1, ymm1, 0b11011000
				vphaddd ymm1, ymm1

				; Presunu barvy
				movups [ colors ], xmm0
				
				pextrw ebx, xmm1, 0
				cmp ebx, 5
				jg clustering_while

	clustering_end:
		ret



toGray:
	xor ebx, ebx

	; Pokud uzivate zadal barvu, tak ji vyberu
	cmp dword [ userColor ], -1
	je toGray_noUserColor
		mov eax, [userColor]
		; mov userColor to ecx
		mov ecx, ebp
		sub ecx, 12
		call distance

		; Vysledne vzdalenosti mam v ymm0
		vpshufd ymm1, ymm0, 0b01001110

		; Algoritmus pro maximum:

		;   D        C        B        A
		;   B        A        D        C
		; ----------------------------------
		; max(BD)  max(AC)  max(BD)  max(AC)
		vpminud ymm1, ymm0, ymm1

		; max(BD)  max(AC)  max(BD)  max(AC)
		; max(AC)  max(BD)  max(AC)  max(BD)
		vpshufd ymm2, ymm1, 0b10110001
		; ----------------------------------
		;          4 x max(ABCD)
		vpminud ymm1, ymm1, ymm2

		; Vytvorim masku
		vpcmpeqd ymm1, ymm0, ymm1
		; Z masky ziskam id
		vmovups ymm0, [ index ]
		vpand ymm0, ymm0, ymm1
		; a = 0  b = 4  c = 8  d = 12
		; Podle id nastavim ebx
		pextrd ebx, xmm0, 3
		test ebx, ebx
		jnz toGray_UserColorContinue

		pextrd ebx, xmm0, 2
		test ebx, ebx
		jnz toGray_UserColorContinue

		pextrd ebx, xmm0, 1

		; v bh mam index shluku
		toGray_UserColorContinue
		mov bh, bl

	toGray_noUserColor:
		mov eax, [ pictureSize ]
		mov ecx, [ input ]    ; Vstup
		mov edx, [ output ]   ; Vystup
			

	toGray_start:
		mov bl, [ edx ]
		cmp bl, bh
		jne toGray_do

		; Staci obnovit jen prvni byte
		mov bl, [ ecx ]
		mov [ edx ], bl
		add ecx, 3

		jmp toGray_end
	

	toGray_do:
		shl ebx, 16
		; Nactu modrou slozku a vydelim 4
		mov bl, [ ecx ]
		shr bl,  2
		shl bx, 8

		; Nactu zelenou slozku, vydelim 2 a prictu k modre
		inc ecx
		mov bl, [ ecx ]
		shr bl, 1
		add bh, bl

		; Nactu cervenou slozku, vydelim 4 a prictu
		inc ecx
		mov bl, [ ecx ]
		shr bl, 2
		add bh, bl

		mov [ edx     ], bh
		mov [ edx + 1 ], bh
		mov [ edx + 2 ], bh

		shr ebx, 16
		inc ecx

	toGray_end:
		; Posunu ukazatele
		add edx, 3

		; Od eax odectu jedna a zkontroluji na 0
		dec eax
		jz toGray_ret    ; Pokud ano tak konec

		jmp toGray_start

	toGray_ret:
		ret



generateRandomColors:
	; Zjistim, zda uzivatel specifikoval vlastni barvu
	mov eax, [ userColor ]
	cmp eax, -1
	; Ne => vygeneruji vsechny 4 nahodne
	je generateRandomColors_noUserColor

		; Ano, ulozim si ji na prvni pozici
		; zbytek vygeneruji nahodne
		mov [ colors ], eax
		jmp generateRandomColors_continue
		
	
	generateRandomColors_noUserColor:
		rdrand eax
		and eax, 0x00FFFFFF
		mov [ colors ], eax

	; Vygeneruji nahodne barvu
	generateRandomColors_continue:
		rdrand eax
		and eax, 0x00FFFFFF
		mov [ colors + 4 ], eax
		
		rdrand eax
		and eax, 0x00FFFFFF
		mov [ colors + 8 ], eax
		
		rdrand eax
		and eax, 0x00FFFFFF
		mov [ colors + 12 ], eax
	
	ret



getUserColor:
	; Otestuji pocet argumentu na 3
	mov eax, [ argc ]
	cmp eax, 3
	jne getUserColor_error

	; Nactu ukazatel na treti argument a porovnam na '#'
	mov edx, [ argv ]
	mov edx, [ edx + 8 ]
	mov  al, [ edx ]
	cmp  al, '#'
	jne getUserColor_error

	; Postupne nactu vstupni barvu a prevedu z hex do dec
	; Musim kontrolovat na /0!
	mov ecx, 0
	getUserColor_while:
		; Nactu prvni znak a zkontroluji na '/0'
		inc edx
		mov ah, [ edx ]
		test ah, ah
		jz getUserColor_error
		
		; Posunu prvni znak, nactu druhy a zkontroluji na '/0'
		inc edx
		mov al, [ edx ]
		test al, al
		jz getUserColor_error

		; Prevedu na dec a pokud se povedlo, tak ulozim
		call hex2dec
		test bh, bh
		jnz getUserColor_error

		mov [ userColor + ecx ], bl

		inc ecx
		cmp ecx, 3
		jne getUserColor_while

	; Vse se povedlo
	mov byte [ userColor + 3 ], 0
	ret

	getUserColor_error:
		mov dword [ userColor ], -1
		ret



;;;;;;;; Prevedu znak v hex v ah a al na dec v bl ;;;;;;
; Pokud vse ok, tak bh == 0, jinak chyba
; bx neni zachovano!
; 0 = 48	9 = 57
; A = 65	F = 70
; a = 97	f = 102
hex2dec:
	xor bx, bx
	; Zjistim, jaky znak mam na vstupu
	inc bh
	sub ah, 48	; Ocekavam minimalne 48
	jc hex2dec_error
	cmp ah, 10	; Na vstupu mam cislo 48 - 59
	jb hex2dec_digit1
	inc bh
	sub ah, 17	; Na vstupu mam 64<
	jc hex2dec_error
	cmp ah, 6
	jb hex2dec_char1

	inc bh
	sub ah, 32	; Na vstupu mam 96<
	jc hex2dec_error
	inc bh
	cmp ah, 6
	ja hex2dec_error

	hex2dec_char1:
		add ah, 10

	hex2dec_digit1:
		shl ah, 4
		mov bl, ah
hex2dec_2:
	; Zjistim, jaky znak mam na vstupu
	inc bh
	sub al, 48	; Ocekavam minimalne 48
	jc hex2dec_error
	
	cmp al, 10	; Na vstupu mam cislo 48 - 59
	jb hex2dec_digit2

	inc bh
	sub al, 17	; Na vstupu mam 64<
	jc hex2dec_error
	cmp al, 6
	jb hex2dec_char2

	inc bh
	sub al, 32	; Na vstupu mam 96<
	jc hex2dec_error
	inc bh
	cmp al, 6
	ja hex2dec_error

	hex2dec_char2:
		add al, 10

	hex2dec_digit2:
		add bl, al
		
	hex2dec_end:
		xor bh, bh

	hex2dec_error:
		ret




;;;;;; Otestuje podporu AVX2 ;;;;;
; V eax vraci 0 pokud AVX2 NENI podporovano
checkAVX2:
	; Otestuji podporu CPUID
	; https://cs.wikipedia.org/wiki/CPUID
	pushfd
	pop eax
	mov ebx, eax
	xor eax, 00200000h
	push eax
	popfd
	pushfd
	pop eax
	cmp eax, ebx
	jz checkAVX2_NOsupport

	; Podporuji CPUID
	; Otestuji, co vse CPUID pordporuje
	xor eax, eax
	cpuid

	cmp eax, 7
	jl checkAVX2_NOsupport

	; Procesor by mohl podporovat AVX2, otestuji to
	mov eax, 7
	mov ecx, 0
	cpuid
	test ebx, 0x20
	jz checkAVX2_NOsupport
	
	; Support
	mov eax, 1
	ret

	checkAVX2_NOsupport:
		xor eax, eax
		ret



;;;;;; Euklidovska vzdalenost dvou barev ;;;;;
; Ocekavam ukazatel na barvu v ecx
; Vysledek vracim na vrcholu zasobniku FPU
; distance = sqrt( (a_1 - a_2)^2 + (b_1 - b_2)^2 + (c_1-c_2)^2 )

distance:
	; Nactu barvu z ecx
	vpbroadcastd ymm0, [ ecx ]
	; ?  R  G  B 	?  R  G  B 		?  R  G  B 		?  R  G  B
	; F6 8D D1 F6	F6 8D D1 F6		F6 8D D1 F6		F6 8D D1 F6
	
	; Nactu docasne barvy
	vmovups ymm1, [ colors ]
	; ?  ?  ?  ? 	?  R  G  B 		?  R  G  B 		?  R  G  B
	; 00 00 00 00	00 FF FE FD		00 06 05 04		00 03 02 01 

	; Rozdelim je na min a max => abych mohl zjistit rozdil
	vpmaxub ymm2, ymm0, ymm1
	vpminub ymm3, ymm0, ymm1

	; V ymm2 mam rozdily jednotliych slozek
	vpsubb ymm2, ymm2, ymm3
	; F6 8D D1 F6 	F6 72 2D 07 	F6 87 CC F2 	F6 8A CF F5 	
	
	; Prevedu z byte na word
	vpunpcklbw ymm0, ymm2, ymm7
	vpunpckhbw ymm1, ymm2, ymm7

	; Vymaskuji
	vpand ymm0, ymm0, [ dword_mask ]
	vpand ymm1, ymm1, [ dword_mask ]

	; Vynasobim
	vpmaddwd ymm0, ymm0, ymm0
	vpmaddwd ymm1, ymm1, ymm1

	; Sectu
	vphaddd ymm0, ymm0, ymm7
	vphaddd ymm1, ymm1, ymm7

	;                            00 00 00 87   00 CC 00 F2   00 00 00 8A    00 CF 00 F5
	;                              \     /       \     /       \     /       \     /   
	;                            00 00 47 31   00 01 87 54   00 00 4A 64    00 01 91 DA
	;   
	;   00 00 00 00  00 00 00 00          00 01 CE 85               00 01 DC 3E

	; Prehodim poradi
	vpshufd ymm1, ymm1, 0b01001111

	; Sloucim vse prevedu do jednoho registru
	vblendps ymm0, ymm0, ymm1, 0b1100
	; 00 00 3A DE   00 01 C1 02   00 01 CE 85   00 01 DC 3E

	; Prevedu na floaty
	vcvtdq2ps ymm1, ymm0

	; Odmocnim
	vsqrtps ymm1, ymm1

	; Prevedu zpet na integery
	vcvtps2dq ymm0, ymm1

	; Vratim vzdaleosti v registru ymm0
	ret



