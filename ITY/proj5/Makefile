#Project 5
PROJECT=proj5
LOGIN=xmarti76

all:$(PROJECT)
$(PROJECT): $(PROJECT).tex
	make clean
	pdflatex proj5.tex
clean:
	rm -f *.aux *.log *.nav *.out *.snm *.toc
zip:
	rm -f *.zip
	zip $(LOGIN)-fit.zip $(PROJECT).tex Makefile obr/*
