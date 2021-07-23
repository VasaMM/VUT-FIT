#Project 2
PROJECT=proj2
LOGIN=xmarti76

all:$(PROJECT)
$(PROJECT): $(PROJECT).tex
	latex $(PROJECT).tex
	latex $(PROJECT).tex
	latex $(PROJECT).tex
	dvips $(PROJECT).dvi -t a4
	ps2pdf $(PROJECT).ps
clean:
	rm -f *.dvi *.ps *.aux *.pdf *.log
zip:
	zip $(LOGIN)-fit.zip $(PROJECT).tex Makefile 
