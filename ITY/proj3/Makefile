#Project 3
PROJECT=proj3
LOGIN=xmarti76

all:$(PROJECT)
$(PROJECT): $(PROJECT).tex
	latex $(PROJECT).tex
	latex $(PROJECT).tex
	latex $(PROJECT).tex
	dvips $(PROJECT).dvi -t a4
	ps2pdf $(PROJECT).ps
clean:
	rm -f *.dvi *.ps *.aux *.pdf *.log *.zip
zip:
	rm -f *.zip
	zip $(LOGIN)-fit.zip $(PROJECT).tex *.sty Makefile data/*
