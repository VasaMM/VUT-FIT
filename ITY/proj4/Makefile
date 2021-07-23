#Project 4
PROJECT=proj4
LOGIN=xmarti76

all:$(PROJECT)
$(PROJECT): $(PROJECT).tex
	make clean
	latex  $(PROJECT)
	bibtex $(PROJECT)
	latex  $(PROJECT)
	latex  $(PROJECT)
	latex  $(PROJECT)
	dvips  $(PROJECT).dvi -t a4
	ps2pdf $(PROJECT).ps
clean:
	rm -f *.dvi *.ps *.aux *.pdf *.log *.zip *.bbl *.blg
zip:
	rm -f *.zip
	zip $(LOGIN)-fit.zip $(PROJECT).tex Makefile $(PROJECT).bib *.bst
	#*.sty data/*
