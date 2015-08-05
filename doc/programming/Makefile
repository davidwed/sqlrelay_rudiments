top_builddir = ../..

include ../../config.mk

all: buildparts html

buildparts:
	cd parts; \
	rm -f *.html; \
	for i in `/bin/ls ../../../examples/*.cpp`; \
	do \
		BASENAME=`/bin/basename $$i`; \
		echo "processing $$BASENAME"; \
		gvim $$i +":runtime! syntax/2html.vim" +"colorscheme default" +":x! ./$$BASENAME.html" +"q!"; \
		sleep 2; \
	done
	for i in `/bin/ls parts/*.html`; \
	do \
		sed -i -e "/<html>/d" -e "/<head>/d" -e "/<title>.*<\/title>/d" -e "/<meta.*/d" -e "/<\/head>/d" -e "/<body.*/d" -e "/<\/body>/d" -e "/<\/html>/d" -e "/<\!DOCTYPE/d" $$i; \
	done

html:
	for i in `/bin/ls *.in`; \
	do \
		BASENAME=`/bin/basename $$i .in`; \
		echo "processing $$BASENAME"; \
		./insert.pl $$i > $$BASENAME; \
	done
