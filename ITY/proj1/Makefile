#Project 1
PROJECT=proj1
LOGIN=xmarti76

all:$(PROJECT)
$(PROJECT): $(PROJECT).tex
	latex $(PROJECT).tex
	dvips $(PROJECT).dvi
	ps2pdf $(PROJECT).ps
clean:
	rm -f *.dvi *.ps *.aux *.pdf *.log *.zip
zip:
	zip $(LOGIN)-fit.zip $(PROJECT).tex Makefile 