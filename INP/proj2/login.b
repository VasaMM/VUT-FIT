+++++ +$   nastav b0 na 6 a uloz 6 do tmp
[          spust cyklus; vysledkem je b0 a b1 na 0; b2 na 6*6*3=108
  > !      nastav b1 na 6
  [
    > +++  zvys b2 o 3
    < -    a to celkem 6krat
  ]
  < -      a to cele opakuj 6krat
]
  >> $     nacti 108 do tmp
  > ++++   nastav b4 na 4
  > !      nastav b5 na 108
  > !      nastav b6 na 108
  <<       4 proved nasledujici cyklus
[
  > +++    pricti 3  k b5 az na 120
  > ---    odecti 3 od b6 az na 96
  << -
]
> .        vytiskni b5
!+.        nastav b5 na 108; zvys na 109 a vytiskni
> +.       zvys   b6 na 97 a vytiskni
< $        uloz b5 {109} do tmp
+++++.     zvys b5 na 114 a vytiskni
++.        zvys b5 na 116 a vytiskni
!----.     nastat b5 na 109; sniz na 105 a vytiskni
> !+       nastav b6 na 109 a zvys na 110
[          nasledujici cyklus probehne 55krat
  > +      zvys b7 o jedna az na 55
  < --     sniz b6 o dva
]
> .        posun se na b7 {55} a vytiskni
-.         sniz b7 na 54 a vytiskni